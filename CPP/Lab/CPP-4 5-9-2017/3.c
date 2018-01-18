#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

typedef struct queue
{
	int info;
	struct queue *link;
}queue;

sem_t notEmpty, notFull;
int t = 0,s;

queue *front,*rear;

queue *getnode()
{
	queue *t;
	t = malloc(sizeof(queue));
	return t;
}

void addq(int item)
{
	queue *new;
	new = getnode();
	if( new==NULL )
	{
		printf("\nOverflow\n");
		return;
	}

	new->info = item;
	new->link = NULL;

	if( front == NULL )
		front = rear = new;
	else
	{
		rear->link = new;
		rear = new;
	}
}

int delq()
{
	int item;
	if( front == NULL )
	{
		printf("\nUnderflow\n");
		return NULL;
	}

	queue *ptr;
	ptr = front;
	if( front == rear )
		front = rear = NULL;
	else
		front = front->link;

	item = ptr->info;
	free(ptr);
	return item;
}

void *producer(void *arg)
{
    while(1)
    {
        sleep(1);
        printf("Produced packet %d\n", ++t);

        sem_wait(&notFull);

        printf("Appended packet. Buffer size = %d\n", --s);

        sem_post(&notEmpty);
    }
}

void *consumer(void *arg)
{
    while(1)
    {
        sem_wait(&notEmpty);

        printf("Removing packet. Buffer size = %d\n", ++s);

        sem_post(&notFull);

        sleep(1);
        printf("Consumed packet %d\n", t--);
    }
}

void main()
{
    front = rear = NULL;
    int n;

	printf("Enter Buffer Size: ");
	scanf("%d", &n);

    s=n;

	sem_init(&notEmpty,0,0);
	sem_init(&notFull,0,n);

	pthread_t x,y;

	pthread_create(&x, NULL, producer, NULL);
	pthread_create(&y, NULL, consumer, NULL);

    pthread_join(x, NULL);
    pthread_join(y, NULL);
}
