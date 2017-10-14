#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
// Add your system includes here.
#include <string.h>
char * hash(FILE * f){
    char * hash_val=malloc(sizeof(char)*8);
    char c;
    int count=0;
    for (int i=0;i<8;i++){
        hash_val[i]='\0';
    }
    if(f){
        while((c=fgetc(f))!=EOF){
            if(count !=0 && count==8){
                count=0;
            }
            hash_val[count]=hash_val[count]^c;
            count++;
        }

    }else{
	perror("fopen");
		exit(1);
	}
    return hash_val;

}
