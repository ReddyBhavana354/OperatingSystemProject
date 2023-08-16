#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>

//initializer for mutex it locks capitals,titans and all other players
pthread_mutex_t  m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c=PTHREAD_COND_INITIALIZER;
pthread_cond_t m_finished=PTHREAD_COND_INITIALIZER;
int count=0,th_count=0;
int capital_score=0,titan_score=0,maximum=0;
/*typedef struct {
 int a;
 int b;
 } myarg_t;*/
void* match(void* arg){
    pthread_mutex_lock(&m);
    //myarg_t *args = (myarg_t *) arg;
    if(th_count<22){
        pthread_cond_wait(&c,&m);
    }
    //printf("Thread match no %d player no %d finished \n",args->a,args->b);
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
        pthread_cond_signal(&m_finished);
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
            //myarg_t arg = {i+1,j+1};
			//printf("Thread match no %d player no %d created \n",i+1,j+1);
			pthread_create(&p[22*i+j],NULL,match,NULL);
            th_count++;
            if(th_count==22){
                pthread_cond_broadcast(&c);
            }
		}
		if(count<22){
            pthread_cond_wait(&m_finished,&m);
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
		count=0;
		maximum=0;
		th_count=0;
	}
	for(int j=0;j<no_play;j++){
			pthread_join(p[j],NULL);
	}
}




