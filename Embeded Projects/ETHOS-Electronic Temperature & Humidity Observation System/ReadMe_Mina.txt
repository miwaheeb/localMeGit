ETHOS, the Electronic Temperature & Humidity Observation System.
    Gathers temperature and relative humidity information from
    an Si7021 I2C-based temperature-humidity sensor and continuously 
    displays the results on an I2C-based 1602A 16x2LCD display.
    ETHOS gathers updated sensor information every 5s interval.
    ETHOS is also capable of providing temperature and humidity 
    information to remote users interactively via RS232 (PuTTy)

------------------main submissions------------------
ETHOS folder           : contains ETHOS project submition of working code

ETHOS_Circuit.asc      : ETHOS Circuit design drawing LTSpice file
ETHOS_Circuit.pdf      : ETHOS Circuit PDF drawing 

--------------what else is in root dir--------------
Code_Itterations folder: contains different itterations of ETHOS version builds each 
		       : with their own read-me progress/bug fixes
I2C_scanner folder     : I2C address scanner
screen_1test folder    : contains inital testing of LCD screen
sensor_test folder     : contains inital testing of sensors