#include"Dio.h"
#include"Dio_config.h"

#define FourBitMask 0xF
#define UnlockMask 0x4C4F434B

void DIO_Init(){
	
	//Give clock to used ports 
	for (uint8 Port = PORTA; Port < portNumbers; Port++){
		
		if(portsStatus[Port].status == Enabled){
			
			SET_BIT(SYSCTL_RCGCGPIO_R,Port);																														//Wait until intialization
			while(BIT_IS_CLEAR(SYSCTL_PRGPIO_R,Port)){}
		}
	}
	
	//Intialize pins as Dio_config file
	for(uint8 pin = PORT_A_PIN_0 ; (pin < pinNumbers)  ; pin++){
																																													
	  	if  (portsStatus[PORT_DIO_cfg[pin].portNumber].status == Disabled){												//Check if the port is intiallized
				
				continue;
		}
		
	  	if(pin==PORT_F_PIN_0){																																		//Enable PortF Pin0
				
			GPIO_PORTF_LOCK_R = UnlockMask;
			SET_BIT(GPIO_PORTF_CR_R,PORT_DIO_cfg[pin].pinNumber);
		}
			
		else if(pin==PORT_D_PIN_7){																																	//Enable PortF Pin0
			
			GPIO_PORTF_LOCK_R = UnlockMask;
			SET_BIT(GPIO_PORTD_CR_R,PORT_DIO_cfg[pin].pinNumber);
		}
		// Guarding to the jtag pins 
		else if(pin==PORT_C_PIN_0 || pin==PORT_C_PIN_1 || pin==PORT_C_PIN_2 || pin==PORT_C_PIN_3){
		}
		
		//Detect the port of each pin
		switch (PORT_DIO_cfg[pin].portNumber){
			
		//PortA pins
		case PORTA:
			
			if(PORT_DIO_cfg[pin].Purpose == GPIO)
			{
				if(PORT_DIO_cfg[pin].SignalType== digital){
					
					CLEAR_BIT(GPIO_PORTA_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else if (PORT_DIO_cfg[pin].SignalType== analog){
					
					SET_BIT(GPIO_PORTA_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
				}
				GPIO_PORTA_PCTL_R &= ~(FourBitMask<<PORT_DIO_cfg[pin].pinNumber*4);
				CLEAR_BIT(GPIO_PORTA_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
				if(PORT_DIO_cfg[pin].direction==input){
					
					CLEAR_BIT(GPIO_PORTA_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].Pulltype==pullUp){
						
						SET_BIT(GPIO_PORTA_PUR_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
						
						SET_BIT(GPIO_PORTA_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				else if (PORT_DIO_cfg[pin].direction==output){
					
					SET_BIT(GPIO_PORTA_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].outputValue==High){
						
						SET_BIT(GPIO_PORTA_DATA_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].outputValue==Low){
						
						CLEAR_BIT(GPIO_PORTA_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				SET_BIT(GPIO_PORTA_DEN_R,PORT_DIO_cfg[pin].pinNumber);    
				if(PORT_DIO_cfg[pin].Drained == opened){
					SET_BIT(GPIO_PORTA_ODR_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else{
					CLEAR_BIT(GPIO_PORTA_ODR_R,PORT_DIO_cfg[pin].pinNumber);
				}  
			}
			else if (PORT_DIO_cfg[pin].Purpose == Preipheral) {
				// to be completed when needed
			}
			break;

		//Port B 
		case PORTB:
			
			if(PORT_DIO_cfg[pin].Purpose == GPIO){
				
				if(PORT_DIO_cfg[pin].SignalType== digital){
					
					CLEAR_BIT(GPIO_PORTB_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else if (PORT_DIO_cfg[pin].SignalType== analog){
					
					SET_BIT(GPIO_PORTB_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
				}
				GPIO_PORTB_PCTL_R &= ~(FourBitMask<<PORT_DIO_cfg[pin].pinNumber*4);
				CLEAR_BIT(GPIO_PORTB_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
				if(PORT_DIO_cfg[pin].direction==input){
					
					CLEAR_BIT(GPIO_PORTB_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].Pulltype==pullUp){
						
						SET_BIT(GPIO_PORTB_PUR_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
						
						SET_BIT(GPIO_PORTB_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				else if (PORT_DIO_cfg[pin].direction==output){
					
					SET_BIT(GPIO_PORTB_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].outputValue==High){
						
						SET_BIT(GPIO_PORTB_DATA_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].outputValue==Low){
						
						CLEAR_BIT(GPIO_PORTB_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				SET_BIT(GPIO_PORTB_DEN_R,PORT_DIO_cfg[pin].pinNumber);      
			}
			else if (PORT_DIO_cfg[pin].Purpose == Preipheral) {
				// to be completed when needed
			}
			break;

		//PortC
		case PORTC:
         
			if(PORT_DIO_cfg[pin].Purpose == GPIO){
				
				if(PORT_DIO_cfg[pin].SignalType== digital){
					
					CLEAR_BIT(GPIO_PORTC_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else if (PORT_DIO_cfg[pin].SignalType== analog){
					
					SET_BIT(GPIO_PORTC_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
				}
				GPIO_PORTC_PCTL_R &= ~(FourBitMask<<PORT_DIO_cfg[pin].pinNumber*4);
				CLEAR_BIT(GPIO_PORTC_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
				if(PORT_DIO_cfg[pin].direction==input){
					
					CLEAR_BIT(GPIO_PORTC_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].Pulltype==pullUp){
						
						SET_BIT(GPIO_PORTC_PUR_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
						
						SET_BIT(GPIO_PORTC_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				else if (PORT_DIO_cfg[pin].direction==output){
					
					SET_BIT(GPIO_PORTC_DIR_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].outputValue==High){
						
						SET_BIT(GPIO_PORTC_DATA_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if(PORT_DIO_cfg[pin].outputValue==Low){
						
						CLEAR_BIT(GPIO_PORTC_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
					}
				}
				SET_BIT(GPIO_PORTC_DEN_R,PORT_DIO_cfg[pin].pinNumber);
			}
			else if (PORT_DIO_cfg[pin].Purpose == Preipheral){
				
				// to be completed when needed
			}
			break;

			//PortD 
			case PORTD:
				
				if(PORT_DIO_cfg[pin].Purpose == GPIO){
					
					if(PORT_DIO_cfg[pin].SignalType== digital){
						
						CLEAR_BIT(GPIO_PORTD_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if (PORT_DIO_cfg[pin].SignalType== analog){
						
						SET_BIT(GPIO_PORTD_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
					}
					GPIO_PORTD_PCTL_R &= ~(FourBitMask<<PORT_DIO_cfg[pin].pinNumber*4);
					CLEAR_BIT(GPIO_PORTD_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].direction==input){
						
						CLEAR_BIT(GPIO_PORTD_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].Pulltype==pullUp){
							
							SET_BIT(GPIO_PORTD_PUR_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
							
							SET_BIT(GPIO_PORTD_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					else if (PORT_DIO_cfg[pin].direction==output){
						
						SET_BIT(GPIO_PORTD_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].outputValue==High){
							
							SET_BIT(GPIO_PORTD_DATA_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].outputValue==Low){
							
							CLEAR_BIT(GPIO_PORTD_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					SET_BIT(GPIO_PORTD_DEN_R,PORT_DIO_cfg[pin].pinNumber);      
				}
				else if (PORT_DIO_cfg[pin].Purpose == Preipheral) {
					
					// to be completed when needed
				}
				break;

			//PortE 
			case PORTE:
				
				if(PORT_DIO_cfg[pin].Purpose == GPIO){
					
					if(PORT_DIO_cfg[pin].SignalType== digital){
						
						CLEAR_BIT(GPIO_PORTE_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if (PORT_DIO_cfg[pin].SignalType== analog){
						
						SET_BIT(GPIO_PORTE_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
					}
					GPIO_PORTE_PCTL_R &= ~(FourBitMask<<PORT_DIO_cfg[pin].pinNumber*4);
					CLEAR_BIT(GPIO_PORTE_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].direction==input){
						
						CLEAR_BIT(GPIO_PORTE_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].Pulltype==pullUp){
							
							SET_BIT(GPIO_PORTE_PUR_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
							
							SET_BIT(GPIO_PORTE_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					else if (PORT_DIO_cfg[pin].direction==output){
						
						SET_BIT(GPIO_PORTE_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].outputValue==High){
							
							SET_BIT(GPIO_PORTE_DATA_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].outputValue==Low){
							
							CLEAR_BIT(GPIO_PORTE_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					SET_BIT(GPIO_PORTE_DEN_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else if (PORT_DIO_cfg[pin].Purpose == Preipheral){
					
					// to be completed when needed
				}
				break;

			//PortF
			case PORTF:
				
				if(PORT_DIO_cfg[pin].Purpose == GPIO){
					
					if(PORT_DIO_cfg[pin].SignalType== digital){
						
						CLEAR_BIT(GPIO_PORTF_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);
					}
					else if (PORT_DIO_cfg[pin].SignalType== analog){
						
						SET_BIT(GPIO_PORTF_AMSEL_R,PORT_DIO_cfg[pin].pinNumber);  
					}
					GPIO_PORTF_PCTL_R &= ~(FourBitMask<<pin*4);
					CLEAR_BIT(GPIO_PORTF_AFSEL_R,PORT_DIO_cfg[pin].pinNumber);
					if(PORT_DIO_cfg[pin].direction==input){
						
						CLEAR_BIT(GPIO_PORTF_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].Pulltype==pullUp){
							
							SET_BIT(GPIO_PORTF_PUR_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].Pulltype==pullDown){
							
							SET_BIT(GPIO_PORTF_PDR_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					else if (PORT_DIO_cfg[pin].direction==output){
						
						SET_BIT(GPIO_PORTF_DIR_R,PORT_DIO_cfg[pin].pinNumber);
						if(PORT_DIO_cfg[pin].outputValue==High){
							
							SET_BIT(GPIO_PORTF_DATA_R,PORT_DIO_cfg[pin].pinNumber);
						}
						else if(PORT_DIO_cfg[pin].outputValue==Low){
							
							CLEAR_BIT(GPIO_PORTF_DATA_R,PORT_DIO_cfg[pin].pinNumber);        
						}
					}
					SET_BIT(GPIO_PORTF_DEN_R,PORT_DIO_cfg[pin].pinNumber);
				}
				else if (PORT_DIO_cfg[pin].Purpose == Preipheral) {
					
					// to be completed when needed
				}
				break;
		}
	}
}

void DIO_WritePin (uint8 channel, uint8 Value){
	
  switch(PORT_DIO_cfg[channel].portNumber){
		
  case PORTA:
    (Value == HIGH)? (SET_BIT(GPIO_PORTA_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTA_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  case PORTB:
    (Value == HIGH)? (SET_BIT(GPIO_PORTB_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTB_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  case PORTC:
    (Value == HIGH)? (SET_BIT(GPIO_PORTC_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTC_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  case PORTD:
    (Value == HIGH)? (SET_BIT(GPIO_PORTD_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTD_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  case PORTE:
    (Value == HIGH)? (SET_BIT(GPIO_PORTE_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTE_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  case PORTF:
    (Value == HIGH)? (SET_BIT(GPIO_PORTF_DATA_R,PORT_DIO_cfg[channel].pinNumber)) : (CLEAR_BIT(GPIO_PORTF_DATA_R,PORT_DIO_cfg[channel].pinNumber));
    break;
	
  default:
    break;
  }
}

void DIO_WritePort (uint8 Port ,  uint8 Value){
	
  switch(Port){
		
  case PORTA:
    GPIO_PORTA_DATA_R=Value;
    break;
	
  case PORTB:
    GPIO_PORTB_DATA_R=Value;
    break;
	
  case PORTC:
    GPIO_PORTC_DATA_R=Value;
    break;
	
  case PORTD:
    GPIO_PORTD_DATA_R=Value;
    break;
	
  case PORTE:
    GPIO_PORTE_DATA_R=Value;
    break;
	
  case PORTF:
    GPIO_PORTF_DATA_R=Value;
    break;
	
  default:
    break;
  }
}

uint8 DIO_ReadPin (uint8 channel){
	
  uint8 value = 0;
  switch(PORT_DIO_cfg[channel].portNumber){
		
  case PORTA:
    value = GET_BIT(GPIO_PORTA_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  case PORTB:
    value = GET_BIT(GPIO_PORTB_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  case PORTC:
    value = GET_BIT(GPIO_PORTC_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  case PORTD:
    value = GET_BIT(GPIO_PORTD_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  case PORTE:
    value = GET_BIT(GPIO_PORTE_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  case PORTF:
    value = GET_BIT(GPIO_PORTF_DATA_R,PORT_DIO_cfg[channel].pinNumber);
    break;
	
  default:
    break;
  }
	
  return value;
}

uint32 DIO_ReadPort (uint8 Port){
	
  uint32 value = 0;
  switch(Port){
		
  case PORTA:
    value = GPIO_PORTA_DATA_R;
    break;
	
	case PORTB:
    value = GPIO_PORTB_DATA_R;
    break;
  
	case PORTC:
    value = GPIO_PORTC_DATA_R;
    break;
  
	case PORTD:
    value = GPIO_PORTD_DATA_R;
    break;
  
	case PORTE:
    value = GPIO_PORTE_DATA_R;
    break;
  
	case PORTF:
    value = GPIO_PORTF_DATA_R;
    break;
  
	default:
    break;
  }
  
	return value;
}