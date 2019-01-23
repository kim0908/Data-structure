#include<time.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>


#define learnN 300 /*learning cycle number*/
#define trainN 4
#define inN 2
#define hidN 2
#define outN 1
#define lespeed 0.5   /*learning speed*/
#define alpha 0.2

#define train_file "learn.tra"
#define weight_file "learn.wei"
#define errorsum_file "learn.errsum" /*square error*/


float random_value(void);

int main()
{

FILE *fp1,*fp2,*fp3;
float X[inN],T[outN],H[hidN],Y[outN]; /*T[]is aims of output*/
float w_xh[inN][hidN],w_hy[hidN][outN];
float dw_xh[inN][hidN],dw_hy[hidN][outN]; /*correction value newW=W+dW*/
float q_h[hidN],q_y[outN];
float dq_h[hidN],dq_y[outN];
float delta_h[hidN],delta_y[outN];
float sum,errorsum;
int Ilearn,Itrain; /*counters for training and data receivingcycle*/
int i,j,h;
long int now;

/*open file*/

fp1=fopen(train_file,"r");
fp2=fopen(weight_file,"w");
fp3=fopen(errorsum_file,"w");

if(fp1==NULL)
{
printf("File not exist.\n");
exit(-1);
}
/*check fp1 is exist*/

/*----------------------*/


srand(time(&now) % 1000);

for(h=0;h<hidN;h++)
{
for(i=0;i<inN;i++)
{
w_xh[i][h]=random_value();
dw_xh[i][h]=0;
}
}


for(j=0;j<=outN;j++)
{
for(h=0;h<hidN;h++)
{
w_hy[h][j]=random_value();
dw_hy[h][j]=0;
}
}


for(h=0;h<hidN;h++)
{
q_h[h]= random_value();
dq_h[h]=0;
delta_h[h]=0;
}


for(j=0;j<outN;j++)
{
q_y[j]=random_value();
dq_y[j]=0;
delta_y[j]=0;
}

/*--initial weight by ran val---*/

/*-----learning-----------*/

for(Ilearn=0;Ilearn<learnN;Ilearn++)
{
errorsum=0.0;

/*-------input train sample and aims of train sample(star one cycle)-----*/

fseek(fp1,0,0);
for(Itrain=0;Itrain<trainN;Itrain++)
{


for(i=0;i<inN;i++)
{
fscanf(fp1,"%f",&X[i]);
}
for(j=0;j<outN;j++)
{
fscanf(fp1,"%f",&T[j]);
}


/*---------comput hidden and output-----*/

for(h=0;h<hidN;h++)
{
sum=0.0;
for(i=0;i<inN;i++)
{
sum+=X[i]*w_xh[i][h];
}
H[h]=(float)1.0/(1.0+exp(-(sum-q_h[h])));
}


for(j=0;j<outN;j++)
{
sum=0.0;
for(h=0;h<hidN;h++)
{
sum+=H[h]*w_hy[h][j];
}
Y[j]=(float)1.0/(1.0+exp(-(sum-q_y[j])));
}

/*-----------------------------------*/
/*-------comput difference between output and hidden-----*/

for(j=0;j<outN;j++)
{
delta_y[j]=Y[j]*(1.0-Y[j])*(T[j]-Y[j]);
}

for(h=0;h<hidN;h++)
{
sum=0.0;
for(j=0;j<outN;j++)
{
sum+=w_hy[h][j]*delta_y[j];
delta_h[h]=H[h]*(1.0-H[h])*sum;
}
}
/*--------------------------------------*/
/*-----comput dq(h and y) dw(xh and hy)-------------------*/

for(h=0;h<hidN;h++)
{
for(i=0;i<inN;i++)
{
dw_xh[i][h]=lespeed*delta_h[h]*Y[j]+alpha*dw_xh[i][h];
}
}



for(j=0;j<outN;j++)
{
for(h=0;h<hidN;h++)
{
dw_hy[h][j]=lespeed*delta_y[j]*H[h]+alpha*dw_hy[h][j];
}
}


for(h=0;h<hidN;h++)
{
dq_h[h]=-lespeed*delta_h[h]+alpha*dq_h[h];
}



for(j=0;j<outN;j++)
{
dq_y[j]=-lespeed*delta_y[j]+alpha*dq_y[j];
}

/*------------------------------------------*/
/*----------update w and q-----------------*/

for(j=0;j<outN;j++)
{
for(h=0;h<hidN;h++)
{
w_hy[h][j]+=dw_hy[h][j];
}
}

for(j=0;j<outN;j++)
{
q_y[j]+=dq_y[j];
}


for(h=0;h<hidN;h++)
{
for(i=0;i<inN;i++)
{
w_xh[i][h]+=dw_xh[i][h];
}
}

for(h=0;h<hidN;h++)
{
q_h[h]+=dq_h[h];
}

/*--------------------------*/
/*-------comput square error-------*/

for(j=0;j<outN;j++){
errorsum+=(T[j]-Y[j])*(T[j]-Y[j]);
printf("Y[%d}=%f\n",j,Y[j]);
}



errorsum=errorsum/trainN;


printf("Ilearn=%d , errorsum=%f\n",Ilearn,errorsum);
fprintf(fp3,"%d %f\n",Ilearn,errorsum);



}

/*--------one cycle learning end-----*/





}

/*---------learning end----------------*/

/*------put w_hy and w_xh in to weight----*/

printf("\n\n");

for(h=0;h<hidN;h++)
{
for(i=0;i<inN;i++)
{
printf("w_xh[%d][%d]=%4f\n",i,h,w_xh[i][h]);
fprintf(fp2,"%4f\n",w_xh[i][h]);
}
printf("\n\n");
fprintf(fp2,"\n\n");
}

for(j=0;j<outN;j++)
{
for(h=0;h<hidN;h++)
{
printf("w_hy[%d][%d]=%4f\n",h,j,w_hy[h][j]);
fprintf(fp2,"%4f\n",w_hy[h][j]);
}
printf("\n\n");
fprintf(fp2,"\n\n");
}

for(h=0;h<hidN;h++)
{
printf("q_h[%d]=%4f\n",h,q_h[h]);
fprintf(fp2,"%4f\n",q_h[h]);
}
printf("\n\n");
fprintf(fp2,"\n\n");

for(j=0;j<outN;j++)
{
printf("q_y[%d]=%4f\n",j,q_y[j]);
fprintf(fp2,"%4f\n",q_y[j]);
}
printf("\n\n");
fprintf(fp2,"\n\n");

/*----------------------------------*/



/*-------close file-----------------*/


fclose(fp1);
fclose(fp2);
fclose(fp3);
return 0;

}
/*-----------end main------*/


float random_value(void)
{
return(((double)rand())/52767.0-0.1);

}
