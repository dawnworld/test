#ifndef __ACSM_DEBUG_H__
#define __ACSM_DEBUG_H__

#include "acsmx2.h"

void Print_DFA(ACSM_STRUCT2 * acsm);
void Write_DFA(ACSM_STRUCT2 * acsm, char * f);
int MatchFound (void* id, void * tree, int index, void *data, void * neg_list);

#endif
