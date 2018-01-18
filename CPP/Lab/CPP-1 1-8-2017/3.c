#include<stdio.h>
#include<pthread.h>

typedef struct student
{
	char name[20];
	char id[10];
	int subject[6];
} student;

void *percentage(void *s)
{
	student *t = (student*)s;
	
	printf("\n\n---------Percentage Thread started---------\n\n");
	
	int i;
	float sum=0;
	
	for(i=0; i<6; i++)
		sum += t->subject[i];	
		
	printf("Percentage of %s(%s) = %.2f\n", t->name, t->id, sum/6 );
	
	printf("\n\n---------Percentage Thread terminated---------\n\n");
}

void main()
{
	printf("\n\n---------Main thread started---------\n\n");
	
	int i,n,j;	
	
	printf("Enter number of students: ");
	scanf("%d", &n);
	
	student s[n];
	pthread_t tid[n];
	
	for(j=0; j<n; j++)
	{
		printf("Enter details for student %d\n", j+1);
		
		printf("Enter Student Name: ");
		scanf("%s", s[j].name);
	
		printf("Enter %s's ID: ", s[j].name);
		scanf("%s", s[j].id);
	
		printf("Enter Marks for Subject:\n");
		for(i=0; i<6; i++)
		{
			printf("%d --> ", i+1);
			scanf("%d", &s[j].subject[i]);
		}
	
		
	
		pthread_create(&tid[j], NULL, percentage, &s[j]);
		pthread_join(tid[j], NULL);
	}
	
	pthread_exit(NULL);
	printf("\n\n---------Main thread terminated---------\n");
}
