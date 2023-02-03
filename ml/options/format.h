#ifndef FORMAT_H_
#define FORMAT_H_                          

void format(const char * const *file_list, const char *real_path,
                                           char **fmt_files, int cnt);

void format_single_file(const char *full_path, char *fmt_file);

#endif

