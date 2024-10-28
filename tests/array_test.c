#include<stdio.h>
#include<stdbool.h>
#include "../allocator/allocator.h"

bool array_test()
{
    int **arr=(int **)mid_alloc(10*sizeof(int*));
    for(int i=0;i<10;i++)
    {
        arr[i]=(int *)mid_alloc(10*sizeof(int));
        for(int j=0;j<10;j++)
        {
            arr[i][j]=-1*(i+j);
        }
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(arr[i][j]!=(-1*(i+j)))
            return false;
        }
    }
    return true;
}