// included configuration file and support libraries
#include "mqtt_btnNode_starter_code.h"     // this project's .h file
#include <ESP8266WiFi.h>                   // wi-fi support
#include <PubSubClient.h>                  // MQTT client
#include <ArduinoJson.h>                   // MQTT payloads are in JSON format
#include <Bounce2.h>                       // pushbutton debouncer library
#include <Ticker.h>                        // allows timed callbacks

WiFiClient wfClient;                       // create a wifi client
PubSubClient psClient(wfClient);           // create a pub-sub object (must be
                                           // associated with a wifi client)

// char buffer to store incoming/outgoing messages
char json_msgBuffer[200];

// buffer to store sprintf formatted strings for printing
char sbuf[80];

// create a Ticker object to periodically check the debounced pushbutton
Ticker pbTimer;

// create a Bounce object for "On" and "Off" PB switches
Bounce ledOnPBDebouncer = Bounce();
Bounce ledOffPBDebouncer = Bounce();

// global millisecond tick counter
unsigned long currentMillis    = millis();

//button states
boolean state_on    = false;
boolean state_off   = false;

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

void processMQTTMessage(char* topic, byte* json_payload, unsigned int length)
{

}

void sendPBtoLED(String senderID, String cmdLED)
{
  // create a parse tree buffer and point to the root of the buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  //root["senderID"] = clientID;
  root["cmd"] = cmdLED;

  // Send message
  root.printTo(json_msgBuffer, sizeof(json_msgBuffer));
  String msgTopic = senderID + "/ledCommand";
  const char *cmd = msgTopic.c_str();  // put into char array form (zero copy!!)
  psClient.publish(cmd, json_msgBuffer);
}

void register_myself()
{
  // register with MQTT broker for topics of interest to this node
  Serial.print("Registering for topics...");
  sprintf(sbuf,"%s/ledCommand", clientID);
  psClient.subscribe(sbuf);
  Serial.println(" done");
}

//updatePBStatus func is called every (currently 8) milliseconds
//to update the state of the pushbutton
void updatePBStatus() 
{
  // ledOnPBDebouncer,ledOffPBDebouncer (adjust the time according to PB characteristics)
  ledOnPBDebouncer.update();
  ledOffPBDebouncer.update();

  if(ledOnPBDebouncer.fell()  && !state_on)
  {
    state_on = true;    
  }
  if(ledOffPBDebouncer.fell() && !state_off)
  {
    state_off = true;      
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    // wait for serial connection
    delay(1);
  }

  //critical push_button setup
  //attach a debouncer to each pushbutton and set debounce interval
  ledOnPBDebouncer.attach(PB_ON  ,INPUT_PULLUP);
  ledOffPBDebouncer.attach(PB_OFF,INPUT_PULLUP);

  // Create a timer to periodically call the updatePBStatus routine
  pbTimer.attach_ms(PB_UPDATE_TIME, updatePBStatus);
  
  // setup Wifi connection
  connect_wifi();

  // finally, flash the on-board LED five times to let user know
  // that the NodeMCU board has been initialized and ready to go
  pinMode(D4, OUTPUT);
  for(int i=0; i<5; i++) {
    digitalWrite(D4, 0);  // active low
    delay(20);
    digitalWrite(D4, 1);
    delay(150);
  }
  Serial.println("Network initialization complete");
}

void loop()
{
  // using built in time ISR
  currentMillis = millis();

  //red pushbutton state - task A
  if(state_on)
  {
    //do something
    sendPBtoLED(ledClientID, "on");
    printf("LED on!\r\n");
    state_on = false;
  }
  else if(state_off)
  {
    //do something
    sendPBtoLED(ledClientID, "off");
    printf("LED off\r\n");
    state_off = false;
  }
  
  delay(10);
  psClient.loop();  // call periodically to keep client alive and well
}
