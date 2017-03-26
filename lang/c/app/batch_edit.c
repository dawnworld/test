/*
 * *********************************************************************
 * batch_edit.c
 * Create 2014-02-15 13:59 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

// #include "batch_edit.h"

#include <stdio.h>
#include <string.h>

static int show_help()
{
    int result = 0;
    printf("this is help\n");
    return result;
}

static int handle_one_line(char * line)
{
    int result = 0;
    if(NULL == line) return -1;

    float param = 0.0;
    char head[4] = "";
    char tail[1024] = "";
    result = sscanf(line, "%3sX%fY%s", head, &param, tail);

    if(result < 3) return result;

    printf("get value:%sX%.3fY%s\n", head, param, tail);
    return result;
}

static int read_file(char * path)
{
    int result = 0;

    if(NULL == path) return -1;

    FILE * p_doc;
    p_doc = fopen(path, "r");
    if(NULL == p_doc) return -1;

    char read_buf[1024] = "";
    int line = 0;
    // while((read_size = fread(read_buf, 1, sizeof(read_buf), p_doc)) != 0)
    while(fgets(read_buf, sizeof(read_buf), p_doc))
    {
        printf("line[%d]: %s", ++line, read_buf);
        result = handle_one_line(read_buf);
        memset(read_buf, 0, sizeof(read_buf));
    }
    fclose(p_doc);
    return result;
}

int main(int argc, char** argv)
{
    int result = 0;
    if(argc < 2)
        return show_help();

    read_file(argv[1]);
    return result;
}

