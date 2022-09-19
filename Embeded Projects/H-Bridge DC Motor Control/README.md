### This code and pushbuttons control the motor-propeller in the following way:
      Upon power-up or processor reset, the motor inital state is off 
      (i.e.not rotating). Pushbutton_1 cycles the motor between 
      three states: Off, Fast, and Slow (in that order). Pushbutton_2 
      changes the direction of spin. The default direction causes air to 
      flow from the propeller toward the motor. Pressing the direction 
      button whether or not the motor is spinning reverses the direction.
      Pressing both buttons simultaneously stops the motor and reset its 
      direction to the default.

### motorControl contains final working code titled "motorControl"
	-sandbox has test/version control code
	-backup inside motorControl contains diff structures of working code
### both circuit and worked lab procedure are in root dir
	-I designed the circuit in oneNote
	-I have a partail circuit done in draw.io but faced some issues
	with alignmnet and hence did the switch to oneNote
