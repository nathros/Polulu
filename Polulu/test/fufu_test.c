/**
 * @file fufu_test.c
 * @brief Fufu's test functions
 * @author Fufu Fang
 */

#include <stdio.h>

#include "../gpio.h"
#include "../led.h"
#include "../mouse.h"
#include "../debug.h"
#include "../motor.h"
#include "../ir.h"
#include "../lcd.h"
#include "../delay.h"
//#include "../vector_logger.h"
//#include "../motor_logger.h"
#include "../coordinate.h"

#include "fufu_test.h"

void size_test()
{
    char a, aa, aaa;
    short b, bb, bbb;
    int c, cc, ccc;
    long d, dd, ddd;
    long long e, ee, eee;

    unsigned char f, ff, fff;
    unsigned short g, gg, ggg;
    unsigned int h, hh, hhh;
    unsigned long i, ii, iii;
    unsigned long long j, jj, jjj;

    a = 1;
    aa = 0;
    do {
        aa += a;
        a <<= 1;
    } while (a > 0);
    aaa = aa + 1;

    b = 1;
    bb = 0;
    do {
        bb += b;
        b <<= 1;
    } while (b > 0);
    bbb = bb + 1;

    c = 1;
    cc = 0;
    do {
        cc += c;
        c <<= 1;
    } while (c > 0);
    ccc = cc + 1;

    d = 1;
    dd = 0;
    do {
        dd += d;
        d <<= 1;
    } while (d > 0);
    ddd = dd + 1;

    e = 1;
    ee = 0;
    do {
        ee += e;
        e <<= 1;
    } while (e > 0);
    eee = ee + 1;

    f = 1;
    ff = 0;
    do {
        ff += f;
        f <<= 1;
    } while (f > 0);
    fff = ff + 1;

    g = 1;
    gg = 0;
    do {
        gg += g;
        g <<= 1;
    } while (g > 0);
    ggg = gg + 1;

    h = 1;
    hh = 0;
    do {
        hh += h;
        h <<= 1;
    } while (h > 0);
    hhh = hh + 1;

    i = 1;
    ii = 0;
    do {
        ii += i;
        i <<= 1;
    } while (i > 0);
    iii = ii + 1;

    j = 1;
    jj = 0;
    do {
        jj += j;
        j <<= 1;
    } while (j > 0);
    jjj = jj + 1;

    dbg_printf("Size of char: %u bytes, max = %d, min= %d \r\n",
               sizeof(char), aa, aaa);
    dbg_printf("Size of short: %u bytes, max = %d, min= %d \r\n",
               sizeof(short), bb, bbb);
    dbg_printf("Size of int: %u bytes, max = %d, min= %d \r\n",
               sizeof(int), cc, ccc);
    dbg_printf("Size of long: %u bytes, max = %ld, min= %ld \r\n",
               sizeof(long), dd, ddd);
    dbg_printf("Size of long long: %u bytes, max = %lld, min= %lld \r\n",
               sizeof(long long), ee, eee);
    dbg_printf("\r\n");
    dbg_printf
        ("Typecasting max_imum of char into an int, the result is: %d",
         (int) aaa);
    dbg_printf("\r\n");
    dbg_printf("Size of unsigned char: %u bytes, max = %d, min= %d \r\n",
               sizeof(char), ff, fff);
    dbg_printf("Size of unsigned short: %u bytes, max = %d, min= %d \r\n",
               sizeof(short), gg, ggg);
    dbg_printf("Size of unsigned int: %u bytes, max = %d, min= %d \r\n",
               sizeof(int), hh, hhh);
    dbg_printf("Size of unsigned long: %u bytes, max = %ld, min= %ld \r\n",
               sizeof(long), ii, iii);
    dbg_printf
        ("Size of unsigned long long: %u bytes, max = %lld, min= %lld \r\n",
         sizeof(long long), jj, jjj);
}

void ir_test()
{
    int a = ir_read(FRONT_LEFT);
    int b = ir_read(FRONT_RIGHT);
    int c = ir_read(BACK_LEFT);
    int d = ir_read(BACK_RIGHT);
    dbg_printf
        ("FRONT_LEFT: %d, FRONT_RIGHT: %d, BACK_LEFT: %d, BACK_RIGHT: %d\r\n",
         a, b, c, d);
}

void lcd_test()
{
    char c = dbg_getchar();
    dbg_printf("%c\r\n", c);
    lcd_printf("%c", c);
}

// void motor_logger_test()
// {
//     int i;
//     motor_logger_init();
//
//     for (i = 1; i < SPEED_LIMIT; i++) {
//         motor_set(MOTOR_LEFT, i);
//         motor_logger_push();
//         delay_start(i * 10);
//     }
//
//     for (i = 1; i < SPEED_LIMIT; i++) {
//         motor_set(MOTOR_RIGHT, i);
//         motor_logger_push();
//         delay_start(i * 10);
//     }
//
//     motor_logger_playback();
// }

void bumping_test()
{
    int i;
    delay_start(3000);
    for (i = 0; i <= SPEED_LIMIT; i += 10) {
        motors_move(i);
        delay_start(1000);
        motors_stop();
        delay_start(1000);
    }
}

