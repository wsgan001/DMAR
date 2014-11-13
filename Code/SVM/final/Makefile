CC = g++
CFLAGS = -Werror -O3

all: convert format arbiter counter remapping

remapping: remapping.cc
	$(CC) $(CFLAGS) remapping.cc -o remapping

arbiter: arbiter.cc
	$(CC) $(CFLAGS) arbiter.cc -o arbiter

convert: convert.cc common.cc common.h
	$(CC) $(CFLAGS) convert.cc common.cc -o convert

format:	format.cc common.cc common.h
	$(CC) $(CFLAGS) format.cc common.cc -o format

counter: counter.cc
	$(CC) $(CFLAGS) counter.cc -o counter

clean:
	rm -f format convert arbiter
	rm -f *.out* *.model* *.train
	rm -f range out
	rm -f counter
