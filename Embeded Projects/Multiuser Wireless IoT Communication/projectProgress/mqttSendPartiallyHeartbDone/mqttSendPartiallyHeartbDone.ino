/////////////////////////////////////////////////////////////////
//libs
#include <Button2.h>;
#include <Rotary.h>;

#include <ESP8266WiFi.h>                   // wi-fi support
#include <PubSubClient.h>                  // MQTT client
#include <ArduinoJson.h>                   // MQTT payloads are in JSON format

#include "rotary7-sigBuzz.h"
#include "mqtt_ringNode.h"
////////////////////////////////////////////////////////////////
//functions
void updatePBStatus();

void myfnUpdateDisplay(byte eightBits);
byte myfnNumToBits(int someNumber);

void ringRing();

WiFiClient wfClient;
PubSubClient psClient(wfClient);           // create a pub-sub object (must be associated with a wifi client)

void connect_wifi();
void reconnect();

/////////////////////////////////////////////////////////////////
//globals
Rotary r;
Button2 b;

char json_msgBuffer[200];   // char buffer to store incoming/outgoing messages
char sbuf[80];              // buffer to store sprintf formatted strings for printing

int hbInt = 5000;
String selectedNode;
String nodeNotation;

/////////////////////////////////////////////////////////////////
//Mqtt function
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

void processMQTTMessage()
{}

//register to receive messages
void register_myself()
{
  // register with MQTT broker for topics of interest to this node
  Serial.print("Registering for topics...");
  sprintf(sbuf,"ece/%s/topics", clientID);
  psClient.subscribe(sbuf);
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
  b.setLongClickHandler(resetPosition);

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
  //psClient.setCallback(processMQTTMessage); // Specify callback function to process messages from broker
}

void loop() {
  // reconnect to MQTT server if connection lost
  if (!psClient.connected()) 
  {
    reconnect();
  }
  
  r.loop();
  b.loop();
  
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

// on change
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
   
   byte bits = myfnNumToBits(r.getPosition());
   myfnUpdateDisplay(bits);
}

// on left or right rotation
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
  
  if(r.getPosition() == 12)
  {
    ringRing();
    Serial.println("button clicked on 12 should be sending mqtt messsage now...\n");
    sendMQTTMessage(clientID, "ringring");
    
  }
  Serial.println(r.getPosition());
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}

/////////////////////////////////////////////////////////////////
//functions that can be afforded to be out the way
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
