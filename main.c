#include "mu_headers.h"

// To turn off debugging, change DEBUG to 0 in mu_headers.h

int main(int argc, char *argv[])
{
    if(DEBUG)
    {
        unit_test();
    }
    else
    {
        init_game();
    }  
    return 0;
}