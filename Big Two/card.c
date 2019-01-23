
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <hiredis/hiredis.h>

int main(int argc, char *argv[])
{

    
    //card
    char Continue;
    int poker[52]={0};
    char kind[4]={'S','D','H','C'};                                               
    char number[13]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};
    int i, j, k;
    //int poker[52]={1,4,8,12,16,20,24,28,32,36,48,49,50};
    srand(time(NULL)); 
     
        for (i=0; i<52; i++)
            poker[i]=i;
        for (i=0; i<52; i++)
        {
            j=rand()%52;
            k=poker[i];
            poker[i]=poker[j];
            poker[j]=k;
        }

   //redis
   
    redisReply *reply; 
    redisReply *r1,*r2,*r3,*r4,*r5,*r6,*r7,*r8,*r9,*r10,*r11,*r12,*r13,*r14,*r15,*r16,*r17,*r18,*r19,*r20,*r21,*r22,*r23;
    redisContext *conn = redisConnect("127.0.0.1",6379);	
	if(conn != NULL && conn->err)
	{
		printf("connected error: %s\n",conn->errstr);
		return 0;
	}
	
    
     /*Create a set  */  
        reply = redisCommand(conn,"FLUSHALL"); //clean all data
        for (i=0; i<13; i++){
        r1 = redisCommand(conn,"SADD %d %d",100,poker[i]);//save to redis 100is all card key
    }
    r2=redisCommand(conn,"SMEMBERS %d",100);//call all the card from redis 
         

 /*Let's check what we have inside the set*/  
 printf("the card what we have\n");
 int count[13];
 for(i=0;i<13;i++){

count[i]=atoi(r2->element[i]->str);
printf("%c%c\n",kind[count[i]%4],number[count[i]/4] );
 }

 //category kind and number
for(i=0;i<13;i++){
	int a,b;
	a=count[i]%4;
	b=count[i]/4;
r3=redisCommand(conn,"SADD %d %d",a,count[i]); //key of kind is 0 to 3
r4=redisCommand(conn,"SADD %d %d",b+4,count[i]);//key of number is 4 to 16
}

int kindcount[4];
for(i=0;i<4;i++){
r5=redisCommand(conn,"SCARD %d",i);
kindcount[i]=r5->integer;
printf("kind of %c have : %d\n",kind[i],kindcount[i]);
freeReplyObject(r5);
}

int numbercount[13];
for(i=0;i<13;i++){
r6=redisCommand(conn,"SCARD %d",i+4);
numbercount[i]=r6->integer;
printf("number %c have : %d\n",number[i],numbercount[i]);
freeReplyObject(r6);
}

//dragon
int ctr=0;	
	for(i=0;i<13;i++){
		if(numbercount[i] == 1){	 
			ctr++;
		}
	}
	if(ctr == 13){
		printf("dragon!!\n");	
	}

//pairs
while(1){
	for(i=0;i<13;i++){
r7=redisCommand(conn,"SMEMBERS %d",i+4);
if(numbercount[i]==2){
	printf("pairs :\n");
	for(j=0;j<2;j++){
	int pair=atoi(r7->element[j]->str);
	printf("%c%c\t",kind[pair%4],number[pair/4]);
}
printf("\n");
}

if(numbercount[i]==3){
	printf("pairs :\n");
	for(j=0;j<3;j++){
	int pair=atoi(r7->element[j]->str);
	printf("%c%c\t",kind[pair%4],number[pair/4]);
}
printf("\n");
}

if(numbercount[i]==4){
	printf("pairs :\n");
	for(j=0;j<4;j++){
	int pair=atoi(r7->element[j]->str);
	printf("%c%c\t",kind[pair%4],number[pair/4]);
}
printf("\n");
}
}
break;
}

