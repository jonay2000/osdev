
#include <serial.h>
#include <IO.h>
#include <stdio.h>
#include <util.h>

#define PORT 0x3f8 /* COM1 */

bool init_serial() {
	outportb(PORT + 1, 0x00); // Disable all interrupts
	outportb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
	outportb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
	outportb(PORT + 1, 0x00); //                  (hi byte)
	outportb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
	outportb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	outportb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    return true;
}

int serial_received() { return inportb(PORT + 5) & 1; }

char read_serial() {
	while(serial_received() == 0);

	return inportb(PORT);
}

int is_transmit_empty() { return inportb(PORT + 5) & 0x20; }

void write_serial(char a) {
	while(is_transmit_empty() == 0)
		;

	outportb(PORT, a);
}

void write_serial_str(char* s) {
	while(*s != 0) {
		write_serial(*s);
        s++;
	}
}


void printf_serial(char * fmt, ...){
	va_list valist;
	va_start(valist, fmt);

	uint32_t length = vsprintf(NULL, fmt, valist);
	char res[length];
	vsprintf(res, fmt, valist);

	write_serial_str(res);

	va_end(valist);
}

//TODO seriallog (LOG macro) __file__ not working fsr
void seriallog(uint32_t line, char * file, char * fmt, ...){

	char res[100];
	itoa(line,res,10);

	write_serial_str(file);
	write_serial_str(" at ");
	write_serial_str(res);
	write_serial_str(": ");

	va_list valist;
	va_start(valist, fmt);

	uint32_t length = vsprintf(NULL, fmt, valist);
	char res1[length];
	vsprintf(res1, fmt, valist);

	write_serial_str(res1);

	write_serial_str("\n");

	va_end(valist);
}


