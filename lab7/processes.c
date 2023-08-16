#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>

int count=22,th_count=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c  = PTHREAD_COND_INITIALIZER;
int capital_score=0,titans_score=0,maximum=0; 

void* match(void* arg){
	pthread_mutex_lock(&m);
	while(th_count<22){
		//printf("It is waiting \n");
		pthread_cond_wait(&c,&m);
		//printf("wait is over \n");
	}
	if(count>=12){
		int x=rand()%100;
		capital_score+=x;
		count--;
		if(maximum<x){
			maximum=x;
		}
	}
	if(count>=1){
		int x=rand()%100;
		titans_score+=x;
		count--;
		if(maximum<x){
			maximum=x;
		}
	}
	pthread_mutex_unlock(&m);
}

int main(int argc,char* argv[]){
	int n;
	scanf("%d",&n);
	int no_play=(n-(n%22));
	int mat=no_play/22;
	pthread_t p[no_play];
	for(int i=0;i<mat;i++){
		for(int j=0;j<22;j++){
			//printf("Thread match no %d player no %d created \n",i,j);
			pthread_create(&p[22*i+j],NULL,match,NULL);
			pthread_mutex_lock(&m);
			th_count++;
			pthread_mutex_unlock(&m);
			if(th_count==22){
				pthread_cond_broadcast(&c);
			}

		}
		int score=0;
		char winner[7] ;
		if(capital_score>titans_score){
			//winner[7]="Capital";
			score=capital_score-titans_score;
		}
		else{
			//winner[7]="Titan";
			score=titans_score-capital_score;
		}
		printf("SCORE: Capitals: %d :: Titans: %d \n",capital_score,titans_score);
		printf("Highest Individual Score: %d \n",maximum);
		//printf("Result : %s won by %d runs \n",winner,score);
		pthread_mutex_lock(&m);
		capital_score=0;
		titans_score=0;
		count=22;
		maximum=0;
		th_count=0;
		pthread_mutex_unlock(&m);

	}
	for(int j=0;j<no_play;j++){
			pthread_join(p[j],NULL);
	}
}




