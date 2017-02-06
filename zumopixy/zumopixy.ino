#include <QTRSensors.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <SPI.h>
#include <Pixy.h>

/* #define DEBUG */

enum zumo_settings {
	MAX_SPEED = 400,
	MIN_SPEED = 70,
	NORMAL_SPEED = 100,
};

enum pixy_settings {
	PIXY_CENTER_X = ((PIXY_MAX_X - PIXY_MIN_X) / 2),
	PIXY_CENTER_Y = ((PIXY_MAX_Y - PIXY_MIN_Y) / 2),
};

enum object_settings {
	MAX_SIZE_DIFF = 6000,
	NO_BLOCKS_CNT = 1000,
};

enum pins {
	led_pin = 13,
};

#define led_on()	(digitalWrite(led_pin, HIGH))
#define led_off()	(digitalWrite(led_pin, LOW))

#define drive_stop()	(motors.setSpeeds(0, 0))

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
Pixy pixy;

static uint32_t no_blocks = 0;
uint32_t start_size;
uint32_t size;

void followBlock(int trackedBlock) {
	/* Average the last 2 samples */
	size = (size + (pixy.blocks[trackedBlock].width * pixy.blocks[trackedBlock].height)) / 2;

	int32_t size_diff = start_size - size;
	size_diff = constrain(size_diff, -MAX_SIZE_DIFF, MAX_SIZE_DIFF);

	int forward_speed = map(size_diff, -MAX_SIZE_DIFF, MAX_SIZE_DIFF, -MAX_SPEED, MAX_SPEED);

	uint16_t x = pixy.blocks[trackedBlock].x;
	int32_t x_diff = ((int32_t) PIXY_CENTER_X) - x;
	int left_speed = map(x_diff, (PIXY_CENTER_X - PIXY_MAX_X), (PIXY_CENTER_X - PIXY_MIN_X), MAX_SPEED, -MAX_SPEED);
	int right_speed = map(x_diff, (PIXY_CENTER_X - PIXY_MAX_X), (PIXY_CENTER_X - PIXY_MIN_X), -MAX_SPEED, MAX_SPEED);

	left_speed = constrain(left_speed + forward_speed, -MAX_SPEED, MAX_SPEED);
	right_speed = constrain(right_speed + forward_speed, -MAX_SPEED, MAX_SPEED);
	
	motors.setLeftSpeed(left_speed);
	motors.setRightSpeed(right_speed);

#ifdef DEBUG
	Serial.print("x: ");
	Serial.print(x);
	Serial.print("    x_diff: ");
	Serial.print(x_diff);
	Serial.print("    left_speed: ");
	Serial.print(left_speed);
	Serial.print("    right_speed: ");
	Serial.print(right_speed);
	Serial.println();
#endif
}

void setup() {

#ifdef DEBUG
	Serial.begin(9600);
	while (!Serial);
	Serial.print("Starting...\n");
#endif

	pinMode(led_pin, OUTPUT);

	drive_stop();
	pixy.init();

	led_off();
	buzzer.play(">g32>>c32");

	/* On button press, get the starting size of the object */
	button.waitForButton();
	buzzer.play(">g32>>c32");
	while (pixy.getBlocks() == 0);
	start_size = pixy.blocks[0].width * pixy.blocks[0].height;
	size = start_size;

#ifdef DEBUG
	Serial.print("size: ");
	Serial.print(start_size);
	Serial.println();
#endif

	delay(1000);
}

void loop() {
	char buf[32];

	uint16_t nblocks = pixy.getBlocks();
	if (nblocks > 0) {
		followBlock(0);
	} else {
		no_blocks++;
		/* Blocks have to be gone for many checks in a row */
		if (no_blocks > NO_BLOCKS_CNT) {
			led_off();
			drive_stop();
			no_blocks = 0;
		}
	}
}
