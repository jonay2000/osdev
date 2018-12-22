
#include <video.h>
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>

void kmain(void * multiboot_structure){
	UNUSED(multiboot_structure);

	system_init();

	kernel_puts("Hello, world!\n");


}