void rotate_test()
{
    lcd_clear();
    lcd_printf("Start");

    lcd_next_line();
    lcd_printf("90");
    motor_rot_rel(90);
    dbg_printf("Rotating 90 degress\r\n");
    delay_start(3000);

    lcd_next_line();
    lcd_printf("-90");
    motor_rot_rel(-90);
    dbg_printf("Rotating -90 degress\r\n");
    delay_start(3000);

    lcd_next_line();
    lcd_printf("180");
    motor_rot_rel(180);
    dbg_printf("Rotating 180 degress\r\n");
    delay_start(3000);

    lcd_next_line();
    lcd_printf("-180");
    motor_rot_rel(-180);
    dbg_printf("Rotating -180 degress\r\n");
    delay_start(3000);

    lcd_next_line();
    lcd_printf("270");
    motor_rot_rel(270);
    dbg_printf("Rotating 270 degress\r\n");
    delay_start(3000);

    lcd_next_line();
    lcd_printf("-270");
    motor_rot_rel(-270);
    dbg_printf("Rotating -270 degress\r\n");
    delay_start(3000);

    motors_stop();
    dbg_printf("Motors stopped\r\n");

}

void IRQ_priority_test()
{
    dbg_printf("The Priority grouping is: %h\n\r", NVIC_GetPriorityGrouping());
    dbg_printf("The priority for TIMER0 is: %h\n\r", NVIC_GetPriority(TIMER0_IRQn));
    dbg_printf("The priority for TIMER1 is: %h\n\r", NVIC_GetPriority(TIMER1_IRQn));
    dbg_printf("The priority for TIMER2 is: %h\n\r", NVIC_GetPriority(TIMER2_IRQn));
    dbg_printf("The priority for TIMER3 is: %h\n\r", NVIC_GetPriority(TIMER3_IRQn));
    dbg_printf("The priority for TIMER3 is: %h\n\r", NVIC_GetPriority(EINT3_IRQn));
    dbg_printf("The priority for TIMER3 is: %h\n\r", NVIC_GetPriority(SysTick_IRQn));
//NVIC_SetPriority
//NVIC_GetPriorityGrouping
}

// void vector_logger_test()
// {
//     int i;
//     vector_logger_init();
//
//     for (i = 1; i < SPEED_LIMIT; i++) {
//         motors_move(i);
//         delay_start(i*10);
//         //Changing this to negative results in speed being recorded as negative?!
//         motor_rot_rel(-10);
//         vector_logger_push();
//     }
//
//     vector_logger_playback();
//     motors_stop();
// }

void motors_move_test()
{
    motors_move(SPEED_LIMIT);
    delay_start(3000);
    motors_stop();
}

void gpio_test()
{
    dbg_printf("Collision Sensor: %d\tButton: %d\r\n", gpio_collision_read(), gpio_button_read());
}

#ifdef COORD_DBG
void coordinate_test(void (*func)(), int size)
{
    int i;
    Coordinate coord[size];
    size -= 1;

    coord[size].x = 0;
    coord[size].y = 0;
    coord[size].t = 0;
    for (i=0; i<size; i++) {
        while(!gpio_collision_read())
            ;

        coordinate_diff_clear();

        func();

        motors_stop();
        coord[i] = coordinate_diff();

        NVIC_DisableIRQ(TIMER2_IRQn);
        dbg_print_coordinate(coord[i]);
        NVIC_EnableIRQ(TIMER2_IRQn);

        coord[size].x += coord[i].x;
        coord[size].y += coord[i].y;
        coord[size].t += coord[i].t;
    }
    coord[size].x /= size;
    coord[size].y /= size;
    coord[size].t /= size;

    while(!gpio_button_read())
        ;

    for (i=0; i<=size; i++) {
        dbg_print_coordinate(coord[i]);
    }
}
#endif

#ifdef MOUSE_DEBUG
void mouse_test(void (*func)(), int size)
{
    int i;
    size += 1;
    Mouse_dbg dump[size];
    size -= 1;

    dump[size].raw_l = 0;
    dump[size].raw_t = 0;
    dump[size].cal_l = 0;
    dump[size].cal_t = 0;
    dump[size].count = 0;
    for (i=0; i<size; i++) {
        while(!gpio_collision_read())
            ;

        mouse_dbg_clear();

        func();
        motors_stop();
        dump[i] = mouse_dbg_dump();

        NVIC_DisableIRQ(TIMER2_IRQn);
        dbg_print_mouse(dump[i]);
        NVIC_EnableIRQ(TIMER2_IRQn);

        dump[size].raw_l += dump[i].raw_l;
        dump[size].raw_t += dump[i].raw_t;
        dump[size].cal_l += dump[i].cal_l;
        dump[size].cal_t += dump[i].cal_t;
        dump[size].count += dump[i].count;

    }
    dump[size].raw_l /= size;
    dump[size].raw_t /= size;
    dump[size].cal_l /= size;
    dump[size].cal_t /= size;
    dump[size].count /= size;

    while(1){
        while(!gpio_button_read())
            ;

        dbg_printf("Result:\r\n");

        for (i=0; i<size; i++) {
            dbg_printf("Entry: %d\t", i);
            dbg_print_mouse(dump[i]);
        }

        dbg_printf("Average:\t");
        dbg_print_mouse(dump[size]);
    }

}
#endif

