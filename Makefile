PORT=30000
FLAGS = -DPORT=$(PORT) -g -Wall -std=gnu99
DEPENDENCIES = hash.h ftree.h

all: server client

server: ftree.o rcopy_server.o hash_functions.o
	gcc ${FLAGS} -o $@ $^

client: rcopy_client.o ftree.o hash_functions.o
	gcc ${FLAGS} -o $@ $^


%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean: 
	rm *.o server client
