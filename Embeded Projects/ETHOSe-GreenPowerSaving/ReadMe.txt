    ETHOS-e, the Electronic Temperature & Humidity Observation System.
    Gathers temperature and relative humidity information from
    an Si7021 I2C-based temperature-humidity sensor.
    ETHOS-s gathers updated sensor information during the processor
    awake period of 5 seconds. Afterwhich, ETHOS-e puts the
    ESP8266 microcontroller to deep sleep for 10 seconds.
    ETHOS-e provides temperature and humidity information to 
    remote users interactively via RS232 (PuTTy)