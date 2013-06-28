int tz[20];

int foo(int a,int b, int c, int d);

void main()
{
    tz[2]=23;
    2+tz[2];
}

int foo(int a, int b, int c, int d)
{
    b=a+b+c+d;
    return b;
}
