#ifndef TOOLS_H_
#define TOOLS_H_

bool anlz_args(int argc, char **argv, char **path, int *pcnt,
                                      bool *has_ops, char *single_file);

void allpath_display(const char * const *paths, int pcnt, const bool *has_ops);

void single_file_display(const char *single_file, const bool *has_ops);
                                           
#endif

