#ifndef __debug_h__
#define __debug_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//#define NODEBUG

#define lambda(l_ret_type, l_arguments, l_body)       \
({                                                    \
    l_ret_type l_anonymous_functions_name l_arguments \
    l_body                                            \
    &l_anonymous_functions_name;                      \
})


#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define check(A, M, ...) if (A) { log_err(M, ##__VA_ARGS__); errno=0; exit(EXIT_FAILURE); }
#define CHECK(A, M, ...) { __label__ error, noerror; if (A) { log_err(M, ##__VA_ARGS__); errno=0; goto error; } else goto noerror; 
#define ONERROR error: 
#define ENDCHECK noerror: ; }

#ifdef NODEBUG
#define log_debug(M, ...)
#else
#define log_debug(M, ...) fprintf(stderr, "[DEBUG] (%s:%d) " M "\n",__FILE__,__LINE__, ##__VA_ARGS__)
#endif

#ifdef NOINFO
#define log_info(M, ...)
#else
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#endif
