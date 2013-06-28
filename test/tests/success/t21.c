int b;

int foo(int a,int b, int c, int d);

void main()
{
    int a;
    a = 2+foo(1,1,1,1);
}

int foo(int a, int b, int c, int d)
{
    b=a+b+c+d;
    return b;
}
