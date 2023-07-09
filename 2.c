#include<stdio.h>
#include<math.h>

int sequence_no;
int variable_sequence[100][100];
int factorial(int n) 
{
   int c;
   int result = 1;
   for (c = 1; c <= n; c++)
   {
        result = result*c;
   }
   return result;
}

int find_ncr(int n, int r) 
{
   int result;
   result = factorial(n)/(factorial(r)*factorial(n-r));
   return result;
}

void permute(int *variable_index,int *store_index,int start,int end,int total_variables,int m)
{
    if(total_variables==m)
    {
        for (int i = 0; i < m; i++)
        {
            variable_sequence[sequence_no][i]=store_index[i];
        }
        sequence_no++;
        return;
    }
    for (int i = start; i<=end && end-i+1>=m-total_variables; i++)
    {
        store_index[total_variables]=variable_index[i];
        permute(&variable_index[0],&store_index[0],i+1,end,total_variables+1,m);
    }
    
}

void start_permutation(int *variable_index,int n,int m)
{
    int store_index[m];
    permute(&variable_index[0],&store_index[0],0,n-1,0,m);
}

int error_check(float *x,float *x_old,float tol,int m)
{
    float error;
    for(int i=0;i<m;i++)
    {
        error=fabs(x[i]-x_old[i])/fabs(x[i]);
        if(error>tol)
        {
            return 0;
        }
    }
    return 1;

}

void gauss_seidel(int m,int n,float *A,float *b,float *x,float tol,int index)
{
    int variables[m];
    float x_copy[m];
    for(int i=0;i<m;i++)
    {
        variables[i]=variable_sequence[index][i];
        x_copy[i]=0;
    }
    float A_copy[m][m];
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(j==variables[i])
            {
                for(int k=0;k<m;k++)
                {
                    for(int l=0;l<m;l++)
                    {
                        A_copy[k][l]=*(A+m*k+j);
                    }
                }
            }

        }
    }
    float sum;
    float x_old[m];
    do
    {
        for(int i=0;i<m;i++)
        {
            x_old[i]=x_copy[i];
        }
        for(int i=0;i<m;i++)
		{
			sum=b[i];
			for(int j=0;j<m;j++)
			{
				if(j<i)
                {
                    sum-=A_copy[i][j]*x_copy[j];
                }
				else if(j>i)
                {
                	sum-=A_copy[i][j]*x_copy[j];    
                }
				x_copy[i]=sum/A_copy[i][i];
			}
		}
    } while (!error_check(x_copy,x_old,tol,m));
    for (int i = 0; i < n; i++)
    {
        x[i]=0;
    }
    
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(i==variables[j])
            {
                x[i]=x_copy[j];
            }
        }
    }
    
}

int main(void)
{
    int m,n;
    printf("give number of equations and variables\n");
    scanf("%d",&m);
    scanf("%d",&n);
    float A[m][n];
    printf("give coefficients of the equation\n");
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf("%f",&A[i][j]);
        }
    }
    float b[m];
    printf("give b:\n");
    for(int i=0;i<m;i++)
    {
        scanf("%f",&b[i]);
    }
    printf("give tolerance:\n");
    float tol;
    scanf("%f",&tol);
    int total_sol=find_ncr(n,m);
    int sequence_no=0;
    int variable_index[m];
    for(int i=0;i<m;i++)
    {
        variable_index[i]=i;
    }
    start_permutation(variable_index,n,m);
    if(sequence_no<total_sol)
    {
        printf("Lesser solutions found than the total solutions\n");
    }
    float x[n];
    for(int i=0;i<total_sol;i++)
    {
        printf("solution %d:\n");
        gauss_seidel(m,n,&A[0][0],b,x,tol,i);
        for(int j=0;j<n;j++)
        {
            printf("x%d=%f\n",i+1,x[j]);
        }
    }
}