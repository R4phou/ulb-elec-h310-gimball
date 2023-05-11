/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and include should be done in header file
*/
#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2500
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define SERVO_ANGLE (MAX_SERVO - MIN_SERVO)/180
#define pi 3.1415
#define MIN_ACC 18800
#define MAX_ACC 27400
#define N_MAX 2000
#define STEP_MIN 10
#define STEP_MAX 100


int mode = 0;  // 1 = glimball | 0 = test
int test_mode = 0; // potentiomètre = 1 | keypad = 0


int iterator = 0;
float sin_wave[N_MAX];
int step = 0;



CY_ISR(isr_sound_handler){
    iterator += step;
    VDAC_SetValue((uint8)(sin_wave[iterator]));
    if (iterator>=N_MAX)iterator=0;
    Timer_ReadStatusRegister();
}

CY_ISR(isr_uart_handler){ // Receive instructions from Computer
    uint8 rxData; // Value readed from computer
    uint8_t status;
    do {
        status = UART_ReadRxStatus();
    if ((status & UART_RX_STS_PAR_ERROR) |
        (status & UART_RX_STS_STOP_ERROR) |
        (status & UART_RX_STS_BREAK) |
        (status & UART_RX_STS_OVERRUN)) {
    // Parity , framing , break or overrun error
        print_screen("UART err", 1, 0);
    }
    if ((status & UART_RX_STS_FIFO_NOTEMPTY)!=0){
        rxData = UART_GetChar();
        UART_PutChar(rxData);
        read_computer(&rxData);
    }
    }while ((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);
}

void read_computer(uint8* data){
    if (!test_mode && !mode){
        if (strcmp((const char *) data, "2") == 0) rotate_left();
        else if(strcmp((const char *) data, "3") == 0) rotate_right();
        else if (strcmp((const char*) data, "1") == 0) switch_mode();
    }
}


void initialize(){
    LCD_Start();
    LCD_ClearDisplay();
    PWM_Init();
    PWM_Start();
    Mux_Start();
    ADC_Start();
    keypadInit();
    Timer_Start();
    VDAC_Start();
    UART_Start();
    isr_uart_StartEx(isr_uart_handler);
    
    fill_sine(N_MAX);
}

void fill_sine(int len){
    for (int i =0; i < len; i++){
        sin_wave[i] = sin(i*2*pi/len)*8 + 8;
    }
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

void switch_to_gimball_mode(){
    mode = 1;
    light_LEDS();
    print_screen("Glim", 0, 0);
}

void switch_to_test_mode(){
    mode = 0;
    turn_off_LEDS();
    print_screen("Test", 0, 0);
}


void read_SW1(){
    if (SW1_Read()){
        CyDelay(200);
        switch_mode();
    }
}

void read_SW2(){
    if (!mode && SW2_Read()){
        CyDelay(200);
        if (test_mode) {
            test_mode = 0;
            print_screen("keypad", 0, 0);
        }
        else if (!test_mode){
            test_mode = 1;
            print_screen("pot", 0, 0);
        } 
    }
}

void switch_mode(){
    mode ? switch_to_test_mode():switch_to_gimball_mode();
}

void error(){
    print_screen("ERROR", 0, 0);
    CyDelay(100000);
}

void activate_sound(){
    ISR_SOUND_StartEx(isr_sound_handler);
}

void rotate_left(){
    uint16 pos = PWM_ReadCompare() - 10*SERVO_ANGLE;
    if (pos < MIN_SERVO) pos = MIN_SERVO;
    PWM_WriteCompare(pos);
    CyDelay(50);
}

void rotate_right(){
    uint16 pos = PWM_ReadCompare() + 10*SERVO_ANGLE;
    if (pos > MAX_SERVO) pos = MAX_SERVO;
    PWM_WriteCompare(pos);
    CyDelay(50);
    
}

void react_to_keypad(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '1'){
        CyDelay(200);
        switch_mode();
        }
    }
}

void print_screen(const char8 * string, int row, int column){
    LCD_ClearDisplay();
    LCD_Position(row,column);
    LCD_PrintString(string);
    UART_PutString(string);
}

void test_pot(){
    uint32_t potval = 0;
    Mux_Select(0);
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    PWM_WriteCompare((uint32_t) MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF));
    CyDelay(50);
}

void test_keyboard(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '2'){
            CyDelay(100);
            rotate_left();
        }
        else if(value == '3'){
            CyDelay(100);
            rotate_right();
        }
    }
}

void testing_mode(){
    read_SW2();
    if (test_mode) test_pot();
    else test_keyboard();
}

void get_angle(uint8* angle){
    // Read the accelerometer
    uint32_t x=0;
    Mux_Select(1); //Selection of the first channel
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) x = ADC_GetResult32();
    
    // Modfify the step (for the sound)
    step = STEP_MIN + (x-MIN_ACC)*(STEP_MAX-STEP_MIN)/(float)(MAX_ACC-MIN_ACC);
    
    // Change the value of the accelerometer into an angle
    *angle = MIN_ANGLE + (x-MIN_ACC)*(MAX_ANGLE-MIN_ANGLE)/(float)(MAX_ACC-MIN_ACC);
    
    // Print the angle on the LCD + UART
    print_angle(angle);
}

void print_angle(uint8* angle){
    LCD_Position(1,0);
    char x_char[12];
    sprintf(x_char, "%.3u", (unsigned int) *angle);
    LCD_PrintString(x_char);
    strcat(x_char, "\n");
    UART_PutString(x_char);
}

void turn_servo(uint8* angle){
    uint32_t servo_value = MIN_SERVO + (*angle-MIN_ANGLE)*(MAX_SERVO-MIN_SERVO)/(float)(MAX_ANGLE-MIN_ANGLE); // à changer
    PWM_WriteCompare(servo_value);
    CyDelay(40);
}

void gimball_mode(uint8* angle){
    turn_servo(angle);   
}

void react(){
    react_to_keypad();
    read_SW1();
}

void modes(){
    uint8 angle = 0;
    get_angle(&angle);
    mode ? gimball_mode(&angle):testing_mode();
}