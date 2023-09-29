#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define maxData 2048

int main(int args, char* argv[]){
  if(args<4){
    perror("Please enter Client ID (CID) (IP) (PORT) in the CMD line Arguments");
    exit(0);
  }
  char Data[maxData];
  int clientSocket, ret,v,PORT=atoi(argv[3]);
  struct sockaddr_in serverAddr;
  FILE *fp;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(clientSocket < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Client Socket is created.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(argv[2]);

  ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(ret < 0){
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Connected to Server.\n");

  while(1){
    memset(Data,0, maxData);
    printf("Client: \t");
    scanf("%s", Data);

    
    if(strcmp(Data, "End") == 0){
      send(clientSocket, Data, strlen(Data), 0);
      close(clientSocket);
      printf("[-]Disconnected from server.\n");
      exit(1);
    }
    else{

      fp=fopen(Data,"r");
      if(fp == NULL) {
        perror("Error in opening file");
      }
      else{
        strcpy(Data+strlen(Data)," ");
        strcpy(Data+strlen(Data),argv[1]);
        strcpy(Data+strlen(Data)," ");
        int c=0, l=strlen(Data); char v;
        while(fscanf(fp,"%c",&v)==1){
         Data[l+(c++)]=v;
        }
        strcpy(Data+strlen(Data)," ;"); 
        send(clientSocket, Data, strlen(Data), 0);
        fclose(fp);
        bzero(Data, sizeof(Data));
        if(recv(clientSocket, Data, maxData, 0) < 0){
          printf("[-]Error in receiving data.\n");
        }
        else{
          printf("Server: \t%s\n", Data);
        }
      }
    }
  }

  return 0;
}

