#ifndef LONG_H_
#define LONG_H_

char get_type(const char *full_path);

char *get_mod(const char *full_path);

char *get_ower(const char *full_path);

char *get_grp(const char *full_path);

unsigned int get_file_size(const char *full_path);

char *get_time(const char *full_path);

void long_list(const char *full_path, bool *color_file,
                                      char *fmt_file);


#endif

