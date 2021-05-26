// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  �� �� ��:  common.h
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��08��03��
//  Compiler:  g++
//  ��    ��:  
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
