double fact(int k)
{
    if (k == 1)
        return (1);
    else {
        return (fact(k - 1) * k);
    }
}
