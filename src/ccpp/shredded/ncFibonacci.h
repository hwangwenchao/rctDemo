#pragma once

long long GetFibonacciSequence(unsigned n)
{
    if(n<=0)
    {
        return 0;
    }

    if(n==1)
    {
        return 1;
    }

    return GetFibonacciSequence(n-1) + GetFibonacciSequence(n-2);

}

long long GetFibonacciSequenceEx(unsigned int n)
{
    int result[2] = {0, 1};
    if(n<2)
    {
        return result[n];
    }

    long long fibBase0 = 0; // f(n-1)
    long long fibBase1 = 1; // f(n-2)
    long long fibN = 0;
    for(unsigned i=2; i<n; i++)
    {
        fibN = fibBase0 + fibBase1;
        fibBase0 = fibBase1;
        fibBase1 = fibN;
    }

    return fibN;
}