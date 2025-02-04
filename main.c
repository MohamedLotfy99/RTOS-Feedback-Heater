#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "TM4C123.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "Dio.h"
#include "lcd.h"
#include "adc.h"
#define Heater_OFF 0
#define Heater_ON  1

char Heater_Status = Heater_OFF;																		//Heater is intialized turned off
char Stored_Temp ;
char Critical_Heat = 30;

SemaphoreHandle_t xBinarySemaphoreActivate_DisplayTemp ;
SemaphoreHandle_t xBinarySemaphoreActivate_TurnOnHeater ;
SemaphoreHandle_t xBinarySemaphoreActivate_TurnOffHeater ;
SemaphoreHandle_t xBinarySemaphoreActivate_ChangeCriticalHeat ;

StackType_t Stack1[configMINIMAL_STACK_SIZE];
StackType_t Stack2[configMINIMAL_STACK_SIZE];
StackType_t Stack3[configMINIMAL_STACK_SIZE];
StackType_t Stack4[configMINIMAL_STACK_SIZE];
StackType_t Stack5[configMINIMAL_STACK_SIZE];

StaticTask_t xTaskBuffer1;
StaticTask_t xTaskBuffer2;
StaticTask_t xTaskBuffer3;
StaticTask_t xTaskBuffer4;
StaticTask_t xTaskBuffer5;

//Prototypes

void init (void);
static void Check_Temp       				(void * pvParameters);
static void Display_Temp     				(void * pvParameters);
static void TurnOn_Heater          	(void * pvParameters);
static void TurnOff_Heater         	(void * pvParameters);
static void Change_Critical_Heat    (void * pvParameters);
void GPIOF_Handler (void);

//end of prototypes

//Some RTOS APIs implementations
//This task should be implemented to give the Idle task a memory to use
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize ){
	
//declared buffers in this functions should be static so that they won`t be erased after the function ends
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

		//pointer to staticTask_t which stores idle state.
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

		//pointer to idle task stack array
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

		//pointer to size of the array mentioned above
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

//this task should be implemented to give the Timer task a memory to use
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize ){
	
//declared buffers in this functions should be static so that they won`t be erased after the function ends
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

		//pointer to staticTask_t which stores idle state.
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    //pointer to idle task stack array
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    //pointer to size of the array mentioned above
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
//End of RTOS APIs implementation

//Printing functions
void outChar(char x){
	
	while ((UART0_FR_R & (1<<5)) != 0);
	UART0_DR_R = x ;
}

void prntStr(char* cArr){
	
	while (*cArr){
		outChar(*(cArr++));
	}
	outChar('\r');
	outChar('\n');
}

char readChar (void){
	
	char c;
	while((UART0->FR & (1<<4)) != 0);
	c = UART0->DR;
	return c;
}

//Function to read Critical Heat (2 digits) from the Putty.
char read2DigitInteger (void){
	
	char d0 = readChar();
	outChar(d0);
	char d1 = readChar();
	outChar(d1);
	outChar('\r');
	outChar('\n');
	return (d0-'0')*10 + (d1-'0');
}

//initialization function
void init(){
	
	SYSCTL_RCGCGPIO_R |= 0x00000020;                                     	// initialize clock
	while((SYSCTL_PRGPIO_R&0x00000020) == 0){}                           	// looping until clock is initiallized
	GPIO_PORTF_LOCK_R = 0x4C4F434B;                                      	// Unlocking PortF Pins
	GPIO_PORTF_CR_R |= (1<<0 | 1<<4);                                    	// unlocking commit register for switch 1 & switch 2 
  GPIO_PORTF_DIR_R |= (1<<1 | 1<<2 | 1<<3);                            	// detrmining the output pins  
	GPIO_PORTF_DIR_R &=~ (1<<0 | 1<<4);                                  	// detrmining the input pins 
	GPIO_PORTF_PUR_R |= (1<<0 | 1<<4);                                   	// configuring the switches with PullUp resistors
  GPIO_PORTF_DEN_R |= (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4);              	// detrmining the pins direction 
	GPIO_PORTF_DATA_R &=~ (1<<1 | 1<<2 | 1<<3);                          	// initially all leds are off
	
	GPIO_PORTF_IS_R  &=~ 1<<4;
	GPIO_PORTF_IBE_R &=~ 1<<4;
	GPIO_PORTF_IEV_R &=~ 1<<4;
	GPIO_PORTF_ICR_R |= 1<<4;
	GPIO_PORTF_IM_R |= 1<<4;
	NVIC_PRI7_R =6 <<21;
	NVIC_EN0_R |= 1<<30;
	__enable_irq();
		
	//UART init
	SYSCTL_RCGCUART_R  |= 1<<0 ;                      										//initiallizing uart0
	SYSCTL_RCGCGPIO_R  |= 1<<0 ;                      										//initiallizing portA pins 
	GPIO_PORTA_AFSEL_R |= (1<<0 | 1<<1);             	 										//choosing the alternative function for pins 0 & 1 (Tx & Rx)
	GPIO_PORTA_PCTL_R  |= (1<<(0*4) | 1<<(1*4));      										//choosing the alternative function to the pin
	GPIO_PORTA_DEN_R   |= (1<<0 | 1<<1);              										//openning digital enable for pins 0 & 1 in PortA
	UART0_CTL_R &=~ 1<<0;     
	
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;
	UART0_LCRH_R = 0x60;
	UART0_CC_R =0x0;
	
	UART0_CTL_R = 0x301;   
}

//PortF ISR Handler
void GPIOF_Handler (void){
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xBinarySemaphoreActivate_ChangeCriticalHeat , &xHigherPriorityTaskWoken);
	GPIO_PORTF_ICR_R |= 1<<4;
	portEND_SWITCHING_ISR (xHigherPriorityTaskWoken);
}

