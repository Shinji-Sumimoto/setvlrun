all: setvlrun

setvlrun: setvlrun.c
	gcc -O2 setvlrun.c -o setvlrun

clean:
	-rm setvlrun
