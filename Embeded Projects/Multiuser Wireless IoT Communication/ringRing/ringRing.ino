/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 04/08/2022
  *    Date last modified       : 04/22/2022
  *    modifications            : 
  *    
  ***********************************************************************************
  
  Multiuser Wireless IoT Communication -“Ring-Ring… Who’s Cal DOOOOM! starts playing”
  
  ETHOS-e gathers temperature and relative humidity information using:
    -NodeMCU
    -7-segment display
    -piezo buzzer
    -Keyes KY-040 rotary encoder with pushbutton shaft
    -74HC595 shift register
    -seven 220Ω resistors
    -Mqtt-spy software

    "Ring-Ring" is a wireless IoT device capable of exchanging Ring-Ring messages
    with other Ring-Ring devices. All messages are transmitted wirelessly via MQTT 
    using the ECE MQTT broker."Ring-Ring" utilizes embedded systems design, 
    hardware interfacing, MQTT messaging protocol, MQTT brokers, JSON payloads, 
    and MQTT-Spy.
*/

/////////////////////////////////////////////////////////////////
//libs
#include <Button2.h>;
#include <Rotary.h>;

#include <ESP8266WiFi.h>    
#include <PubSubClient.h>               
#include <ArduinoJson.h>          

#include "rotary7-sigBuzz.h"
#include "mqtt_ringNode.h"
////////////////////////////////////////////////////////////////
//functions
void updatePBStatus();

void myfnUpdateDisplay(byte eightBits);
byte myfnNumToBits(int someNumber);

void ringRing();

WiFiClient wfClient;
PubSubClient psClient(wfClient);        

void connect_wifi();
void reconnect();

/////////////////////////////////////////////////////////////////
//globals
Rotary r;
Button2 b;

char json_msgBuffer[200];   // char buffer to store incoming/outgoing messages
char sbuf[80];              // buffer to store sprintf formatted strings for printing

int hbInt = 3000;
String selectedNode;
String nodeNotation;

int topicIndex;
int senderNodeMCU;
int msg;

byte bits;

/////////////////////////////////////////////////////////////////
//Mqtt "Ring-Ring" function
void sendMQTTMessage(String senderID, String ringring)  
{
  // create a parse tree buffer and point to the root of the buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["srcNode"]  = senderID;
  root["dstNode"]  = ringring;

  //send message
  root.printTo(json_msgBuffer, sizeof(json_msgBuffer));
  String msgTopic = "ece/" + ringring + "/ringring";
  const char *cmd = msgTopic.c_str();
  psClient.publish(cmd, json_msgBuffer);
}

//Mqtt "heartbeat" function
void sendHeartbeat(String senderID, String heartbeat)
{
  // create a parse tree buffer and point to the root of the buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["NodeName"] = senderID;
  root["NodeType"] = heartbeat;

  //send message
  root.printTo(json_msgBuffer, sizeof(json_msgBuffer));
  String msgTopic = "ece/" + senderID + "/heartbeat";
  const char *cmd = msgTopic.c_str();
  psClient.publish(cmd, json_msgBuffer);
}

//Mqtt "registerdFor" function
void registerdForResponse(String senderID)
{
  // create a parse tree buffer and point to the root of the buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["NodeName"] = senderID;
  root["topics"] = "[topics, ringring, beattime]";

  //send message
  root.printTo(json_msgBuffer, sizeof(json_msgBuffer));
  String msgTopic = "ece/" + senderID + "/registerdFor";
  const char *cmd = msgTopic.c_str();
  psClient.publish(cmd, json_msgBuffer);
}

//Mqtt "process-Message" function
void processMQTTMessage(char* topic, byte* json_payload, unsigned int length)
{
  //process messages by topic
  for(int j = 0; j <= sizeof(arrayRec); j++)
  {
    sprintf(sbuf,"ece/%s/%s", clientID, arrayRec[j]);
    
    if(strcmp(topic, sbuf) == 0)
    {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject((char*)json_payload);

      String regfor   = root["topics"];
      String senderID = root["srcNode"];
      String clientID = root["dstNode"];
      msg             = root["time"];

      if(senderID == NULL && j == 1)
        j = -1;
      
      senderID.remove(0,4);
      senderNodeMCU = senderID.toInt();
      
      topicIndex = j;
      
      break;
   }
  }
  //see which topic mqtt is communicating to
  Serial.println(topicIndex);
  switch(topicIndex)
  {
    case 0:
      Serial.print("in topics 0=");
      Serial.println(topicIndex);

      registerdForResponse(clientID);
      break;
    case 1:
      Serial.print("in ringring 1=");
      Serial.println(topicIndex);
      
      r.resetPosition(senderNodeMCU,true);
      bits = myfnNumToBits(senderNodeMCU);
      myfnUpdateDisplay(bits);
      ringRing();
      break;
    case 2:
      Serial.print("in beattime 2=");
      Serial.println(topicIndex);
      hbInt = msg;
      break;
  }
}


