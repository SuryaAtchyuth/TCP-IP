#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define maxData 2048
#define FIFO "dfile"
int M,A,Q,D;
FILE *dF,*oF;

void* ms(void* in);
void* Split(void *arr);
int ms_rec(int *a,int l, int r);
int merger(int *a,int l,int mid,int r);

typedef struct{
    int* a;
    int l;
    int r;
}TD;

typedef struct{
    int CID;
    char File[50];
    int* A;
    int L;
    int sA;
}MTD;

//-------------------------------------------
void admin();

void initialize();
int getFA();


//-------  

typedef struct{
    int FA;
    struct fr *next;
}fr;

fr *fh, *ft;

typedef struct{
    int CID;
    char File[50];
    int Len;
    int *A;
    int sA;
    struct pipeQueuee *next;
}pipeQueuee;