//
// Created by fetch150zy on 1/9/23.
//

#ifndef LINUX_C_MACRO_H
#define LINUX_C_MACRO_H

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

#endif //LINUX_C_MACRO_H
