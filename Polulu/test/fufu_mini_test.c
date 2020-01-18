#include <stdio.h>
int main(){
    /* Assume the wheel encoder gives 360 degree readings */
    int angle, distance, left_wheel, right_wheel;

    printf("Readings for left wheel:\n");
    scanf("%d", &left_wheel);
    printf("Readings for right wheel:\n");
    scanf("%d", &right_wheel);

    printf("Distance: %d, Angle: %d.\n", distance, angle);
    return 0;
}
