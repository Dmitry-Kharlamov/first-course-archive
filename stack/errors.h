#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED



//=========DESCRIPTIONS OF ERRORS======================

const char* ERRDESCRIP[] =
{
    "START",
    "ERROR! Trying to work with null address!\n",
    "ERROR! Stack counter isn't in range(0, len of stack)!\n",
    "ERROR! Stack length isn't in range(0, MAX LEN)!\n",
    "ERROR! Trying pop empty stack!\n",
    "ERROR! Trying push full stack!\n",
    "ERROR! Can't open log file!\n",
    "NO ERROR!"

};


int error_processing(stack_t* _this, int errnum);

int start_logging(char* log_file);

int end_logging();

#endif // ERRORS_H_INCLUDED
