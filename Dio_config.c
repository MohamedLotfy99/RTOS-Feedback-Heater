#include"Dio_config.h"

//Array containing the status of all ports.
Status portsStatus[portNumbers] = {
  {Enabled}, 	//0  PORTA 
  {Enabled}, 	//1  PORTB  
  {Disabled}, //2  PORTC 
  {Disabled}, //3  PORTD 
  {Disabled}, //4  PORTE 
  {Enabled}   //5  PORTF 
};

//Array holding the values of all the pins
pin_Cfg PORT_DIO_cfg[pinNumbers]= {
	
		{PORTA, PIN0, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//0    PA0
		{PORTA, PIN1, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//1    PA1
		{PORTA, PIN2, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//2    PA2
		{PORTA, PIN3, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//3    PA3
		{PORTA, PIN4, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//4    PA4
		{PORTA, PIN5, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//5    PA5
		{PORTA, PIN6, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//6    PA6
		{PORTA, PIN7, output, GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//7    PA7
		{PORTB, PIN0, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//8    PB0
		{PORTB, PIN1, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//9    PB1
		{PORTB, PIN2, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//10   PB2
		{PORTB, PIN3, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//11   PB3
		{PORTB, PIN4, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//12   PB4
		{PORTB, PIN5, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//13   PB5
		{PORTB, PIN6, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain},														//14   PB6
		{PORTB, PIN7, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//15   PB7
		{PORTC, PIN0, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//16   PC0
		{PORTC, PIN1, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//17   PC1
		{PORTC, PIN2, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//18   PC2
		{PORTC, PIN3, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//19   PC3
		{PORTC, PIN4, input, GPIO, pullUp, initialOutputValue, digital , closed}, 																							//20   PC4
		{PORTC, PIN5, input, GPIO, pullUp, initialOutputValue, digital , closed}, 																							//21   PC5
		{PORTC, PIN6, input, GPIO, pullUp, initialOutputValue, digital , closed}, 																							//22   PC6
		{PORTC, PIN7, input, GPIO, pullUp, initialOutputValue, digital , closed}, 																							//23   PC7
		{PORTD, PIN0, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//24   PD0
		{PORTD, PIN1, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//25   PD1
		{PORTD, PIN2, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//26   PD2
		{PORTD, PIN3, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//27   PD3
		{PORTD, PIN4, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//28   PD4
		{PORTD, PIN5, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//29   PD5
		{PORTD, PIN6, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//30   PD6
		{PORTD, PIN7, initialDirection, initialType, initialPullType, initialOutputValue, initialtype , initialDrain}, 					//31   PD7
		{PORTE, PIN0, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//32   PE0
		{PORTE, PIN1, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//33   PE1
		{PORTE, PIN2, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//34   PE2
		{PORTE, PIN3, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//35   PE3
		{PORTE, PIN4, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain}, 													//36   PE4
		{PORTE, PIN5, output, GPIO, initialPullType, initialOutputValue, initialtype , initialDrain},													 	//37   PE5
		{PORTF, PIN0, input, GPIO, pullUp, initialOutputValue, digital , initialDrain}, 																				//38   PF0 (SW2)
		{PORTF, PIN1, output,GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//39   PF1 (RED Led)
		{PORTF, PIN2, output,GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//40   PF2 (Green Led)
    {PORTF, PIN3, output,GPIO, initialPullType, Low, initialtype , initialDrain}, 																					//41   PF3 (Blue Led)
		{PORTF, PIN4, input, GPIO, pullUp, initialOutputValue, digital , initialDrain}  																				//42   PF4 (SW1)	
};