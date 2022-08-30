# Disk Scheduling Simulator
This project is a command line program written in C programming language. This program is an implementation of a  system  to  simulate disk scheduling using algorithms such as FCFS, SSTF, SCAN, LOOK etc... Disk scheduling algorithms are used to minimize the seek time. From this program, you could see how each algorithm perform. The program is implemented using POSIX Pthreads and Pthread's mutex and cond for mutual exclusion.

## Algorithms
### FCFS - First Come First Server
FCFS is the simplest of all the Disk Scheduling Algorithms. In FCFS, the requests are addressed in the order they arrive in the disk queue

![FCFS](https://media.geeksforgeeks.org/wp-content/uploads/20200608201201/fcfs3.jpg) 

[Img Source - GeeksForGeeks]

### SSTF - Shortest Seek Time First
In SSTF (Shortest Seek Time First), requests having shortest seek time are executed first. So, the seek time of every request is calculated in advance in the queue and then they are scheduled according to their calculated seek time. As a result, the request near the disk arm will get executed first. SSTF is certainly an improvement over FCFS as it decreases the average response time and increases the throughput of system

![SSTF](https://media.geeksforgeeks.org/wp-content/uploads/20200608201702/sstf1.jpg)

[Img Source - GeeksForGeeks]

### SCAN
In SCAN algorithm the disk arm moves into a particular direction and services the requests coming in its path and after reaching the end of disk, it reverses its direction and again services the request arriving in its path. So, this algorithm works as an elevator and hence also known as **elevator algorithm.** As a result, the requests at the midrange are serviced more and those arriving behind the disk arm will have to wait

![SCAN](https://media.geeksforgeeks.org/wp-content/uploads/20200608202008/scan4.jpg)

[Img Source - GeeksForGeeks]

### CSCAN - Circular SCAN
In CSCAN algorithm the disk arm instead of reversing its direction goes to the other end of the disk and starts servicing the requests from there. So, the disk arm moves in a circular fashion and this algorithm is also similar to SCAN algorithm and hence it is known as C-SCAN (Circular SCAN)

![CSCAN](https://media.geeksforgeeks.org/wp-content/uploads/20200608202230/cscan1.jpg)

[Img Source - GeeksForGeeks]

### LOOK
It is similar to the SCAN disk scheduling algorithm except for the difference that the disk arm in spite of going to the end of the disk goes only to the last request to be serviced in front of the head and then reverses its direction from there only. Thus it prevents the extra delay which occurred due to unnecessary traversal to the end of the disk

![LOOK](https://media.geeksforgeeks.org/wp-content/uploads/20200608202613/look1.jpg)

[Img Source - GeeksForGeeks]

### CLOOK - Circular LOOK
In CLOOK, the disk arm in spite of going to the end goes only to the last request to be serviced in front of the head and then from there goes to the other end’s last request. Thus, it also prevents the extra delay which occurred due to unnecessary traversal to the end of the disk

![CLOOK](https://media.geeksforgeeks.org/wp-content/uploads/20200608202846/clook1.jpg)

[Img Source - GeeksForGeeks]

## Input File

The input file should contain the positions of the cylinders and the amount. See the example file below

**`200` `53` `65`** `98` `183` `37` `122` `14` `124` `65` `67`

The first number in the file represents **total cylinders** n of the disk i.e., n=200 cylinders. The second number represents **current position** of the disk’s read/write head, i.e., it is currently at cylinder 53. The third number represents the **previous disk request**, i.e., 65. Thus, from the information of previous disk request (65) and current position (53), we know the direction of the head’s movement, i.e., from 65 to 53, i.e., the head moves towards smaller cylinder numbers. Note that if the current position is 65, and previous request is 53, the head goes towards larger cylinder numbers. Each of the remaining numbers in the file represents cylinder number, i.e., a set of disk requests for sectors located in cylinders 98, 183, 37, 122, 14, 124, 65, and 67. Here, we assume that all disk requests come at the same time, and there is no further request. The simulator aims to generate a schedule to serve the requests that minimizes the movement of the disk’s read/write head, i.e., the seek time. Place the text file in the root directory with valid data (This is already done, see file `input`)

## How To Run The Program
 To run the program, open the directory in the terminal and run `make all` to compile the program, then run `simulator` . After the program launches, input the file name to run the algorithms and get the output. Type `QUIT` to terminate the program. Detailed instructions and explanations are provided in the ***Report.PDF*** 

### Indexed in Turn-It In Global Referencing Scheme

***This project should not be used for any coursework related activity and all codes have been submitted to `Turn-It In global referencing platform`, where usage of this code may be caught for Plagiarism.***
