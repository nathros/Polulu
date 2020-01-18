main.o: main.c motor.h ir.h config.h uart.h debug.h coordinate.h mouse.h \
 line_search.h gpio.h pid.h rit.h advanced_movement.h odometry.h
debug.o: debug.c common.h lcd.h debug.h coordinate.h mouse.h
uart.o: uart.c uart.h
ir.o: ir.c ir.h config.h
lcd.o: lcd.c lcd.h uart.h
gpio.o: gpio.c config.h led.h odometry.h debug.h coordinate.h mouse.h \
 gpio.h
led.o: led.c led.h
motor.o: motor.c config.h debug.h coordinate.h mouse.h uart.h delay.h \
 motor.h
delay.o: delay.c led.h debug.h coordinate.h mouse.h delay.h
line_sensors.o: line_sensors.c uart.h line_sensors.h
utilities.o: utilities.c uart.h debug.h coordinate.h mouse.h utilities.h
wall_follow.o: wall_follow.c wall_follow.h ir.h config.h motor.h delay.h \
 gpio.h
mouse.o: mouse.c KeyboardHost.h config.h common.h lcd.h led.h debug.h \
 coordinate.h mouse.h
coordinate.o: coordinate.c debug.h coordinate.h mouse.h lcd.h motor.h \
 common.h advanced_movement.h odometry.h
advanced_movement.o: advanced_movement.c common.h debug.h coordinate.h \
 mouse.h motor.h pid.h advanced_movement.h odometry.h
rit.o: rit.c rit.h
pid.o: pid.c line_sensors.h motor.h pid.h debug.h coordinate.h mouse.h \
 gpio.h line_follow.h advanced_movement.h odometry.h
common.o: common.c common.h
odometry.o: odometry.c advanced_movement.h coordinate.h odometry.h \
 common.h debug.h mouse.h led.h motor.h
