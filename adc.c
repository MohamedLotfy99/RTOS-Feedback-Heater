#include "adc.h"
#include "tm4c123gh6pm.h"

void ADC0_init(void){
	
	SYSCTL_RCGCGPIO_R|=(1<<ADC_PORT_ID);										//PORTE clock enable
	while((SYSCTL_PRGPIO_R&(1<<ADC_PORT_ID))==0);

	SYSCTL_RCGCADC_R|=(1<<ADC0_ID);													//ADC0 clock enable
	while((SYSCTL_PRADC_R&(1<<ADC0_ID))==0);
	
	GPIO_PORTE_AFSEL_R |= (1<<AN0_PIN_ID);									// AN0/PE3 alternate function
	GPIO_PORTE_DEN_R &=~ (1<<AN0_PIN_ID);										// Disable digital function of AN0/PE3
	GPIO_PORTE_AMSEL_R |= (1<<AN0_PIN_ID);									// Set AN0/PE3 Analog mode
	ADC0_ACTSS_R &=~ (1<<SS3_ID);														// Deactivate SS3 during initialization
	ADC0_EMUX_R &=~ (0x0f<<EM3_ID);													
	ADC0_SSCTL3_R |= (1<<END0_ID)|(1<<IE0_ID);							// SS3 takes a single sample and raises flag when done
	ADC0_ACTSS_R |= (1<<SS3_ID);														// Reactivate SS3 
}

uint16_t ADC0_readChannel(void){
	uint16_t ADC_value;

	ADC0_PSSI_R|=(1<<SS3_ID);															//Start sampling from SS3
	while((ADC0_RIS_R&(1<<IN3_ID))==0);										//Pull interrupt flag
	
	//Get converted ADC value
	ADC_value = ADC0_SSFIFO3_R;
	ADC0_ISC_R = (1<<IN3_ID);
	
	return ADC_value;
}