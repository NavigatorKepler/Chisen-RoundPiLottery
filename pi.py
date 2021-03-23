import time

def pi(places=10):
    # 3 + 3*(1/24) + 3*(1/24)*(9/80) + 3*(1/24)*(9/80)*(25/168)
    # The numerators 1, 9, 25, ... are given by (2x + 1) ^ 2
    # The denominators 24, 80, 168 are given by (16x^2 -24x + 8)
    extra = 8
    one = 10 ** (places+extra)
    t, c, n, na, d, da = 3*one, 3*one, 1, 0, 0, 24

    while t > 1:
        n, na, d, da = n+na, na+8, d+da, da+32
        t = t * n // d
        c += t
    return c // (10 ** extra)

def pi_t(n=10):
    t1=time.time()
    t=pi(n)
    t2=time.time()
    print('elapsed: ', (t2-t1)/1000, 's')
    return t

def pi2(n=10):
    r=6*(10**n)*1000
    p=0
    k=0
    c=r//2
    d=c//(2*k+1)
    while d>0:
        p=p+d
        k=k+1
        k2=2*k
        c=c*(k2-1)//(4*k2)
        d=c//(k2+1)
    return p//1000

def pi2_t(n=10):
    t1=time.time()
    t=pi2(n)
    t2=time.time()
    print('elapsed: ', (t2-t1)/1000, 's')
    return t