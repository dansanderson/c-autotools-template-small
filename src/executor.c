#include "executor.h"
#include "cfgfile.h"
#include <stdio.h>

void executor_print_message(void) {
    puts("executor message START");
    cfgfile_print_message();
    puts("executor message END");
}

int executor_do_math(int n) {
    return n * cfgfile_add_one(n);
}
