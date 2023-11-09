#include "test/ktest.h"
#include "common.h"

void TEST_EX_INT(void *(*f)(void *), void *args, int expects) 
{
    if (f(args) != expects)
        panic();
}