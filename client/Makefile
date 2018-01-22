all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o tester.o
	gcc tester.o -Wall -lm -lev3dev-c -o tester
	gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o
	gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c i2c.c -o i2c.o
	gcc i2c.o -Wall -lm -lev3dev-c -o i2c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c testSonar.c -o testSonar.o
	gcc testSonar.o -Wall -lm -lev3dev-c -o testSonar

color_sensor.o: color_sensor.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c color_sensor.c -o color_sensor.o

color_sensor: color_sensor.o
	gcc color_sensor.o -Wall -lm -lev3dev-c -o color_sensor

map_construction.o: map_construction.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c map_construction.c -o map_construction.o

map_construction: robotclient.o map_construction.o
	gcc map_construction.o robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o map_construction

motors_servo.o: motors_servo.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o

motors_servo: motors_servo.o
	gcc motors_servo.o -Wall -lm -lev3dev-c -o motors_servo

sensor_compass.o: sensor_compass.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_compass.c -o sensor_compass.o

sensor_compass: sensor_compass.o
	gcc sensor_compass.o -Wall -lm -lev3dev-c -o sensor_compass

sensor_gyro.o: sensor_gyro.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_gyro.c -o sensor_gyro.o

sensor_gyro: sensor_gyro.o
	gcc sensor_gyro.o -Wall -lm -lev3dev-c -o sensor_gyro

sensor_sonar.o: sensor_sonar.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o

sensor_sonar: sensor_sonar.o
	gcc sensor_sonar.o -Wall -lm -lev3dev-c -o sensor_sonar
	./sensor_sonar

touch_sensor.o: touch_sensor.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c touch_sensor.c -o touch_sensor.o

touch_sensor: touch_sensor.o
	gcc touch_sensor.o -Wall -lm -lev3dev-c -o touch_sensor
	./touch_sensor

robotclient.o: robotclient.c
	gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o

robotclient: robotclient.o
	gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	./robotclient

motors_wheels.o: motors_wheels.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o

motors_wheels: sensor_gyro.o motors_wheels.o
	gcc motors_wheels.o -Wall -lm -lev3dev-c -o motors_wheels
	./motors_wheels

servo_sonar.o: servo_sonar.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o

servo_sonar: motors_servo.o sensor_sonar.o
	gcc servo_sonar.o motors_servo.o sensor_sonar.o -Wall -lm -lev3dev-c -o servo_sonar
	./servo_sonar

mvt_forward.o: mvt_forward.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_forward.c -o mvt_forward.o

mvt_rotate.o: mvt_rotate.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_rotate.c -o mvt_rotate.o

object_interaction.o: object_interaction.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c object_interaction.c -o object_interaction.o

test_final_algo_recursif.o: test_final_algo_recursif.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_algo_recursif.c -o test_final_algo_recursif.o

test_final_algo_recursif: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o robotclient.o map_construction.o object_interaction.o color_sensor.o test_final_algo_recursif.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_algo_recursif.o robotclient.o map_construction.o object_interaction.o color_sensor.o -Wall -lm -lbluetooth -lev3dev-c -o test_final_algo_recursif
	
test_final_algo_chenille.o: test_final_algo_chenille.c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_algo_chenille.c -o test_final_algo_chenille.o

test_final_algo_chenille: sensor_gyro.o motors_wheels.o motors_servo.o sensor_sonar.o servo_sonar.o mvt_rotate.o mvt_forward.o robotclient.o map_construction.o object_interaction.o color_sensor.o test_final_algo_chenille.o
	gcc -pthread sensor_gyro.o motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_algo_chenille.o robotclient.o map_construction.o object_interaction.o color_sensor.o -Wall -lm -lbluetooth -lev3dev-c -o test_final_algo_chenille