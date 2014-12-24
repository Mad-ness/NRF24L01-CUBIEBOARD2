all:start testrx printregs
objects = testrx.o pyinclude.o NRF24L01.o gpio_lib.o
objects1 = test24L01.o pyinclude.o NRF24L01.o gpio_lib.o
objects2 = printregs.o pyinclude.o NRF24L01.o gpio_lib.o
start : $(objects1)
	cc -o start $(objects1)
testrx : $(objects)
	cc -o testrx $(objects)
printregs : $(object2)
	cc -o printregs $(objects2)
test24L01.o : NRF24L01.h pyinclude.h gpio_lib.h
NRF24L01.o : NRF24L01.h pyinclude.h gpio_lib.h
testrx.o : NRF24L01.h pyinclude.h gpio_lib.h
printregs.o : NRF24L01.h pyinclude.h gpio_lib.h
pyinclude.o : pyinclude.h gpio_lib.h
gpio_lib.o : gpio_lib.h
.PHONY : clean
clean :
	-rm test24L01.o start testrx  printregs printregs.o $(objects) 
