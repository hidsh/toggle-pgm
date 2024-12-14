TARG = toggle-pgm

$(TARG): main.c
	gcc -o $@ $<

clean:
	rm -f *.o $(TARG)
