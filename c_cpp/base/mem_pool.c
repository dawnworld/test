/*
 * mem_pool.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "mem_pool.h"

#define INIT_SIZE 2 * 1024
// split alignment: [0]8, [1]16, [2]24, [3]32, [4]64 ... [15]128
#define MAX_FREE 16

#define MAX_ALLOC_SIZE 128

static void * free_start = NULL;
static void * free_end   = NULL;

static MemObj* free_queue_head[MAX_FREE];
static MemObj* free_queue_tail[MAX_FREE];

static pthread_mutex_t pool_lock;

long use_count = 0;
long cur_pool_size = 0;
long direct_alloc_size = 0;

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

    cur_pool_size += INIT_SIZE;

    if(NULL == segment_list)
    {
        segment_list = node;
        printf(" append another segment:%d, segment:%lx\n", INIT_SIZE, (long)node->segment_start);
        return;
    }

    /* append to segment list */
    SegmentNode * tmp_node = segment_list;
    while(NULL != tmp_node->next)
        tmp_node = tmp_node->next;

    tmp_node->next = node;
    printf(" append another segment:%d, segment:%lx\n", INIT_SIZE, (long)node->segment_start);
}

void init_pool(void)
{
    pthread_mutex_init(&pool_lock, NULL);
    pthread_mutex_lock(&pool_lock);

    free_start = calloc(1, INIT_SIZE);
    append_segment(free_start);
    free_end = free_start + INIT_SIZE;

    pthread_mutex_unlock(&pool_lock);
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
    int real_size = count * size;

    if(0 == real_size) return NULL;

    if(real_size >= MAX_ALLOC_SIZE) {
        return calloc(count, size);
        
//        void * pMem = malloc(++real_size);
//        memset(pMem, 0, real_size);
//        *(char *)pMem = 1;
//        return pMem + 1;
    }

    pthread_mutex_lock(&pool_lock);

    int align = align_size(real_size + 1);
    int index = get_free_queue_index(align);
    
    /* first get memory from queue */
    if(NULL != free_queue_head[index])
    {
        MemObj * result_obj = free_queue_head[index];
        free_queue_head[index] = result_obj->next;

        if(NULL == free_queue_head[index])
            free_queue_tail[index] = NULL;

        use_count += align;
        /* store this memory chunk size */
        result_obj->data[0] = align;
        /* lock thread */
        pthread_mutex_unlock(&pool_lock);
        return (void *)(result_obj->data + 1);
    }

    if(NULL == free_start) init_pool();

    int free_left = free_end - free_start;
    if(free_left >= align)
    {
        char * pRes = free_start;
        free_start += align;
        use_count += align;
        //memset(pRes, 0, real_size);
        /* store this memory chunk size */
        pRes[0] = align;
        pthread_mutex_unlock(&pool_lock);

        return (void *)(pRes + 1);
    }

    /* free left can not be use align, add left to free queue first */
    use_count += free_left;

    pthread_mutex_unlock(&pool_lock);
    *(char *)free_start = free_left;
    pool_free(free_start + 1, free_left);
    pthread_mutex_lock(&pool_lock);
    
    printf(" will increase block, use count:%ld, align:%d, size:%d\n", use_count, align, size);
    /* then increase memory pool */
    free_start = calloc(1, INIT_SIZE);
    append_segment(free_start);
    free_end = free_start + INIT_SIZE;

    pthread_mutex_unlock(&pool_lock);

    return pool_alloc(count, size);
}

static void pool_free_mem(void * elem)
{
//    int align;
    int index;
    unsigned char size;

    if(NULL == elem ) return;

    size = *(char *)(--elem);

    pthread_mutex_lock(&pool_lock);

    //align = align_size(size);
    index = get_free_queue_index(size);

    memset(elem, 0, size);
    MemObj * append_obj = (MemObj *)elem;
    append_obj->next = NULL;

    if(NULL != free_queue_tail[index])
        free_queue_tail[index]->next = append_obj;

    free_queue_tail[index] = append_obj;

    if(NULL == free_queue_head[index])
        free_queue_head[index] = append_obj;

    use_count -= size;

    pthread_mutex_unlock(&pool_lock);
}

void pool_free(void * elem, int size)
{
    SegmentNode * tmp_node = segment_list;
    while(NULL != tmp_node)
    {
        if(elem > tmp_node->segment_start && elem < tmp_node->segment_start + INIT_SIZE)
            return pool_free_mem(elem);
        
        tmp_node = tmp_node->next;
    }
    free(elem);
}

void destroy_pool(void)
{
    pthread_mutex_lock(&pool_lock);

    SegmentNode * tmp_node = segment_list;
    while(NULL != tmp_node)
    {
        printf(" ready to free a segment, start:%lx\n", (long)tmp_node->segment_start);
        SegmentNode * cur_node = tmp_node;
        tmp_node = tmp_node->next;

        if(NULL != tmp_node)
            printf(" next node:%lx\n", (long)tmp_node->segment_start);
        free(cur_node->segment_start);
        free(cur_node);
        cur_pool_size -= INIT_SIZE;
    }
    free_end = NULL;
    free_start = NULL;

    pthread_mutex_unlock(&pool_lock);
}
