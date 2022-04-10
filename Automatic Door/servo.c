#include <servo.h>

void SetServoPosition(uint32_t angle){
	uint32_t range = SERVO_MAX_ANGLE - SERVO_MIN_ANGLE;      // opseg PWM signala 3000-400=2600
	uint32_t unit = range / 180;                             //PWM vrijednost za 1 stepen ugla 2600/180=14,444

	uint32_t step = unit * angle;                           //PWM vrijednost za trazeni ugao

	if(step == 0)
		step = SERVO_MIN_ANGLE;                       

	if(step > SERVO_MAX_ANGLE)
		step = SERVO_MAX_ANGLE;

	
	servoPulse(step);
	
}

//setup PWM and PB6 pi, initial value ServoMinAngle
 void pwm(uint32_t period, uint16_t psc){
 initPins();                              //inicijalizacija pina PB6
 
 //Setup Timer defaults
 //reset DIR bit(Direction),CMS bit (Center aligned mode selection), CKD bit (clock division)
 TIM4->CR1&=~(TIM_CR1_DIR|TIM_CR1_CMS);
 TIM4->CR1|=0x0000;
 TIM4->CR1&=~(TIM_CR1_CKD);
 TIM4->CR1|=0x0000;
 // Control register 1 (CR1) CMS bits (00 -edge aligned mode), DIR bits (0- counter used as upcounter), CKD bits (00).
 TIM4->ARR=period; 
 TIM4->PSC=psc;
 
 TIM4->EGR|=0X0001; //set UG bit in EGR ,update event to reload prescaler
 
 //timer for pwm kanal 1
 //reset polarity level
 TIM4-> CCER &= (uint16_t)~TIM_CCER_CC1P;
 TIM4->CCER |= 0x0000; //OC1 active high
 //reset CC1E bit, disable the channel 1 
 TIM4->CCER &= (uint16_t)~TIM_CCER_CC1E;
 TIM4->CCER|=0x0001; //CC1 channel configured as active output
 //reset ccmr, reset OC1M (output compare 1 mode)
 TIM4->CCMR1 &= (uint16_t)~TIM_CCMR1_OC1M;
 TIM4->CCMR1 &= (uint16_t)~TIM_CCMR1_CC1S; //CC1 channel is configured as output
 TIM4->CCMR1 |= 0x0060;  //(110) PWM mode 1

 TIM4->CCR1 =0; //pulse
 
 //oc1 preload config
 //reset OC1PE bit
 TIM4-> CCMR1 &= (uint16_t) ~ TIM_CCMR1_OC1PE;
 
 TIM4-> CCMR1 |= 0x0008; // preload register on TIM4_CCR1 enable
 
 TIM4->CR1|=TIM_CR1_ARPE; //enable auto-reload bit 
 
 //start timer
 
 TIM4->CR1|= TIM_CR1_CEN;
 }
 
 void servoPulse(uint32_t pulse){
 
 //timer for pwm kanal 1
 //reset polarity level
 TIM4-> CCER &= (uint16_t)~TIM_CCER_CC1P;
 TIM4->CCER |= 0x0000;
 //reset CC1E bit, disable the channel 1 
 TIM4->CCER &= (uint16_t)~TIM_CCER_CC1E;
 TIM4->CCER|=0x0001;;
 //reset ccmr
 TIM4->CCMR1 &= (uint16_t)~TIM_CCMR1_OC1M;
 TIM4->CCMR1 &= (uint16_t)~TIM_CCMR1_CC1S;
 TIM4->CCMR1 |= 0x0060;

 TIM4->CCR1 =pulse; //pulse
 
 //oc1 preload config
 //reset OC1PE bit
 TIM4-> CCMR1 &= (uint16_t) ~ TIM_CCMR1_OC1PE;
 
 TIM4-> CCMR1 |= 0x0008;
 
 } 
 

	  

