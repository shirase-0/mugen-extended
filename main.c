#include "mu_headers.h"

// To turn off debugging, change DEBUG to 0 in mu_headers.h

int main(int argc, char *argv[])
{
    if(DEBUG)
    {
        //unit_test();
        Game *mugen = init_game();
        run_game(mugen);
    }
    else
    {
        Game *mugen = init_game();
        run_game(mugen);
    }  
    return 0;
}