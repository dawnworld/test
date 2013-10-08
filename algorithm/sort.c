#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define random(x) (rand()%x)
#define COUNT     10

int swap(int* a, int* b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
    return 0;
}

// print array
int print_array(int* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d\t", *(array + i));
        if(size % 10 == 9)
          printf("\n");
    }
    return 0;
}

int bubble_sort(int* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(*(array + i) > *(array + j))
                swap(array + i, array + j);
        }
        //print_array(array, size);
    }
    //print_array(array, size);
    return 0;
}

int quick_sort(int array[], int left, int right)
{
    if(left >= right) return 0;

    int pivot_key = array[left];
    int i = left, j = right;

    while(i < j)
    {
        while(i < j && array[j] >= pivot_key)
          j--; // get smaller the privot_key
        if(i < j) array[i++] = array[j];
        while(i < j && array[i] <= pivot_key)
          i++;
        if(i < j) array[j--] = array[i];
        //printf("current i: %d, j: %d\n", i, j);
        //print_array(array, COUNT);
    }

    array[i] = pivot_key;

    quick_sort(array, left, i - 1);
    quick_sort(array, i + 1, right);

    return 0;
}

int select_sort(int array[], int size)
{
    int min;
    for(int i = 0; i < size; i ++)
    {
        min = i;
        int j;
        for(j = i + 1; j < size; j++)
        {
            if(array[j] < array[min])
              min = j;
        }
        if(min != i)
          swap(array + i, array + min);
    }

    return 0;
}

int insert_sort(int array[], int size)
{
    int i, j;
    for(i = 1; i < size; i++)
    {
        /* this is first method */
        /*
        for(j = i - 1; j >= 0; j--)
            if(array[i] > array[j]) break;

        if(j == i - 1) continue;
        int temp = array[i];
        int k;
        for(k = i; k > j + 1; k--)
          array[k] = array[k - 1];
        array[j + 1] = temp;
        print_array(array, COUNT);
        */

        /* this is the second method */
        int temp = array[i];
        for(j = i - 1; j >= 0 && array[j] > temp; j--)
          array[j + 1] = array[j]; 
        array[j + 1] = temp;
    }
    return 0;
}

int help()
{
    printf("\t-h\t Help\n");
    printf("\t-b\t Bubble Sort\n");
    printf("\t-s\t Select Sort\n");
    printf("\t-q\t Quick Sort\n");
    printf("\t-i\t Insert Sort\n");
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf(" please choose which method to sort\n");
        help();
        return 0;
    } else if(!strcmp("-h", argv[1])) {
        help();
        return 0;
    }
    // set seed
    srand((int)time(0));
    int array[COUNT];
    printf(" now we will generate %d numbers to sort\n", COUNT);
    for(int i = 0; i < COUNT; i++)
        array[i] = random(100);

    print_array(array, COUNT);

    if(!strcmp("-b", argv[1]))
      bubble_sort(array, COUNT);
    else if(!strcmp("-s", argv[1]))
      select_sort(array, COUNT);
    else if(!strcmp("-q", argv[1]))
      quick_sort(array, 0, COUNT - 1);
    else if(!strcmp("-i", argv[1]))
      insert_sort(array, COUNT);
    else
      help();

    printf("\t------------------------- sorted ------------------------\n");

    print_array(array, COUNT);
    return 0;
}
