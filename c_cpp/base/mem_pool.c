/*
 * mem_pool.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mem_pool.h"

#define INIT_SIZE 1 * 1 * 204
// split alignment: [0]8, [1]16, [2]24, [3]32, [4]64 ... [15]128
#define MAX_FREE 16

#define MAX_ALLOC_SIZE 128

static void * free_start = NULL;
static void * free_end   = NULL;

static MemObj* free_queue_head[MAX_FREE];
static MemObj* free_queue_tail[MAX_FREE];

long use_count = 0;

typedef struct s_segment_node
{
    void * segment_start;
    struct s_segment_node * next;
}SegmentNode;

static SegmentNode * segment_list = NULL;

static void append_segment(void * p_start)
{
    if(NULL == p_start) return;

    SegmentNode * node = (SegmentNode *)calloc(1, sizeof(SegmentNode));
    node->segment_start = p_start;
    node->next = NULL;

    if(NULL == segment_list)
    {
        segment_list = node;
        printf(" append another segment:%d, segment:%lx\n", INIT_SIZE, node->segment_start);
        return;
    }

    /* append to segment list */
    SegmentNode * tmp_node = segment_list;
    while(NULL != tmp_node->next)
        tmp_node = tmp_node->next;

    tmp_node->next = node;
    printf(" append another segment:%d, segment:%lx\n", INIT_SIZE, node->segment_start);
}

static void init_mem_pool(void)
{
    free_start = calloc(1, INIT_SIZE);

    append_segment(free_start);

    free_end = free_start + INIT_SIZE;
}

static int align_size(int obj_size)
{
    return ((obj_size + 7) & ~7);
}

static int get_free_queue_index(int size)
{
    return ((size + 7) / 8 - 1);
}


void * pool_alloc(int count, int size)
{
    if(0 == size) return NULL;

    if(size >= MAX_ALLOC_SIZE)
        return calloc(count, size);

    int align = align_size(size);
    int index = get_free_queue_index(align);
    
    /* first get memory from queue */
    if(NULL != free_queue_head[index])
    {
        MemObj * result_obj = free_queue_head[index];
        free_queue_head[index] = result_obj->next;
        
        if(NULL == free_queue_head[index])
            free_queue_tail[index] = NULL;
        
        use_count += align;

        return (void *)(result_obj->data);
    }

    if(NULL == free_start) init_mem_pool();

    int free_left = free_end - free_start;
    if(free_left >= align)
    {
        char * pRes = free_start;
        free_start += align;
        use_count += align;
        return (void *)pRes;
    }

    /* free left can not be use align, add left to free queue first */
    use_count += free_left;
    pool_free(free_start, free_left);
    
    /* then increase memory pool */
    free_start = calloc(1, INIT_SIZE);
    append_segment(free_start);
    free_end = free_start + INIT_SIZE;

    return pool_alloc(count, size);
}

void pool_free(void * elem, int size)
{
    int align;
    int index;

    if(NULL == elem || 0 == size)
        return;

    if(size >= MAX_ALLOC_SIZE)
        return free(elem);

    align = align_size(size);
    index = get_free_queue_index(size);

    MemObj * append_obj = (MemObj *)elem;
    append_obj->next = NULL;
    if(NULL != free_queue_tail[index])
        free_queue_tail[index]->next = append_obj;
    
    free_queue_tail[index] = append_obj;
    
    if(NULL == free_queue_head[index])
        free_queue_head[index] = append_obj;

    use_count -= align;
}

void destroy_pool(void)
{
    SegmentNode * tmp_node = segment_list;
    while(NULL != tmp_node)
    {
        printf(" ready to free a segment, start:%lx\n", tmp_node->segment_start);
        SegmentNode * cur_node = tmp_node;
        tmp_node = tmp_node->next;

        if(NULL != tmp_node)
            printf(" next node:%lx\n", tmp_node->segment_start);
        free(cur_node->segment_start);
        free(cur_node);
    }
    free_end = NULL;
    free_start = NULL;
}
