#ifndef LONG_H_
#define LONG_H_

#include <stdbool.h>

void get_fmt_lens(const char *real_path, const char * const *file_list,
                                  int cnt, int *fmt_lens);

void long_list(const char *full_path, const bool *color_file,
                                      const char *fmt_file, 
                                      const int *fmt_lens);

char get_type(const char *full_path);

char *get_mod(const char *full_path);

char *get_ower(const char *full_path);

char *get_grp(const char *full_path);

unsigned long get_file_size(const char *full_path);

char *get_time(const char *full_path);


#endif

