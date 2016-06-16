CS553 : CLOUD COMPUTING
PROGRAMMING ASSIGNMENT 1 - Benchmarking

Author :
1. Kedar Kaushikkar(CWID : A20355218) - kkaushik@hawk.iit.edu

The Assignment Directory contains following documents and folders:

1. Source Code of the program for Benchmarking: (Source Code)
2. Performance Evaluation Report.pdf
3. Design Document.pdf
4. Output Logs

1. Source Code folder
-This folder contains all the executables and source files to run the benchmarking code.
- The file listing is as follows,

->CPU Benchmarking : cpu.c
->CPU 600 Samples : cpu_samples.c
->DISK Benchmarking : disk.c
->MEMORY Benchmarking : memory.c
->Deliverables for all benchmarking
->'PA1_keypair.pem' file used to create amazon instance on which this benchmarking programs have been tested.
->'PA1_RUNME' bash script that will execute all the benchmarking deliverables present in the source code folder.
-> Readme.txt

******Steps to run the benchmarking*********
1) The Source Code folder contains the bash script(PA1_RUNME) that will run all the benchmarking in the order of CPU , CPU_600_Samples , MEMORY ,DISK.
2) To run the bash script , change the permissions of the bash script to read write all mode, using the below command

 =>  chmod 777 PA1_RUNME

3) Once the permissions are available , execute the bash script using below command

 => ./PA1_RUNME

********************************************

2. Performance_Evaluation_Report.pdf
- This file contains all the benchmarking results and data charts to analyze the benchmarking results.

3. Design_Document.pdf
- This file contains the structure and the logic used to implement the benchmarking program.
- It also contains certain measures to improve the accuracy of the benchmarking results

4. Output Logs.
- This folder contains the output logs generated during running those benchmarks on amazon t2 micro instance.
- THe results of those log files have been used to create data charts in the performance evaluation report.
