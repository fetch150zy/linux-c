#include <stdio.h>
#include <stdlib.h>

#include "help.h"


void usage(int status)
{
    printf("\033[38;5;46m");
    printf("ml");
    printf("\033[38;5;253m");
    printf(" 0.23.1");
    printf("\nA ls command with colors.\n\n");

    printf("\033[38;5;190m");
    printf("USAGE:\n");
    printf("\033[38;5;253m");
    printf("    ml [OPTIONS] [FILE]...\n\n");

    printf("\033[38;5;190m");
    printf("ARGS:\n");
    printf("\033[38;5;154m");
    printf("    <FILE>...");
    printf("\033[38;5;253m");
    printf("   [default: .]\n\n");

    printf("\033[38;5;190m");
    printf("OPTIONS:\n");

    char ops[6][20] = {
        "-a, --all",
        "-A, --almost-all",
        "-l, --long",
        "-f, --format",
        "-t, --mtsort",
        "-h, --help"
    };
    char ops_detail[6][50] = {
        "Do not ignore entries starting with .",
        "Do not list implied . and ..",
        "Display extended file metadata as a table",
        "Append indicator (one of */=>@|) to entries WHEN",
        "List the sorted items by ctime",
        "Show you the help info about ml"
    };
    for (int i = 0; i < 6; ++i) {
        printf("\033[38;5;118m");
        printf("    %-18s", ops[i]);
        printf("\033[38;5;253m");
        printf("%s\n", ops_detail[i]);
    }

    exit(status);
}