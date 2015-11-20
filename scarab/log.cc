/*
 * =====================================================================================
 *
 *       Filename:  log.cc
 *
 *    Description:  implement the log function
 *
 *        Version:  1.0
 *        Created:  12/11/2014 04:13:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "log.h"

static const char *getTimestamp()
{
    static char buf[20];

    /* TODO: option for showing timestamp or not*/
    if (1) {
        time_t pt = time(NULL);
        tm* local = localtime(&pt);
        sprintf(buf, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
        return buf;
    }
    else
        return "";
}

void report(int level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[%s]", getTimestamp());
    for (int i = 1; i < level; i++)
        fprintf(stdout, "\t");
    fprintf(stdout, "[%d] ", level);

    vfprintf(stdout , fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}
