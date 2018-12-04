#include <pthread.h>
#include <stdio.h>

#define NTHREADS 10

int counter[10];
pthread_mutex_t lock_arr[10];

//	The thread starter function
void *myThread(void* lock_index)
{
	int j = (int) lock_index;
	for(int i=0; i<1000; i++)
	{
		pthread_mutex_lock(&lock_arr[j]);
		counter[j]++; // or whatever your critical section is
		pthread_mutex_unlock(&lock_arr[j]);
	}
}

int main()
{
	// Create space for pthread variables
	pthread_t tid[NTHREADS];
	int rc;

	for(int i=0; i<10l; i++)
	{
		counter[i] = 0;
		rc = pthread_mutex_init(&lock_arr[i], NULL);
		if(rc!=0)
			return 1;
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Create a thread with default attributes and no arguments
		pthread_create(&tid[i], NULL, myThread, (void*) i);
	}

	for(int i=0; i<10; i++)
	for(int j=0; j<1000; j++)
	{
		pthread_mutex_lock(&lock_arr[i]);
		counter[i]++;
		pthread_mutex_unlock(&lock_arr[i]);
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	for(int i=0; i<10; i++)
	{
		printf("Counter[%d] = %d\n", i, counter[i]);			
	}

}