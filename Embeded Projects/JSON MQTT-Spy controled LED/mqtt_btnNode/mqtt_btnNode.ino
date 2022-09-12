// included configuration file and support libraries
#include "mqtt_btnNode.h"                    // this project's .h file
#include <ESP8266WiFi.h>                   // wi-fi support
#include <PubSubClient.h>                  // MQTT client
#include <ArduinoJson.h>                   // MQTT payloads are in JSON format
#include <Bounce2.h>                       // pushbutton debouncer library
#include <Ticker.h>                        // allows timed callbacks

/*
  ***********************************************************************************
  *    This code was created by : Dr.Nordstrom
  *    Code modified by         : Mina Waheeb
  *    Date modified            : 3/04/2022
  *    Date last modified       : 3/11/2022
  *    
  ***********************************************************************************
  
  Lab05: MQTT LED wireless commnunication...very cool!

  Lab05:
  using the Message Queuing Telemetry Transport (MQTT) protocol to communicate wirelessly with
  a remote “LED node,” sending messages to turn the node’s LED on or off using two pbs, and 
  receiving messages from the LED node confirming the LED’s current state.

  This was done by constructing an MQTT-based btnNode05 node using a second NodeMCU. 
  The btnNode05 node has two pushbuttons (“On” and “Off”). Pressing either buttons sends a MQTT
  formatted  ledCommand message to the ledNode05 node, causing its LED to turn on or off. Also, 
  btnNode05 node receives ledStatus messages sent by the ledNode05 node and display the message 
  text on a serial monitor (115200N81) connected to btnNode05.
  
*/

WiFiClient wfClient;                       // create a wifi client
PubSubClient psClient(wfClient);           // create a pub-sub object (must be associated with a wifi client)

char json_msgBuffer[200];                  // char buffer to store incoming/outgoing messages
char sbuf[80];                             // buffer to store sprintf formatted strings for printing

Ticker pbTimer;                            // create a Ticker object to periodically check the debounced pushbutton
Bounce ledOnPBDebouncer = Bounce();
Bounce ledOffPBDebouncer = Bounce();

unsigned long currentMillis = millis();    // global millisecond tick counter

/**********************************************************
 * Helper functions
 *********************************************************/
void connect_wifi()
{
  // in an attempt to remove the annoying garbled text on
  // startup, print a couple of blank lines (with delay)
  Serial.println();
  delay(100);
  Serial.println();
  delay(100);

  // attempt to connect to the WiFi network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" network");
  delay(10);
  #ifdef LIPSCOMB
    WiFi.begin(ssid);            // Lipscomb WiFi does NOT require a password
  #else
    WiFi.begin(ssid, password);  // For WiFi networks that DO require a password
  #endif

  // advance a "dot-dot-dot" indicator until connected to WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  // report to console that WiFi is connected and print IP address
  Serial.print(" connected as ");
  Serial.println(WiFi.localIP());
}


/*This code is called whenever a message previously registered for is received from the broker. 
 * Incoming messages are selected by topic,then the payload is parsed and appropriate action taken. 
 * (NB: If only a single message type has been registered for, then there's no need to select 
 * on a given message type. In practice this may be rare though...)*/
void processMQTTMessage(char* topic, byte* json_payload, unsigned int length)
{
  // process messages by topic
  sprintf(sbuf,"%s/ledStatus", ledClientID);
  if(strcmp(topic, sbuf) == 0) 
  {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject((char*)json_payload);

    /* JSON name-value pairs have been parsed into the parse tree, 
     *  so extract values associated with the names "senderID", 
     *  "ledStatus", and "msg" are stored values in string variables*/
    if(root.success()) 
    {
      String senderID = root["senderID"];
      String ledStatus= root["ledStatus"];
      String msg      = root["msg"];

      if(ledStatus == "on") 
      {
        Serial.print(msg);
        Serial.print("\r\n");
      }
      else if(ledStatus == "off") {
        Serial.print(msg);
        Serial.print("\r\n");
      }
      else
      {
        Serial.print("Unknown command received (");
        Serial.print(ledStatus);
        Serial.println(")\r\n");
      }
    }
    else 
    {
      sprintf(sbuf,"failed to parse JSON payload (topic: %s)\r\n", topic);
      Serial.print(sbuf);
      return;
    }
  }
  else 
  {
    sprintf(sbuf,"Topic: \"%s\" unhandled\r\n", topic);
    Serial.print(sbuf);
  }
}

//send command to node led on button-push
void sendPBtoLED(String senderID, String cmdLED)
{
  // create a parse tree buffer and point to the root of the buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["senderID"] = senderID;
  root["cmd"] = cmdLED;
   
  // Send message
  root.printTo(json_msgBuffer, sizeof(json_msgBuffer));
  String msgTopic = senderID + "/ledCommand";
  const char *cmd = msgTopic.c_str();  // put into char array form (zero copy!!)
  psClient.publish(cmd, json_msgBuffer);
}

//update push-buttons
void updatePBStatus() 
{
  ledOnPBDebouncer.update();
  ledOffPBDebouncer.update();

  if(ledOnPBDebouncer.fell())
  {
    sendPBtoLED(ledClientID, "on");
    //printf("LED on!\r\n");   
  }
  if(ledOffPBDebouncer.fell())
  {
    sendPBtoLED(ledClientID, "off");
    //printf("LED off!\r\n");   
  }
}

void register_myself()
{
  // register with MQTT broker for topics of interest to this node
  Serial.print("Registering for topics...");
  sprintf(sbuf,"%s/ledStatus", ledClientID);
  psClient.subscribe(sbuf);
  Serial.println(" done");
}

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

//setup function
void setup() 
{
  Serial.begin(115200);
  while(!Serial) 
  {
    delay(1);  // wait for serial connection
  }

  //pushbutton debouncer
  ledOnPBDebouncer.attach(PB_ON  ,INPUT_PULLUP);
  ledOffPBDebouncer.attach(PB_OFF,INPUT_PULLUP);
  pbTimer.attach_ms(PB_UPDATE_TIME, updatePBStatus);
  
  connect_wifi();

  psClient.setServer(mqttBroker, mqttPort); // specify MQTT broker's domain name (or IP address) and port number
  psClient.setCallback(processMQTTMessage); // Specify callback function to process messages from broker

  // flash the on-board LED five times to let user know
  // that the NodeMCU board has been initialized and ready to go
  pinMode(D4, OUTPUT);
  for(int i=0; i<5; i++) 
  {
    digitalWrite(D4, 0);  // active low
    delay(20);
    digitalWrite(D4, 1);
    delay(150);
  }
  Serial.println("Network initialization complete");
}

/*This is largely a reactive program, and as such only uses
 * the main loop to maintain the MQTT broker connection and
 * periodically call the psClient.loop() code to ensure the
 client stays active*/
void loop()
{
  currentMillis = millis();
  
  // reconnect to MQTT server if connection lost
  if (!psClient.connected()) 
  {
    reconnect();
  }

  delay(10);
  psClient.loop();  // call periodically to keep client alive and well
}
