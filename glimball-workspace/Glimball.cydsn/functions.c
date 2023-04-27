/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and include should be done in header file
*/
#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2500


int mode = 0;  // 1 = glimball | 0 = test


void initialize(){
    LCD_Start();
    LCD_ClearDisplay();
    PWM_Init();
    PWM_Start();
    ADC_Start();
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
    mode = 1;
    light_LEDS();
    const char8* var = "Glim";
    print_screen(var);
}

void switch_to_test_mode(){
    mode = 0;
    turn_off_LEDS();
    const char8* var = "Test";
    print_screen(var);
}


void read_SW1(){
    if (SW1_Read()){
        CyDelay(200);
        switch_mode();
    }
}


void switch_mode(){
    if (mode){
        switch_to_test_mode();
    }
    else if (!mode){
        switch_to_glimball_mode();
    }
    else{
        error();
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
    if (value != 'z'){
        CyDelay(200);
        switch (value){
            case '1': switch_mode();
            //case '2': rotate_left();
            //case '3': rotate_right();
        }
    }   
}

void print_screen(const char8 * string){
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString(string);
}


void testing_mode(){ 
    uint32_t potval = 0;
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    potval = MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF); // transférer le max du potentiomètre en min et max du CMP du servo
    PWM_WriteCompare(potval);
    CyDelay(50);
}


void glimball_mode(){
    // TO DO
}


void react(){
    react_to_keypad();
    read_SW1();
}

void modes(){   
   if (!mode){
        testing_mode();
    }
    else{
        glimball_mode();
    }
}