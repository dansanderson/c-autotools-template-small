#include "reporter.h"
#include "cfgfile.h"
#include <stdio.h>

void reporter_print_message(void) {
    puts("reporter message START");
    cfgfile_print_message();
    puts("reporter message END");
}
