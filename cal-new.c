
#include "sync.h"

sem_t sem;

void* ms(void* in){
    MTD *p=in;
    int N=p->L,inc=N/M,mid=0,i,t,L=0;
    if(D){
       print(p->A,p->CID, p->File,p->L,++L);
    }
    TD **arr=(TD**)calloc(M,sizeof(TD*));
          fflush(dF);
 	      fprintf(dF,"\nSorter Array -> %d : Creating // Threads\n",p->sA);
    pthread_t thread[M];
    sem_init(&sem,0,1);     
    for(i=0,t=0;i<N;i+=inc,t++){
        arr[t]=(TD*)calloc(1,sizeof(TD));
        arr[t]->a=p->A;arr[t]->l=i;arr[t]->r=i+inc-1;  
        pthread_create(&thread[t],NULL,Split,arr[t]);
    }
    for(i=0;i<M;i++){
        pthread_join(thread[i],NULL);
    }
    sem_wait(&sem);
    if(D){
       print(p->A,p->CID, p->File,p->L,++L);
    }
    for(i=0;i<N;i+=inc){
        merger(p->A,0,i-1,i+inc-1);
        if(D){
           print(p->A,p->CID, p->File,p->L,++L);
        }
    }
    sem_post(&sem);
    sem_destroy(&sem);
    fflush(oF);
    fprintf(oF,"\n==================== CID=%d, file=%s, N=%d \n\t",p->CID,p->File,N);
    for(i=0;i<N;i++){
        fprintf(oF," %d ",p->A[i]);
        fflush(oF);
    }
    putFA(p->sA);
    free(in);
}

void print(int *a, int cid, char file[50], int n,int l){
    int i;
    fflush(dF);
    fprintf(dF,"\n==================== CID=%d, file=%s, N=%d, L=%d\n\t",cid,file,n,l);
    for(i=0;i<n;i++){
        fprintf(dF," %d ",a[i]); 
        fflush(dF);
    }
    fprintf(dF,"\n");
    fflush(dF);
}

void* Split(void *arr){
    TD *p=arr;
    int l=p->l,r=p->r;
    ms_rec(p->a,l,r);
}

int ms_rec(int *a,int l, int r){
    if(l<r){
        int mid=l+((r-l)/2);
        ms_rec(a,l,mid);
        ms_rec(a,mid+1,r);
        sem_wait(&sem);
        merger(a,l,mid,r);
        sem_post(&sem);
    }
}

int merger(int *a,int l,int mid,int r){
    int len_l=mid-l+1, 
        len_r=r-mid,
        *tmp_l=calloc(len_l,sizeof(int)),
        *tmp_r=calloc(len_r, sizeof(int)),
        i,j,k;
    
    for(i=0;i<len_l;i++){
        tmp_l[i]=a[l+i];
    }
    for(i=0;i<len_r;i++){
        tmp_r[i]=a[mid+1+i];
    }    
    
    for(i=0,j=0,k=l;k<=r;k++){
        if(i<len_l &&( j>=len_r || tmp_l[i]<=tmp_r[j] )){
            a[k]=tmp_l[i++];
        }
        else{
            a[k]=tmp_r[j++];
        }
    }
    free(tmp_l);
    free(tmp_r);
}

void putFA(int p){
    int i;
    fr *n;    
    n=(fr*)calloc(1,sizeof(fr));
    n->FA=p;
    if(fh==NULL){
        fh=n;
        ft=n;
    }
    else{
        ft->next=n;
        ft=n;
    }
    ft->next=NULL;
}

