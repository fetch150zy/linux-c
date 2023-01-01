#ifndef LIST_H_
#define LIST_H_

#include <errno.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>


extern int errno;

typedef unsigned char byte;


char get_type(const char *file_name);

char *get_mod(const char *file_name);

char *get_ower(const char *file_name);

char *get_grp(const char *file_name);

u_int32_t get_file_size(const char *file_name);

char *get_time(const char *file_name);


#endif

