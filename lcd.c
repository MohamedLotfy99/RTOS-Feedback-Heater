#include "lcd.h"

//Prototypes
uint8_t LCD_Cursor = 0x80;
void LCD_Handle_Cursor();
void LCD_Write_Command(char cmd);
void LCD_Delay();
//End of Prototypes


void delay_m(int n){
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<3180;j++){
	}
}

void delay_u(int n){
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<3;j++){
	}
}

void LCD_Delay() {	
	//Sent commands need to have a delay time to make sure that a command has finished.
	delay_m(2);
} 

//Initialize lcd
void LCD_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x22;            														//Set clock for ports A & B
	
	//Port B pins intialization for the LCD data pins.
	CTRL_PORT_AFSEL=0; 
	CTRL_PORT_PCTL=0;
	CTRL_PORT_AMSEL=0;
	DATA_PORT_DIR=0xFF;               																//Make all pins direction be output D0->D7
	DATA_PORT_DEN=0xFF;               																//All output values are digital

	//Port A pins intialization for the LCD control pins.
	DATA_PORT_AFSEL=0;
	DATA_PORT_PCTL=0;
	DATA_PORT_AMSEL=0;
	CTRL_PORT_DIR |= (1<<RS) | (1<<RW) | (1<<E) ;                 		//Use last 3 pins as output 
	CTRL_PORT_DEN |= (1<<RS) | (1<<RW) | (1<<E) ;
	
	//Delay to make sure all pins intialized
	delay_m(40);
	LCD_Write_Command(TWO_LINE_LCD_Eight_BIT_MODE);           				//Command to Intialize two rows in the LCD   
	delay_u(50);
	LCD_Write_Command(CURSOR_ON);           													//Command to Display cursor
	delay_u(50);
	LCD_Write_Command(CLEAR_COMMAND);           											//Command to Clear LCD
	delay_m(2);
	LCD_Delay();																											//Delay to make sure everything is set (without it 1st char is missing)
	delay_m(2);

	LCD_Write_Command(LCD_ON); 																				//Command to make cursor display
	LCD_Write_Command(RETURN_HOME); 																	//Command to make cursor point at first char (top left)
}

//Write command
void LCD_Write_Command(char cmd){
	CTRL_PORT &= ~(1<<RS);         																		//Control pins set to 0 for command
	CTRL_PORT &= ~(1<<RW);
	DATA_PORT = cmd;
	CTRL_PORT |= (1<<E);        	 																		//E=1
	delay_m(2);
	CTRL_PORT &= ~(1<<E);          																		//E=0
}

//Write charachter
void LCD_Write_Char(char data){

	LCD_Handle_Cursor();																							//Update cursor to keep track of new position 		
	CTRL_PORT |= (1<<RS);          																		//Set rs to 1 to write data
	CTRL_PORT &= ~(1<<RW);
	DATA_PORT = data;
	CTRL_PORT |= (1<<E);      	   																		//E=1
	delay_m(2);
	CTRL_PORT &= ~(1<<E);          																		//E=0
}

//Write integer
void LCD_Write_Integer(int x){
	
	char mySize = 0;
	char Str[32];
	if (x == 0){
		
		LCD_Write_Char('0');
	}
	//print digits from array
	else{
		while (x != 0){
			
			Str[mySize]=x%10;
			x=x/10;
			mySize++;
		}
	
		for (int i=mySize-1;i>=0;i--){
			
			LCD_Write_Char(48+Str[i]);
		}
	}
}

void LCD_Write_ADC_Value_of_Half(int data){
	
	if (data % 2 == 0){
		
		LCD_Write_Integer(data/2);
		LCD_Write_Char('.');
		LCD_Write_Char('0');
	}		
	else{
		
		LCD_Write_Integer(data/2);
		LCD_Write_Char('.');
		LCD_Write_Char('5');
	}
}

//Write a string
void LCD_Write_String(char* string){
	
	uint8_t i = 0;
	while ( string[i] != '\0' )																					//loop the string untill NULL is reached
	{		
		LCD_Write_Char(string[i] );
		i++ ;
		delay_m(2);
	}	
}

//Clear everything
void LCD_Clear(){
	
	LCD_Write_Command(CLEAR_COMMAND); 																	//Command to clear the screen
	LCD_Delay(); 																												
	LCD_Write_Command(RETURN_HOME);
	LCD_Delay();
	LCD_Cursor = 0x80;
}

