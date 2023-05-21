/**
* Header file: list all the name and type of the functions that will be written
*/

#include "project.h"
#include "keypad.h"
#include "math.h"
#include "stdio.h"
#include "string.h"


/*
 * Initialisation de tous les composants de bases
*/
void initialize();


/**
 * Function that lights all the LEDS
*/
void light_LEDS();


/**
 * Function that turn off all the LEDS
*/
void turn_off_LEDS();


/*
 * Function that rotates the servo of the given angle right=positive, left=negative
 * @param int8 step_angle value of the angle (delta) that the servo will need to rotate
*/
void rotate_servo(int8 step_angle);



/**
 * Function that fills the vector of the sine (len = lenght of the vector)
*/
void fill_sine(int len);

/**
 * Print the string on the LCD at the wanted position
*/
void print_screen(const char8 * string, int row, int column);


/**
 * Function that reads the input of the computer and turn the servo
*/
void read_computer(uint8* rxData);


/**
 * Function that reads the accelerometer and modify the angle
 * @param the angle of the platform
*/
void get_angle(uint8* angle);


/**
 * Function that make a moving average on angles values
 * @return the average of precedent positions
*/
uint8 moving_average();


/**
 * Print the angle on the LCD and communicate it with UART
*/
void print_angle(uint8* angle);


/**
 * Controller of the testing mode
 * Decides what type of mode is used and call them
*/
void testing_mode();


/**
 * Control the servo with the potentiometer
*/
void test_pot();

/**
 * Control the servo with the keyboard
*/
void test_keyboard();

/*
* Control the servo with joystick
*/
void test_joystick();


/**
 * Gimball mode, receive the value of the position which the servo need to be at and makes the rotation
 * @param uint8* angle pointer of the value of the position which the servo will need to be at the end
*/
void gimbal_mode(uint8* angle);


/**
 * Function that switch modes
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_gimbal_mode();


/**
 * Function that switch to the test mode
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_test_mode();


/**
 * Test the reaction on the buttons and keyboard
*/
void react();


/**
 * Function that concerns the reading of the keypad to switch the mode
 * @return 1 if needs to switch the mode
*/
int react_to_keypad();


/**
 * Function that reads the button SW1
 * @return 1 if the button is pressed
*/
int read_SW1();


/**
 * Function that reads the button 2 and change the testing mode 
*/
void read_SW2();


/**
 * Switch the mode
*/
void switch_mode();


/**
 * Execute the different modes depending on the mode
 * @param uint8* angle, pointer of the value that will be modified by the reading of the accelerometer
*/
void modes(uint8* angle);