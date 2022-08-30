#ifndef FILEIO_H
#define FILEIO_H
#define FALSE 0
#define TRUE !FALSE

void fileRead(char * fileName, int ** dataArr, int * nCylinders, int * curPos, int * prePos, int * nData);

#endif