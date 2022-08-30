/* ********************************************************************
* File:       simulator.c
* Author:     G.G.T.Shashen
* Created:    05/05/2022
* Modified:   13/05/2022
* Desc:       Simulator implementation to use POSIX threads to run the program
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "simulator.h"
#include "fileIO.h"

/* declare mutexes and conditional variables */
pthread_mutex_t algo_mutex;
pthread_mutex_t read_mutex;
pthread_cond_t data;
pthread_cond_t conA;
pthread_cond_t conB;
pthread_cond_t conC;
pthread_cond_t conD;
pthread_cond_t conE;
pthread_cond_t conF;

/* declare global variables and buffers*/
int cont;
int boolA;
int boolB;
int boolC;
int boolD;
int boolE;
int boolF;

struct fileData
{
    int *array;
    int nCylinders;
    int curPos;
    int prePos;
    int nData;
}buffer1;

int buffer2;

/* main method implementation */
int main(int argc, char * argv[])
{
    char *fileName;
    /* declare threads */
    pthread_t A;
    pthread_t B;
    pthread_t C;
    pthread_t D;
    pthread_t E;
    pthread_t F;
    /* initialize mutexes and conditional variables */
    pthread_mutex_init(&algo_mutex, NULL);
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init(&data, NULL);
    pthread_cond_init(&conA, NULL);
    pthread_cond_init(&conB, NULL);
    pthread_cond_init(&conC, NULL);
    pthread_cond_init(&conD, NULL);
    pthread_cond_init(&conE, NULL);
    pthread_cond_init(&conF, NULL);
    cont = 1;
    fileName = (char*)malloc(sizeof(int)*10);
    /* do while loop to run the program until QUIT */
    do
    {
        printf("\nDisk Scheduler Simulation : ");
        scanf("%s", fileName);
        /* check if the file exists and user didn't enter QUIT */
        if (access( fileName, F_OK ) != 0 && strcmp( fileName, "QUIT") != 0)
        {
            printf("\n[WARNING] : File does not exist!\n");
        }else 
        {
            /* initialize data */
            buffer1.nCylinders = 0;
            buffer1.curPos = 0;
            buffer1.prePos = 0;
            buffer1.nData = 0;
            buffer2 = 0;
            boolA = 0;
            boolB = 0;
            boolC = 0;
            boolD = 0;
            boolE = 0;
            boolF = 0;
            /* check if input is QUIT to end the loop */
            if (strcmp( fileName, "QUIT") == 0)
            {
                cont = 0;
                /* create and join threads */
                pthread_create(&A, NULL, fcfs, NULL);
                pthread_create(&B, NULL, sstf, NULL);
                pthread_create(&C, NULL, scan, NULL);
                pthread_create(&D, NULL, cscan, NULL);
                pthread_create(&E, NULL, look, NULL);
                pthread_create(&F, NULL, clook, NULL);
                pthread_join(A, NULL);
                pthread_join(B, NULL);
                pthread_join(C, NULL);
                pthread_join(D, NULL);
                pthread_join(E, NULL);
                pthread_join(F, NULL);
                
            }else 
            {
                /* call apropriate methods to get the file data and run the algorithms */
                /* create threads */
                pthread_create(&A, NULL, fcfs, NULL);
                pthread_create(&B, NULL, sstf, NULL);
                pthread_create(&C, NULL, scan, NULL);
                pthread_create(&D, NULL, cscan, NULL);
                pthread_create(&E, NULL, look, NULL);
                pthread_create(&F, NULL, clook, NULL);
                printf("For %s:\n", fileName);

                /* signal algorithms to read data */
                pthread_mutex_lock(&algo_mutex);
                fileRead(fileName, &buffer1.array, &buffer1.nCylinders, &buffer1.curPos, &buffer1.prePos, &buffer1.nData);
                pthread_cond_signal(&data);
                pthread_mutex_unlock(&algo_mutex);

                
                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conA,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);
            
                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conB,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);

                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conC,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);

                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conD,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);

                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conE,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);

                pthread_mutex_lock(&read_mutex);
                /* waiting until buffer2 data returns */
                if (buffer2 == 0)
                {
                    pthread_cond_wait(&conF,&read_mutex);
                }
                pthread_mutex_unlock(&read_mutex);

                /* join threads */
                pthread_join(A, (void**)&boolA);
                pthread_join(B, (void**)&boolB);
                pthread_join(C, (void**)&boolC);
                pthread_join(D, (void**)&boolD);
                pthread_join(E, (void**)&boolE);
                pthread_join(F, (void**)&boolF);

                /* print the result */
                printf("FCFS : %d\n",boolA);
                printf("SSTF : %d\n",boolB);
                printf("SCAN : %d\n",boolC);
                printf("CSCAN : %d\n",boolD);
                printf("LOOK : %d\n",boolE);
                printf("CLOOK : %d\n",boolF);

            }  
        }
    } while (cont == 1);
    /* destroy mutexes and conditional variables */
    pthread_mutex_destroy(&algo_mutex);
    pthread_mutex_destroy(&read_mutex);
    pthread_cond_destroy(&data);
    pthread_cond_destroy(&conA);
    pthread_cond_destroy(&conB);
    pthread_cond_destroy(&conC);
    pthread_cond_destroy(&conD);
    pthread_cond_destroy(&conE);
    pthread_cond_destroy(&conF);
    /* free malloc data */
    free(fileName);
    free(buffer1.array);
    return 0;
}

