/*
 * =====================================================================================
 *
 *       Filename:  sort.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/22/2015 14:32:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#define N 10

void insertion_sort(int num[], int a)
{
    int number[a]; 
    int i, j;
    for (i = 0; i < a; i++)
        number[i] = num[i];

    for (i = 0; i < a; i++) {
        for (j = i; j > 0; j--) {
            if (number[j] < number[j-1]) {
                int temp = number[j];
                number[j] = number[j-1];
                number[j-1] = temp;
            }
        }
    }
    /*char *memory = (char*)alloca(a);*/
    /*for (i = 0; i < 3; i++)*/
        /*memory[i] = 'a' + i;*/
    /*printf("%s", memory);*/

    for (i = 0; i < a; i++)
        printf("%d ", number[i]);
    printf("\n");
}

int main()
{
    int number[] = {4, 2, 3, 5, 6, 1};
    insertion_sort(number, sizeof(number) / sizeof(int));
}

