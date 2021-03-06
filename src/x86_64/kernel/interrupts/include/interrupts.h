
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <intdef.h>
#include <stdio.h>

#define IDTSIZE 286

typedef struct idt_entry{
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
}idt_entry_t;

typedef struct registers{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
}registers_t;
 
idt_entry_t IDT[IDTSIZE];

//TODO add register states
//custom handlers are functions that get executed on interrupt. if succesfull they return true
//else false. if this interrupt was a non fatal exception and false is returned a kernel panic is executed
typedef bool (* interrupthandler)(registers_t * regs);

int load_idt();

void idt_print_entry(uint32_t num);
bool init_idt();


void interrupt_register_handler(uint32_t intid, interrupthandler handler);
void interrupt_try_register_handler(uint32_t intid, interrupthandler handler);
void interrupt_deregister_handler(uint32_t intid);
bool interrupt_check_handler(uint32_t intid);
bool interrupt_exec_handler(uint32_t intid, registers_t * regs);

#endif