/* function to calculate the First Come First Server algorithm */
void* fcfs(void *arg)
{
    /* declare & initialize variables */
    int tot;
    int i;
    int j;
    int *arrayC;
    int size;

    i = 0;
    j = 0;
    size = 0;
    tot = 0;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }

    /* waiting for file data from parent thread */
    pthread_mutex_lock(&algo_mutex);
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    size = buffer1.nData + 1;
    arrayC = (int*)malloc(sizeof(int)*size);

    /* add current position to the first array index */
    arrayC[0] = buffer1.curPos;


    /* add the cylinder sequence from the original array to array copy */
    for ( i = 1; i <= buffer1.nData; i++)
    {
        arrayC[i] = buffer1.array[i-1];
    }

    /* calculate the cynlinders previous prosition and current position 
    according to the sequence */
    for ( j = 0; j < buffer1.nData; j++)
    {
        if (arrayC[j] > arrayC [j+1])
        {
            tot = tot + (arrayC[j] - arrayC[j+1]);
        }else
        {
            tot = tot + (arrayC[j+1] - arrayC[j]);
        }
    }

    pthread_mutex_unlock(&algo_mutex);
    
    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conA);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayC);

    return (void*)buffer2;
}

/* function to calculate the Shortest Seek Time First algorithm */
void* sstf(void *arg)
{
    /* declare and initialize variables */
    int i;
    int j;
    int temp;
    int head;
    int tot;
    int *arrayC;
    int *swap;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }
    temp = 0;
    tot = 0;

    pthread_mutex_lock(&algo_mutex);

    /* waiting for file data from parent thread */
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    arrayC = (int*)malloc(sizeof(int)*buffer1.nData);
    swap = (int*)malloc(sizeof(int)*buffer1.nData);

    /* copy data from original array to array copy */
    for ( i = 0; i < buffer1.nData; i++)
    {
        arrayC[i] = buffer1.array[i];
    }
    
    /* initialize head with the current position from the file */
    head = buffer1.curPos;

    /* calculate the difference between the sequence and add to another array */
    for( i = 0; i < buffer1.nData; i++)
    {
        if (head > arrayC[i])
        {
            swap[i] = head - arrayC[i];
        }else
        {
            swap[i] = arrayC[i] - head;
        }
    }

    /* sort the array according to the difference of the sequence */
    for( i = 0; i < buffer1.nData; i++)
    {
        for( j = i+1; j < buffer1.nData; j++)
        {
            if(swap[i] > swap[j])
            {
                temp = swap[i];
                swap[i] = swap[j];
                swap[j] = temp;
                temp = arrayC[i];
                arrayC[i] = arrayC[j];
                arrayC[j] = temp;
            }
        }
    }
    
    /* calculate the cylinder movement time for each sorted sequence */  
    for( i = 0; i < buffer1.nData; i++)
    {
        if (head > arrayC[i])
        {
            tot = tot + head - arrayC[i];
        }else
        {
            tot = tot + arrayC[i] - head;
        }
        head = arrayC[i];
    }

    pthread_mutex_unlock(&algo_mutex);

    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conB);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayC);
    free(swap);
    
    return (void*) buffer2;
    
}

