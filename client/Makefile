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
	
sensor_sonar:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc sensor_sonar.o -Wall -lm -lev3dev-c -o sensor_sonar
	./sensor_sonar

touch_sensor:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c touch_sensor.c -o touch_sensor.o
	gcc touch_sensor.o -Wall -lm -lev3dev-c -o touch_sensor
	./touch_sensor

color_sensor:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c color_sensor.c -o color_sensor.o
	gcc color_sensor.o -Wall -lm -lev3dev-c -o color_sensor
	./color_sensor

getSonarValue:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c getSonarValue.c -o getSonarValue.o
	gcc getSonarValue.o -Wall -lm -lev3dev-c -o getSonarValue
	./getSonarValue

setMotorsValue:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c setMotorsValue.c -o setMotorsValue.o
	gcc setMotorsValue.o -Wall -lm -lev3dev-c -o setMotorsValue
	./setMotorsValue

client:
	gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o
	gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	./robotclient

motors_wheels:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc motors_wheels.o -Wall -lm -lev3dev-c -o motors_wheels
	./motors_wheels

motors_servo:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc motors_servo.o -Wall -lm -lev3dev-c -o motors_servo
	./motors_servo
	
servo_sonar:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc servo_sonar.o motors_servo.o sensor_sonar.o -Wall -lm -lev3dev-c -o servo_sonar
	./servo_sonar

test_1:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_1.c -o test_1.o
	gcc -pthread motors_wheels.o servo_sonar.o motors_servo.o sensor_sonar.o test_1.o -Wall -lm -lev3dev-c -o test_1
	./test_1

test_1V2:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c robotclient.c -o robotclient.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_1.c -o test_1.o
	gcc -pthread motors_wheels.o robotclient.o servo_sonar.o motors_servo.o sensor_sonar.o test_1.o -Wall -lm -lbluetooth -lev3dev-c -o test_1V2
	./test_1V2
	
test_mvt_forward:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_forward.c -o mvt_forward.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_mvt_forward.c -o test_mvt_forward.o
	gcc -pthread motors_wheels.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_mvt_forward.o -Wall -lm -lev3dev-c -o test_mvt_forward
	./test_mvt_forward
	
test_mvt_rotate:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_rotate.c -o mvt_rotate.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_mvt_rotate.c -o test_mvt_rotate.o
	gcc -pthread motors_wheels.o mvt_rotate.o servo_sonar.o motors_servo.o sensor_sonar.o test_mvt_rotate.o -Wall -lm -lev3dev-c -o test_mvt_rotate
	./test_mvt_rotate
	
test_final_1:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_rotate.c -o mvt_rotate.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_forward.c -o mvt_forward.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_1.c -o test_final_1.o
	gcc -pthread motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_1.o -Wall -lm -lev3dev-c -o test_final_1
	./test_final_1

test_final_2:	
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_wheels.c -o motors_wheels.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor_sonar.c -o sensor_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c servo_sonar.c -o servo_sonar.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_rotate.c -o mvt_rotate.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt_forward.c -o mvt_forward.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_final_2.c -o test_final_2.o
	gcc -pthread motors_wheels.o mvt_rotate.o mvt_forward.o servo_sonar.o motors_servo.o sensor_sonar.o test_final_2.o -Wall -lm -lev3dev-c -o test_final_2
	./test_final_2
	
test_servo:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors_servo.c -o motors_servo.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_servo.c -o test_servo.o
	gcc motors_servo.o test_servo.o -Wall -lm -lev3dev-c -o test_servo
	./test_servo

run:
	./tester
