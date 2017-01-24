#include <stdio.h>
#include <stdlib.h>

int *items;
int n;

void getInput() {
    printf("Enter the number of elements:\n");
    scanf("%d", &n);
    printf("Enter %d integers:\n", n);
    int i;
    items = malloc(sizeof(int)*n);
    for(i = 0; i<n; i++) {
        scanf("%d", items+i);
    }
    return;
}

void sortItems() {
    int swap = 1;
    int i;
    while(swap) {
        swap = 0;

        for(i = 0; i<n-1; i++) {
            if(items[i]>items[i+1]) {
                items[i] = items[i+1] + items[i] - (items[i+1] = items[i]);
                swap = 1;
            }
        }
    }

    return;
}

void returnResult() {
    printf("Sorted list in ascending order:\n");
    int i;
    for(i=0; i<n; i++) {
        printf(" %d ", items[i]);
    }
    return;
}

int main()
{
    getInput();
    sortItems();
    returnResult();
}

