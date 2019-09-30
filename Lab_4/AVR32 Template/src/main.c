#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile int TEST_A_FLAG = 1;
volatile int TEST_B_FLAG = 1;
volatile int TEST_C_FLAG = 1;


__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
	
	gpio_configure_pin (TEST_A, GPIO_DIR_INPUT);
	gpio_configure_pin (TEST_B, GPIO_DIR_INPUT);
	gpio_configure_pin (TEST_C, GPIO_DIR_INPUT);
	gpio_configure_pin (RESPONSE_A, (GPIO_DIR_OUTPUT | GPIO_INIT_HIGH));
	gpio_configure_pin (RESPONSE_B, (GPIO_DIR_OUTPUT | GPIO_INIT_HIGH));
	gpio_configure_pin (RESPONSE_C, (GPIO_DIR_OUTPUT | GPIO_INIT_HIGH));
	gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
	
	
    busy_delay_init(BOARD_OSC0_HZ);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

void test_a(){
	 while(1){
		/*
        gpio_toggle_pin(LED0_GPIO);

        printf("tick\n");
        
        busy_delay_ms(500);
		*/
		while(gpio_get_pin_value(TEST_A)){
			
		}
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
		}
}


__attribute__((__interrupt__)) static void interrupt_J3(void){ 
	
	
	if (gpio_get_pin_interrupt_flag(TEST_A)){
		volatile TEST_A_FLAG = 1;
		gpio_clear_pin_interrupt_flag(TEST_A);
	}
	if (gpio_get_pin_interrupt_flag(TEST_B)){
		volatile TEST_B_FLAG = 1;
		gpio_clear_pin_interrupt_flag(TEST_B);
	}
	if (gpio_get_pin_interrupt_flag(TEST_C)){
		volatile TEST_C_FLAG = 1;
		gpio_clear_pin_interrupt_flag(TEST_C);
	}
}


int main (void){
    init();
	
	while(1){
		if (TEST_A_FLAG){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
			volatile TEST_A_FLAG = 0;
		}
		if (TEST_B_FLAG){
			busy_delay_us(100);
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
			volatile TEST_B_FLAG = 0;
		}
		if (TEST_C_FLAG){
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
			volatile TEST_C_FLAG = 0;
		}
	}
	
}
