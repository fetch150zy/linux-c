#ifndef LONG_H_
#define LONG_H_

#include <stdbool.h>

char get_type(const char *full_path);

char *get_mod(const char *full_path);

char *get_ower(const char *full_path);

char *get_grp(const char *full_path);

unsigned long get_file_size(const char *full_path);

char *get_time(const char *full_path);

void long_list(const char *full_path, bool *color_file,
                                      char *fmt_file,
                                      int *fmt_lens);
                                      
void get_lens_of_ower_and_grp(char *fullpath,
                                char **file_list,
                                int cnt,    
                                int *fmt_lens);

#endif

