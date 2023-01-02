#ifndef TOOLS_H_
#define TOOLS_H_

void anlz_args(int argc, char **argv,
                         char **path,
                         char **ops,
                         int *pcnt, int *ocnt,
                         bool *has_ops);

void display_ml(char **path, int pcnt,
                             char **ops, 
                             int ocnt, 
                             bool *has_ops);

#endif

