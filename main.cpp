#include "header.h"

//Used for tests.
unsigned int gOutputCount = 0;

int main()
{
    //Test Logic
    //runTests();

    //Actual Logic
    AprioriTrie* aprioriTrie = new AprioriTrie;

    if( !loadAprioriTrie( aprioriTrie ) )
    {
        system( "pause" );
        return 1;
    }

    mainLoop( aprioriTrie );

    delete aprioriTrie;
    
    system( "pause" );
    return 0;
}