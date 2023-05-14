CC=gcc
CCFLAGS=-std=c11 -g -pedantic -Wall -Wvla -Werror -D_POSIX_C_SOURCE -D_DEFAULT_SOURCE
ALL=controller zombie labo 

all: $(ALL)

controller: controller.o
	$(CC) $(CCFLAGS) -o controller controller.o utils_v2.o network.o

controller.o: controller.c utils_v2.o network.o header.h
	$(CC) $(CCFLAGS) -c controller.c

zombie: zombie.o
	$(CC) $(CCFLAGS) -o zombie zombie.o utils_v2.o network.o

zombie.o: zombie.c utils_v2.o network.o header.h
	$(CC) $(CCFLAGS) -c zombie.c 

labo: labo.o
	$(CC) $(CCFLAGS) -o labo labo.o utils_v2.o network.o

labo.o: labo.c utils_v2.o network.o header.h 
	$(CC) $(CCFLAGS) -c labo.c 

network.o: network.c network.h
	$(CC) $(CCFLAGS) -c network.c
	
utils_v2.o: utils_v2.c utils_v2.h
	$(CC) $(CCFLAGS) -c utils_v2.c 


########## Cleaning - ipcrm -a ##########

clean:
	rm -f *.o
	rm -f $(ALL)
	