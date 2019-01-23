all : card.c
	gcc -o card card.c -L/usr/local/lib/ -lhiredis
	./card
clean:
	rm -f card
