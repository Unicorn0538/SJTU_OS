
#include "task.h"


//extern struct node **head;

void insert(struct node **head,Task *t);
void delete(struct node **head,Task *t);

void add(char *name,int priority,int burst,int N);


void compute_time(int count,int *turnaround, int *waiting, int *response ); //计算平均时间


