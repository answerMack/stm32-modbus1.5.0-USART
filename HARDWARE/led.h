 #ifndef __LED_H
 #define __LED_H
 #include "sys.h"



#define LED1 PCout(12)
#define LED2 PDout(2)
#define LED3 PAout(8)
#define erjiguanD PBout(6) 
 
void LED_init(void);
 
 
 
#endif
