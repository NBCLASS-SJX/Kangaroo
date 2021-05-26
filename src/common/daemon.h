// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  common.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _DAEMON_H_H_H
#define _DAEMON_H_H_H

#if defined(__GNUC__)

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef void(*sighandler_t)(int);

/* run with deamon */
void run_daemon();

#endif

#endif
