OBJS=main.o file_opts.o cab.o handle.o
CC=gcc
FLAG= -g
dir=./src

target: parse
parse: $(OBJS)
	$(CC) $(OBJS) $(FLAG) -o parse -lm

files_opt.o: file_opts.c files_opts.h
	$(CC) $(FLAG) -c  file_opts.c 
#-g  -o src/file_opts.o

cab.o: cab.c cab.h
	$(CC) $(FLAG) -c  cab.c 
#-g -o src/cab.o

handle_methods.o: handle.c handle.h 
	$(CC) $(FLAG) -c  handle.c 
#-g -o src/handle.o

main.o: main.c
	$(CC) $(FLAG) -c  main.c 
#-g -o src/main.o -lm


install: 
	cp -f parse ../ 
clean: 
	rm -f *.o
	rm -f parse
	rm -f ../parse