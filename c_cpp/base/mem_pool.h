/*
 * mem_pool.h
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#pragma pack(push, 1)


typedef union mem_obj
{
    union mem_obj * next;
    char  data[1];
} MemObj;

/*
 * alloc memory from memory pool
 * @return memory pointer
 */
void * pool_alloc(int count, int size);

/*
 * free pool memory
 * @param elem, alloc element memory
 */
void   pool_free(void * elem, int size);

void   destroy_pool(void);

#pragma pack(pop)
#endif /* !__MEM_POOL_H__ */

