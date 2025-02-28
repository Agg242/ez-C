/*
** File: dbg.h
*/
#ifndef DBG_H__
#define DBG_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ASSIGNED(x__) (NULL != x__)

#if defined KDEBUG
	#if defined __amigaos4__
		#include <proto/exec.h>
	#endif
#endif


/*--- Assert during compiling (not run-time) ---*/
#define compiler_assert(exp__) extern char _compiler_assert[(exp__)?1:-1]

#define __PRIV_log(lvl__, fmt__, ...) fprintf(stderr, "[" lvl__ "] (%s:%d: errno: %s) " fmt__ "\n", __FILE__, __LINE__, clean_errno(), __VA_ARGS__)

#ifndef DEBUG
	#define debug(...)
#else
	#if defined KDEBUG && defined __amigaos4__
		#define __PRIV_debug(fmt__, ...) DebugPrintF("DEBUG %s:%d: "fmt__"\n", __FILE__, __LINE__, __VA_ARGS__)
		#undef __PRIV_log
		#define __PRIV_log(lvl__, fmt__, ...) DebugPrintF("[" lvl__ "] (%s:%d: errno: %s) " fmt__ "\n", __FILE__, __LINE__, clean_errno(), __VA_ARGS__)
	#else
		#define __PRIV_debug(fmt__, ...) fprintf(stderr, "DEBUG %s:%d: "fmt__"\n", __FILE__, __LINE__, __VA_ARGS__)
	#endif
	#define debug(...) __PRIV_debug(__VA_ARGS__, 0)
#endif


#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(...) __PRIV_log("ERROR", __VA_ARGS__, 0)

#define log_warn(...) __PRIV_log("WARN", __VA_ARGS__, 0)

#define log_info(...) __PRIV_log("INFO", __VA_ARGS__, 0)

#define check(A__, ...) if(!(A__)) { log_err(__VA_ARGS__); errno=0; goto on_error; }
#define check_err(A__, E__, ...) if(!(A__)) { log_err(__VA_ARGS__); errno=0; result=E__; goto on_error; }

#define sentinel(...)  { log_err(__VA_ARGS__); errno=0; goto on_error; }
#define check_mem(A__) check((A__), "Out of memory.")
#define check_debug(A__, ...) if(!(A__)) { debug(__VA_ARGS__); errno=0; goto on_error; }
 

#define BYTES_PER_DUMP_LINE 16
#define BYTES_PER_LONGWORD 4

#define hexdump(buffer__, len__) do {\
    char *ptr__ = (char *)buffer__;\
    int size__ = len__;\
    int bytes__ = 0;\
    char ascii__[BYTES_PER_DUMP_LINE + 1];\
    int idx__ = 0;\
    int ofs__ = 0;\
    ascii__[BYTES_PER_DUMP_LINE] = '\0';\
    printf("%04x: ", ofs__);\
    while(size__ > 0)\
    {\
        printf("%02hhx", *ptr__);\
        if((*ptr__ >= ' ') && (*ptr__ <= '~'))\
        {\
            ascii__[idx__] = *ptr__;\
        }\
        else\
        {\
            ascii__[idx__] = '.';\
        }\
        idx__++;\
        ptr__++;\
        --size__;\
        bytes__++;\
        if(idx__ == BYTES_PER_DUMP_LINE)\
        {\
            printf(" %s\n", ascii__);\
            if(size__ != 0)\
            {\
                ofs__ += BYTES_PER_DUMP_LINE;\
                printf("%04x: ", ofs__);\
            }\
            idx__ = 0;\
            bytes__ = 0;\
        }\
        else if(BYTES_PER_LONGWORD == bytes__)\
        {\
            printf(" ");\
            bytes__ = 0;\
        }\
    }\
    if(idx__ != 0)\
    {\
        ascii__[idx__] = '\0';\
        for(size__ = (BYTES_PER_DUMP_LINE - idx__ - 1) / BYTES_PER_LONGWORD; size__ > 0; size__--) printf(" ");\
        while(idx__ < BYTES_PER_DUMP_LINE)\
        {\
            printf("  ");\
            idx__++;\
        }\
        printf(" %s\n", ascii__);\
    }\
} while(0)


#endif // DBG_H__
