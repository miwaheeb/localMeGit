    "Ring-Ring" is a wireless IoT device capable of exchanging Ring-Ring messages
    with other Ring-Ring devices. All messages are transmitted wirelessly via MQTT 
    using the ECE MQTT broker."Ring-Ring" utilizes embedded systems design, 
    hardware interfacing, MQTT messaging protocol, MQTT brokers, JSON payloads, 
    and MQTT-Spy.

more to read inside RingRing


//////////////////////////////////////////////////
//j topics order

topics   = 0
ringing  = 1
beattime = 2

//////////////////////////////////////////////////
//how to communicate with topics

Topic: ece/node12/topics
Data : none

//////////////////////////////////////////////////
//how to communicate with ringring

Topic: ece/node12/ringring
Data : {"srcNode":"node05","dstNode":"node12"}

//////////////////////////////////////////////////
//how to communicate with beattime

Topic: ece/node12/beattime
Data : {"time":"5000"}