/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  Definition of log function
 *
 *        Version:  1.0
 *        Created:  12/11/2014 04:08:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_LOG_H
#define GDBUG_SCARAB_LOG_H

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>

#define RL_ONE      1
#define RL_TWO      2
#define RL_THREE    3
#define RL_FOUR     4
#define RL_FIVE     5

void report(int level, const char *fmt, ...);

#endif
