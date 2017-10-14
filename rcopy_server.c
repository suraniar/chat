#include <stdio.h>
#include "ftree.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#ifndef PORT
  #define PORT 30000
#endif

int main(int argc, char **argv) {
    rcopy_server(PORT);

    // Should never get here!
    fprintf(stderr, "Server reached exit point.");
    return 1;
}



