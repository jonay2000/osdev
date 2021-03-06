
#include <interrupts.h>
#include <timer.h>
#include <stdio.h>
#include <util.h>

uint64_t tick = 0;

bool timer_callback(registers_t * regs)
{
    UNUSED(regs);
    tick++;
    return true;
}

uint64_t ms_since_bootup() { 
  return tick; 
}


void sleep(uint64_t ms) {
	uint64_t t = tick + ms;
	while(tick < t);
}

bool init_timer() {
	/* Install the function we just wrote */

	interrupt_register_handler(32, timer_callback);
	

	/* Get the PIT value: hardware clock at 1193180 Hz */
	uint32_t divisor = 1193180 / 1000;
	printf("Running at %i Hz\n", divisor);
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) % 0xFF);

	/* Send the command */
	outportb(0x43, 0x36); /* Command port */
	outportb(0x40, low);
	outportb(0x40, high);
	return true;
}