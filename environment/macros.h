// MACROS


#ifndef MACRO_H
    #define MACRO_H

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
        #define TRACE( format, ... )   printf( "%s::%s(%d)" format, __FILE__, __FUNCTION__,  __LINE__, __VA_ARGS__ )
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

    #define LOG2(x) do { register u32 r, t, tt

    #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    static const char LogTable256[256] =
    {
    
        -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
        LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
        LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
    };

    register unsigned int LOG2N ( unsigned int x )
    {
        register unsigned int ret, t, tt; // temp var2
        
        if (tt = x >> 16)
            ret = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
        else
            ret = (t = x >> 8) ? 8 + LogTable256[t] : LogTable256[x];
        
        return r;
    }



#endif