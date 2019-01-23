#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define weight_file "learn.wei"
#define testN 30
#define inN 2
#define outN 1
#define hidN 2


int main()
{

FILE *fp1;
float X[inN],Y[outN],H[hidN];
float w_xh[inN][hidN],w_hy[hidN][outN];
float q_h[hidN],q_y[outN];
float sum,errorsum;
int  Itest,i,j,h;
char ch='0';
/*---------file open------------*/
fp1=fopen(weight_file,"r");


if(fp1==NULL)
{
printf("File1 not exist.\n");
exit(1);
}


/*-----check fp1 fp2 exist----*/
/*--------read weight file-----*/
fseek(fp1,0,0);

for(h=0;h<hidN;h++)
{
for(i=0;i<inN;i++)
{
fscanf(fp1,"%f",&w_xh[i][h]);
}
}

for(j=0;j<outN;j++)
{
for(h=0;h<hidN;h++)
{
fscanf(fp1,"%f",&w_hy[h][j]);
}
}

for(h=0;h<hidN;h++)
{
fscanf(fp1,"%f",&q_h[h]);
}

for(j=0;j<outN;j++)
{
fscanf(fp1,"%f",&q_y[j]);
}



/*-------------------*/
/*------test---------*/
do{

for(i=0;i<inN;i++)
{
printf("input X[%d]=",i);
scanf("%4f",&X[i]);
}

/*----comput hidden and output----*/

for(h=0;h<hidN;h++)
{
sum=0.0;
for(i=0;i<inN;i++)
{
sum=sum+X[i]*w_xh[i][h];
H[h]=(float)1.0/(1.0+exp(-(sum-q_h[h])));
}
}

for(j=0;j<outN;j++)
{
sum=0.0;
for(h=0;h<hidN;h++)
{
sum+=H[h]*w_hy[h][j];
Y[j]=(float)1.0/(1.0+exp(-(sum-q_y[j])));
}
}

printf("result is=\n");
for(j=0;j<outN;j++)
{
printf("Y[j]=%4f\n\n\n",Y[j]);
}



}while (ch!='b');
/*------end test------------*/

fclose(fp1);

return 0 ;
/*--close file---------*/

}

