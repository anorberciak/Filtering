#include <stdio.h>
#include <stdlib.h>


#define N 5
#define M 15

int buff[M];//Filter circular buffer
int tab[N];
const int coeff[N]={6553, 6553, 6553, 6553, 6553,};//0,2 == 6553 in q15
int input[M]={0, 0, 0, 500, 500, 500, 500, 500, 0, 0, 0, 0, 0, 0, 0};

int main()
{
    int idx = 0; //index, circular buffer
    int pos=idx; //position, circular buffer
    long result=0; //storage, result of filtration
    int i, j;
    for(j=0;j<M;j++)
    {
    printf("input:  %d", input[j]);

    buff[idx]=input[j];
    pos=idx;
    result=0;

    for (i=0; i<N; i++)
    {
       result += (long)buff[pos] * coeff[i];
       pos += 1;
       if(pos == N)
              {
                  pos-=N;
              }
    }

    int output = (int)((result+16384)>>15);
        
    idx +=1;
    if(idx>=N)
    {
       idx -=N;
    }

   printf("   output: %d", output);
   printf("\n");

    }
    return 0;
}
