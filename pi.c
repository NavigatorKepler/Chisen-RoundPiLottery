#include <stdio.h>
#define L 10000 //求10000位PI值
#define N L/4+1
 
// L 为位数，N是array长度
 
/*圆周率后的小数位数是无止境的，如何使用电脑来计算这无止境的小数是一些数学家与程式设计师所感兴趣的，在这边介绍一个公式配合 大数运算，可以计算指定位数的圆周率。
 
J.Marchin的圆周率公式：
PI = [16/5 - 16 / (3*53) + 16 / (5*55) - 16 / (7*57) + ......] -  [4/239 - 4/(3*2393) + 4/(5*2395) - 4/(7*2397) + ......]
 
*/
void add ( int*, int*, int* );
void sub ( int*, int*, int* );
void div ( int*, int, int* );
 
int main ( void )
{
    int s[N+3] = {0};
    int w[N+3] = {0};
    int v[N+3] = {0};
    int q[N+3] = {0};
    int n = ( int ) ( L/1.39793 + 1 );
    int k;
 
    w[0] = 16*5;
    v[0] = 4*239;
 
    for ( k = 1; k <= n; k++ )
    {
        // 套用公式
        div ( w, 25, w );
        div ( v, 239, v );
        div ( v, 239, v );
        sub ( w, v, q );
        div ( q, 2*k-1, q );
 
        if ( k%2 ) // 奇数项
            add ( s, q, s );
        else    // 偶数项
            sub ( s, q, s );
    }
 
    printf ( "%d.", s[0] );
    for ( k = 1; k < N; k++ )
        printf ( "%04d", s[k] );
    printf ( "\n" );
    return 0;
}
 
void add ( int *a, int *b, int *c )
{
    int i, carry = 0;
 
    for ( i = N+1; i >= 0; i-- )
    {
        c[i] = a[i] + b[i] + carry;
        if ( c[i] < 10000 )
            carry = 0;
        else   // 进位
        {
            c[i] = c[i] - 10000;
            carry = 1;
        }
    }
}
 
void sub ( int *a, int *b, int *c )
{
    int i, borrow = 0;
    for ( i = N+1; i >= 0; i-- )
    {
        c[i] = a[i] - b[i] - borrow;
        if ( c[i] >= 0 )
            borrow = 0;
        else   // 借位
        {
            c[i] = c[i] + 10000;
            borrow = 1;
        }
    }
}
 
void div ( int *a, int b, int *c ) // b 为除数
{
    int i, tmp, remain = 0;
    for ( i = 0; i <= N+1; i++ )
    {
        tmp = a[i] + remain;
        c[i] = tmp / b;
        remain = ( tmp % b ) * 10000;
    }
}
