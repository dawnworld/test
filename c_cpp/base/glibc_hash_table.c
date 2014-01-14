/*
 * glibc_hash_table.c
 * Copyright (C) 2013 mac <mac@Dawnworld>
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

static GHashTable * rai_hash_table = NULL;
static GHashTable * ptmsi_hash_table = NULL;
static GHashTable * imsi_hash_table = NULL;

#define MAX_LOOP 100 * 10000
// typedef struct s_rai_ptmsi {
//     uint32 pLMNid;
//     uint16 lac;
//     uint8  rac;    
// } RaiPtmsi;

typedef struct s_plmn_id {
    uint16 m_mcc;
    uint8  m_mnc;
}PlmnId;

typedef struct s_rai {
    uint16 m_lac;
    uint8  m_rac;
}RAI;


// static long gen_hash_func(gconstpointer p_key)
// {
//     RaiPtmsi * pkey = (RaiPtmsi *)p_key;
//     long key = 0;
//     key = (key | pkey->pLMNid) << 16;
//     key = (key | pkey->lac) << 8;
//     key = key | pkey->rac;

//     return key;
// }

static int parse_plmn_id(uint32 data, PlmnId * plmn_id)
{
    int result = 0;
    if(NULL == plmn_id)
        return -1;
    // printf("data : %0x, <<12:%x, <<8:%x, <<20:%x\n", data, data<<12, data<<8, data<<20);
    uint16 mcc = 0, mnc = 0;
    // printf("data << 12 then >> 28: %x, %d\n", (data << 12) >> 28, (data << 12) >> 28);
    // printf("data << 8 then >> 28: %x, %d\n", (data << 8) >> 28, (data << 8) >> 28);
    // printf("data << 20 then >> 28: %x, %d\n", (data << 20) >> 28, (data << 20) >> 28);
    mcc += ((data << 12) >> 28) * 100 + ((data << 8) >> 28) * 10 + ((data << 20) >> 28);
    mnc += ((data << 28) >> 28) * 10 + ((data << 24) >> 28);

    // printf("mcc:%d, mnc:%d\n", mcc, mnc);

    plmn_id->m_mcc = mcc;
    plmn_id->m_mnc = mnc;
    
    return result;
}

static gboolean plmn_equal_func(gconstpointer key_a, gconstpointer key_b)
{
    PlmnId * plmn_key_a = (PlmnId *)key_a;
    PlmnId * plmn_key_b = (PlmnId *)key_b;

    if(plmn_key_a->m_mcc != plmn_key_b->m_mcc ||
       plmn_key_a->m_mnc != plmn_key_b->m_mnc)
        return FALSE;

    return TRUE;
}

static gboolean rai_equal_func(gconstpointer key_a, gconstpointer key_b)
{
    RAI * rai_a = (RAI *) key_a;
    RAI * rai_b = (RAI *) key_b;

    if(rai_a->m_lac != rai_b->m_lac ||
       rai_b->m_rac != rai_b->m_rac)
        return FALSE;

    return TRUE;
}

static gboolean ptmsi_equal_func(gconstpointer key_a, gconstpointer key_b)
{
    uint32 * ptmsi_a = (uint32 *)key_a;
    uint32 * ptmsi_b = (uint32 *)key_b;

    return *ptmsi_a == *ptmsi_b ? TRUE : FALSE;
}

static void print_hash_table(gpointer key, gpointer value, gpointer user_data)
{
    PlmnId * plmn_id = (PlmnId *)key;
    RAI * rai = (RAI *) value;
    if(NULL == rai) return;
    uint32 * p_tmsi = (uint32 *)g_hash_table_lookup(ptmsi_hash_table, (gconstpointer)rai);
    if(NULL == p_tmsi) return;
    uint64* imsi = (uint64 *) g_hash_table_lookup(imsi_hash_table, (gconstpointer)p_tmsi);
    printf("mcc:%d, mnc:%d, lac:%x, rac:%x, p-tmsi:%x, imsi:%ld\n",
            plmn_id->m_mcc, plmn_id->m_mnc, rai->m_lac, rai->m_rac, *p_tmsi, *imsi);
}

guint plmn_hash_func(gconstpointer g_key)
{
    guint hash_key = 0;
    PlmnId * p_key = (PlmnId *)g_key;
    hash_key = (hash_key | p_key->m_mcc) << 8;
    hash_key = (hash_key | p_key->m_mnc);

    // printf(" mcc:%d(%0hx), mnc:%hhd(%0hhx), hash_key:%0x\n", 
    //         p_key->m_mcc, p_key->m_mcc, p_key->m_mnc, p_key->m_mnc, hash_key);

    return hash_key;
}

guint rai_hash_func(gconstpointer g_key)
{
    guint hash_key = 0;
    RAI * r_key = (RAI *) g_key;
    hash_key = (hash_key | r_key->m_lac) << 8;
    hash_key = hash_key | r_key->m_rac;
    
    // printf(" lac:%0hx, rac:%0hhx, hash_key:%x\n", r_key->m_lac, r_key->m_rac, hash_key);
    return hash_key;
}
int main()
{
    if(NULL == rai_hash_table)
        rai_hash_table = g_hash_table_new(plmn_hash_func, plmn_equal_func);
    if(NULL == ptmsi_hash_table)
        ptmsi_hash_table = g_hash_table_new(rai_hash_func, rai_equal_func);
    if(NULL == imsi_hash_table)
        imsi_hash_table = g_hash_table_new(g_int_hash, ptmsi_equal_func);

    int i;
    struct timeval insert_start_time, insert_end_time;
    gettimeofday(&insert_start_time, NULL);
    for(i = 0; i < MAX_LOOP; i++)
    {
        uint32 payload_plmn = 0x64f010 + i;
        PlmnId *plmn_id;
        plmn_id = (PlmnId *)calloc(1, sizeof(PlmnId));

        parse_plmn_id(payload_plmn, plmn_id);

        RAI *rai = (RAI *)calloc(1, sizeof(RAI));
        rai->m_lac = 0xac31 + i;
        rai->m_rac = 0x96;
        g_hash_table_insert(rai_hash_table, (gpointer)plmn_id, (gpointer)rai);

        uint32 *p_tmsi = (uint32 *)calloc(1, sizeof(uint32));
        *p_tmsi = 0xc3051071 + i;
        g_hash_table_insert(ptmsi_hash_table, (gpointer)rai, (gpointer)p_tmsi);

        uint64 *imsi = (uint64 *)calloc(1, sizeof(uint64));
        *imsi = 460016151927589;
        g_hash_table_insert(imsi_hash_table, (gpointer)p_tmsi, (gpointer)imsi);

    }
    gettimeofday(&insert_end_time, NULL);
    int time_use_usec = 1000000 * (insert_end_time.tv_sec - insert_start_time.tv_sec) +
        (insert_end_time.tv_usec - insert_start_time.tv_usec);

    printf(" time_use: %d seconds, %d millseconds, %d useconds\n", time_use_usec / 1000000,
            (time_use_usec % 1000000) / 1000, time_use_usec % 1000);

    printf("rai_hash_table size:%u, ptmsi_hash_table size:%u, imsi_hash_table size:%u\n",
            g_hash_table_size(rai_hash_table), g_hash_table_size(ptmsi_hash_table),
            g_hash_table_size(imsi_hash_table));

    struct timeval search_start_time, search_end_time;
    gettimeofday(&search_start_time, NULL);
    uint32 s_payload = 0x64f010;
    PlmnId * s_plmn_id;
    s_plmn_id = (PlmnId *)calloc(1, sizeof(PlmnId));

    parse_plmn_id(s_payload, s_plmn_id);
    gpointer look_result = g_hash_table_lookup(rai_hash_table, (gconstpointer)s_plmn_id);
    if(NULL == look_result)
        return -1;
    RAI * look_rai = (RAI *)look_result;
    // printf("RAI lac:%x, rac:%x\n", look_rai->m_lac, look_rai->m_rac);

    look_result = g_hash_table_lookup(ptmsi_hash_table, (gconstpointer)look_rai);
    if(NULL == look_result)
        return -1;
    uint32 * look_ptmsi = (uint32 *)look_result;
    // printf("Get ptmsi:%x\n", *look_ptmsi);

    look_result = g_hash_table_lookup(imsi_hash_table, (gconstpointer)look_ptmsi);
    if(NULL == look_result)
        return -1;
    uint64 * look_imsi = (uint64 *)look_result;

    gettimeofday(&search_end_time, NULL);
    time_use_usec = 1000000 * (search_end_time.tv_sec - search_start_time.tv_sec) +
        (search_end_time.tv_usec - search_start_time.tv_usec);

    printf(" time_use: %d seconds, %d millseconds, %d useconds\n", time_use_usec / 1000000,
            (time_use_usec % 1000000) / 1000, time_use_usec % 1000);

    printf("Get imsi:%lx\n", *look_imsi);

    // g_hash_table_foreach(rai_hash_table, print_hash_table, NULL);

    return 0;
}

