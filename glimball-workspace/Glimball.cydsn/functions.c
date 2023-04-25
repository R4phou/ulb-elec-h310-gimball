/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and include should be done in header file
*/
#include "functions.h"

int test_mode = 1;  // 1 = true | 0 = false
int glimbal_mode = 0; // 1 = true | 0 = false


void initialize(){
    LCD_Start();
    LCD_ClearDisplay();
    keypadInit();
}


void light_LEDS(){
    LED1_Write(1);
    LED2_Write(1);
    LED3_Write(1),
    LED4_Write(1);
}

void turn_off_LEDS(){
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0),
    LED4_Write(0);
}

void switch_to_glimball_mode(){
    glimbal_mode = 1;
    test_mode = 0;
    light_LEDS();
}

void switch_to_test_mode(){
    glimbal_mode = 0;
    test_mode = 1;
    turn_off_LEDS();
}


void switch_mode(){
    if (SW1_Read()){
        if (!glimbal_mode && test_mode){
            switch_to_glimball_mode();
        }
        else if (glimbal_mode && !test_mode){
            switch_to_test_mode();
        }
        else{
            error();
        }
    }
}

void error(){
    LCD_Position(0,0);
    LCD_PrintString("ERROR");
    CyDelay(100000);
}

void rotate_left();

void rotate_right();


void react_to_keypad(){
    uint8_t value = keypadScan();
    switch (value){
        case 1: switch_mode();
        case 2: rotate_left();
        case 3: rotate_right();
    }
}