/* function to calculate the SCAN algorithm */
void* scan(void *arg)
{
    /* declare and initialize variables */
    int i;
    int j;
    int count1;
    int count2;
    int temp;
    int head;
    int tot;
    int *arrayR;
    int *arrayL;
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }
    
    pthread_mutex_lock(&algo_mutex);
    /* waiting for file data from parent thread */
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    arrayR = (int*)malloc(sizeof(int)*buffer1.nData);
    arrayL = (int*)malloc(sizeof(int)*buffer1.nData);

    /* initialize head with the current position from the file */
    head = buffer1.curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < buffer1.nData; i++)
    {
        if (buffer1.curPos <= buffer1.array[i])
        {
            arrayR[count1] = buffer1.array[i];
            count1++;
        }else
        {
            arrayL[count2] = buffer1.array[i];
            count2++;
        }
    }

    /* sort the greater array as acsending  */
    for( i = 0; i < count1; i++)
    {
        for( j = i; j < count1; j++)
        {
            if(arrayR[i] > arrayR[j])
            {
                temp = arrayR[i];
                arrayR[i] = arrayR[j];
                arrayR[j] = temp;
            }
        }
    }

    /* sort the lesser array as acsending */
    for( i = 0; i < count2; i++)
    {
        for( j = i; j < count2; j++)
        {
            if(arrayL[i] > arrayL[j])
            {
                temp = arrayL[i];
                arrayL[i] = arrayL[j];
                arrayL[j] = temp;
            }
        }
    }

    /* check and calculate the cylinders according the current poisition
    and previous position */
    if (buffer1.curPos >= buffer1.prePos)
    {
        /* calculate the cylinder movement time for greater sequence first 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }

        /* reach the end of the cylinder and add */
        tot = tot + ((buffer1.nCylinders - 1) - head);
        /* make the current head the end of the cylinders */
        head = buffer1.nCylinders - 1;

        /* calculate the cylinder movement time for lesser sequence last 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

    }else
    {
        /* calculate the cylinder movement time for lesser sequence first 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

        /* add the head to the total time */
        tot = tot + head;
        head = 0;

        /* calculate the cylinder movement time for greater sequence last 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }
    }

    pthread_mutex_unlock(&algo_mutex);

    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conC);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    return (void*) buffer2;
    
}

/* function to calculate the CSCAN algorithm */
void* cscan(void *arg)
{
    /* declare and initialize variables */
    int i;
    int j;
    int count1;
    int count2;
    int temp;
    int head;
    int tot;
    int *arrayR;
    int *arrayL;
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }

    pthread_mutex_lock(&algo_mutex);
    /* waiting for file data from parent thread */
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    arrayR = (int*)malloc(sizeof(int)*buffer1.nData);
    arrayL = (int*)malloc(sizeof(int)*buffer1.nData);
    
    /* initialize head with the current position from the file */
    head = buffer1.curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < buffer1.nData; i++)
    {
        if (buffer1.curPos <= buffer1.array[i])
        {
            arrayR[count1] = buffer1.array[i];
            count1++;
        }else
        {
            arrayL[count2] = buffer1.array[i];
            count2++;
        }
    }

    /* sort the greater array as acsending  */
    for( i = 0; i < count1; i++)
    {
        for( j = i; j < count1; j++)
        {
            if(arrayR[i] > arrayR[j])
            {
                temp = arrayR[i];
                arrayR[i] = arrayR[j];
                arrayR[j] = temp;
            }
        }
    }

    /* sort the lesser array as acsending  */
    for( i = 0; i < count2; i++)
    {
        for( j = i; j < count2; j++)
        {
            if(arrayL[i] > arrayL[j])
            {
                temp = arrayL[i];
                arrayL[i] = arrayL[j];
                arrayL[j] = temp;
            }
        }
    }

    /* check and calculate the cylinders according the current poisition
    and previous position */
    if (buffer1.curPos >= buffer1.prePos)
    {
        /* calculate the cylinder movement time for greater sequence first 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }

        /* add the difference of last head and the last cylinder to the total*/
        tot = tot + ((buffer1.nCylinders - 1) - head);
        head = 0;
        /* reach the end of the cylinder and add */
        tot = tot + (buffer1.nCylinders - 1);

        /* calculate the cylinder movement time for lesser sequence last 
        from first array index to last */
        for ( i = 0; i < count2; i++)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }
    }else
    {
        /* calculate the cylinder movement time for lesser sequence first 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

        /* add the current head to the total */
        tot = tot + head;
        /* make head the last cylinder */
        head = buffer1.nCylinders - 1;
        /* add the last cylinder to the total */
        tot = tot + (buffer1.nCylinders - 1);

        /* calculate the cylinder movement time for greater sequence last 
        from last array index to first */
        for ( i = count1-1; i >= 0; i--)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }
    }

    pthread_mutex_unlock(&algo_mutex);

    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conD);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    return (void*) buffer2;
    
}

