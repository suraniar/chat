#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ftree.h"
#include <libgen.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

int rcopy_client(char *src_path, char *dest_path, char *host_ip, int port) {
    printf("SRC: %s\n", src_path);
    printf("DEST: %s\n", dest_path);
    printf("IP: %s\n", host_ip);
    printf("PORT: %d\n", port);
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if (sock_fd<0){
        perror("server: socket");
        exit(1);
    }

    struct sockaddr_in sockClient;
    sockClient.sin_family= AF_INET;
    sockClient.sin_port=htons(port);
    sockClient.sin_addr.s_addr=INADDR_ANY;

    if(inet_pton(AF_INET,host_ip,&sockClient.sin_addr)<1){
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *)&sockClient, sizeof(sockClient))==-1){
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }
    printf("Succesfully Connected to Server!\n");
    //write(sock_fd,"hello\r\n",7);
    printf("yo\n");
    while(1){
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        fd_set fdlist;
        int curFD=sock_fd;
        FD_ZERO(&fdlist);
        FD_SET(sock_fd, &fdlist);
        if (select(curFD + 1, &fdlist, NULL, NULL, &tv) < 0) {
            perror("select");
        }
        char buf[200];
        if (FD_ISSET(sock_fd, &fdlist)){
            char buf1[200];
            read(sock_fd,buf1,sizeof(buf1));
            printf("%s\n",buf1);
        }else{
            fgets(buf, sizeof(buf), stdin);
            write(sock_fd, buf,sizeof(buf));
        }
    }
    close(sock_fd);
    return 0;
}

void rcopy_server(int port) {

    printf("PORT: %d\n", port);
    int clientFDS[20];
    int count=0;
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if (sock_fd<0){
        perror("server: socket");
        exit(1);
    }
    struct sockaddr_in sockServer;
    sockServer.sin_family= AF_INET;
    sockServer.sin_port=htons(port);
    sockServer.sin_addr.s_addr=INADDR_ANY;

    memset(&sockServer.sin_zero, 0, 8);
    if(bind(sock_fd,(struct sockaddr *)&sockServer,sizeof(sockServer))){
        perror("server: bind");
        close(sock_fd);
        exit(1);
    }

    if (listen(sock_fd, 5) < 0) {
        perror("server: listen");
        close(sock_fd);
        exit(1);
    }

    while(1){
        fd_set fdlist;
        int curFD=sock_fd;
        FD_ZERO(&fdlist);
        FD_SET(sock_fd, &fdlist);
        int p;
        for(p=0;p<count;p++){
            //printf("fd: %d\n",p->fd);
            FD_SET(clientFDS[p], &fdlist);
            if (clientFDS[p]> curFD){
                curFD = clientFDS[p];
            }
        }
        //printf("%d\n",port);
        if (select(curFD + 1, &fdlist, NULL, NULL, NULL) < 0) {
            perror("select");
        }else{
            //fflush(stdout);
            //printf("hi\n");
            //fflush(stdout);
            int p,q;
            for (p = 0; p<count; p++){
                if (FD_ISSET(clientFDS[p], &fdlist)){
                    char buf[200];
                    read(clientFDS[p],buf,sizeof(char)*200);
                    for (q=0;q<count;q++){
                        if(q!=p){
                            write(clientFDS[q],buf,sizeof(char)*200);
                        }
                    }
                    printf("%s\n",buf);
                }
            }

            if (FD_ISSET(sock_fd, &fdlist)){
                int client_fd=accept(sock_fd,NULL,NULL);
                if (client_fd<0){
                    perror("server: accept");
                    close(sock_fd);
                    exit(1);
                }
                clientFDS[count]=client_fd;
                count++;
            }
        }

    }
    int i;
    for(i=0;i<count;i++){
        close(clientFDS[i]);
    }
    close(sock_fd);
    exit(1);
}


