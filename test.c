#include <stdio.h>
#include <stdlib.h>
#include "r305Parser.h"
#include "../c-periphery/src/serial.h"

data_package txPackage;
serial_t *serial;

void printFn(uint8_t* str, uint32_t size) {
	uint32_t var;

	if (serial_flush(serial) != 0){
		printf("serial port flush problem\n");
		return ;
	}

	if (serial_write(serial, str, size)!=size)
	{
		printf("Serial writing problem\n");
		return ;
	}

	printf("Sent: ");
	
	for (var = 0; var < size; ++var) {
		printf("%.2X ", str[var]);
	}
	printf("\n");
}

void scanFn(uint8_t *str, uint32_t size) {
	if (serial_read(serial, str, size, 2000)!=size){
		printf("Read problem\n");
	}
	printf("recvd: ");
	for (int var = 0; var < size; ++var) {
		printf("%2x ",str[var]);
	}
	printf("\n");
}

int main(int argc, char const *argv[]) {
	int i;
	
	serial = serial_new();
    if (serial == NULL) {
		printf("Serial opening problem 1\n");
		return -1;
	}
	if (serial_open(serial, "/dev/ttyAMA0", 57600U) != 0){
		printf("Serial opening problem 2\n");
		return -1;
	}
	
	data_package rxPackage, txPackage;
	uint8_t tmp[] = { COMMAND_GET_RANDOM_CODE, 0xff, 0xff, 0xff, 0xff };
	getBasePackage(&txPackage, COMMAND_PACKAGE, LEN_COMMAND_GET_RANDOM_CODE, tmp);
	printf("Test Pass if all the outputs from below looks the exact same\n");
	for (i = 0; i < txPackage.package_size; i++) {
		printf("%.2X ", txPackage.package_string[i]);
	}
	printf("\n");
	sendPackage(&txPackage, printFn);
	fflush(stdout);
	
	recvPackage(&rxPackage, txPackage.package_size, scanFn);
	printf("\n");
	//sendPackage(&rxPackage, printFn);
	//printf("\n");
	return 0;
}
