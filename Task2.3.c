#include <stdio.h>
#include <stdlib.h>

int *items;
int n;
float scale = 100;

void safeMoveForward(float length) {
    printf("Moving forward %f mm\n", length);
}

void flashLights() {
    printf("Flashing Lights\n");
}

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
    int i,j;
    int currentPos = 0;
    while(swap) {
        swap = 0;

        for(i = 0; i<n-1; i++) {
            if(items[i]>items[i+1]) {
                safeMoveForward(((float)(i-currentPos) + 0.5F)*scale);
                printf("%d - %d\n", i, currentPos);
                currentPos = i;
                items[i] = items[i+1] + items[i] - (items[i+1] = items[i]);
                flashLights();
                safeMoveForward(-0.5F*scale);
                swap = 1;
            }
            for(j=0; j<n; j++) {
                printf(" %d ", items[j]);
            }
            printf("\n");
        }
    }
    safeMoveForward((n/2-currentPos)*scale);
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

