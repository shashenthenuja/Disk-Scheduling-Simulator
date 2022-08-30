/* *******************************************************************
* File:       scheduler.c
* Author:     G.G.T.Shashen
* Created:    29/04/2022
* Modified:   12/05/2022
* Desc:       Scheduler implementation to run scheduling algorithms
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "scheduler.h"
#include "fileIO.h"

/* main method implementation */
int main(int argc, char * argv[])
{
    char *fileName;
    int *array;
    int nCylinders;
    int curPos;
    int prePos;
    int nData;
    int cont;
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
            /* check if input is QUIT to end the loop */
            if (strcmp( fileName, "QUIT") == 0)
            {
                cont = 0;
            }else 
            {
                /* call apropriate methods to get the file data and run the algorithms */
                fileRead(fileName, &array, &nCylinders, &curPos, &prePos, &nData);
                printf("For %s:\n", fileName);
                fcfs(array, nCylinders, curPos, prePos, nData);
                sstf(array, nCylinders, curPos, prePos, nData);
                scan(array, nCylinders, curPos, prePos, nData);
                cscan(array, nCylinders, curPos, prePos, nData);
                look(array, nCylinders, curPos, prePos, nData);
                clook(array, nCylinders, curPos, prePos, nData);
            }
        }  
    } while (cont == 1);

    /* free malloc data */
    free(array);
    free(fileName);

    return 0;
}

/* funtion to calculate the First Come First Server algorithm */
void fcfs(int * array, int nCylinders, int curPos, int prePos, int nData)
{
    /* declare & initialize variables */
    int tot;
    int i;
    int j;
    int size;
    int *arrayC;

    i = 0;
    j = 0;
    tot = 0;
    size = 0;

    size = nData + 1;
    arrayC = (int*)malloc(sizeof(int)*size);
    /* add current position to the first array index */
    arrayC[0] = curPos;

    /* add the cylinder sequence from the original array to array copy */
    for ( i = 1; i <= nData; i++)
    {
        arrayC[i] = array[i-1];
    }

    /* calculate the cynlinders previous prosition and current position 
    according to the sequence */
    for ( j = 0; j <= nData-1; j++)
    {
        if (arrayC[j] > arrayC [j+1])
        {
            tot = tot + (arrayC[j] - arrayC[j+1]);
        }else
        {
            tot = tot + (arrayC[j+1] - arrayC[j]);
        }
    }

    /* free malloc data */
    free(arrayC);
    
    /* print the calculated total */
    printf("FCFS : %d\n",tot);
}

/* funtion to calculate the Shortest Seek Time First algorithm */
void sstf(int * array, int nCylinders, int curPos, int prePos, int nData)
{
    /* declare and initialize variables */
    int i;
    int j;
    int temp;
    int head;
    int tot;
    int *arrayC;
    int *data;
    arrayC = (int*)malloc(sizeof(int)*nData);
    data = (int*)malloc(sizeof(int)*nData);
    temp = 0;
    tot = 0;

    /* copy data from original array to array copy */
    for ( i = 0; i < nData; i++)
    {
        arrayC[i] = array[i];
    }
    
    /* initialize head with the current position from the file */
    head = curPos;

    /* calculate the difference between the sequence and add to another array */
    for( i = 0; i < nData; i++)
    {
        if (head > arrayC[i])
        {
            data[i] = head - arrayC[i];
        }else
        {
            data[i] = arrayC[i] - head;
        }
    }

    /* sort the array according to the difference of the sequence */
    for( i = 0; i < nData; i++)
    {
        for( j = i+1; j < nData; j++)
        {
            if(data[i] > data[j])
            {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
                temp = arrayC[i];
                arrayC[i] = arrayC[j];
                arrayC[j] = temp;
            }
        }
    }
    
    /* calculate the cylinder movement time for each sorted sequence */  
    for( i = 0; i < nData; i++)
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

    /* free malloc data */
    free(arrayC);
    free(data);

    /* print the calculated total */
    printf("SSTF : %d\n",tot);
    
}

/* funtion to caluclate the SCAN algorithm */
void scan(int * array, int nCylinders, int curPos, int prePos, int nData)
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
    arrayR = (int*)malloc(sizeof(int)*nData);
    arrayL = (int*)malloc(sizeof(int)*nData);
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* initialize head with the current position from the file */
    head = curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < nData; i++)
    {
        if (curPos <= array[i])
        {
            arrayR[count1] = array[i];
            count1++;
        }else
        {
            arrayL[count2] = array[i];
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
    if (curPos >= prePos)
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
        tot = tot + ((nCylinders - 1) - head);
        /* make the current head the end of the cylinders */
        head = nCylinders - 1;

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

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    /* print the calculated total */
    printf("SCAN : %d\n",tot);
    
}

/* funtion to calculate the CSCAN algorithm */
void cscan(int * array, int nCylinders, int curPos, int prePos, int nData)
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
    arrayR = (int*)malloc(sizeof(int)*nData);
    arrayL = (int*)malloc(sizeof(int)*nData);
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* initialize head with the current position from the file */
    head = curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < nData; i++)
    {
        if (curPos <= array[i])
        {
            arrayR[count1] = array[i];
            count1++;
        }else
        {
            arrayL[count2] = array[i];
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
    if (curPos >= prePos)
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
        tot = tot + ((nCylinders - 1) - head);
        head = 0;
        /* reach the end of the cylinder and add */
        tot = tot + (nCylinders - 1);

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
        head = nCylinders - 1;
        /* add the last cylinder to the total */
        tot = tot + (nCylinders - 1);

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

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    /* print the calculated total */
    printf("CSCAN : %d\n",tot);
    
}

/* function to calculate the LOOK algorithm */
void look(int * array, int nCylinders, int curPos, int prePos, int nData)
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
    arrayR = (int*)malloc(sizeof(int)*nData);
    arrayL = (int*)malloc(sizeof(int)*nData);
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* initialize head with the current position from the file */
    head = curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < nData; i++)
    {
        if (curPos <= array[i])
        {
            arrayR[count1] = array[i];
            count1++;
        }else
        {
            arrayL[count2] = array[i];
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
    if (curPos >= prePos)
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

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    /* print the calculated total */
    printf("LOOK : %d\n",tot);
    
}

/* function to calculate the CLOOK algorithm */
void clook(int * array, int nCylinders, int curPos, int prePos, int nData)
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
    arrayR = (int*)malloc(sizeof(int)*nData);
    arrayL = (int*)malloc(sizeof(int)*nData);
    count1 = 0;
    count2 = 0;
    temp = 0;
    tot = 0;

    /* initialize head with the current position from the file */
    head = curPos;

    /* seperate the cylinders which are greater and less than the current head */
    for ( i = 0; i < nData; i++)
    {
        if (curPos <= array[i])
        {
            arrayR[count1] = array[i];
            count1++;
        }else
        {
            arrayL[count2] = array[i];
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
    if (curPos >= prePos)
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

    /* free malloc data */
    free(arrayL);
    free(arrayR);

    /* print the calculated total */
    printf("CLOOK : %d\n",tot);
    
}