#include "pursuit_rewind/random_source.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void random_source_seed(void)
{
    srand((unsigned int)time(NULL));
}

int random_source_between(int low, int high)
{
    uint64_t range;
    uint64_t source_size;
    uint64_t acceptance_limit;
    uint64_t sample;

    if (low > high) {
        return low;
    }

    range = (uint64_t)((int64_t)high - (int64_t)low + 1);
    source_size = (uint64_t)RAND_MAX + 1U;
    acceptance_limit = source_size - (source_size % range);

    do {
        sample = (uint64_t)rand();
    } while (sample >= acceptance_limit);

    return low + (int)(sample % range);
}

