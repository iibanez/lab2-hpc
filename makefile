DIRO = ./build
DIRI = ./src
UNAME = $(shell uname -s)

ifeq ($(UNAME), Linux)
	COMPILADOR = gcc
else
	COMPILADOR = gcc-8
endif

all: release

release:
	mkdir -p $(DIRO)
	gcc -o $(DIRO)/mandelbrot $(DIRI)/mandelbrot.c -lm
	$(COMPILADOR) -o $(DIRO)/mandelbrotp $(DIRI)/mandelbrotp.c -fopenmp -lm

clean:
	rm -rf $(DIRO)
