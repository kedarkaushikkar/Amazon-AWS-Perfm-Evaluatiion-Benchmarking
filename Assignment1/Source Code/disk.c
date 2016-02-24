#include<stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Structure
int num_Bytes;
long number_of_operations;
char access_str;
char file_mode[5];
int thread_val;
FILE* fp;
float seq_thrPut_Read_array[10],seq_thrPut_Write_array[10], rand_thrPut_Read_array[10], rand_thrPut_Write_array[10];
float seq_latency_Read_array[10],seq_latency_Write_array[10], rand_latency_Read_array[10],  rand_latency_Write_array[10];
struct timeval tm;

//Function to calculate throughput .
//Parameter : Time Elapsed for performing Num Operations.
float calculate_throughput(double timeDiff)
{
	float result=0.0;
	result = (float) (number_of_operations*num_Bytes)/(timeDiff*1024*1024);
	return result;
}

//Function to find the avg throughput of all the concurrent threads.
float find_average_throughput(int numThread,char mode)
{
	float sum=0.0,result=0.0;
	int i;
	if(access_str=='S')
	{
		if(mode == 'R')
		{
			for(i=0;i<numThread;i++)
			{
				sum=sum+seq_thrPut_Read_array[i];
			}
		}
		else if(mode == 'W')
		{
			for(i=0;i<numThread;i++)
			{
				sum=sum+seq_thrPut_Write_array[i];
			}
		}
	}
	else if(access_str=='R')
	{
		if(mode == 'R')
		{
			for(i=0;i<numThread;i++)
			{
				sum=sum+rand_thrPut_Read_array[i];
			}
		}
		else if(mode == 'W')
		{
			for(i=0;i<numThread;i++)
			{
				sum=sum+rand_thrPut_Write_array[i];
			}
		}
	}
	result=sum/numThread;
	return result;
}

//Function to find the avg latency of all the concurrent threads.
float find_average_latency(int numThread,char mode)
{
	float sum1=0.0,result1=0.0;
	int i;
	if(access_str=='S')
	{
		if(mode == 'R')
		{
			for(i=0;i<numThread;i++)
			{
				sum1=sum1+seq_latency_Read_array[i];
			}
		}
		else if(mode == 'W')
		{
			for(i=0;i<numThread;i++)
			{
				sum1=sum1+seq_latency_Write_array[i];
			}
		}
	}
	else if(access_str=='R')
	{
		if(mode == 'R')
		{
			for(i=0;i<numThread;i++)
			{
				sum1=sum1+rand_latency_Read_array[i];
			}
		}
		else if(mode == 'W')
		{
			for(i=0;i<numThread;i++)
			{
				sum1=sum1+rand_latency_Write_array[i];
			}
		}
	}
	result1=sum1/numThread;
	return result1;
}

