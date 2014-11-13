all: counter sample convert format remapping arbiter

sample: public/sample.c
	gcc -O3 -I./public public/sample.c -o sample -lm

arbiter: arbiter.cc
	g++ -O3 arbiter.cc -o arbiter

counter: counter.cc
	g++ -O3 counter.cc -o counter

convert: convert.cc common.cc common.h
	g++ -O3 convert.cc common.cc -o convert

remapping: remapping.cc
	g++ -O3 remapping.cc -o remapping

format:	format.cc common.cc common.h
	g++ -O3 format.cc common.cc -o format

clean:
	rm -f sample counter format convert
