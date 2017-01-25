#include <stdio.h>
#include <stdlib.h>

int towers[3] = {0,0,0};
int n = -1;
int endIndex = -1, startIndex;

void printTower()
{
    int i;
    for(i=0; i<3; i++) printf("  %3d", towers[i]);
    printf("\n");
}


void getInput()
{
    char pos;
    int index = -1;
    do
    {
        printf("Enter the position of the Disk(A,B,C)\n");
        scanf("%c", &pos);
        switch(pos)
        {
            case 'a':
            case 'A':
                index = 0;
                break;
            case 'b':
            case 'B':
                index = 1;
                break;
            case 'c':
            case 'C':
                index = 2;
                break;
        }
        getchar();
    }
    while(index == -1);
    startIndex = index;
    do
    {
        printf("Enter the end position of the Disk(A,B,C)\n");
        scanf("%c", &pos);
        switch(pos)
        {
            case 'a':
            case 'A':
                endIndex = 0;
                break;
            case 'b':
            case 'B':
                endIndex = 1;
                break;
            case 'c':
            case 'C':
                endIndex = 2;
                break;
        }
    }
    while(endIndex == -1);

    printf("Enter the number of disks:\n");
    scanf("%d",  towers+index);
    n = towers[index];
    printTower();
    return;
}


/*
Consider
  0    -   -             -       -    -          -    -    0
 000   -   -     ===>    -       0    -   ===>   -    -   000
00000  -   -           00000    000   -          -    -  00000
Thus we can consider the task of having to move a stack of n to another spot,
as being composed of the smaller problem of moving a stack of n-1 to the
remaining free spot.
*/

void moveStack(int start, int end, int number)
{
    //If we're just trying to move one - w. prerequisite assumption that
    //we're calling the function on a valid value of start and end, we can
    //move and do nothing else;
    if(number == 1) {
        towers[start]--;
        towers[end]++;
        printTower();
        return;
    }
    //If we can't do that, then we must solve the subproblem off moving the n-1 stack to an aux pos,
    // before moving the plate.

    int sidePos; //variable to hold the auxiliary position we use to move the stack of n-1;
    switch(start+end)
    {
        //If start = 0 or 1 and end = 1 or 0
        case 1:
            sidePos = 2;
            break;
        //If start = 0 or 2 and end = 2 or 0
        case 2:
            sidePos = 1;
            break;
        //If start = 1 or 2 and end = 2 or 1
        case 3:
            sidePos = 0;
            break;
    }
    //Move the subStack.
    moveStack(start, sidePos, number-1);
    //Move the remaining one plate.
    moveStack(start, end, 1);
    //Move the substack from sidePos to end.
    moveStack(sidePos, end, number-1);
}



int main()
{
    getInput();
    moveStack(startIndex, endIndex, n);
}