//full house
while(1){
for(i=0;i<13;i++){
if(numbercount[i]>=3)
{
    r7=redisCommand(conn,"SMEMBERS %d",i+4);
    r6=redisCommand(conn,"SCARD %d",i+4);
	printf("Full house :\n");
    
for(j=0;j<13;j++){
if(numbercount[j]>=2)
{ 
	r8=redisCommand(conn,"SMEMBERS %d",j+4);
    r20=redisCommand(conn,"SDIFFSTORE %d %d %d",3000+j,j+4,i+4);
    r21=redisCommand(conn,"SMEMBERS %d",3000+j);
    r22=redisCommand(conn,"SCARD %d",3000+j);
    int a=r22->integer;
    int b=r6->integer;
    if(a!=0){
    for(k=0;k<b;k++)
	{	
    int fullhouse=atoi(r7->element[k]->str);
	printf("%c%c\t",kind[fullhouse%4],number[fullhouse/4]);
	}printf("/"); if(b==4)printf("(choose three cards)");
	
    for(k=0;k<a;k++)
	{
    int fullhouse=atoi(r21->element[k]->str);
	printf("%c%c\t",kind[fullhouse%4],number[fullhouse/4]);
	}if(a!=2)printf("(choose two cards)");printf("\n");
    }
}
	}

}
}
	break;
}

//four of a kind
while(1){
		for(i=0;i<13;i++){
			if(numbercount[i]==4){
			    r7=redisCommand(conn,"SMEMBERS %d",i+4);	
				printf("four of a kind:\n");
                    for(j=0;j<4;j++){
					int four=atoi(r7->element[j]->str);
	                printf("%c%c\t",kind[four%4],number[four/4]);
	                r9=redisCommand(conn,"SADD %d %d",400,four);
				    }
				    printf("\n");
				}
				}
				for(j=0;j<13;j++){
				if(numbercount[j]==4){
				r10=redisCommand(conn,"SDIFFSTORE %d %d %d",200,100,400);
				r11=redisCommand(conn,"SCARD %d",200);
				r12=redisCommand(conn,"SMEMBERS %d",200);
				int a=r11->integer;
                printf("You can choose one to complete the four of a kind \n");
				for(i=0;i<a;i++){
			   int test=atoi(r12->element[i]->str);
	           printf("%c%c\t",kind[test%4],number[test/4]);}printf("\n");
			break;}
		}

		
			break;}

//straight
while(1){
    for(i=0;i<10;i++){
    	int num=0;
    	if(i==9){ //TJQKA
		for(j=9;j<13;j++)
		{
			if(numbercount[j]>0)
				num++;
		}
		if(numbercount[0]>0)
			num++;
	}
//TJQKA end
	else{
    	for(j=i;j<i+5;j++){
    	if(numbercount[j]>0) //every number have one
    	{
    		num++;
    	}}
    }
    	if(num==5){  
        int g[10];
    	for(j=(4*i);j<(4*(i+5));j++){
    		if(i==9){ //TJQKA
    			for(j=36;j<56;j++){
    				
    			 int k[25]={j,0,1,2,3};
    			 int b;
    			 
    			 for(b=0;b<5;b++)
    	r13=redisCommand(conn,"SADD %d %d",i+1000,k[b]);
    	}
    	}//TJQKA end
else{
	r13=redisCommand(conn,"SADD %d %d",i+1000,j);
}
}

r14=redisCommand(conn,"SINTERSTORE %d %d %d",g[i],i+1000,100)	;
r15=redisCommand(conn,"SMEMBERS %d ",g[i]);
r16=redisCommand(conn,"SCARD %d",g[i]);
int a=r16->integer;

printf("straight\n");
for(j=0;j<a;j++)
	{
    int straight=atoi(r15->element[j]->str);
	printf("%c%c\t",kind[straight%4],number[straight/4]);
	}printf("\n");

//straight flush
int q;
for(q=0;q<4;q++){
r17=redisCommand(conn,"SINTERSTORE %d %d %d",q+2000,g[i],q);
r18=redisCommand(conn,"SMEMBERS %d",q+2000);
r19=redisCommand(conn,"SCARD %d",q+2000);
if(r19->integer==5){
printf("straight flush\n");
int b;
for(b=0;b<5;b++){
int straight=atoi(r18->element[b]->str);
printf("%c%c\t",kind[straight%4],number[straight/4]);
}printf("\n");
}

}

}
}
break;
}

  return 0;    
}