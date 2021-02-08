#ifndef LOWPOWER_H
#define LOWPOWER_H

#define 	WAKEUP_15MS      0
#define 	WAKEUP_30MS      1
#define 	WAKEUP_60MS      2
#define 	WAKEUP_120MS     3
#define 	WAKEUP_250MS     4
#define 	WAKEUP_500MS     5
#define 	WAKEUP_1S        6
#define 	WAKEUP_2S        7
#define 	WAKEUP_4S        8
#define 	WAKEUP_8S        9


void goToSleep(uint8_t wakeup_period);


#endif