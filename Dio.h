#ifndef DIO_H_
#define DIO_H_

#include"Macro.h"
#include"types.h"
#include"tm4c123gh6pm.h"

void DIO_Init();
void DIO_WritePin (uint8 cahnnel, uint8 Value);
void DIO_WritePort (uint8 Port ,  uint8 Value);
uint8 DIO_ReadPin (uint8 channel);
uint32 DIO_ReadPort (uint8 Port);

#endif 