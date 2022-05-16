how to use!

include 
#include "timeInterrupt_func.h" //timeInterrupt_func.h file

declare 
void updateSensors_func(); //time interrupt setup

global vars:
extern int token;

check out C:\Users\mina_\Desktop\localMe\localMeGit\Aquarium\floatVlaveWscreen\valveWScreen
^^ for implementation

funcs:
call updateSensors_func(); //initialize and call time interrupt SR from void (from void loop)

returns token value of 1 when interval time is up, else token = 0