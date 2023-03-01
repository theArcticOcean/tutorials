#include <stdio.h>

// export the function.
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif
int compute( int a, int b )
{
    int ans = 1;
    while( 1 )
    {

    };

    for( int i = 0; i < b; ++i )
    {
        ans = ans * a;
    }
    return ans;
}