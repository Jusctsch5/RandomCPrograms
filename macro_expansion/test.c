# include <stdio.h>
# include <stdarg.h>
# include <stdint.h>

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif /* ifndef unlikely */

void
__my_panic(
    const char *file,
    int         line,
    const char *fmt,
    ...)
{
    char    buf[256];
    va_list argp;

    va_start(argp, fmt);
    vsnprintf(buf, sizeof(buf), fmt, argp);
    va_end(argp);
    printf("%s\n", buf);
}

#define my_panic(...) { \
        __my_panic(__FILE__, __LINE__, __VA_ARGS__); \
}

#define test_panic(...)      { \
        my_panic(__VA_ARGS__); \
}

#define test_assert_if(x)                { \
        if (unlikely(!(x))) { \
            test_panic("assertion ("#x ") %s:%d failed!", \
                         __FILE__, __LINE__); \
        } \
}


int
main()
{
    test_assert_if(0);

    uint64_t a = 10000;
    uint64_t b = 10001;
    test_assert_if(a % b == 0);

    return 0;
}

// Compile program
// gcc -g -o test test.c

// Compile program with Werror=format
// gcc -g -o test -Werror=format test.c

// gcc -g -o test -Wall -Werror -Wno-unused-but-set-variable -Wno-unused-function -Wformat -std=gnu99 -fPIC -fvisibility=hidden test.c