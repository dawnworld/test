/*
 * *********************************************************************
 * main.c
 * Create 2014-04-07 08:24 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*HandleCallBack)(char* buf, FILE* fd, void *data);

typedef struct _Point {
    float   x;
    float   y;
}Point;

static int write_to_new_file(char *buf, FILE * fd)
{
    int res = 0;

    if(NULL == fd || NULL == buf) return -1;
    fwrite(buf, 1, strlen(buf) + 1, fd);
    return res;
}

static int handle_each_line(char * line, Point *last_point)
{
    int res = 0;
    Point point;

    if(NULL == line) return -1;

    memset(&point, 0, sizeof(Point));
    res = sscanf(line, "%f\t%f", &point.x, &point.y);

    if(res < 2) return res;

    if(last_point->x != -1 && last_point->y != -1)
    {
        if(point.y - last_point->y >= 0.025f)
            return -1;
        if(last_point->y - point.y >= 0.025f)
            return -1;
    }

    last_point->x = point.x;
    last_point->y = point.y;

    return res;
}

static void show_help()
{
    printf(" Usage: FilterNum [CheckValue like: 0.025] [Source data.txt] [Save data.txt]");
    return;
}

int main(int argc, char** argv)
{
    int res = 0;

    FILE * read_fd = NULL;
    FILE * new_fd = NULL;

    char read_buf[1024] = "";
    int line = 0;
    float check_value;
    Point last_point;

    if(argc < 4) {
        show_help();
        return -1;
    }

    check_value = atof(argv[1]);

    read_fd = fopen(argv[2], "r");
    printf("\n##################################################\n");
    if(NULL == read_fd)
    {
        printf("ERROR: could not read file, maybe it does not exist\n");
        return -1;
    }

    printf(" [INFO] Open Source Txt[%s] Success!\n", argv[2]);

    new_fd = fopen(argv[3], "w");
    if(NULL == new_fd)
    {
        printf("ERROR: could not write to file\n");
        fclose(read_fd);
        return -1;
    }

    printf(" [INFO] Open Save Txt[%s] Success!\n", argv[3]);
    printf("##################################################\n\n");

    last_point.x = -1;
    last_point.y = -1;
    // while((read_size = fread(read_buf, 1, sizeof(read_buf), p_doc)) != 0)
    while(fgets(read_buf, sizeof(read_buf), read_fd))
    {
        ++line;
        res = handle_each_line(read_buf, &last_point); 

        if(res == -1)
            printf(" [INFO][WRONG DATA] Line:[%d]: %s", line, read_buf); 
        else 
            write_to_new_file(read_buf, new_fd);

        memset(read_buf, 0, sizeof(read_buf));
    }
    printf("\n");

    fclose(read_fd);
    fclose(new_fd);

    return res;
}



