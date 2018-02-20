CC=gcc
Files=prototype.c request.c checkip.c
Progname=dnstest

all: compile run
compile:
	$(CC) $(Files) -o $(Progname)  -lssl
run:
	./$(Progname)
