
/*
A simple game with the use of multithreading,
there are 4 players who randomly meet in pairs,
when they meet, they decide who wins the dice match

*/

//#include <windows.h>    //for windows
#include <stdio.h>  
#include <time.h> 


#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


int dice_data[5] = {0,0,0,0,0}; // {Stan,dice1,dice2,resultsum1,resultsum2}
int sleep_time = 0;
sem_t semafor;

int dice_random(int min, int max)   
{
    int tmp;
    if (max>=min)
        max-= min;
    else
    {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    if(rand() % max + min){

    }
    return max ? (rand() % max + min) : min;
}

void* dice_thread(void *thrID){    

    int myid = *(int*)thrID;
    int result_tab[2];
    int sum;
    while(1){
        srand(time(NULL) + myid);
        result_tab[0] = dice_random(1,7);
        result_tab[1] = dice_random(1,7);
        sum = result_tab[0]+result_tab[1];
        sem_wait(&semafor);

        while(dice_data[1] != myid && dice_data[2] != myid && sleep_time <=2 ){

        if(dice_data[0] == 0){
            printf("New Game Start \n");
            dice_data[0] = 1;
            dice_data[1] = myid;
            dice_data[3] = sum;

        }else if(dice_data[0] == 1){
            dice_data[0] = 2;
            dice_data[2] = myid;
            dice_data[4] = sum;
            printf("Player nr. %d vs %d \n",dice_data[1],dice_data[2]);
            printf("Compare sum %d ?? %d \n",dice_data[3],dice_data[4]);
            if(dice_data[4] == dice_data[3]){
                printf("Draw, sum of score = %d \n",dice_data[3]);
            }else if(dice_data[4] > dice_data[3]){
                printf("Player nr %d win with sum of score = %d\n",dice_data[2],dice_data[4]);
            }else{
                printf("Player nr %d win with sum of score = %d\n",dice_data[1],dice_data[3]);
            }
            #ifdef _WIN32
                Sleep(1000);
            #endif
            #ifdef __linux__
                sleep(1);
            #endif
            dice_data[0] = 0;
            dice_data[1] = 0;
            dice_data[2] = 0;
            dice_data[3] = 0;
            dice_data[4] = 0;
            sleep_time = 0;
        }
        sleep_time++;

       }
       sem_post(&semafor);

    }
    return 0;
}

int main(){
    pthread_t w1, w2, w3, w4,w_reset;
    sem_init(&semafor,0,1);
    int tabb[4] = {1,2,3,4};
    pthread_create(&w1, 0, dice_thread,tabb);
    pthread_create(&w2, 0, dice_thread,tabb+1);
    pthread_create(&w3, 0, dice_thread,tabb+2);
    pthread_create(&w4, 0, dice_thread,tabb+3);

    pthread_join(w1,NULL);
    pthread_join(w2,NULL);
    pthread_join(w3,NULL);
    pthread_join(w4,NULL);

return 0;
}
