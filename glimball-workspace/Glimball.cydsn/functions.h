/**
* Header file: list all the name and type of the functions that will be written
*/
#include "project.h"
#include "keypad.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

/**
 * Function that make a moving average on angles values
*/
float moving_average();

/**
 * Function that lights all the LEDS
*/
void light_LEDS();


/**
 * Function that turn off all the LEDS
*/
void turn_off_LEDS();

/**
 * Function that fills the vector of the sine (len = lenght of the vector)
*/
void fill_sine(int len);


/**
 * Function that reads the input of the computer and turn the servo
*/
void read_computer(uint8* rxData);


/**
 * Function that switch modes
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_gimball_mode();


/**
 * Function that switch to the test mode
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_test_mode();


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
 * Prints ERROR on the LCD screen and delays for 10 seconds
*/
void error();


/**
 * Function that concerns the reading of the keypad to switch the mode
 * @return 1 if needs to switcht the mode
*/
int react_to_keypad();


/*
* Function that rotates the servo of the given angle right=positive, left=negative
*/
void rotate_servo(int8 step_angle);

void turn_servo(uint8* angle);

/**
 * Test the reaction on the buttons and keyboard
*/
void react();

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
 * What is done in the testing mode
*/
void testing_mode();


/**
 * What is done in the glimball mode
*/
void gimball_mode();


/**
 * Execute the different modes depending on the mode
*/
void modes();

/**
 * Function that reads the accelerometer and modify the angle
 * @param the angle of the platform
*/
void get_angle(uint8* angle);


/**
 * Print the angle on the LCD and communicate it with UART
*/
void print_angle(uint8* angle);

/**
 * Print the string on the LCD at the wanted position
*/
void print_screen(const char8 * string, int row, int column);

/*
    Initialisation de tous les composants de bases
*/
void initialize();