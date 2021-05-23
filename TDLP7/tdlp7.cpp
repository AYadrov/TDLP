#include <iostream>
#include <string.h>
#include <ctype.h>

using namespace std;


int z(int x){
    return 0;
}

int pred(int x){
    if (x == 0)
        return 0;
    else
        return x - 1;
}

int subtr(int x, int y){
    if (y == 0)
        return x;
    else
        return pred(subtr(x, y - 1));
}

int add(int x, int  y){
    if (y == 0)
        return x;
    else
        return add(x, y - 1) + 1;
}

int mult(int x, int y){
    if (y == 0)
        return 0;
    else
        return add(x, mult(x, y - 1));
}

int power(int x, int y)
{
    if (y > 0)
        return double(mult(x, power(x, y - 1)));
    return 1.0; // if y == 0
}

double calculation (int x)
{
    if (x < 0)
    {
        x *= -1;
        return 1.0 / double(power(3, (power(x, 3))));
    }
    else
        return power(3, (power(x, 3)));
}

int char_to_int(char* string)
{
    int number = 0;
    bool minus = false;
    for (int i = 0; i < strlen(string); i++)
    {
        if (i == 0 && string[i] == '-')
        {
            minus = true;
            continue;
        }
        number = number*10 + ((int)string[i] - 48);

    }
    if (minus)
        number*=(-1);
    return number;
}
int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        int x = char_to_int(argv[1]);
        cout << "3^(" << x << "^3) = " << calculation(x) << endl;
        return 0;
    }
    else
    {
        cout << "Incorrect number of input elements";
        return -1;
    }
}