/* function to calculate the LOOK algorithm */
void* look(void *arg)
{
    /* declare and initialize variables */
    int i;
    int j;
    int count1;
    int count2;
    int temp;
    int head;
    int tot;
    int *arrayR;
    int *arrayL;
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }

    pthread_mutex_lock(&algo_mutex);
    /* waiting for file data from parent thread */
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    arrayR = (int*)malloc(sizeof(int)*buffer1.nData);
    arrayL = (int*)malloc(sizeof(int)*buffer1.nData);

    /* initialize head with the current position from the file */
    head = buffer1.curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < buffer1.nData; i++)
    {
        if (buffer1.curPos <= buffer1.array[i])
        {
            arrayR[count1] = buffer1.array[i];
            count1++;
        }else
        {
            arrayL[count2] = buffer1.array[i];
            count2++;
        }
    }

    /* sort the greater array as acsending  */
    for( i = 0; i < count1; i++)
    {
        for( j = i; j < count1; j++)
        {
            if(arrayR[i] > arrayR[j])
            {
                temp = arrayR[i];
                arrayR[i] = arrayR[j];
                arrayR[j] = temp;
            }
        }
    }

    /* sort the lesser array as acsending  */
    for( i = 0; i < count2; i++)
    {
        for( j = i; j < count2; j++)
        {
            if(arrayL[i] > arrayL[j])
            {
                temp = arrayL[i];
                arrayL[i] = arrayL[j];
                arrayL[j] = temp;
            }
        }
    }

    /* check and calculate the cylinders according the current poisition
    and previous position */
    if (buffer1.curPos >= buffer1.prePos)
    {
        /* calculate the cylinder movement time for greater sequence first 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }

        /* calculate the cylinder movement time for lesser sequence last 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

    }else
    {
        /* calculate the cylinder movement time for lesser sequence first 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

        /* calculate the cylinder movement time for greater sequence last 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }
    }

    pthread_mutex_unlock(&algo_mutex);

    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conE);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    return (void*) buffer2;
    
}

/* function to calculate the CLOOK algorithm */
void* clook(void *arg)
{
    /* declare and initalize variables */
    int i;
    int j;
    int count1;
    int count2;
    int temp;
    int head;
    int tot;
    int *arrayR;
    int *arrayL;
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* terminate and print thread if the program quits */
    if (cont == 0)
    {
        printf("%lu has been terminated\n", pthread_self());
        return NULL;
    }

    pthread_mutex_lock(&algo_mutex);
    /* waiting for file data from parent thread */
    while (buffer1.nData == 0)
    {
        pthread_cond_wait(&data,&algo_mutex);
    }

    arrayR = (int*)malloc(sizeof(int)*buffer1.nData);
    arrayL = (int*)malloc(sizeof(int)*buffer1.nData);

    /* initialize head with the current position from the file */
    head = buffer1.curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < buffer1.nData; i++)
    {
        if (buffer1.curPos <= buffer1.array[i])
        {
            arrayR[count1] = buffer1.array[i];
            count1++;
        }else
        {
            arrayL[count2] = buffer1.array[i];
            count2++;
        }
    }

    /* sort the greater array as acsending  */
    for( i = 0; i < count1; i++)
    {
        for( j = i; j < count1; j++)
        {
            if(arrayR[i] > arrayR[j])
            {
                temp = arrayR[i];
                arrayR[i] = arrayR[j];
                arrayR[j] = temp;
            }
        }
    }

    /* sort the lesser array as acsending  */
    for( i = 0; i < count2; i++)
    {
        for( j = i; j < count2; j++)
        {
            if(arrayL[i] > arrayL[j])
            {
                temp = arrayL[i];
                arrayL[i] = arrayL[j];
                arrayL[j] = temp;
            }
        }
    }

    /* check and calculate the cylinders according the current poisition
    and previous position */
    if (buffer1.curPos >= buffer1.prePos)
    {
        /* calculate the cylinder movement time for greater sequence first 
        from first array index to last */
        for ( i = 0; i < count1; i++)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }

        /* calculate the cylinder movement time for lesser sequence last 
        from first array index to last */
        for ( i = 0; i < count2; i++)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }
    }else
    {
        /* calculate the cylinder movement time for lesser sequence first 
        from last array index to first */
        for ( i = count2-1; i >= 0; i--)
        {
            if (head > arrayL[i])
            {
                tot = tot + head - arrayL[i];
            }else
            {
                tot = tot + arrayL[i] - head;
            }
            head = arrayL[i];
        }

        /* calculate the cylinder movement time for greater sequence last
        from last array index to first */
        for ( i = count1-1; i >= 0; i--)
        {
            if (head > arrayR[i])
            {
                tot = tot + head - arrayR[i];
            }else
            {
                tot = tot + arrayR[i] - head;
            }
            head = arrayR[i];
        }
    }

    pthread_mutex_unlock(&algo_mutex);

    /* signal parent thread to read buffer2 data */
    pthread_mutex_lock(&read_mutex);
    buffer2 = tot;
    pthread_cond_signal(&conF);
    pthread_mutex_unlock(&read_mutex);

    /* wait until parent thread gives signal
    and signal following thread */
    pthread_mutex_lock(&algo_mutex);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&algo_mutex);

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    return (void*) buffer2;
}