//register to receive messages
void register_myself()
{
  // register with MQTT broker for topics of interest to this node
  Serial.print("Registering for topics...");
  for(int i = 0; i <= sizeof(arrayRec); i++)
  {
    sprintf(sbuf,"ece/%s/%s", clientID, arrayRec[i]);
    psClient.subscribe(sbuf);
  }
  //sprintf(sbuf,"ece/%s/ringring", clientID);
  //psClient.subscribe(sbuf);
  Serial.println(" done");
}

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Counter");

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);
  r.setUpperBound(16);
  r.setLowerBound(-1);

  b.begin(BUTTON_PIN, INPUT_PULLUP);
  b.setTapHandler(click);
  //b.setLongClickHandler(resetPosition);

  currentMillis = millis();
  
  // attach a debouncer to rotary pushbutton and set debounce interval
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(DEBOUNCE_INTERVAL);

  // Create a timer to periodically call the updatePBStatus routine
  pbTimer.attach_ms(PB_UPDATE_TIME, updatePBStatus);

  // initialize counters
  pressedCount = 0;
  releasedCount = 0;

  // initialize I/O pins for 7-sig
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  connect_wifi();
  
  psClient.setServer(mqttBroker, mqttPort); // specify MQTT broker's domain name (or IP address) and port number
  psClient.setCallback(processMQTTMessage); // Specify callback function to process messages from broker
}

void loop() {
  // reconnect to MQTT server if connection lost
  if (!psClient.connected()) 
  {
    reconnect();
  }

  //check rotary and button
  r.loop();
  b.loop();

  //if heartbeat is zero, stop pulsing mqtt
  if(hbInt != 0)
  {
    if(millis() - currentMillis >= HEARTBEAT_INTERVAL)
    {
      Serial.println("heartbeat");
      sendHeartbeat(clientID, "ESP8266");
      currentMillis = millis();
    }
  }

  psClient.loop();  // call periodically to keep client alive and well
}

/////////////////////////////////////////////////////////////////

//rotary on change function
void rotate(Rotary& r) {
  
   Serial.println(r.getPosition());
   
   if(r.getPosition() == r.getUpperBound())
   {
    r.resetPosition(0, true);
   }
   else if(r.getPosition() == r.getLowerBound())
   {
    r.resetPosition(15,true);
   }
   
   bits = myfnNumToBits(r.getPosition());
   myfnUpdateDisplay(bits);
}

//rotary on left or right rotation function
void showDirection(Rotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

// single click
void click(Button2& btn) {
  Serial.println("Click!\n");
  if(r.getPosition() <= 9)
  {
    nodeNotation = "node0";
  }
  else if(r.getPosition() >= 10)
  {
    nodeNotation = "node";
  }
  
  selectedNode = nodeNotation + (String)r.getPosition();
  sendMQTTMessage(clientID, selectedNode);
  
  Serial.println(r.getPosition());
}

/*uncomment if you want the long press functionality*/
// long click
//void resetPosition(Button2& btn) {
//  r.resetPosition();
//  Serial.println("Reset!");
//}

/////////////////////////////////////////////////////////////////
//functions that can be afforded to be out the way

//reconnect to wifi
void reconnect()
{
  // Loop until the pub-sub client connects to the MQTT broker
  while (!psClient.connected()) {
    // attempt to connect to MQTT broker
    Serial.print("Connecting to MQTT broker (");
    Serial.print(mqttBroker);
    Serial.print(") as ");
    Serial.print(clientID);
    Serial.print("...");
    if (psClient.connect(clientID)) {
      Serial.println(" connected");
      // clientID MUST BE UNIQUE for all connected clients
      // can also include username, password if broker requires it
      // (e.g. psClient.connect(clientID, username, password)

      // once connected, register for topics of interest
      register_myself();
      sprintf(sbuf,"MQTT initialization complete\r\nReady!\r\n\r\n");
      Serial.print(sbuf);
    }
    else {
      // reconnect failed so print a console message, wait, and try again
      Serial.println(" failed.");
      #ifdef LIPSCOMB
        Serial.println("Trying again in 5 sec. (Is processor whitelisted?)");
      #else
        Serial.println("Trying again in 5 sec.");
      #endif
      // wait 5 seconds before retrying
      delay(5000);
    }
  }
}
