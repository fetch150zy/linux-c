#ifndef LINUX_C_UTILS_H
#define LINUX_C_UTILS_H

#include <stdlib.h>

#define exit_if(condition, failed_ret, stream, format, args...) \
            if (condition) { \
                fprintf(stream, format, ##args); \
                exit(failed_ret); \
            }

#endif
