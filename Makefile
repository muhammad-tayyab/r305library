all:r305Parser test_exe test_avr 

test_exe:test.c r305Parser
	mingw32-gcc test.c r305Parser.o -g --static -o test.exe

test_avr:test_avr.c r305Parser_avr
	avr-gcc -g -mmcu=atmega32 -Os -c test_avr.c
	avr-gcc -g -mmcu=atmega32 -o test_avr.elf test_avr.o r305Parser.o
	avr-objdump -h -S test_avr.elf > test_avr.lst
	cat test_avr.lst

r305Parser:r305Parser.c
	mingw32-gcc -c -g r305Parser.c

r305Parser_avr:r305Parser.c
	avr-gcc -mmcu=atmega32 -Os -c -g r305Parser.c
	
doc:doxy.Doxyfile
	doxygen doxy.Doxyfile

clean:
	rm *.o *.lst *.elf *.exe

