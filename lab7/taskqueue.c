#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
int count=1;
int q[1500];
int create[1500];
pthread_mutex_t  m=PTHREAD_MUTEX_INITIALIZER;
//void processtask(long number);
typedef struct {
 int a;
 int b;
 } myarg_t;

void* processtask(void* arg)
{
    // simulate burst time
    myarg_t *args = (myarg_t *) arg;
    int number=args->b;
    sleep(number);

    // update global variables
    pthread_mutex_lock(&m);
    sum += number;
    if (number % 2 == 1)
    {
        odd++;
    }
    else
    {
        even++;
    }
    if (number < min)
    {
        min = number;
    }
    if (number > max)
    {
        max = number;
    }
    q[count]=args->a;
    count++;
    pthread_mutex_unlock(&m);
    printf("Task completed\n");
}

int main(int argc, char *argv[])
{
    int counter=0;
    if (argc != 2)
    {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    char type;
    long num;
    int n_threads;
    scanf("%d",&n_threads);
    pthread_t p[n_threads];
    for(int i=0;i<n_threads;i++){
        q[count]=i;
        count++;
    }
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        if (type == 'p')
        { // processing task
            while(count<=0);
            if(count>0){
                myarg_t args = { q[count-1] ,num};
                count--;
                create[counter]=q[count];
                pthread_create(&p[q[count]], NULL,processtask , &args);
            }
            counter++;
            
        }
        else if (type == 'w')
        { // waiting period
            sleep(num);
            printf("Wait Over\n");
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fin);
    // Print global variables
    //sort(create,create+counter);
    for(int i=0;i<counter;i++){
        pthread_join(p[create[i]],NULL);
    }
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
