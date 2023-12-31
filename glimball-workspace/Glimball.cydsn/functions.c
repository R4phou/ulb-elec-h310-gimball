/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and includes should be done in matching header file
*/

#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2500
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define SERVO_ANGLE (MAX_SERVO - MIN_SERVO)/(MAX_ANGLE - MIN_ANGLE)
#define pi 3.1415
#define MIN_ACC 18500
#define MAX_ACC 27500
#define REF_ACC (MAX_ACC+MIN_ACC)/2
#define N 100
#define DELAY 5
#define WINDOW 8

/* Global variables */

int mode = 1;  // test = 0 | gimbal = 1
int test_mode = 0; // keypad = 0 | Potentiometer = 1 | joystick = 2

int iterator = 0;
float sin_wave[N];

uint8 angle_buffer[10] = {0}; // Saving data for moving average
int buffer_index = 0;



/*                      Interruptions handlers               */

CY_ISR(isr_sound_handler){ // Handle the sound
    iterator++;
    VDAC_SetValue((uint8)(sin_wave[iterator]));
    if (iterator>=N)iterator=0;
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
    ISR_SOUND_StartEx(isr_sound_handler);
    fill_sine(N);
    print_screen("Gimbal: ", 0, 0);
}

/*          Useful functions                                */

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

void rotate_servo(int8 step_angle){
    uint16 pos = PWM_ReadCompare() + step_angle*SERVO_ANGLE;
    if (pos > MAX_SERVO) pos = MAX_SERVO;
    if (pos < MIN_SERVO) pos = MIN_SERVO;
    PWM_WriteCompare(pos);
    CyDelay(DELAY);
}

void fill_sine(int len){
    for (int i =0; i < len; i++){
        sin_wave[i] = sin(i*2*pi/len)*64 + 64;
    }
}

void print_screen(const char8 * string, int row, int column){
    LCD_ClearDisplay();
    LCD_Position(row,column);
    LCD_PrintString(string);
    UART_PutString(string);
}

void read_computer(uint8* data){
    if (*data == 't') switch_mode();
    if (!mode && !test_mode){
        if (*data == 'l') rotate_servo(-10);
        else if(*data == 'r') rotate_servo(10);
    }
}

/*          Accelerometer information to angle              */
void get_angle(uint8* angle){
    // Read the accelerometer
    uint32_t x=0;
    Mux_Select(1); //Selection of the first channel
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) x = ADC_GetResult32();
    ADC_StopConvert();
    x = (x > MAX_ACC) ? MAX_ACC : ((x < MIN_ACC) ? MIN_ACC : x);

    // Modify the period of the timer (for the sound)
    Timer_WritePeriod(450+(x-MIN_ACC)*(2000)/(float)10000);
    
    // Change the value of the accelerometer into an angle using the moving average
    angle_buffer[buffer_index] =  MIN_ANGLE + (x-MIN_ACC)*(MAX_ANGLE-MIN_ANGLE)/(float)(MAX_ACC-MIN_ACC);
    *angle = moving_average();
    buffer_index = (buffer_index + 1) % WINDOW;
    
    // Send the value of the accelerometer to the UART
    char x_char[20];
    sprintf(x_char, "acc %.3u\n", (unsigned int) x);
    UART_PutString(x_char);
    
    // Print the angle on the LCD + UART
    print_angle(angle);
}

uint8 moving_average() {
    uint8 sum = 0;
    for (int i = 0; i < WINDOW; i++) {
        sum += angle_buffer[i];
    }
    return (uint8)(sum / WINDOW);
}

void print_angle(uint8* angle){
    LCD_Position(1,0);
    char x_char[12];
    sprintf(x_char, "%.3d", *angle);
    LCD_PrintString(x_char);
    strcat(x_char, "\n");
    UART_PutString(x_char);
}

/*          Test mode               */

void testing_mode(){
    read_SW2();
    switch (test_mode) {
    case 0:
        test_keyboard();
        break;
    case 1:
        test_pot();
        break;
    case 2:
        test_joystick();
        break;
    }
}

void test_pot(){
    uint32_t potval = 0;
    Mux_Select(0);
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    ADC_StopConvert();
    PWM_WriteCompare((uint32_t) MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF)); // Divide range of servo by range of pot
    CyDelay(DELAY);
}

void test_keyboard(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '2'){
            CyDelay(100); // For the sensibility of the button
            rotate_servo(-10);
        }
        else if(value == '3'){
            CyDelay(100); // For the sensibility of the button
            rotate_servo(10);
        }
    }
}

void test_joystick(){
    int32_t joyval = 0;
    Mux_Select(2);
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        int32_t result = ADC_GetResult32();
        if (result > joyval) rotate_servo(1);
        else if (result < joyval) rotate_servo(-1);
        joyval = result;
    }
    ADC_StopConvert();
    PWM_WriteCompare((uint32_t) MIN_SERVO + (joyval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF));
    CyDelay(DELAY);
}


/*          Gimbal mode               */

void gimbal_mode(uint8* angle){
    PWM_WriteCompare(MIN_SERVO + (*angle-MIN_ANGLE)*(MAX_SERVO-MIN_SERVO)/(float)(MAX_ANGLE-MIN_ANGLE)); 
    // Don't need delay here thanks to the moving average 
}


/*          Reaction to keypad and buttons                */

void react(){
    if(read_SW1() || react_to_keypad()) switch_mode();
}

int react_to_keypad(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '1'){
        CyDelay(200); // For the sensibility of the button
        return 1;
        }
    }
    return 0;
}

int read_SW1(){
    if (SW1_Read()){
        CyDelay(200); // For the sensibility of the button
        return 1;
    }
    return 0;
}

void read_SW2(){
    if (SW2_Read()){
        CyDelay(200); // For the sensibility of the button
        if (test_mode == 2) {
            test_mode = 0;
            print_screen("keypad ", 0, 0);
        }
        else if (test_mode == 0){
            test_mode = 1;
            print_screen("pot ", 0, 0);
        }
        else if (test_mode==1){
            test_mode = 2;
            print_screen("joystick ", 0, 0);
        }
    }
}


/*          Determination of the mode                */

void switch_to_gimbal_mode(){
    mode = 1;
    light_LEDS();
    print_screen("Gimbal: ", 0, 0);
}

void switch_to_test_mode(){
    mode = 0;
    turn_off_LEDS();
    print_screen("Test: ", 0, 0);
}

void switch_mode(){
    mode ? switch_to_test_mode():switch_to_gimbal_mode();
}

void modes(uint8* angle){
    get_angle(angle);
    mode ? gimbal_mode(angle):testing_mode();
}