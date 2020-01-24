#include <stdlib.h>         
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include<time.h> 

void *TA_Behavior();
void *studentsBehavior(void* student_number);

pthread_t *students_array;  //array of threads for each student
pthread_t ta;   //thread for TA
int available_chairs = 3;   //number of available chairs
int n = 5;  //number of students
int chair_index = 0;
int ac, ac1;
pthread_mutex_t ta_chair_available;
sem_t sleeping; //semephore for ta sleeping/awake state
sem_t chairs_semaphores[3]; //array of 3 chairs semophores
sem_t student_helping;  //student being helped or not

void main(int argc, char* argv[])
{
    
    if(argc<2)
	{
		printf("using default value 5\n");
		n = 5;
	}
	else
	{
		
		n = atoi(argv[1]);
        printf("Number of students is  %d\n", n);
	}

    //create array of students threads
    students_array = (pthread_t*) malloc(sizeof(pthread_t)*n);

    //initialize mutex
    pthread_mutex_init(&ta_chair_available, NULL);

    //initialize semephores
    sem_init(&sleeping, 0, 0);

    for(int i = 0; i < 3; i++)          
        sem_init(&chairs_semaphores[i], 0, 0);

    sem_init(&student_helping, 0, 0);

    //initialize threads
    pthread_create(&ta, NULL, TA_Behavior, NULL);

    for(int student_number = 0; student_number < n; student_number++)
    {
        pthread_create(&students_array[student_number], NULL, studentsBehavior,(void*) (long) student_number);
        sleep(0.5);
    }

    int c = getchar();

}
void *TA_Behavior()
{
    while(1)
    {
        printf("Ta Sleeping \n");
        sem_wait(&sleeping); //let TA sleep first
        printf("TA woke up \n");

        /* TA is now awake*/
        
        //lock TA chair
        pthread_mutex_lock(&ta_chair_available);
        ac1 = available_chairs;    //get number of available chairs
        //unlock TA chair
        pthread_mutex_unlock(&ta_chair_available);

        while(ac1 < 3) //while there are occupied chairs
        {
            //lock TA chair
            pthread_mutex_lock(&ta_chair_available);
            sem_post(&chairs_semaphores[chair_index]);  //let some student in
            printf("Let some student in \n");
            pthread_mutex_unlock(&ta_chair_available);
            
            sleep(3);   //help student for 3 seconds
            sem_post(&student_helping); //announce student done and leaving
            printf("Student done and leaving \n");
        }
    }
}
void *studentsBehavior(void* student_number)
{   //student_number = *(int*) student_number;
    while(1)
    {
        srand(time(0)); 
        
        //do some programming for random amount of time
        printf("%d does some programming \n", student_number);
        int programming_duration = rand() % 10;
        sleep(programming_duration);
        printf("%d stuck and needs help \n", student_number);

        //lock TA chair
        pthread_mutex_lock(&ta_chair_available);
        ac = available_chairs;    //get number of available chairs
        //unlock TA chair
        pthread_mutex_unlock(&ta_chair_available);

        //if there's available chair(s)
        if(ac > 0)
        {
            //if there is no one sitting
            if(ac == 3)
            {
                //wake up ta
                sem_post(&sleeping);
            }            

            //lock TA chair
            pthread_mutex_lock(&ta_chair_available);
            if(available_chairs>0)
            {
                available_chairs--; //sit and decrement available chairs
                printf("%d Sat on chair ", student_number);
                printf("remaining: %d \n", available_chairs);
            
            
            //unlock TA chair
            pthread_mutex_unlock(&ta_chair_available);

            sem_wait(&chairs_semaphores[chair_index]);  //waiting to leave chair

            pthread_mutex_lock(&ta_chair_available);
            available_chairs++; //increment available chairs
            printf("%d left chair ", student_number);
            printf("remaining: %d \n", available_chairs);
            chair_index++;  //increment chairs semaphores index
            if(chair_index == 3)    //reset index if passes 2
                chair_index = 0;
            pthread_mutex_unlock(&ta_chair_available);
            
            sem_wait(&student_helping); //left chair, waiting to go in to TA
            }
            else
            {
                printf("No available chairs \n");
            }
        }
        else
        {
            printf("No available chairs \n");
        }
    }
}