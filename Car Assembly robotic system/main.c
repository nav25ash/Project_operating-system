/*fcfs scheduler algorith*/

#include<stdio.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include "config.h"



//node *front,*rear,*processListFront,*processListRear;
//pthread_mutex_t m1;

int ItemList[10] = { 1,2,5,6,7,3,4,5,6,7 };
robotNode *robot[NUMBER_OF_ROBOTS];

void addProcessQ();
void deleteProcessQ();
void addq(node *);
// Adds list of packages ready to be processed
void delq();
// Remove the Package Number after the items are placed in the box
void display();
int isIteamsAvailable(int list[],int num);

void main()
{
    int temp1,temp2,i;
    
    pthread_t tid[2];  //Total number of treads created
    pthread_t Robotarm[NUMBER_OF_ROBOTS];
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&m1,NULL);  //Mutex for synchronization
    
    
    temp1=pthread_create(&tid[0],&attr,(void*)addProcessQ,NULL);
    
    temp2=pthread_create(&tid[1],&attr,(void*)deleteProcessQ,NULL);
    
    for(i=0;i<NUMBER_OF_ROBOTS;i++)
    {
        robot[i]=(robotNode*)malloc(sizeof(robot_arm));
        robot[i]->robotNo=i;
        temp1=pthread_create(&Robotarm[i],&attr,(void*) robot_arm,robot[i]);
        
    }
    
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    
}

void addProcessQ()
{
    
    while(1)
    {
        node *new;
        char str[]="0";
        int i;
        new=(node*)malloc(sizeof(node));
        printf("ENTER THE PACKAGE DETAILS\n");
        printf("Package Number\tTotal number of items\t List of Items\n");
        scanf("%d%d",&new->PackageNumber,&new->TotalItems);
        printf("total=%d\n",new->TotalItems);
        for(i=0;i < new->TotalItems;i++)
        {
            scanf("%d",new->ListofItems+i);
            //printf("list =%d\t",*(new->ListofItems+i));
            
        }
        new->Status=WAITING;
        new->ptr=NULL;
        
        
            //searching in items Q
            if(isIteamsAvailable(new->ListofItems,new->TotalItems) )
            {
                addq( new );
            }
        else
        {
        
        if(front==NULL && rear== NULL)
        {
            front=new;
        
        }
        else
        {
            rear->ptr=new;
        }
        rear=new;
        }
    }
    
}

void addq(node * ProcessPackage)
{
    //int option=1;
    //time_t end;
        node *new;
        new=ProcessPackage;
        new->Status=IN_PROCESS;
        new->ptr=NULL;
        pthread_mutex_lock(&m1);
        if(processListFront==NULL && processListRear== NULL)
        {
            processListFront=new;
        }
        else
        {
            processListRear->ptr=new;
        }
        processListRear=new;
        pthread_mutex_unlock(&m1);
        display();
    
}

void deleteProcessQ()
{
    node *temp;
    pthread_mutex_lock(&m1);
    temp=processListFront;
    while(1)
    {
        if(processListFront==NULL)
        {
            printf("NO packages in queue\n");
            
            pthread_mutex_unlock(&m1);
            sleep(20);
        }
        else
        {
            pthread_mutex_lock(&m1);
            if(processListFront->Status == COMPLETE)
            {
                
            if(processListFront->ptr==NULL)
            {
                processListRear=NULL;
                processListFront=NULL;
            }
            else
            {
                
                processListFront=processListFront->ptr;
            }
            free(temp);
            display();
            pthread_mutex_unlock(&m1);
            }
            else
                pthread_mutex_unlock(&m1);
                sleep(5);
        }
        
        
    }
}

void display()
{
    node *curnode;
    if(processListFront==NULL)
    {
        printf("No processes in the queue\n");
    }
    else
    {
        curnode=processListFront;
        printf("\nPackage in Assembly line\n");
        printf("Package Number\tTotal number of items\t List of Items\t Status\n");
        while(curnode!=NULL)
        {
            printf("%d\t\t%d\t\t\t\t%d\t\t%d\n",curnode->PackageNumber,curnode->TotalItems,
                   *curnode->ListofItems,curnode->Status);
            curnode=curnode->ptr;
        }
    }
}

//Verify if the iteams required of the packages is vailable in the itemsList
int isIteamsAvailable(int list[],int num)
{
    int i,j;
    int flag=0;
    
    for(i=0;i<num;i++)
    {
        flag=0;
        for(j=0;j<NELEMS(ItemList);j++)
        {
            if (list[i] == ItemList[j])
            {
                flag=1;
                break;
            }
        }
        if(flag == 0)
            return 0;
    }
    return 1;
}

