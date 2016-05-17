////////////////////////////////////////////////////////////////////
// AUTHOR:   Robert Morouney <069001422>
// EMAIL:    robert@morouney.com
// FILE:     macros.h
// CREATED:  2016-04-21 12:03:42
// MODIFIED: 2016-04-22 20:07:35
////////////////////////////////////////////////////////////////////

#ifndef MACRO_H
    #define MACRO_H
    
    #include "types.h"

    #define BLOCK_START {

    #define BLOCK_END   }

    #define FALSE (0)

    #define TRUE (!FALSE)

    #define MIN(a, b)  (((a) < (b)) ? (a) : (b))

    #define MAX(a, b)  (((a) > (b)) ? (a) : (b))

    #define ABS(a)	   (((a) < 0) ? -(a) : (a))

    // % used for assertions. e.g assert(IMPLIES(n > 0, array != NULL));
    #define IMPLIES(x, y) (!(x) || (y))

    // % gt 1 => x > y, eq 0 => x == y , lt 0 => x < y
    #define COMPARE(x, y) (((x) > (y)) - ((x) < (y)))

    // % return true if x is greater than 1
    #define SIGN(x) COMPARE(x, 0)

    // % determine the size of an array
    #define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

    // % swap 2 values T is the type. e.g: SWP(a,b,int)
    #define SWAP(x, y, T) do { T tmp = (x); (x) = (y); (y) = tmp; } while(0)

    // % name says it all T = type
    #define QSORT(a, b, T) do { if ((a) > (b)) SWAP((a), (b), T); } while (0)

    // % i dont actually understand this one lol
    #define SET(d, n, v) do{ size_t i_, n_; for (n_ = (n), i_ = 0; n_ > 0; --n_, ++i_) (d)[i_] = (v); } while(0)


    #define ZERO(d, n) SET(d, n, 0)

    // % For Each Loop.  pretty easy to get your head around.
    #define foreach(item, array) \
        for(int keep=1, \
                count=0,\
                size=sizeof (array)/sizeof *(array); \
            keep && count != size; \
            keep = !keep, count++) \
        for(item = (array)+count; keep; keep = !keep)

    // % For Debugging it prints a stack trace.
    #if defined NDEBUG
        #define TRACE( format, ... ) ( ( void ) 0 )
    #else
        #define TRACE( format )   printf( "%s::%s(%d)" format, __FILE__, __FUNCTION__,  __LINE__)
    #endif

    // % Print error info and exit
    #define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                        perror(source),kill(0,SIGKILL),			\
                        exit(EXIT_FAILURE))

    // % Debug
    #define DBG(source) (fprintf(stderr,"%s:%d\t",__FILE__,__LINE__),\
                            perror(source),

    // % Call function and exit if return value != result
    #define CALL_AND_CHECK(function, result)\
    do {\
    if (function != result)\
    {\
    ERR(#function);\
    }\
    } while (0)

    // % Call function and exit if error occured
    #define CALL_AND_EXIT_ON_ERR(function) CALL_AND_CHECK(function, 0)
#endif
