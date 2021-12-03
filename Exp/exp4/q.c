/*********************************
 * @Author: wang
 * @LastEditTime: 2021-12-01 16:04:45
 * @FilePath: /Compiler/Exp/exp4/q.c
 * @Description:
 *********************************/
#include <stdio.h>
#include "qlibc.h"

qhashtbl_t *symTab;
qstack_t *scope;

symTab = qhashtbl(0, QHASHTBL_THREADSAFE);
scope = qstack(QSTACK_THREADSAFE);