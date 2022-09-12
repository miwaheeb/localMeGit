// This file accompanies the mqtt_btnNode_starter_code.ino v2.1 program
// system defines

// uncomment for Lipscomb broker, comment out for "brokerX"
#define LIPSCOMB

// Network and MQTT broker credentials
#ifdef LIPSCOMB
  const char* mqttBroker = "10.51.97.101";  // ECE mosquitto server (wlan0)
  const char* ssid = "LipscombGuest";       // no PW needed for Lipscomb guest wifi
#else
  const char* mqttBroker = "1.1.1.1";       // address of brokerX
  const char* ssid = "ssid";                // ssid of brokerX's network
  const char* password = "password";        // password for brokerX's network
#endif    

int mqttPort = 1883;

// Client ID of this LED controller
const char* clientID = "node12";       // Change 05 to your two-digit ID

// Client ID of the remote LED node
//const String clientID = "node12";   // Change 05 to your two-digit ID

////////////////////////////////////////////////////////////////////////////////////
//system functions

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

/*----------------------------------------------------------------------*/
