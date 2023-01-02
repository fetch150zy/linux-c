#ifndef TOOLS_H_
#define TOOLS_H_

#include "alias.h" 

void anlz_args(int argc, char **argv,
                         char **path,
                         char **ops,
                         byte *pcnt, byte *ocnt,
                         bool *has_ops);

void display_ml(char **path, byte pcnt,
                             char **ops, 
                             byte ocnt, 
                             bool *has_ops);

#endif