//Main function called from the create_thread which will initialize buffer , record time to run operations , and populate latency and throughput.
void *ComputeBenchmark_disk(void *args)
{
	clock_t timer_start,timer_end;
	float timer_elapsed=0.0;
	int threadID=(int) args;
	double Seq_time_Read_Elapsed,Seq_time_Write_Elapsed,Rand_time_Read_Elapsed,Rand_time_Write_Elapsed;
	float throughput=0.0,latency=0.0;
	int i,j,k;
	char *readBlockBuffer = (char*) malloc(sizeof(char)*(num_Bytes));
	char *writeBlockBuffer = (char*) malloc(sizeof(char)*(num_Bytes));
	double time_1, time_2;

	int writef;

	CreateFile();
	for(i=0 ; i < num_Bytes ; i++)
	{
		//Initialize to random character
		int n = rand() % 26;
		char c = (char)(n+65);
		readBlockBuffer[i] = c;
	}

	for(j=0 ; j < num_Bytes ; j++)
	{
		//Initialize to random character
		int n1 = rand() % 10;
		char c1 = (char)(n1+65);
		writeBlockBuffer[i] = c1;
	}

	// Writing random data to the text file

	for(i=0;i<number_of_operations; i++)
	{
		writef = fwrite(readBlockBuffer, 1 , num_Bytes , fp);
		if(writef < num_Bytes)
		{
			fclose(fp);
			printf("Write for txt1 file failed\n");
		}
	}
	fsync(fp);

	if(access_str == 'S')
	{
		if(strcmp(file_mode, "Read") == 0)
		{
			fseek(fp,0,SEEK_SET);

			gettimeofday(&tm, NULL);
			time_1=tm.tv_sec + (tm.tv_usec/1000000.0);

			for(k=0 ; k<number_of_operations ; k++)
			{
				fread(readBlockBuffer,1,num_Bytes,fp);
			}
			gettimeofday(&tm, NULL);
			time_2=tm.tv_sec + (tm.tv_usec/1000000.0);

			Seq_time_Read_Elapsed = time_2-time_1;
			throughput = calculate_throughput(Seq_time_Read_Elapsed);
			latency = 1/throughput;
			seq_thrPut_Read_array[threadID] = throughput;
			seq_latency_Read_array[threadID] = latency;

		}
		else if(strcmp(file_mode, "Write") == 0)
		{
			fseek(fp,0,SEEK_SET);

			gettimeofday(&tm, NULL);
			time_1=tm.tv_sec + (tm.tv_usec/1000000.0);

			for(k=0 ; k<number_of_operations ; k++)
			{
				fwrite(writeBlockBuffer,1,num_Bytes,fp);
				fsync(fp);
			}

			gettimeofday(&tm, NULL);
			time_2=tm.tv_sec + (tm.tv_usec/1000000.0);

			Seq_time_Write_Elapsed = time_2-time_1;
			throughput = calculate_throughput(Seq_time_Write_Elapsed);
			latency = 1/throughput;
			seq_thrPut_Write_array[threadID] = throughput;
			seq_latency_Write_array[threadID] = latency;
		}
	}
	else if(access_str == 'R')
	{
		int rand_num;
		int tempMod;
		if(strcmp(file_mode, "Read") == 0)
		{

			fseek(fp,0,SEEK_SET);

			gettimeofday(&tm, NULL);
			time_1=tm.tv_sec + (tm.tv_usec/1000000.0);

			/*if(number_of_operations < 100)
				tempMod = 1000;
			else
				tempMod = number_of_operations;*/

			for(k=0 ; k<number_of_operations ; k++)
			{
				rand_num = rand()%number_of_operations;
				fseek(fp,rand_num*num_Bytes,SEEK_SET);
				fread(readBlockBuffer,1,num_Bytes,fp);
			}

			gettimeofday(&tm, NULL);
			time_2=tm.tv_sec + (tm.tv_usec/1000000.0);

			Rand_time_Read_Elapsed = time_2-time_1;
			throughput = calculate_throughput(Rand_time_Read_Elapsed);
			latency = 1/throughput;
			rand_thrPut_Read_array[threadID] = throughput;
			rand_latency_Read_array[threadID] = latency;
		}
		else if(strcmp(file_mode, "Write") == 0)
		{
			fseek(fp,0,SEEK_SET);
			gettimeofday(&tm, NULL);
			time_1=tm.tv_sec + (tm.tv_usec/1000000.0);


			for(k=0 ; k<number_of_operations ; k++)
			{
				rand_num = rand()%number_of_operations;
				fseek(fp,rand_num*num_Bytes,SEEK_SET);
				fwrite(writeBlockBuffer,1,num_Bytes,fp);
				fsync(fp);
			}

			gettimeofday(&tm, NULL);
			time_2=tm.tv_sec + (tm.tv_usec/1000000.0);

			Rand_time_Write_Elapsed = time_2-time_1;
			throughput = calculate_throughput(Rand_time_Write_Elapsed);
			latency = 1/throughput;
			rand_thrPut_Write_array[threadID] = throughput;
			rand_latency_Write_array[threadID] = latency;
		}
	}

}

