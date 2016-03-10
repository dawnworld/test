/*
 * test_glib_base.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */


#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <sys/time.h>

typedef unsigned int   uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;
typedef unsigned long  uint64;

uint32 alloc_size = 0;
#define HASH_TABLE_CALLOC(A)    (alloc_size += A, calloc(1, A));
#define HASH_TABLE_FREE(A, B)   (alloc_size -= B, free(A));

static GHashTable * test_table;


static gboolean test_equal_func(gconstpointer key_a, gconstpointer key_b)
{
    int * a = (int *)key_a;
    int * b = (int *)key_b;

    return *a == *b;
}

static void destroy_key(gpointer data)
{
    int * key = (int *)data;
    HASH_TABLE_FREE(key, sizeof(int));
}

static void destroy_value(gpointer data)
{
    int * value = (int *)data;
    HASH_TABLE_FREE(value, sizeof(int));
}

static void test_hash_table()
{
    test_table = g_hash_table_new_full(g_int_hash, test_equal_func, destroy_key, destroy_value);
    int * key1 = HASH_TABLE_CALLOC(sizeof(int));
    int * value1 = HASH_TABLE_CALLOC(sizeof(int));
    *key1 = 1;
    *value1 = 2;
    g_hash_table_insert(test_table, (gpointer) key1, (gpointer)value1);

    int * key2 = HASH_TABLE_CALLOC(sizeof(int));
    int * value2 = HASH_TABLE_CALLOC(sizeof(int));
    *key2 = 1;
    *value2 = 4;
    g_hash_table_replace(test_table, (gpointer) key2, (gpointer)value2);

    g_hash_table_destroy(test_table);
    test_table = NULL;

}

int main(int argc, char** argv)
{
    int result = 0;
    printf(" test glib hash table\n");
    test_hash_table();
    printf(" hash_table use memory:%d\n", alloc_size);

    return result;
}
