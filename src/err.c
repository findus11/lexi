#include "lexi/err.h"

#include <stdio.h>
#include <stdlib.h>

// Report an error and exit the program.
void die(const char *msg) {
    perror(msg);
    exit(1);
}

