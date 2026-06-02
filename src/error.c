#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static const char *error_strings[] = {
    "SUCCESS",
    "NULL_POINTER_EXCEPTION",
    "MEMORY_ALLOCATION_FAILED",
    "OUT_OF_BOUNDS_ERROR"
};

void log_error(ErrorCode code, const char *context, bool is_fatal) {
    // TODO: export logs to an external log file
    fprintf(stderr, "[ERROR] [%s] in context: %s\n", error_strings[code], context);

    if (is_fatal) {
        fprintf(stderr, "[FATAL] Critical error encountered. Terminating application.\n");

        // TODO: save app state/config before force exit
        exit(code);
    }
}
