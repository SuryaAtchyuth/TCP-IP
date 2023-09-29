#include "sync.h"

#define PORT 8888

pipeQueuee *pQHead, *pQTail;

void insert_pipeQueuee(char data[maxData]);
void admin ();
void* pDataCollection();
void* CalProcessThread();
void* ms(void* in);
void handle_sigcont(int sig);

int main(int args, char* argv[]){
    struct sigaction sa;
    sa.sa_handler = &handle_sigcont;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
  if(args<5){
    fprintf(stdout,"Please enter (CMD) line argumets:\n\tMergers[M] Sorter Arrays[A] Queuee Size[Q] Debug[D]\n");
    exit(0);
  }
  M=atoi(argv[1]); A=atoi(argv[2]); Q=atoi(argv[3]); D=atoi(argv[4]);
  initialize();
  admin ();
  return 0;
}
void handle_sigcont(int sig) {
  int pipe_fd;
  char Data[maxData];
  pipe_fd = open (FIFO, O_WRONLY);
  memset(Data,0,maxData);
  strcpy(Data,"Exit");
  write(pipe_fd,Data,maxData);
  exit(0);
}

void admin ()
{
  pid_t Process;
  int pipe_fd;
  char Data[maxData];

  memset(Data,0,maxData);
  Process = fork ();

  if (Process > 0){                   //Server
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    pid_t childpid;

    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
      printf ("[-]Error in connection.\n");
      exit (1);
    }
    printf ("[+]Server Socket is created.\n");

    memset (&serverAddr, '\0', sizeof (serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons (PORT);
    serverAddr.sin_addr.s_addr = inet_addr ("127.0.0.1");

    ret =  bind (sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr));
    if (ret < 0){
      printf ("[-]Error in binding.\n");
      exit (1);
    }
    printf ("[+]Bind to port %d\n", PORT);

    if (listen (sockfd, 10) == 0){
      printf ("[+]Listening....\n");
    }
    else{
      printf ("[-]Error in binding.\n");
    }

    pipe_fd = open (FIFO, O_WRONLY);
    while (1){
      newSocket = accept (sockfd, (struct sockaddr *) &newAddr, &addr_size);
      if (newSocket < 0){
        exit (1);
      }
      printf ("Connection accepted from %s:%d\n",
      inet_ntoa (newAddr.sin_addr), ntohs (newAddr.sin_port));

      if ((childpid = fork ()) == 0){                          //Clients
        close (sockfd);

        while (1){
          recv (newSocket, Data, maxData, 0);
          if (strcmp (Data, "End") == 0){

            printf ("Disconnected from %s:%d\n",
            inet_ntoa (newAddr.sin_addr),
            ntohs (newAddr.sin_port));
            break;
          }
          else{
            write(pipe_fd,Data,maxData);
            printf ("Client: %s\n", Data);
            send (newSocket, "Recieved", strlen ("Recieved"), 0);
            memset (Data,0,maxData);
          }
        }
      }
    }
    close (newSocket);
  }
  else{  
    pthread_t pipeQueuee,cpThread;
    pthread_create(&pipeQueuee,NULL,pDataCollection,NULL);
    pthread_create(&cpThread,NULL,CalProcessThread,NULL);
    pthread_join(pipeQueuee,NULL);
    pthread_join(cpThread,NULL); 
  } 
}
void initialize(){
  if(unlink (FIFO)<0){}
  mkfifo (FIFO, 0777);
  dF=fopen("debug","w");
  oF=fopen("Output","w");
  fflush(dF);
  fflush(oF);
    int i; fr *n[A];
    for(i=0;i<A;i++){
    n[i]=(fr*)malloc(sizeof(fr*));
    n[i]->FA=i;
    }
    fh=n[0];
    ft=n[0];
    for(i=1;i<A;i++){
       ft->next=n[i];
       ft=ft->next;
       ft->next=NULL;
    }   
}

void* CalProcessThread(){
  int sA;
  pthread_t thread;
  pthread_attr_t detachedThread;
  pthread_attr_init(&detachedThread);
  while(1){
    if(pQHead!=NULL){
      while(1){
        sA=getFA();
        if(sA>-1){
              MTD* in;
              in=(MTD*)calloc(1,sizeof(MTD));
              strcpy(in->File,pQHead->File); 
              in->CID=pQHead->CID; in->sA=sA;
              in->A=pQHead->A; in->L=pQHead->Len;
              pQHead=pQHead->next;
              pthread_create(&thread,NULL,ms,in);
              pthread_detach(thread);
          break;
        } 
      }
    } 
  }
  pthread_attr_destroy(&detachedThread);
  pthread_exit(0);
}


void* pDataCollection(){
  char Data[maxData];
  int pipe_fd = open (FIFO, O_RDONLY);
  while (read (pipe_fd, Data, maxData) > 0){
      if(strcmp(Data,"Exit")==0){
        exit(0);
      }
      insert_pipeQueuee(Data);
  }
}

int getFA(){
    int FA;
    if(fh==NULL){
        return -1;
    }
    else{
        FA=fh->FA;
        fh=fh->next;
        return FA;
    }
}


void insert_pipeQueuee(char data[maxData]){
  char file[50], dstr[maxData], *cursor = dstr;
  int *v,i=0;
  pipeQueuee *node;
  node = (pipeQueuee*)calloc(1,sizeof(pipeQueuee));

  sscanf(data,"%s",node->File);
  sscanf(data,"%*s %[^;]",dstr);
  
  node->CID = strtol(cursor, &cursor, 10);
  node->Len = strtol(cursor, &cursor, 10);
  v=(int*)calloc(node->Len ,sizeof(int));
  while (cursor != dstr + strlen(dstr)) {
    v[i++] = strtol(cursor, &cursor, 10);
    while (*cursor == ' ' || *cursor >57) {
      cursor++;
    }
  }
    node->A=v;
    if(pQHead==NULL){
       pQHead=node;
       pQTail=node;
    }
    else{
       pQTail->next=node; 
       pQTail=pQTail->next;
    }
    pQTail->next=NULL;
}
