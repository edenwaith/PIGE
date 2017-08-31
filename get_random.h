#include <stdlib.h>
#include <time.h>

int get_random(int low, int high)
{
    static int seeded = 0;
    int value;

    if (seeded == 0)
    {
        srand((unsigned)time((time_t *)NULL));
        seeded = 1;
    }

    value = rand()%(high - low + 1) + low;
    
    return (value);
}
