#include "config.h"
#include "executor.h"
#include "reporter.h"
#include <stdio.h>

int main(int argc, char **argv) {
    puts("main START");
    executor_print_message();
    reporter_print_message();
    puts("main END");
}
