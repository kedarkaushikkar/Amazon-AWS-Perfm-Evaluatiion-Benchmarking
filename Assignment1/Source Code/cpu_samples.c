#include<stdio.h>
#include <pthread.h>
#include <time.h>
#define MAX_TIME_THREAD(t1, t2) (((t1)>(t2))?(t1):(t2))

#define NUM_OPERATION 10000000.0

//float iops_array[10], flops_array[10];
float iops_time_array[10], flops_time_array[10];
int thread_val;
char dataType_str;


float max_time(numThread)
{
	float max_thread_time=0.0;
	int i;
	if(dataType_str=='I')
	{
		for(i=0;i<numThread;i++)
		{
			max_thread_time=MAX_TIME_THREAD(max_thread_time, iops_time_array[i]);
		}
	}
	else if(dataType_str=='F')
	{
		for(i=0;i<numThread;i++)
		{
			max_thread_time=MAX_TIME_THREAD(max_thread_time, flops_time_array[i]);
		}
	}
	return max_thread_time;
}

//main function to perform arithmatic operations over Integer and floating point instructions
void *ComputeBenchmark(void *args)
{
	clock_t timer_start,timer_end;
	float timer_elapsed;
	int threadID=(int) args;
	int i;
	int int_Value,int_Value1,int_Value2;
	float float_Value=0.0,float_Value1=1.1,float_Value2=2.2;
	double int_time_elapsed, float_time_elapsed;
	double iops_val,flops_val;
	int a=1, b=2, c=3;
	float a1=1.1, b1=2.1, c1=3.1;


	if(dataType_str == 'I')
	{
		timer_start = clock();
		for(i=0;i<NUM_OPERATION;i++)
		{
			500+510+520+530+540+550+560+570+580*2+590;
			700+710+720+730*2+740+750+760+770+780+790;
			900*2+910+920+930+940+950+960+970+980+990;

		}
		timer_end = clock();
		timer_elapsed=(double) timer_end-timer_start;
		int_time_elapsed = ((double)timer_elapsed) / CLOCKS_PER_SEC;
		iops_time_array[threadID]=int_time_elapsed;
	}
	else if(dataType_str == 'F')
	{
		timer_start = clock();
		for(i=0;i<NUM_OPERATION;i++)
		{

			0.500+5.10+5.20+5.30+5.40+5.50+5.60+5.70+5.80*0.2+5.90;
			0.700+7.10+7.20+7.30*0.2+7.40+7.50+7.60+7.70+7.80+7.90;
			0.900*0.2+9.10+9.20+9.30+9.40+9.50+9.60+9.70+9.80+9.90;
		}
		timer_end = clock();
		timer_elapsed=(double) timer_end-timer_start;
		float_time_elapsed = (double) timer_elapsed / CLOCKS_PER_SEC;
		flops_time_array[threadID]=float_time_elapsed;
	}
}

void processThreads(int numThread, char str)
{
	//defining clock header variables

	int blnSuccess;
	float resultSpeed=0.0,resultTime=0.0;
	dataType_str = str;

	pthread_t arrThreads[numThread];

	//create threads;
	int i,j=0;
	for(i=0;i<numThread;i++)
	{

		blnSuccess = pthread_create(&arrThreads[i], NULL, ComputeBenchmark, (void*) i);
		if (blnSuccess)
		{
			if(dataType_str == 'I')
				printf("Could not create thread for integer operation");

			if(dataType_str == 'F')
				printf("Could not create thread for Float operation");
		}
	}

	//join threads
	for(j=0;j<numThread;j++)
	{
		thread_val=j;
		pthread_join(arrThreads[j],NULL);
	}

	if(dataType_str == 'I')
	{
		//resultSpeed=find_average_speed(numThread);
		resultTime=max_time(numThread);
		resultSpeed=((NUM_OPERATION*(30+2)*numThread)/resultTime)/1000000000;
		printf("Number of GIOPS for %d threads => %0.9f , Time Taken : %f\n", numThread, resultSpeed , resultTime);

	}
	else if(dataType_str == 'F')
	{
		resultTime=max_time(numThread);
		resultSpeed=((NUM_OPERATION*(30+2)*numThread)/resultTime)/1000000000;
		printf("Number of GFLOPS for %d threads => %f , Time Taken : %f\n", numThread, resultSpeed , resultTime);
	}
}



int main()
{
	int no_of_threads;

	int i;
	no_of_threads = 4;
	printf("\n\n CPU BENCHMARKING 600 Samples \n\n");
	printf ( "Output for 600 Samples of GIOPS /n");

	for(i=0;i<600;i++)
	{
		processThreads(no_of_threads,'I');
		//processThreads(no_of_threads,'F');
	}

	printf ( "/n Output for 600 Samples of GFLOPS /n");

	for(i=0;i<600;i++)
	{
		//processThreads(no_of_threads,'I');
		processThreads(no_of_threads,'F');
	}
	printf("\n\n CPU BENCHMARKING 600 Samples \n\n");
	pthread_exit(NULL);
	return 1;
}
