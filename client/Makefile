all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o tester.o
	gcc tester.o -Wall -lm -lev3dev-c -o tester
	gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o
	gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c i2c.c -o i2c.o
	gcc i2c.o -Wall -lm -lev3dev-c -o i2c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c testSonar.c -o testSonar.o
	gcc testSonar.o -Wall -lm -lev3dev-c -o testSonar

i2c:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c i2c.c -o i2c.o
	gcc i2c.o -Wall -lm -lev3dev-c -o i2c
	./i2c

sensor_gyro.o: sensor_gyro.c sensor_gyro.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_gyro.c -o sensor_gyro.o

sensor_gyro:
	gcc sensor_gyro.o -Wall -lm -lev3dev-c -o sensor_gyro

sensor_sonar.o: sensor_sonar.c sensor_sonar.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o

sensor_sonar: sensor_sonar.o
	gcc sensor_sonar.o -Wall -lm -lev3dev-c -o sensor_sonar
	./sensor_sonar

touch_sensor.o: touch_sensor.c touch_sensor.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c touch_sensor.c -o touch_sensor.o

touch_sensor: touch_sensor.o
	gcc touch_sensor.o -Wall -lm -lev3dev-c -o touch_sensor
	./touch_sensor

color_sensor.o: color_sensor.c color_sensor.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c color_sensor.c -o color_sensor.o

color_sensor: color_sensor.o
	gcc color_sensor.o -Wall -lm -lev3dev-c -o color_sensor
	./color_sensor

getSonarValue.o: getSonarValue.c getSonarValue.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c getSonarValue.c -o getSonarValue.o

getSonarValue: getSonarValue.o
	gcc getSonarValue.o -Wall -lm -lev3dev-c -o getSonarValue
	./getSonarValue

setMotorsValue.o: setMotorsValue.c setMotorsValue.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c setMotorsValue.c -o setMotorsValue.o

setMotorsValue:
	gcc setMotorsValue.o -Wall -lm -lev3dev-c -o setMotorsValue
	./setMotorsValue

robotclient.o: robotclient.c robotclient.h
	gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o

robotclient: robotclient.o
	gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	./robotclient

motors_wheels.o: motors_wheels.c motors_wheels.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o

motors_wheels: sensor_gyro.o motors_wheels.o
	gcc motors_wheels.o -Wall -lm -lev3dev-c -o motors_wheels
	./motors_wheels

motors_servo.o: motors_servo.c motors_servo.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o

motors_servo: motors_servo.o
	gcc motors_servo.o -Wall -lm -lev3dev-c -o motors_servo
	./motors_servo

servo_sonar.o: servo_sonar.c servo_sonar.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o

servo_sonar: motors_servo.o sensor_sonar.o
	gcc servo_sonar.o motors_servo.o sensor_sonar.o -Wall -lm -lev3dev-c -o servo_sonar
	./servo_sonar

test_1.o: test_1.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_1.c -o test_1.o

test_1: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o test_1.o
	gcc -pthread sensor_gyro.o motors_wheels.o servo_sonar.o motors_servo.o sensor_sonar.o test_1.o -Wall -lm -lev3dev-c -o test_1
	./test_1

test_1V2: test_1V2.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c robotclient.c -o robotclient.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_1.c -o test_1.o
	gcc -pthread sensor_gyro.o motors_wheels.o robotclient.o servo_sonar.o motors_servo.o sensor_sonar.o test_1.o -Wall -lm -lbluetooth -lev3dev-c -o test_1V2
	./test_1V2

mvt_forward.o: mvt_forward.c mvt_forward.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_forward.c -o mvt_forward.o

test_mvt_forward.o: test_mvt_forward.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_mvt_forward.c -o test_mvt_forward.o

test_mvt_forward: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_forward.o test_mvt_forward.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_mvt_forward.o -Wall -lm -lev3dev-c -o test_mvt_forward
	./test_mvt_forward
	
test_avancee_simple.o: test_avancee_simple.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_avancee_simple.c -o test_avancee_simple.o

