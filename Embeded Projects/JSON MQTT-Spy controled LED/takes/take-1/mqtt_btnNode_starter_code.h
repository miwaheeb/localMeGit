// This file accompanies the mqtt_btnNode_starter_code.ino v2.1 program

// system defines
#define DEBOUNCE_INTERVAL    5  // 5mS works well for circuit-mount PBs
#define PB_UPDATE_TIME       8  // number of mS between button status checks
#define PB_ON               D5  // pin connected to led "on" switch
#define PB_OFF              D6  // pin connected to led "off" switch

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
const char* clientID = "btnNode05";       // Change 05 to your two-digit ID

// Client ID of the remote LED node
const String ledClientID = "ledNode05";   // Change 05 to your two-digit ID
