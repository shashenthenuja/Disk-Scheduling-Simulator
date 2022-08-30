/* *********************************************************************
* File:       fileIO.c
* Author:     G.G.T.Shashen
* Created:    29/04/2022
* Modified:   10/05/2022
* Desc:       FileIO implementation to extract data from data file
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "fileIO.h"

/* funtion to read data from a given input file */
void fileRead(char * fileName, int ** dataArr, int * cylinders, int * cur, int * pre, int * nData)
{
    /* create file pointer and open file to read */
    FILE * fp = fopen(fileName,"r");
    /* error checking */
    if (fp == NULL)
    {
        perror("Error opening file");

    }else
    {
        /* create and initialize variables */
        int done = FALSE;
        int nRead;
        int count;
        int nCylinders;
        int curPos;
        int prePos;
        int *data;
        count = 0;
        /* malloc array to default size of 100 index */
        data = (int*)malloc(sizeof(int)*100);
        /* read the first 3 values of the file */
        nRead = fscanf(fp, "%d %d %d", &nCylinders, &curPos, &prePos);
        /* while loop to read and store the rest of the sequence until the line end */
        while (!done)
        {
            /* read each cylinder of the sequence */
            nRead = fscanf(fp, "%d", &data[count]);
            if (nRead != 1)
            {
                done = TRUE;
            }else
            {
                count++;
            }
            
        }
        /* store the read data to a pointer array and variables */
        *dataArr = data;
        *cylinders = nCylinders;
        *cur = curPos;
        *pre = prePos;
        *nData = count;
        /* error checking */
        if (ferror(fp))
        {
            perror("Error reading from file");
        }
        fclose(fp);
        fp = NULL;
    }
}