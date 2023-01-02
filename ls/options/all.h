#ifndef ALL_H_
#define ALL_H_

#include "../alias.h"

void normal(const char *path, byte *cnt,
                              char **file_list,
                              const byte file_name_length);

void all(const char *path, byte *cnt,
                           char **file_list,
                           const byte file_name_length);

void almost_all(const char *path, byte *cnt,
                           char **file_list,
                           const byte file_name_length);

#endif