//This function takes the returned digital value from adc.c and converts it to conform with the temperature range (0 to 64) upon which it controls the heater status
static void Check_Temp(void * pvParameters){
	
	for (;;){
		
		uint16_t Input_ADC;
		Input_ADC = ADC0_readChannel(); 																										//Take the value from adc.c and put it in a variable
		uint8_t Measured_Temp = Input_ADC / 32; 																						//Heat conversion (0 to 64)
		if(Measured_Temp != Stored_Temp){																										//Check if the temperature changed
			
			Stored_Temp = Measured_Temp;																											//If changed store the new value
			xSemaphoreGive(xBinarySemaphoreActivate_DisplayTemp);
			if((Stored_Temp < Critical_Heat*2) && (Heater_Status == Heater_OFF)){							//Check to turn heater on
				
				Heater_Status = Heater_ON;																											//Switch flag
				xSemaphoreGive(xBinarySemaphoreActivate_TurnOnHeater);
			}
			else if((Stored_Temp >= Critical_Heat*2) && (Heater_Status == Heater_ON)){				//Check to turn heater off
				
				Heater_Status = Heater_OFF;																											//Switch flag
				xSemaphoreGive(xBinarySemaphoreActivate_TurnOffHeater);
			}	
		}
		
		vTaskDelay(400/portTICK_RATE_MS);
	}
}

//This function prints measured and setpoint temperatures on the LCD
static void Display_Temp (void * pvParameters){
	
	for (;;){
		
		xSemaphoreTake(xBinarySemaphoreActivate_DisplayTemp, portMAX_DELAY);
		LCD_Clear();																																				//Reset the LCD
		LCD_Write_String("Measured: ");
		LCD_Write_ADC_Value_of_Half(Stored_Temp);																						//Print measured temp (stored)
		LCD_Set_Cursor_Position(1,0);																												//Move cursor to second row
		LCD_Write_String("SetPoint: ");
		LCD_Write_Integer(Critical_Heat);																										//Print setpoint temp (user entered)
		
	}
}

//Function to turn off the heater
static void TurnOff_Heater (void * pvParameters){
	
	for (;;){
		
		xSemaphoreTake(xBinarySemaphoreActivate_TurnOffHeater, portMAX_DELAY);
		CLEAR_BIT(GPIO_PORTA_DATA_R,2);																											//Clear A2 pin value
	}
}

//Function to turn on the heater
static void TurnOn_Heater (void * pvParameters){
	
	for (;;){
		
		xSemaphoreTake(xBinarySemaphoreActivate_TurnOnHeater, portMAX_DELAY);
		SET_BIT(GPIO_PORTA_DATA_R,2);																												//Set A2 pin 
	}
}

//This function prints a string on the Putty and takes the setpoint value from the user (using keyboard)
static void Change_Critical_Heat (void * pvParameters){
	
	char c; 
	for(;;){
		
		xSemaphoreTake(xBinarySemaphoreActivate_ChangeCriticalHeat, portMAX_DELAY);
		prntStr("Enter 2 Digits for the new SETPOINT:");																		//String to be printed on the Putty
		Critical_Heat = read2DigitInteger();																								//Call function to take entered value and pass it to a variable

	}
}

//Main Application
int main (void){
	
DIO_Init();
init();
LCD_Init();
LCD_Clear();
ADC0_init();

xTaskCreateStatic(Change_Critical_Heat, (const portCHAR*)"Change Critical Heat", 	configMINIMAL_STACK_SIZE, NULL , 4, Stack5, &xTaskBuffer5);
xTaskCreateStatic(TurnOff_Heater, 			(const portCHAR*)"Turn Off Heater Task", 	configMINIMAL_STACK_SIZE, NULL , 3, Stack3, &xTaskBuffer3);
xTaskCreateStatic(TurnOn_Heater, 				(const portCHAR*)"Turn On Heater Task", 	configMINIMAL_STACK_SIZE, NULL , 3, Stack4, &xTaskBuffer4);
xTaskCreateStatic(Check_Temp, 					(const portCHAR*)"Check Temp Task", 			configMINIMAL_STACK_SIZE, NULL , 2, Stack1, &xTaskBuffer1);	
xTaskCreateStatic(Display_Temp, 				(const portCHAR*)"Display Temp Task", 		configMINIMAL_STACK_SIZE, NULL , 1, Stack2, &xTaskBuffer2);

vSemaphoreCreateBinary(xBinarySemaphoreActivate_ChangeCriticalHeat);
vSemaphoreCreateBinary(xBinarySemaphoreActivate_TurnOffHeater);
vSemaphoreCreateBinary(xBinarySemaphoreActivate_TurnOnHeater);
vSemaphoreCreateBinary(xBinarySemaphoreActivate_DisplayTemp);


vTaskStartScheduler();
while(1){}
}
