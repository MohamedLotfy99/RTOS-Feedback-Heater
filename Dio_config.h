#ifndef DIO_CFG_H_
#define DIO_CFG_H_

#include"Platform_types.h"
#include"types.h"

typedef enum {input=0, output=1, initialDirection=0} pinDirection;				//Determine direction of each pin and makes its defult value being an input.
typedef enum {GPIO=0, Preipheral=1, initialType=0} pinType;								//Determine the pin Type.
typedef enum {pullDown=0, pullUp=1, initialPullType=1}inputPullType;			//Determine the pin pull type.
typedef enum {Low=0 , High=1, initialOutputValue =0} Value;								//Determine the output value.
typedef enum {Disabled=0 , Enabled=1, initialPortStatus =0} initialized;	//Determine if the port is intialized.
typedef enum {digital=0 , analog=1, initialtype =0} Signal;								//Determine if the pin input analog or digital.
typedef enum {closed=0 , opened=1, initialDrain=0} openDrained;

//Structure for a pin
typedef struct {
  
  uint8 portNumber: 3;
  uint8 pinNumber: 3;
  pinDirection direction: 1;
  pinType Purpose: 1;
  inputPullType Pulltype: 1;
  Value outputValue: 1;
  Signal SignalType: 1;
  openDrained Drained: 1;
} pin_Cfg;

//Structure for port status
typedef struct {
      initialized status: 1;
} Status;

extern pin_Cfg PORT_DIO_cfg[pinNumbers];
extern Status portsStatus[portNumbers];

#endif /* CFG_H_ */