//Move cursor to a specific position
void LCD_Set_Cursor_Position(uint8_t row, uint8_t col ){
	
	uint8_t Address ;
	if (row == 0)
		Address = 0x80 + col; 																						//Set the address of first line to be 0x80, address of the seconed element in the first line is 0x81 and so on
	else if (row == 1)			
		Address = 0xc0 + col; 																						//Set the address of second line to be C0 is address of seconed line
	
	LCD_Cursor = Address;
	
	if( (row != 0 && row != 1 ) || col < 0 || col > 15 ){ 							//If user tried to use more than 2rows or enter a value that takes more than 15 columns
		
		LCD_Clear();
		LCD_Write_String("ERROR");
		LCD_Blink();
	}
	
	LCD_Write_Command(Address); 																				//Command to set the cursor position at the given address
	LCD_Delay();

}

//write charachter in a specific position
void LCD_Write_Char_Position(uint8_t row, uint8_t col , char c){
	
	LCD_Set_Cursor_Position(row,col);
	LCD_Write_Char(c);
}

//write String in a specific position
void LCD_Write_String_Position( uint8_t row, uint8_t col , char* string){
	
	LCD_Set_Cursor_Position(row,col);
	LCD_Write_String(string);
}

void LCD_Shift_Cursor_Left(){
	
	if(LCD_Cursor == 0xc0 ){ 																				//If the cursor on the first index of the seconed line

		LCD_Cursor = 0x8F;																						//Last index in the first line
		LCD_Write_Command(LCD_Cursor); 																//Set the cursor position with the LCD_Cursor variable
	}
																																	//Normal case 
	else{
		
		LCD_Cursor --;
		LCD_Write_Command(LCD_Cursor);
	}	
	
}
void LCD_Shift_Cursor_Right(){
	
	if(LCD_Cursor == 0x8F ){ 																				//If the cursor on the last index of the first line

		LCD_Cursor = 0xc0;																						//First index in the seconed line
		LCD_Write_Command(LCD_Cursor);
	}
	else
	{
		LCD_Cursor ++;
		LCD_Write_Command(LCD_Cursor);
	}
}

//Function to delete
void LCD_Back(){

	LCD_Shift_Cursor_Left(); 																				//The current cursor is on the position after the last char, so shift left to get it
	LCD_Write_Char(' ');		 																				//White space for delete but cursor will move right by one
	LCD_Shift_Cursor_Left(); 																				//Shift the cursor left
}

//Function to Blink the LCD
void LCD_Blink(){

	for(int i = 0 ; i < 3 ; i ++)
	{
		delay_m(BLINK_TIME_ms);
		LCD_Write_Command(LCD_OFF);
		delay_m(BLINK_TIME_ms);
		LCD_Write_Command(LCD_ON);
	}
	LCD_Write_Command(LCD_ON);
}
//Move Cursor Up
void LCD_Shift_Cursor_Up(){
	
	uint8_t cursor_position = LCD_Cursor;
	if(cursor_position >= 0x80 && cursor_position <= 0x8F ) 				//Check if cursor at first row
		return;
	else if (cursor_position >= 0xc0 && cursor_position <= 0xcF) 		//Check if cursor in second row, if so get the difference
		
		LCD_Write_Command(cursor_position - 0x40);
	
}
//Move Cursor Down
void LCD_Shift_Cursor_Down(){
	
	uint8_t cursor_position = LCD_Cursor;
	
	if(cursor_position >= 0x80 && cursor_position <= 0x8F )  				//Check if cursor in first row, if so get the difference
		LCD_Write_Command(cursor_position + 0x40); 										//1st address = 0x80, 2nd address = 0xc0 ===> 0xc0 - 0x80 = 0x40
	
	else if (cursor_position >= 0xc0 && cursor_position <= 0xcF) 		//Check if cursor at 2nd row
		return;
}

//Update Cursor
void LCD_Handle_Cursor(){
	
	if(LCD_Cursor > 0x8F && LCD_Cursor < 0xc0 ){ 										//Check if cursor at the last index.

		LCD_Cursor = 0xc0;
		LCD_Write_Command(LCD_Cursor);
	}
	LCD_Cursor ++; 																									//Update anyway
} 