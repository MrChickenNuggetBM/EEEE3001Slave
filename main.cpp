#include "main.h"

bool setup()
{
    return true;
}

bool loop(ullint i)
{
    if (i > 100)
        return false;

    cout << i << endl;
    return true;
}