#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

struct read_write_lock
{
pthread_mutex_t lock;
pthread_mutex_t writelock; // used to allow ONE writer or MANY readers
int readers; // count of readers reading in critical section
int sleeping_writers; // count of sleeping writers
}rwlock;

long int data = 0;			//	Shared data variable

struct argument_t
{
	int delay;
	int thread_id;
};

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
	int rc1 = pthread_mutex_init(&rw->lock, NULL);
	int rc2 = pthread_mutex_init(&rw->writelock, NULL);
	rw->readers = 0;
	rw->sleeping_writers = 0;
}

// The pthread based reader lock
void ReaderLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	rw->readers++;
	if (rw->readers == 1)
		pthread_mutex_lock(&rw->writelock); // first reader acquires writelock
	pthread_mutex_unlock(&rw->lock);
}	

// The pthread based reader unlock
void ReaderUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	rw->readers--;
	if (rw->readers == 0)
		pthread_mutex_unlock(&rw->writelock); // first reader acquires writelock
	pthread_mutex_unlock(&rw->lock);
}

// The pthread based writer lock
void WriterLock(struct read_write_lock * rw)
{	
	pthread_mutex_lock(&rw->writelock);
}

// The pthread based writer unlock
void WriterUnlock(struct read_write_lock * rw)
{
	pthread_mutex_unlock(&rw->writelock);
}

//	Call this function to delay the execution by 'delay' ms
void delay(int delay)
{
	struct timespec wait;

	if(delay <= 0)
		return;

	wait.tv_sec = delay / 1000;
	wait.tv_nsec = (delay % 1000) * 1000 * 1000;
	nanosleep(&wait, NULL);
}

// The pthread reader start function
void *ReaderFunction(void *args)
{
	struct argument_t *arg = (struct argument_t *) args; 
	//	Delay the execution by arrival time specified in the input
	delay(arg->delay);
	//	....
	
	//  Get appropriate lock
	ReaderLock(&rwlock);

	//	Display  thread ID and value of the shared data variable
	printf("\tReader %d, data: %d\n",arg->thread_id, data);
    //  Add a dummy delay of 1 ms before lock release  
	delay(1);
	ReaderUnlock(&rwlock);
}

// The pthread writer start function
void *WriterFunction(void *args)
{
	struct argument_t *arg = (struct argument_t *) args;
	//	Delay the execution by arrival time specified in the input
	delay(arg->delay);
	//	....

	//  Get appropriate lock
	WriterLock(&rwlock);
	//	Increment the shared data variable
	data++;
	//	Display  thread ID and value of the shared data variable
	printf("Writer %d, data: %d\n",arg->thread_id, data);
    //  Add a dummy delay of 1 ms before lock release
   	delay(1);
	WriterUnlock(&rwlock);
}

int main(int argc, char *argv[])
{
	pthread_t *threads;
	struct argument_t *arg;
	
	long int reader_number = 0;
	long int writer_number = 0;
	long int thread_number = 0;
	long int total_threads = 0;	
	
	int count = 0;			// Number of 3 tuples in the inputs.	Assume maximum 10 tuples 
	int rws[10];			
	int nthread[10];
	int delay[10];

	//	Verifying number of arguments
	if(argc<4 || (argc-1)%3!=0)
	{
		printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");		
		printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
		exit(1);
	}

	//	Reading inputs
	for(int i=0; i<(argc-1)/3; i++)
	{
		char rw[2];
		strcpy(rw, argv[(i*3)+1]);

		if(strcmp(rw, "r") == 0 || strcmp(rw, "w") == 0)
		{
			if(strcmp(rw, "r") == 0)
				rws[i] = 1;					// rws[i] = 1, for reader
			else
				rws[i] = 2;					// rws[i] = 2, for writer
			
			nthread[i] = atol(argv[(i*3)+2]);		
			delay[i] = atol(argv[(i*3)+3]);

			count ++;						//	Number of tuples
			total_threads += nthread[i];	//	Total number of threads
		}
		else
		{
			printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");
			printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
			exit(1);
		}
	}

	InitalizeReadWriteLock(&rwlock);

	// Create space for pthread variables
	pthread_t tid[total_threads];
	struct argument_t args[total_threads];

	//	Create reader/writer threads based on the input and read and write.

	for(int i=0; i<count; i++)
	{
		if(rws[i] == 1)
		{
			for(int j=0; j<nthread[i]; j++)
			{
				args[thread_number].delay = delay[i];
				args[thread_number].thread_id = ++reader_number;
				pthread_create(&tid[thread_number], NULL, ReaderFunction, &args[thread_number]);
				thread_number++;
			}
		}
		else if(rws[i] == 2)
		{
			for(int j=0; j<nthread[i]; j++)
			{
				args[thread_number].delay = delay[i];
				args[thread_number].thread_id = ++writer_number;
				pthread_create(&tid[thread_number], NULL, WriterFunction, &args[thread_number]);
				thread_number++;
			}
		}
	}

	//  Clean up threads on exit

	for(int i=0; i<total_threads; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	exit(0);
}
