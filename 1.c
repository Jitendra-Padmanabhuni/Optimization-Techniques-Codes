#include<stdio.h>
#include<math.h>

int diag_dom_check(float *A,int m)
{
    int flag=1;
    float sum;
    int B[m];
    for(int i=0;i<m;i++)
    {
        sum=0;
        for(int j=0;j<m;j++)
        {
            if(i!=j)
            {
                sum=sum+fabs(*(A+m*i+j));
            }
            if(sum<fabs(*(A+(m+1)*i)))
            {
                B[i]=1;
            }
            else
            {
                B[i]=0;
                break;
            }
        }
    }
    for(int i=0;i<m;i++)
    {
        if(B[i]==0)
        {
            flag=0;
            break;
        }
    }
    return flag;
}

void swap_rows(float *A,int m,int a,int b)
{
    float temp;
    for(int i=0;i<m;i++)
    {
       temp=*(A+m*b+i);
       *(A+m*b+i)=*(A+m*a+i);
       *(A+m*a+i)=temp; 
    }
}

void swap(float *a, float *b)
{
    float temp=*b;
    *b=*a;
    *a=temp;
}

void make_diag_dom(float *A,float *b,int m)
{
    if(diag_dom_check(A,m))
    {
        return;
    }
    else
    {
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<m;j++)
            {
                swap_rows(A,m,i,j);
                swap(b+i,b+j);
                if(diag_dom_check(A,m))
                {
                    return;
                }
                else
                {
                    swap_rows(A,m,i,j);
                    swap(b+i,b+j);
                }
            }
        }
    }
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

int main(void)
{
    int m;
    printf("give number of equations and variables\n");
    scanf("%d",&m);
    float A[m][m];
    printf("give coefficients of the equation\n");
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
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
    make_diag_dom(&A[0][0],b,m); //this only works if the matrix can be made diagonally dominant by swapping it's rows
    printf("give tolerance:\n");
    float tol;
    scanf("%f",&tol);
    float x[m];
    float x_old[m];
    for(int i=0;i<m;i++)
    {
        x[m]=0;
    }
    float sum;
    do
    {
        for(int i=0;i<m;i++)
        {
            x_old[i]=x[i];
        }
        for(int i=0;i<m;i++)
		{
			sum=b[i];
			for(int j=0;j<m;j++)
			{
				if(j<i)
                {
                    sum-=A[i][j]*x[j];
                }
				else if(j>i)
                {
                	sum-=A[i][j]*x[j];    
                }
				x[i]=sum/A[i][i];
			}
		}
    } while (!error_check(x,x_old,tol,m));
    
    for(int i=0;i<m;i++)
    {
        printf("%f\n",x[i]);
    }
}