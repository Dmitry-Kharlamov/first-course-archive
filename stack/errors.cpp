#include <stdio.h>
#include "errors.h"

int error_processing(stack_t* _this, int errnum)
{
    if (!errnum) return 0;

    stack_Dump(_this);
    end_logging();

    fprintf(stderr, "%s", ERRDESCRIP[errnum - STARTERRORS]);

}

int start_logging(char* log_file)
{
    ISNOTNULL(log_file);

    fflush(stderr);
    fclose(stderr);

    if(!freopen(log_file, "w", stderr)) return ELOGFILE;

    return 0;
}

int end_logging()
{
    fflush(stderr);
    fclose(stderr);

    dup2(fileno(stdout), fileno(stderr));

    return 0;
}

