#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>

typedef long long int ll;

int *value,*b;
ll *hasil;

typedef struct pair{
    int a;
    int b;
    int index;
}pair;

void input(){
    for(int i=0;i<24;i++)
        scanf("%d",&b[i]);
    // for(int i=0;i<24;i++)
    //     printf("%d ",b[i]);

}

void *hitung(void *y){
    pair *tmp = (pair *) y;
    if(!(tmp->a) || !(tmp->b)){
        hasil[tmp->index] = 0;
    }
    else{
        int max = tmp->b;
        int i = tmp->a;
        ll total = 1;
        while(i--){
            max--;
            total *= (i+1);
            if(!max){
                break;
            }
            if(total < 0){
                printf("overflow\n");
            }
        }
        hasil[tmp->index] = total;
    }
}

int main(){
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    b = (int*) malloc(24 * sizeof(int));
    hasil = (ll*) malloc(24 * sizeof(ll));
    input();

    pthread_t threads[24];

    for(int i=0;i<24;i++){
        pair *tes = (pair*)malloc(sizeof(*tes));
        tes->a = value[i];
        tes->b = b[i];
        tes->index = i;
        if(pthread_create(&threads[i],NULL,hitung,(void *)tes)!=0){
            fprintf(stderr, "error: Cannot create thread # %d\n",i);
        }
    }

    for (int i = 0; i < 24; ++i){
      if (pthread_join(threads[i], NULL))
        {
          fprintf(stderr, "error: Cannot join thread # %d\n", i);
        }
    }

    for(int i=0;i<24;i++){
        if(i%6==0)
            printf("\n");
        printf("%lld ",hasil[i]);
    }
    printf("\n");
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}