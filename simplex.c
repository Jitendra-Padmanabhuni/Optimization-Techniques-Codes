#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

float *min_check(float *A,int n)
{
    float min=A[0];
    float index=0;
    int i;
    for(i=0;i<n;i++)
    {
        if(min>A[i])
        {
            min=A[i];
            index=i;
        }
    }
    float *B;
    B=(float *)malloc(n*sizeof(float));
    B[0]=min;
    B[1]=index;
    return B;
}

void print_2D_array(float **A,int m,int n)
{
    int i,j;
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            printf("%f ",A[i][j]);
        }
        printf("\n");
    }
}

void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

float **copy_2d_array(float **A,int m,int n)
{
    float **B;
    B=(float**)malloc((m)*sizeof(float *));
    int i,j;
    for(i=0;i<m;i++)
    {
        B[i]=(float *)malloc((n)*sizeof(float));
    }
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            B[i][j]=A[i][j];
        }
    }
    return B;
}

int main(void)
{
    printf("give number of inequations and variables:\n");
    int m,n;
    scanf("%d%d",&m,&n);
    int basic_variables[m];
    int non_basic_variables[n];
    printf("give inequations and optimization equation:\n");
    float **A;
    A=(float**)malloc((m+1)*sizeof(float *));
    int i,j;
    for(i=0;i<m+1;i++)
    {
        A[i]=(float *)malloc((n+1)*sizeof(float));
    }
    float value_store;
    for(i=0;i<m+1;i++)
    {
        for(j=0;j<n+1;j++)
        {
            if(i!=m)
            {
                scanf("%f",&A[i][j]);
            }
            else
            {
                scanf("%f",&value_store);
                A[i][j]=(-1)*value_store;
            }
        }
    }
    printf("condensed simplex tableau:\n");
    print_2D_array(A,m+1,n+1);
    for(i=0;i<n;i++)
    {
        non_basic_variables[i]=i;
    }
    for(i=0;i<m;i++)
    {
        basic_variables[i]=n+i;
    }
    float *min;
    int iterate=0;
    while(1)
    {
        min=min_check(A[m],n);
        if(min[0]<0)
        {
            iterate++;
            int pivot_column;
            int pivot_row=0;
            pivot_column=(int)min[1];
            float minimum_ratio=__FLT_MAX__;
            int flag=0;
            for(i=0;i<m;i++)
            {
                if(minimum_ratio>A[i][n]/A[i][pivot_column] && A[i][pivot_column]>0)
                {
                    minimum_ratio=A[i][n]/A[i][pivot_column];
                    pivot_row=i;
                }
                if(A[i][pivot_column]>0)
                {
                    flag=1;
                }
            }
            if(!flag)
            {
                printf("problem is unbounded\n");
                break;
            }
            printf("value and index of pivot element: A[%d][%d]=%f\n",pivot_row,pivot_column,A[pivot_row][pivot_column]);
            swap(&basic_variables[pivot_row],&non_basic_variables[pivot_column]);
            float **temp;
            temp=copy_2d_array(A,m+1,n+1);
            for(i=0;i<m+1;i++)
            {
                for(j=0;j<n+1;j++)
                {
                    if((i==pivot_row)&&(j==pivot_column))
                    {
                        A[i][j]=1/temp[i][j];
                    }
                    else if(i==pivot_row)
                    {
                        A[i][j]=temp[i][j]/temp[pivot_row][pivot_column];
                    }
                    else if(j==pivot_column)
                    {
                        A[i][j]=(-1)*temp[i][j]/temp[pivot_row][pivot_column];
                    }
                    else
                    {
                        A[i][j]=(temp[i][j]*temp[pivot_row][pivot_column]-temp[i][pivot_column]*temp[pivot_row][j])/temp[pivot_row][pivot_column];
                    }
                }
            }
            printf("condensed simplex tableau after %d iteration:\n",iterate);
            print_2D_array(A,m+1,n+1);
            printf("objective value: %f\n",A[m][n]);
            printf("basic variables:\n");
            for (int i = 0; i < m; i++)
            {
                printf("x%d ",basic_variables[i]+1);
            }
            printf("\n");
            printf("non-basic variables:\n");
            for(int i=0;i<n;i++)
            {
                printf("x%d ",non_basic_variables[i]+1);
            }
            printf("\n");
            printf("basic feasible solution:\n");
            for(int i=0;i<n;i++)
            {
                printf("x%d=0 ",non_basic_variables[i]+1);
            }
            for (int i = 0; i < m; i++)
            {
                printf("x%d=%f ",basic_variables[i]+1,A[i][n]);
            }
            printf("\n");
            free(temp);
            free(min);
        }
        else
        {
            printf("optimum basic feasable solution reached\n");
            break;
        }
    }
}