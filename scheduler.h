#ifndef SCHEDULER_H
#define SCHEDULER_H

void fcfs(int * array, int nCylinders, int nCurPos, int prePos, int nData);
void sstf(int * array, int nCylinders, int curPos, int prePos, int nData);
void scan(int * array, int nCylinders, int curPos, int prePos, int nData);
void cscan(int * array, int nCylinders, int curPos, int prePos, int nData);
void look(int * array, int nCylinders, int curPos, int prePos, int nData);
void clook(int * array, int nCylinders, int curPos, int prePos, int nData);

#endif