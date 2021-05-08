#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int a[4][3],b[3][6];

void input(){
    for(int i=0;i<4;i++)
        for(int j=0;j<3;j++)
            scanf("%d",&a[i][j]);
    for(int i=0;i<3;i++)
        for(int j=0;j<6;j++)
            scanf("%d",&b[i][j]);
}


int main(){
    input();
    // calculate();
    key_t key = 1234;
    int *value;
    int count = 24;

    int shmid = shmget(key, sizeof(int)*count, IPC_CREAT | 0666);
    if(shmid  == -1){
        printf("error\n");
        return 0;
    }
    value = shmat(shmid, NULL, 0);

    for(int i=0;i<4;i++){
        for(int x=0;x<6;x++){
            int tmp=0;
            for(int j=0;j<3;j++){
                tmp+=a[i][j]*b[j][x];
            }
            value[6*i+x] = tmp;
        }
    }

    for(int i=0; i<count; i++){
        if(i%6==0){
            printf("\n");
        }
        printf("%d ", value[i]);
    }
    printf("\n");

    shmdt((void *) value);
    shmdt(value);
}
// 1 2 3
// 4 5 6
// 7 8 9
// 1 2 3
// 1 2 3 4 5 6
// 7 8 9 1 2 3
// 4 5 6 7 8 9