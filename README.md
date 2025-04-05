# Clock_Robot
This work was created as part of a project in my mechatronics course at Hochschule Wismar, University of Applied Sciences: Technology, Business and Design. It is a stanford arm which uses a magnet to write the current time to a magic table. The microcontroller is the FRDM MCXA153 from NXP. The time is received by a GPS module and passed on to the controller via UART. The motor drivers are the TMC2209, which are installed on a self-developed board. Below is the repository in which the board is explained: [Repo](https://github.com/Fi-schi/StepperDriver-Shield_FRDM-MCXA153)

Explanation of the 5 buttons:

| Button | Description | 
|-------|-------|
| EN | enable all motors  | 
| DIS| disable all motors   | 
| HOME | perform homing and clear the board  |  
| SWITCH1| start the robot   |  
| SWITCH2 |  start the robot   |

The following table explains the colors of the status LED:

| Color | Description | 
|-------|-------|
| red | error in the configuration of the motor drivers  | 
| green| successfully configure the motor drivers  | 
| white | parsing the UART message from the GPS-module  |  
| purple| error during parsing (can't find the time position in the string)   |  
| blue |  error during parsing (wrong number at the time position in the string e.g. all 0 or ,)   |

How to start the robot:
* plug in the power supply
* switch on the power supply (LED should be green)
* push the EN button (caution: the robot starts moving)
* waiting till the automatic homing and cleaning is done
* if necessary press the homing button (cleaning the board again)
* press SWITCH1 or SWITCH2 to start writing

Now the controller will parse the UART message every 5 seconds. When starting for the first time, it may take a few minutes for the GPS module to receive and transmit the correct time. In this case, the LED should always alternate between white and blue until a correct time has been received. Then the robot starts to write the time and the LED should briefly light up white again every 5 seconds. 
