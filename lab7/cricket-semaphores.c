#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<semaphore.h>
#include<pthread.h>

//initializer for mutex it locks capitals,titans and all other players
int count=0,th_count=0;
sem_t mutex1,mutex2,mutex3;
int capital_score=0,titan_score=0,maximum=0;
/*typedef struct {
 int a;
 int b;
 } myarg_t;*/
void* match(void* arg){
    if(th_count<22){
        sem_wait(&mutex1);
    }
    //printf("match started\n");
	sem_wait(&mutex3);
    if(count<12){
		int x=rand()%100;
		capital_score+=x;
		if(maximum<x){
			maximum=x;
		}
	}
	if(count>=12){
		int x=rand()%100;
		titan_score+=x;
		if(maximum<x){
			maximum=x;
		}
	}
    count++;
    if(count==22){
        sem_post(&mutex2);
    }
	sem_post(&mutex3);
}
int main(int argc,char* argv[]){
	int n;
	scanf("%d",&n);
	int no_play=(n-(n%22));
	int mat=no_play/22;
	pthread_t p[no_play];
	sem_init(&mutex1, 1, 0);
    sem_init(&mutex2, 1, 0);
	sem_init(&mutex3, 1, 1);
	for(int i=0;i<mat;i++){
		for(int j=0;j<22;j++){
            //printf("player %d created \n",j+1);
			pthread_create(&p[22*i+j],NULL,match,NULL);
            th_count++;
            if(th_count==22){
				for(int i=0;i<22;i++){
                	sem_post(&mutex1);
				}
            }
		}
		if(count<22){
            sem_wait(&mutex2);
        }
        int score=0;
		if(capital_score>titan_score){
			score=capital_score-titan_score;
		}
		else{
			score=titan_score-capital_score;
		}
		printf("SCORE: Capitals: %d :: Titans: %d \n",capital_score,titan_score);
		printf("Highest Individual Score: %d \n",maximum);
        if(capital_score>titan_score){
            printf("Result : Capitals won by %d \n",capital_score-titan_score);
        }
        else{
            printf("Result : Titans won by %d \n",titan_score-capital_score);
        }
		capital_score=0;
		titan_score=0;
		maximum=0;
        count=0;
        th_count=0;
	}
	for(int j=0;j<no_play;j++){
			pthread_join(p[j],NULL);
	}
}




