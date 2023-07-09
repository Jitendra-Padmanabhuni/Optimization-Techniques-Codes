#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

float *min_check(float *A,int n)
{
    float *temp;
    temp=(float *)malloc(2*sizeof(float));
    temp[0]=A[0];
    temp[1]=0;
    int i;
    for(i=0;i<n;i++)
    {
        if(temp[0]>A[i])
        {
            temp[0]=A[i];
            temp[1]=i;
        }
    }
    return temp;
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

float **make_matrix(int m,int n)
{
    float **A;
    A=(float**)malloc((m)*sizeof(float *));
    int i,j;
    for(i=0;i<m;i++)
    {
        A[i]=(float *)malloc((n)*sizeof(float));
    }
    return A;
}

float **matrix_multiply(float **A,float **B,int m1,int m,int n2)
{
    int i,j,k;
    float **C=make_matrix(m1,n2);
    for(i=0;i<m1;i++)
    {
        for(j=0;j<n2;j++)
        {
            C[i][j]=0;
            for(k=0;k<m;k++)
            {
                C[i][j]=C[i][j]+A[i][k]*B[k][j];
            }
        }
    }
    return C;
}

float **matrix_inverse(float **B,float **B_inv,float **B_new,float **P,int m,int c)
{
    float **product1;
    product1=matrix_multiply(B_inv,P,m,m,1);
    float **eeta;
    eeta=make_matrix(m,1);
    for(int i=0;i<m;i++)
    {
        if(i==c)
        {
            eeta[i][0]=(1/product1[i][0]);
        }
        else
        {
            eeta[i][0]=(-1)*(product1[i][0]/product1[c][0]);
        }
    }
    float **E_r;
    E_r=make_matrix(m,m);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(j==c)
            {
                E_r[i][j]=eeta[i][0];
            }
            else if(i==j)
            {
                E_r[i][j]=1;
            }
            else
            {
                E_r[i][j]=0;
            }
        }
    }
    float **inv;
    inv=matrix_multiply(E_r,B_inv,m,m,m);
    free(product1);
    free(eeta);
    free(E_r);
    return inv;
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
    A=make_matrix(m+1,n+1);
    int i,j,k;
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
    float **B;
    B=make_matrix(m,m);
    for(i=0;i<m;i++)
    {
        for(j=0;j<m;j++)
        {
            if(i==j)
            {
                B[i][j]=1;
            }
            else
            {
                B[i][j]=0;
            }
        }
    }
    float **B_inv;
    B_inv=copy_2d_array(B,m,m);
    float **B_next;
    float **B_next_inv;
    float **C;
    C=make_matrix(1,m);
    for(i=0;i<m;i++)
    {
        C[0][i]=0;
    }
    float min[n];
    float min_ratio[m];
    int iterate=0;
    float **Y;
    float **b;
    float **alpha;
    b=make_matrix(m,1);
    for(i=0;i<m;i++)
    {
        b[i][0]=A[i][n];
    }
    float **X_b;
    float **P;
    P=make_matrix(m,1);
    int entering_variable;
    int leaving_variable;
    int pivot_row;
    int pivot_column;
    do
    {
        Y=matrix_multiply(C,B_inv,1,m,m);
        X_b=matrix_multiply(B_inv,b,m,m,1);
        for(i=0;i<n;i++)
        {
            min[i]=0;
            for(j=0;j<m;j++)
            {
                min[i]=min[i]+Y[0][j]*A[j][i];
            }
            min[i]=min[i]-(-A[m][i]);
        }    
        if(min_check(min,n)[0]<0)
        {
            iterate++;
            printf("ITERATION %d:\n",iterate);
            pivot_column=min_check(min,n)[1];
            entering_variable=non_basic_variables[(int)min_check(min,n)[1]];
            printf("entering variable is x%d\n",entering_variable+1);
            for(i=0;i<m;i++)
            {
                P[i][0]=A[i][(int)min_check(min,n)[1]];
            }
            alpha=matrix_multiply(B_inv,P,m,m,1);
            for(i=0;i<m;i++)
            {
                min_ratio[i]=X_b[i][0]/alpha[i][0];
            }
            pivot_row=min_check(min_ratio,m)[1];
            leaving_variable=basic_variables[(int)min_check(min_ratio,m)[1]];
            printf("leaving variable is x%d\n",leaving_variable+1);
            swap(&basic_variables[pivot_row],&non_basic_variables[pivot_column]);
            B_next=make_matrix(m,m);
            for(i=0;i<m;i++)
            {
                for(j=0;j<m;j++)
                {
                    if(j==pivot_row)
                    {
                        B_next[i][j]=P[i][0];
                    }
                    else
                    {
                        B_next[i][j]=B[i][j];
                    }
                }
            }
            B_next_inv=matrix_inverse(B,B_inv,B_next,P,m,pivot_row);
            for(i=0;i<m;i++)
            {
                for(j=0;j<m;j++)
                {
                    B[i][j]=B_next[i][j];
                    B_inv[i][j]=B_next_inv[i][j];
                }
            }
            printf("B:\n");
            print_2D_array(B,m,m);
            for(i=0;i<m;i++)
            {
                if(basic_variables[i]<n)
                {
                    C[0][i]=(-1)*A[m][basic_variables[i]];
                }
                else
                {
                    C[0][i]=0;
                }
            }
            printf("C:\n");
            for(i=0;i<m;i++)
            {
                printf("%f\n",C[0][i]);
            }
            free(X_b);
            X_b=matrix_multiply(B_inv,b,m,m,1);
            printf("X_B:\n");
            print_2D_array(X_b,m,1);
            free(B_next);
            free(B_next_inv);
            free(Y);
            free(X_b);
        }
        else
        {
            float **z=matrix_multiply(C,X_b,1,m,1);
            printf("z* = %f\n",z[0][0]);
            printf("optimum value reached\n");
            break;
        }
    } while (1);  
}
