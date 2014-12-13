#include "stm32f4xx.h"                  // Device header

void timer2_config(int precaler,int autorreload);
void timer2_start(void);
void timer2stop(void);

void led_config(void);
void led_output(int pin_val);

int ledState=0;

int main(){
	led_config();
	led_output(ledState);
	timer2_config(15999,499);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	timer2_start();
	while(1){
	} 
}

void timer2_start(void){
	TIM2->CR1|=(0x1<<0);//CEN 1: counter enabled
}

void timer2stop(void){
	TIM2->CR1&=~(0x1<<0);//CEN 0: counter disabled
}

void timer2_config(int precaler,int autorreload){
	RCC->APB1ENR|=(0x1);//Enable TIM2
	
	//configure timer mode and time base settings
	TIM2->CR1=0;
	TIM2->PSC=precaler;
	TIM2->ARR=autorreload;
	TIM2->CR1|=((0x0<<8)//CKD 00: tds=tcd_int/1
						|(0x0<<7)//ARPE 0: TIMxARR is not buffered
						|(0x0<<5)//CMS Don't care
						|(0x0<<4)//DIR 0:Counter is used upcounter
						|(0x0<<3)//OPM 0:Counter is not stoped at update event
						|(0x1<<2)//URS 1:Only overflows/underflow generate an update interrupt
						|(0x0<<1)//UDIS 0:Update events enabled
						|(0x0<<0)//CEN 0:Counter disabled;
						);
	//configure timer interrupts
	TIM2->DIER|=(0x1<<0);//UIE 1:Update interrupt enabled
}

void led_config(void){
	RCC->AHB1ENR|=(0x1);//Enable GPIOA
	GPIOA->MODER|=(0x1<<10);// Pin_A5 as general purpose output
}

void led_output(int pin_val){
	if(pin_val==0){
		GPIOA->BSRRH=(0x1<<5);// reset Pin_A5
	}else{
		GPIOA->BSRRL=(0x1<<5);// set Pin_A5
	}
}

void TIM2_IRQHandler(void){
	TIM2->SR=~(0x1<<0);// Clear Update interrupt flag (UIF) by writing 0
	ledState=(ledState==0)?1:0;
	led_output(ledState);
}

