#include<stdio.h>
#include<pthread.h>

#define NUM_PRINTER 3
#define MAX 20

int pri_que[MAX];
int priority[MAX];
int front, rear;
int pLock[3];
int ppri[3];
int n;

pthread_mutex_t monitor;
pthread_cond_t printerAvail;

void insert(int);
void delete(int);
void check(int);
 
int acquire(int p)
{
	pthread_mutex_lock(&monitor);
		int prid,i;
	
		if(pLock[0] && pLock[1] && pLock[2])
		{
			insert(priority[p]);
			pthread_cond_wait(&printerAvail, &monitor);
			int z = pri_que[0];
			delete(z);
			for(i=0; i<n; i++)
			{
				if(priority[i]==z)
					break;
			}
			p = i;
		}
	
		if(!pLock[0])
		{
			prid=0;
			ppri[0] = p;
		}
		else if(!pLock[1])
		{
			prid=1;
			ppri[1] = p;
		}
		else if(!pLock[2])
		{
			prid=2;
			ppri[2] = p;
		}
	
		pLock[prid] = 1;
		printf("    Process %d acquired printer\n", p+1);
		
	pthread_mutex_unlock(&monitor);
	
	return prid;
}

void release(int p)
{
	pthread_mutex_lock(&monitor);
		int i;
		for(i=0; i<3; i++)
		{
			if(ppri[i]==p)
				break;
		}
		pLock[i] = 0;
		pthread_cond_signal(&printerAvail);
	pthread_mutex_unlock(&monitor);
}

/*
void sync(int *a,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(pLock[0] && pLock[1] && pLock[2])
		{
			if(ppri[0]<=ppri[1] && ppri[0]<=ppri[2])
				release(0);
			else if(ppri[1]<=ppri[2] && ppri[1]<=ppri[0])
				release(1);
			else if(ppri[2]<=ppri[1] && ppri[2]<=ppri[0])
				release(2);
			
			printf("Printer %d is Assigned to process %d", acquire(i,a[i]), i+1);
		}
		else
			printf("Printer %d is Assigned to process %d", acquire(i,a[i]), i+1);
	}
}
*/

void *print(void *arg)
{
	int y = (int)arg;
	
	while(1)
	{
		printf("    Process %d : Requested printer\n", y+1);		
		
		acquire(y);
		
		sleep(2);

		printf("    Process %d : released printer\n", y+1);
		release(y);
				
		sleep(1);
	}
}

int main()
{
	int i;
	
	front = rear = -1;
	
	for(i=0; i<NUM_PRINTER; i++)
	{
		pLock[i] = 0;
	}
	
	printf("Enter the number of process: ");
	scanf("%d", &n);
	
	for(i=0; i<n; i++)
	{
		printf("Enter the priority of %d processes: ", i+1);
		scanf("%d", &priority[i]);
	}
	
	pthread_mutex_init(&monitor, NULL);
	pthread_cond_init(&printerAvail, NULL);
	
	pthread_t tid[n];
	for(i=0;i<n;i++)
		pthread_create(&tid[i], NULL, print, (void*)i);
		
	for(i=0;i<n;i++)
		pthread_join(tid[i],NULL);
	
	//sync(a,n);
}



void insert(int data)
{
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        pri_que[rear] = data;
        return;
    }    
    else
        check(data);
    rear++;
}
 
/* Function to check priority and place element */
void check(int data)
{
    int i,j;
 
    for (i = 0; i <= rear; i++)
    {
        if (data >= pri_que[i])
        {
            for (j = rear + 1; j > i; j--)
            {
                pri_que[j] = pri_que[j - 1];
            }
            pri_que[i] = data;
            return;
        }
    }
    pri_que[i] = data;
}
 
/* Function to delete an element from queue */
void delete(int data)
{
    int i;
 
    if ((front==-1) && (rear==-1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }
 
    for (i = 0; i <= rear; i++)
    {
        if (data == pri_que[i])
        {
            for (; i < rear; i++)
            {
                pri_que[i] = pri_que[i + 1];
            }
 
        pri_que[i] = -99;
        rear--;
 
        if (rear == -1) 
            front = -1;
        return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}