test_avancee_simple: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_forward.o test_avancee_simple.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_avancee_simple.o -Wall -lm -lev3dev-c -o test_avancee_simple
	./test_avancee_simple

mvt_rotate.o: mvt_rotate.c mvt_rotate.h
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_rotate.c -o mvt_rotate.o

test_mvt_rotate.o: test_mvt_rotate.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_mvt_rotate.c -o test_mvt_rotate.o

test_mvt_rotate: sensor_gyro.o motors_wheels.o sensor_sonar.o motors_servo.o servo_sonar.o mvt_rotate.o mvt_forward.o test_mvt_rotate.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o servo_sonar.o motors_servo.o sensor_sonar.o mvt_forward.o test_mvt_rotate.o -Wall -lm -lev3dev-c -o test_mvt_rotate

test_scan_attrape.o: test_scan_attrape.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_scan_attrape.c -o test_scan_attrape.o

test_scan_attrape: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o color_sensor.o mvt_rotate.o mvt_forward.o object_interaction.o test_scan_attrape.o
	gcc -pthread  sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o color_sensor.o mvt_rotate.o mvt_forward.o object_interaction.o test_scan_attrape.o -Wall -lm -lev3dev-c -o test_scan_attrape

object_interaction.o: object_interaction.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c object_interaction.c -o object_interaction.o

test_final_1.o: test_final_1.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_1.c -o test_final_1.o

test_final_1: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o test_final_1.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_1.o -Wall -lm -lev3dev-c -o test_final_1
	./test_final_1

test_final_2.o: test_final_2.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_2.c -o test_final_2.o

test_final_2: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o test_final_2.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_2.o -Wall -lm -lev3dev-c -o test_final_2
	./test_final_2

test_final_5.o: test_final_5.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_5.c -o test_final_5.o
	
test_final_5: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o test_final_5.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_5.o -Wall -lm -lev3dev-c -o test_final_5
	./test_final_5
	
test_algo_simple.o: test_algo_simple.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_algo_simple.c -o test_algo_simple.o
	
test_algo_simple: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o test_algo_simple.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_algo_simple.o -Wall -lm -lev3dev-c -o test_algo_simple
	./test_algo_simple

test_final_6.o: test_final_6.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_6.c -o test_final_6.o

test_final_6: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o robotclient.o test_final_6.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o robotclient.o test_final_6.o -Wall -lm -lbluetooth -lev3dev-c -o test_final_6
	./test_final_6

test2_final1.o: test2_final1.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test2_final1.c -o test2_final1.o

test2_final1: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o robotclient.o test2_final1.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o robotclient.o test2_final1.o -Wall -lm -lbluetooth -lev3dev-c -o test2_final1
	./test2_final_1

test_servo.o: test_servo.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_servo.c -o test_servo.o

test_servo: motors_servo.o test_servo.o
	gcc motors_servo.o test_servo.o -Wall -lm -lev3dev-c -o test_servo
	./test_servo

test3.o: test3.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test3.c -o test3.o

test3: color_sensor.o sensor_gyro.o sensor_sonar.o servo_sonar.o motors_servo.o motors_wheels.o object_interaction.o mvt_forward.o mvt_rotate.o test3.o
	gcc color_sensor.o sensor_gyro.o sensor_sonar.o servo_sonar.o motors_servo.o motors_wheels.o object_interaction.o mvt_forward.o mvt_rotate.o test3.o -Wall -lm -lev3dev-c -o test3

test4.o: test4.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test4.c -o test4.o

test4: color_sensor.o sensor_gyro.o sensor_sonar.o servo_sonar.o motors_servo.o motors_wheels.o object_interaction.o mvt_forward.o mvt_rotate.o test4.o
	gcc color_sensor.o sensor_gyro.o sensor_sonar.o servo_sonar.o motors_servo.o motors_wheels.o object_interaction.o mvt_forward.o mvt_rotate.o test4.o -Wall -lm -lev3dev-c -o test4

run:
	./tester
