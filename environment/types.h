#ifndef INTDEF
    #include <stdint.h>
    #define INTDEF
    #ifdef uint128_t
        typedef uint128_t       u128;
    #endif
    typedef uint64_t            u64;
    typedef uint32_t            u32;
    typedef uint16_t            u16;
    typedef uint8_t             u08;
    
    #ifdef int128_t
        typedef int_128_t       s128;
    #endif
    typedef int64_t             s64;
    typedef int32_t             s32;
    typedef int16_t             s16;
    typedef int8_t              s08;
#endif

#ifndef BOOLDEF
    #define BOOLDEF
    typedef bool *             b_lst;
#endif
