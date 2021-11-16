#include <stdio.h>

const int N = 6000;
int primer[N],count;
char visit[N] = { 0 };
void GetPrimers()
{
    count = 0;
    for (int i = 2; i <= N; i++)//quick query
    {
        if (!visit[i]) primer[++count] = i;   //  means primer.
        for (int j = 1; j <= count && primer[j] * i <= N; j++)
        {
            visit[i*primer[j]] = 1;
            if (i % primer[j] == 0)break;
        }
    }
}

int main()
{
    GetPrimers();
    printf( "start to work, count is %d\n", count );
    int end = 100000;
    for( int i = 0; i < count; ++i)
    {
        if( primer[i] <= end )
        {
            printf("%d\t", primer[i]);
        }
        else {
            break;
        }
    }
    printf("\n");
    return 0;
}