void processThreads_disk(int numThread, char str, char mode[5], long numOperations)
{
	int blnSuccess;

	float resultThroughput=0.0,resultLatency=0.0;
	access_str = str;
	number_of_operations=numOperations;
	strcpy(file_mode, mode);
	pthread_t arrThreads[numThread];

	//create threads and perform read / write operations accessing the file sequentially / randomly;
	int i,j=0;
	for(i=0;i<numThread;i++)
	{

		blnSuccess = pthread_create(&arrThreads[i], NULL, ComputeBenchmark_disk, (void*) i);
		if (blnSuccess)
		{
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
		if(strcmp(file_mode, "Read") == 0)
		{
			resultThroughput=find_average_throughput(numThread,'R');
			resultLatency=find_average_latency(numThread,'R');
			printf("Sequential Access with Read Operation having %d thread => Throughput : %f , Latency : %0.9f\n", numThread, resultThroughput , resultLatency);
		}

		if(strcmp(file_mode, "Write") == 0)
		{
			resultThroughput=find_average_throughput(numThread,'W');
			resultLatency=find_average_latency(numThread,'W');
			printf("Sequential Access with Write Operation having %d thread => Throughput : %f , Latency : %0.9f\n", numThread, resultThroughput , resultLatency);
		}
	}
	else if(access_str == 'R')
	{
		if(strcmp(file_mode, "Read") == 0)
		{
			resultThroughput=find_average_throughput(numThread,'R');
			resultLatency=find_average_latency(numThread,'R');
			printf("Random Access with Read Operation having %d thread => Throughput : %f , Latency : %0.9f\n", numThread, resultThroughput , resultLatency);
		}

		if(strcmp(file_mode, "Write") == 0)
		{
			resultThroughput=find_average_throughput(numThread,'W');
			resultLatency=find_average_latency(numThread,'W');
			printf("Random Access with Write Operation having %d thread => Throughput : %f , Latency : %0.9f\n", numThread, resultThroughput , resultLatency);
		}
	}
}

// Create file to read/write
void CreateFile()
{
	fp = fopen("txt1.txt","w+");
}

int main()
{
	int no_of_threads;

	printf("\n\n DISK BENCHMARK \n\n");
	//For 1 thread with block size 1 Byte  - Read Operation
	no_of_threads = 1;
	num_Bytes = 1;
	processThreads_disk(no_of_threads,'S',"Read", 40000000);
	processThreads_disk(no_of_threads,'R',"Read", 40000000);

	//For 1 thread with block size 1 Byte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40000000);
	processThreads_disk(no_of_threads,'R',"Write", 40000000);

	//For 2 thread with block size 1 Byte  - Read Operation
	no_of_threads = 2;
	processThreads_disk(no_of_threads,'S',"Read", 40000000);
	processThreads_disk(no_of_threads,'R',"Read", 40000000);

	//For 2 thread with block size 1 Byte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40000000);
	processThreads_disk(no_of_threads,'R',"Write", 40000000);


	//For 1 thread with block size 1 KByte  - Read Operation
	no_of_threads = 1;
	num_Bytes = 1*1024;
	processThreads_disk(no_of_threads,'S',"Read", 40000);
	processThreads_disk(no_of_threads,'R',"Read", 40000);

	//For 1 thread with block size 1 KByte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40000);
	processThreads_disk(no_of_threads,'R',"Write", 40000);

	//For 2 thread with block size 1 KByte  - Read Operation
	no_of_threads = 2;
	processThreads_disk(no_of_threads,'S',"Read", 40000);
	processThreads_disk(no_of_threads,'R',"Read", 40000);

	//For 2 thread with block size 1 KByte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40000);
	processThreads_disk(no_of_threads,'R',"Write", 40000);

	//For 1 thread with block size 1 Mb  - Read Operation
	no_of_threads = 1;
	num_Bytes = 1*1024*1024;
	processThreads_disk(no_of_threads,'S',"Read", 40);
	processThreads_disk(no_of_threads,'R',"Read", 40);

	//For 1 thread with block size 1 KByte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40);
	processThreads_disk(no_of_threads,'R',"Write", 40);

	//For 2 thread with block size 1 Mb  - Read Operation
	no_of_threads = 2;
	processThreads_disk(no_of_threads,'S',"Read", 40);
	processThreads_disk(no_of_threads,'R',"Read", 40);

	//For 2 thread with block size 1 KByte  - Write Operation
	processThreads_disk(no_of_threads,'S',"Write", 40);
	processThreads_disk(no_of_threads,'R',"Write", 40);
	printf("\n\n \n\n");
	fclose(fp);
	pthread_exit(NULL);
	return 1;
}
