#include <stdio.h>
#include <stdlib.h>

#define F_COUNT 25

int f(int max_num) {
    int before = 1;
    int next   = 1;
    while(next < max_num) {
        int tmp = next;
        next = next + before;
        before = tmp;
    }
    printf(" the max number is %d\n", before);
    return 0;
}

int fibonacci(int f) {
    if(f == 1 || f == 2) return 1;
    return fibonacci(f - 1) + fibonacci(f - 2);
}

int main(int argc, char** argv) {
    if(argc != 3)
        printf("please input the correct arguments\n");
    
    printf("you put the number is %s, and fibonacci count is %s\n", argv[1], argv[2]);

    int max_num = atoi(argv[1]);
    int fibo_count = atoi(argv[2]);

    f(max_num);
    
    for(int i = 1; i < fibo_count; i++) {
        printf("%d\t", fibonacci(i));
    }
    printf("\n");
    //printf("the calc fibonacci is %d\n", fibonacci(fibo_count));
    return 0;
}
