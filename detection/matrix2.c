#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

double **create_empty_mat(int rows, int cols)
{
    double **Ar = malloc(sizeof(double*)*rows);
    for(int i  = 0; i < rows; i++)
    {
        Ar[i] = malloc(sizeof(double)*cols);
    }
    for(int i = 0; i<rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            Ar[i][j] = 0;
        }
    }
    return Ar;
}

void transvection(double **A, int k, int i, double alpha, int n)
{
    for(int j = 0; j < n; j++)
    {
        A[k][j] = A[k][j] - alpha*A[i][j];
    }
}

void exchange_line(double **A, int i, int j, int n)
{
    for(int k= 0; k < n; k++)
    {
        double tmp = A[i][k];
        A[i][k] = A[j][k];
        A[j][k] = tmp;
    }
}

int p_search(double **A, int i, int n)
{
    int i_mem = i;
    for(int j = i+1; j < n; j++)
    {
        if(fabs(A[i][j])>fabs(A[i_mem][i]))
        {
            i_mem = j;
        }
    }
    return i_mem;
}

double **dot_matrix(double** mat1, double** mat2, int rows1, int cols1, int rows2, int cols2)
{
    if(cols1 == rows2)
    {
        printf("WIW");
        double** res = create_empty_mat(rows1, cols2);
        for(int i = 0; i < rows1; i++)
        {
            for(int j = 0; j < cols2; j++)
            {
                double tmp = 0;
                for(int n = 0; n < rows2; n++)
                {
                    printf("i,n: %d,%d ", i,n);
                    printf("n,j: %d,%d\n", n,j);
                    //printf("Mat1: %f ", mat1[i][n]);
                   // printf("i:%d,MAT2: %f ", n,mat2[n][j]);
                    tmp += mat1[i][n] * mat2[n][j];
                }
                printf("\n");
                res[i][j] = tmp;
            }
        }
        printf("Finished!");
        return res;
    }
    else
    {
        errx(1,"dim are not good for a dot ! m1 : %d m2 : %d",cols1,rows2);
    }
}

void transpose(double **A, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j<n; j++)
        {
            double tmp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = tmp;
        }
    }
}

void substitution(double **A, int i, double a, double *line, int n)
{
    for(int j = 0; j < n; j++)
    {
        A[i][j] = A[i][j]/a;
    }
    for(int k = n-1; k != i; k--)
    {
        transvection(A, i, k, line[k]/a, n);
    }
}

double **copy_mat(double **A, int n)
{
    double **Ar = malloc(sizeof(double*)*n);
    for(int i  = 0; i < n; i++)
    {
        Ar[i] = malloc(sizeof(double)*n);
    }
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            Ar[i][j] = A[i][j];
        }
    }
    return Ar;
}
double **create_id_mat(int n)
{
    double **Ar = malloc(sizeof(double*)*n);
    for(int i  = 0; i < n; i++)
    {
        Ar[i] = malloc(sizeof(double)*n);
    }
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i == j)
            {
                Ar[i][j] = 1;
            }
            else
            {
                Ar[i][j] = 0;
            }
        }
    }
    return Ar;
}


double **inverse(double **A0, int n)
{
    double **A =copy_mat(A0, n);
    double **inv = create_id_mat(n);
    for(int i = 0; i < n; i++)
    {
        int j = p_search(A,i,n);
        if(j>i)
        {
            exchange_line(A,i,j,n);
            exchange_line(inv,i,j,n);
        }
        for(int k = i+1; k<n; k++)
        {
            double alpha = A[k][i]/A[i][i];
            transvection(A,k,i,alpha,n);
            transvection(inv,k,i,alpha,n);
        }
    }
    for(int i = n-1; i>-1; i--)
    {
        double a = A[i][i];
        double *line = malloc(sizeof(double)*n);
        for(int z = 0; z<n; z++)
        {
            line[z] = A[i][z];
        }
        substitution(A, i, a, line, n);
        substitution(inv, i, a, line, n);
        free(line);
    }
    return inv;
}
/*
int main()
{
    double **A = create_empty_mat(3,3);
    A[0][0] = 1;
    A[0][1] = 2;
    A[0][2] = 3;
    A[1][0] = 4;
    A[1][1] = 5;
    A[1][2] = 6;
    A[2][0] = 0.5;
    A[2][1] = 3;
    A[2][2] = 4;

    double **inv = inverse(A, 3);
    //double **inv = create_id_mat(3);
    //transvection(Ab,0,1,2,3);
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            printf("%f ", inv[i][j]);
        }
        printf("\n");
    }
    free(inv);
    free(A);
*/
