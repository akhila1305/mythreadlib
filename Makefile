PROGRAMS=thread_clone 
all: $(PROGRAMS)

thread_clone: thread1.o mainthreads1.o
	$(CC) $(LDFLAGS) thread1.o mainthreads1.o -o thread_clone 
	
thread1.o: libthread1.h
mainthreads1.o: libthread1.h

clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true
debug: clean
	make "CC=gcc -g -Wall -pedantic -DLF_DEBUG"
