#include<stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int num_Bytes;
char access_str;
long number_of_operations;
int thread_val;
float seq_thrPut_array[10], rand_thrPut_array[10];
float seq_latency_array[10], rand_latency_array[10];
struct timeval tm;

//Function to calculate throughput .
//Parameter : Time Elapsed for performing Num Operations.
float calculate_throughput(double timeDiff)
{
	float result=0.0;
	// 2 is the number of memory operations done.
	result = (float) (number_of_operations*2*num_Bytes)/(timeDiff*1024*1024);
	return result;
}

//Function to find the avg throughput of all the concurrent threads.
float find_average_throughput(int numThread)
{
	float sum=0.0,result=0.0;
	int i;
	if(access_str=='S')
	{
		for(i=0;i<numThread;i++)
		{
			sum=sum+seq_thrPut_array[i];
		}
	}
	else if(access_str=='R')
	{
		for(i=0;i<numThread;i++)
		{
			sum=sum+rand_thrPut_array[i];
		}
	}
	result=sum/numThread;
	return result;
}

//Function to find the avg latency of all the concurrent threads.
float find_average_latency(int numThread)
{
	float sum1=0.0,result1=0.0;
	int i;
	if(access_str=='S')
	{
		for(i=0;i<numThread;i++)
		{
			sum1=sum1+seq_latency_array[i];
		}
	}
	else if(access_str=='R')
	{
		for(i=0;i<numThread;i++)
		{
			sum1=sum1+rand_latency_array[i];
		}
	}
	result1=sum1/numThread;
	return result1;
}

//Main function called from the create_thread which will initialize buffer , record time to run operations , and populate latency and throughput.
void *ComputeBenchmark_memory(void *args)
{
	clock_t timer_start,timer_end;
	float timer_elapsed=0.0;
	int threadID=(int) args;
	double Seq_time_Elapsed,Rand_time_Elapsed;
	float throughput=0.0,latency=0.0;
	char *string1= malloc(sizeof(char)*(num_Bytes*number_of_operations));
	char *string2= malloc(sizeof(char)*(num_Bytes*number_of_operations));
	int i;
	double time_1, time_2;

	for(i=0;i<num_Bytes*number_of_operations;i++)
	{
		//Initialize to random character
		int n = rand() % 26;
		char c = (char)(n+65);
		string1[i] = c;
	}

	//Sequential Access
	if(access_str == 'S')
	{
		gettimeofday(&tm, NULL);
		time_1=tm.tv_sec + (tm.tv_usec/1000000.0);
		for(i=0;i<number_of_operations;i++)
		{
			memmove(&string2[i*num_Bytes],&string1[i*num_Bytes],num_Bytes);// one read and one write
		}

		gettimeofday(&tm, NULL);
				time_2=tm.tv_sec + (tm.tv_usec/1000000.0);

		Seq_time_Elapsed =time_2-time_1;
		throughput = calculate_throughput(Seq_time_Elapsed);
		latency = 1/throughput;
		seq_thrPut_array[threadID] = throughput;
		seq_latency_array[threadID] = latency;
	}

	int rand_ind;
	//Random Access
	if(access_str == 'R')
	{
		gettimeofday(&tm, NULL);
		time_1=tm.tv_sec + (tm.tv_usec/1000000.0);
		for(i=0;i<number_of_operations;i++)
		{
			rand_ind=rand()%number_of_operations;
			memmove(&string2[rand_ind*num_Bytes],&string1[rand_ind*num_Bytes],num_Bytes);// one read and one write
		}

		gettimeofday(&tm, NULL);
		time_2=tm.tv_sec + (tm.tv_usec/1000000.0);
		Rand_time_Elapsed = time_2-time_1;//(float) timer_elapsed / CLOCKS_PER_SEC;
		throughput = calculate_throughput(Rand_time_Elapsed);
		latency = 1/throughput;
		rand_thrPut_array[threadID] = throughput;
		rand_latency_array[threadID] = latency;
	}
	free(string1);
	free(string2);
}

void processThreads_memory(int numThread, char str, long numOperations)
{
	//defining clock header variables
	int blnSuccess;
	number_of_operations=numOperations;
	float resultThroughput=0.0,resultLatency=0.0;
	access_str = str;
	pthread_t arrThreads[numThread];

	//create threads;
	int i,j=0;
	for(i=0;i<numThread;i++)
	{
		blnSuccess = pthread_create(&arrThreads[i], NULL, ComputeBenchmark_memory, (void*) i);
		if (blnSuccess)
		{
			if(access_str == 'S')
				printf("Could not create thread for Sequential Access operation for %i Byte BlockSize",num_Bytes);

			if(access_str == 'R')
				printf("Could not create thread for Random Access operation for %i Byte BlockSize",num_Bytes);
		}
	}

	//join threads
	for(j=0;j<numThread;j++)
	{
		thread_val=j;
		pthread_join(arrThreads[j],NULL);
	}

	if(access_str == 'S')
	{
		resultThroughput=find_average_throughput(numThread);
		resultLatency=find_average_latency(numThread);
		printf("Sequential Access with %d thread for %i Bytes => Throughput : %f , Latency : %0.9f\n", numThread, num_Bytes, resultThroughput , resultLatency );

	}
	else if(access_str == 'R')
	{
		resultThroughput=find_average_throughput(numThread);
		resultLatency=find_average_latency(numThread);
		printf("Random Access with %d thread for %i Bytes  => Throughput : %f , Latency : %0.9f\n", numThread, num_Bytes, resultThroughput , resultLatency  );
	}
}

int main()
{
	int no_of_threads;

	printf("\n\n MEMORY BENCHMARK \n\n");
	//For 1 thread with block size 1 Byte
	no_of_threads = 1;
	num_Bytes = 1;
	processThreads_memory(no_of_threads,'S', 100000000);
	processThreads_memory(no_of_threads,'R',100000000 );

	//For 2 thread with block size 1 Byte
	no_of_threads = 2;
	processThreads_memory(no_of_threads,'S',100000000);
	processThreads_memory(no_of_threads,'R', 100000000);


	//For 1 thread with block size 1 KByte
	no_of_threads = 1;
	num_Bytes = 1*1024;
	processThreads_memory(no_of_threads,'S', 100000);
	processThreads_memory(no_of_threads,'R', 100000);

	//For 2 thread with block size 1 KByte
	no_of_threads = 2;
	processThreads_memory(no_of_threads,'S', 100000);
	processThreads_memory(no_of_threads,'R', 100000);

	//For 1 thread with block size 1 MByte
	no_of_threads = 1;
	num_Bytes = 1*1024*1024;
	processThreads_memory(no_of_threads,'S', 100);
	processThreads_memory(no_of_threads,'R', 100);

	//For 2 thread with block size 1 MByte
	no_of_threads = 2;
	processThreads_memory(no_of_threads,'S', 100);
	processThreads_memory(no_of_threads,'R', 100);
	printf("\n\n \n\n");
	pthread_exit(NULL);
	return 1;
}
