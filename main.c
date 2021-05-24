/* 
// 05/24/2021
// Dan Maher
//
// Simple state machine for a breadboard traffic light.
// Two-way intersection with vehicle sensors, two traffic lights,
// a walk button and walk lights.
//
*/

#include <stdint.h>
#include "TM4C123.h"
	
//PROTOTYPES//
void delayms(int time);
void systemInit(void);

//GLOBALS//
struct State{
	unsigned long StackOut;
	unsigned long WalkOut;
	int time;
	unsigned long next[8];
};

#define GoWest 0
#define WaitWest 1
#define GoSouth 2
#define WaitSouth 3
#define Walk 4
#define DontWalkOn0 5
#define DontWalkOff0 6
#define DontWalkOn1	7
#define DontWalkOff1 8
#define DontWalkOn2 9
#define DontWalkOff2 10


struct State TrafficState[11] = {
	{0x0C, 0x02, 1000, 	{GoWest, GoWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest}},
	{0x14, 0x02, 500, 	{GoSouth, GoSouth, GoSouth, GoSouth, Walk, Walk, Walk, Walk}},
	{0x21, 0x02, 1000, 	{GoSouth, WaitSouth, GoSouth, WaitSouth, WaitSouth, WaitSouth, WaitSouth, WaitSouth}},
	{0x22, 0x02, 500, 	{GoWest, GoWest, GoWest, GoWest, Walk, Walk, Walk, Walk}},
	{0x24, 0x08, 1000, 	{DontWalkOn0, DontWalkOn0, DontWalkOn0, DontWalkOn0, DontWalkOn0, DontWalkOn0, DontWalkOn0, DontWalkOn0}},
	{0x24, 0x02, 200, 	{DontWalkOff0, DontWalkOff0, DontWalkOff0, DontWalkOff0, DontWalkOff0, DontWalkOff0, DontWalkOff0, DontWalkOff0}},
	{0x24, 0x00, 200, 	{DontWalkOn1, DontWalkOn1, DontWalkOn1, DontWalkOn1, DontWalkOn1, DontWalkOn1, DontWalkOn1, DontWalkOn1}},
	{0x24, 0x02, 200, 	{DontWalkOff1, DontWalkOff1, DontWalkOff1, DontWalkOff1, DontWalkOff1, DontWalkOff1, DontWalkOff1, DontWalkOff1}},
	{0x24, 0x00, 200,	 	{DontWalkOn2, DontWalkOn2, DontWalkOn2, DontWalkOn2, DontWalkOn2, DontWalkOn2, DontWalkOn2, DontWalkOn2}},
	{0x24, 0x02, 200, 	{DontWalkOff2, DontWalkOff2, DontWalkOff2, DontWalkOff2, DontWalkOff2, DontWalkOff2, DontWalkOff2, DontWalkOff2}},
	{0x24, 0x00, 200,		{GoWest, GoWest, GoSouth, GoWest, GoWest, GoWest, GoSouth, GoWest}},
};

unsigned long index = 0;
unsigned long input;

//MAIN//
int main(void){
	systemInit();
	
	while(1){
		GPIOB->DATA = TrafficState[index].StackOut;
		GPIOF->DATA = TrafficState[index].WalkOut;
		delayms(TrafficState[index].time);
		input = ~GPIOE->DATA & 0x07 ;
		index = TrafficState[index].next[input];
	}
}

//DELAY FUNCTION//
void delayms(int time){
	volatile int x, y;
	for (x = 0; x < time; x++){
		for (y = 0; y < 7900; y++){}
	}
}

//SYSTEM INITIALIZATION//
// West Red PB5
// West Yellow PB4
// West Green PB3
// South Red PB2
// South Yellow PB1
// South Green PB0
// South Sensor PE1
// West Sensor PE0
// Walk Red PF1
// Walk Green PF3
void	systemInit(){
	SYSCTL->RCC |= 0x02400540;
	SYSCTL->RCGCGPIO |= 0x32;
	
	//PB0-PB2 NORTH/SOUTH LIGHT
	//PB3-PB5 EAST/WEST LIGHT
	GPIOB->DIR |= 0x3F;
	GPIOB->DEN |= 0x3F;
	
	//WALK LIGHT
	GPIOF->DIR |= 0x0A;
	GPIOF->DEN |= 0x0A;
	
	//PE0 EAST/WEST SENSOR
	//PE1 NORTH/SOUTH SENSOR
	//PE2 WALK SENSOR
	GPIOE->DIR &= ~(0x07);
	GPIOE->DEN |= 0x07;
	GPIOE->PUR |= 0x07;
}
	