int arr[5];

int main()
{
    int i;
    int x;

    i = 0;

    while(i < 5)
    {
        arr[i] = i * 10;
        i = i + 1;
    }


    x = arr[0];
    println(x);

    x = arr[1];
    println(x);

    x = arr[2];
    println(x);

    x = arr[3];
    println(x);

    x = arr[4];
    println(x);


    return 0;
}