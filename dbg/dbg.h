/*
** File: dbg.h
*/
#ifndef DBG_H__
#define DBG_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ASSIGNED(x) (x != NULL)

#if defined KDEBUG
	#if defined __amigaos4__
		#include <proto/exec.h>
	#endif
#endif


/*--- Assert during compiling (not run-time) ---*/
#define compiler_assert(exp) extern char _compiler_assert[(exp)?1:-1]

#define __PRIV_log(lvl, fmt, ...) fprintf(stderr, "[" lvl "] (%s:%d: errno: %s) " fmt "\n", __FILE__, __LINE__, clean_errno(), __VA_ARGS__)

#ifndef DEBUG
	#define debug(M, ...)
	#define hexdump(buffer, size)
#else
	#if defined KDEBUG && defined __amigaos4__
		#define __PRIV_debug(fmt, ...) DebugPrintF("DEBUG %s:%d: "fmt"\n", __FILE__, __LINE__, __VA_ARGS__)
		#undef __PRIV_log
		#define __PRIV_log(lvl, fmt, ...) DebugPrintF("[" lvl "] (%s:%d: errno: %s) " fmt "\n", __FILE__, __LINE__, clean_errno(), __VA_ARGS__)
	#else
		#define __PRIV_debug(fmt, ...) fprintf(stderr, "DEBUG %s:%d: "fmt"\n", __FILE__, __LINE__, __VA_ARGS__)
	#endif
	#define debug(...) __PRIV_debug(__VA_ARGS__, 0)
#endif


#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(...) __PRIV_log("ERROR", __VA_ARGS__, 0)

#define log_warn(...) __PRIV_log("WARN", __VA_ARGS__, 0)

#define log_info(...) __PRIV_log("INFO", __VA_ARGS__, 0)

#define check(A, ...) if(!(A)) { log_err(__VA_ARGS__); errno=0; goto on_error; }
#define sentinel(...)  { log_err(__VA_ARGS__); errno=0; goto on_error; }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, ...) if(!(A)) { debug(__VA_ARGS__); errno=0; goto on_error; }
 

#define BYTES_PER_DUMP_LINE 16

#define hexdump(buffer, len) do {\
    char *ptr = (char *)buffer;\
    int size = len;\
    char ascii[BYTES_PER_DUMP_LINE + 1];\
    int idx = 0;\
    int ofs = 0;\
    ascii[BYTES_PER_DUMP_LINE] = '\0';\
    printf("%04x  ", ofs);\
    while(size > 0)\
    {\
        printf("%02hhx ", *ptr);\
        if((*ptr >= ' ') && (*ptr <= '~'))\
        {\
            ascii[idx] = *ptr;\
        }\
        else\
        {\
            ascii[idx] = '.';\
        }\
        idx++;\
        ptr++;\
        --size;\
        if(idx == BYTES_PER_DUMP_LINE)\
        {\
            printf(" %s\n", ascii);\
            if(size != 0)\
            {\
                ofs += BYTES_PER_DUMP_LINE;\
                printf("%04x: ", ofs);\
            }\
            idx = 0;\
        }\
    }\
    if(idx != 0)\
    {\
        ascii[idx] = '\0';\
        while(idx < BYTES_PER_DUMP_LINE)\
        {\
            printf("   ");\
            idx++;\
        }\
        printf(" %s\n", ascii);\
    }\
} while(0)


#endif // DBG_H__
