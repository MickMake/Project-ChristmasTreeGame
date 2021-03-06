/*
** Author: Mick Hellstrom
** Copyright: Meh, buy me a beer if you see me, (has to be Gluten Free).
** 
** This is the source code to my YouTube tutorial video on how to make an Internet connected Christmas tree game.
** It'll sit waiting for a "command" coming from the UART and respond.
** Numbers 0-9 will "throw" a snowball and "c" will clear the display.
** 
** The Teensy 3.0 can support up to 8 LED strips, (I think up to 1000 per Teensy).
** So you could expand it considerably. Just be careful about powering the rig! LEDs can consume a lot of juice, and you don't want to feedback power into your PC.
** 
** I used socat to provide a TCP port that the website could interact with.
** socat -gx TCP-LISTEN:1234,fork,max-children=10,reuseaddr OPEN:/dev/ttyACM0,b115200,flusho=1
** 
** MickMake YouTube channel:	https://www.youtube.com/c/MickMakes
** Video:			https://www.youtube.com/watch?v=tHWXnCrZYtg
** Website article:		https://mickmake.com/archives/1550
** Game interface:		http://santa.mickmake.com/
*/


#include <OctoWS2811.h>

const int ledsPerStrip = 60;
const int ledStrips = 5;

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);


#define SANTA	0xFF0000
#define ELF	0x0000FF
#define OFF	0x000000
#define BALL	0xFFFFFF
#define BOOM	0x100400


/*********************************************************************************
** Elf related stuff.
*/
int MaxElfThrow = 25;						// Maximum throw distance of each elf.
signed int ElfFire[] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// Which elf has thrown a snowball.
signed int ElfHomeX[] = { 0,29, 0,29, 0,29, 0,29, 0,29};	// The X home position of each elf.
signed int ElfHomeY[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4};	// The Y home position of each elf.
signed int ElfBallX[10];					// The current X position of a snowball.
signed int ElfBallY[10];					// The current Y position of a snowball.
signed int ElfBallLast[10][3] = {				// Keeps track of the LED colour underneath a snowball.
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF},
			{1,1,OFF}
			};
unsigned int BoomDelay = 0;					// Countdown to delay the next Santa appearing, (will be set to a millis() value when Santa is hit).


/*********************************************************************************
** Santa related stuff.
*/
signed int SantaXPos = 8;					// The default X start position of Santa.
signed int SantaYPos = 1;					// The default Y start position of Santa.
signed int HowClose = 1;					// How close Santa can get to an elf.
signed int SantaLast[] = {8, 1, OFF};				// Keeps track of the LED colour underneath Santa.


/*********************************************************************************
** High level functions.
*/
void setup()
{
	Serial.begin(115200);

	pinMode(1, OUTPUT);
	digitalWrite(1, HIGH);

	digitalWrite(1, LOW);
	leds.begin();

	Clear();
}


void loop()
{
	while(leds.busy()){}	// Just in case.

	UpdateSanta();
	UpdateElfBall(0);
	UpdateElfBall(1);
	UpdateElfBall(2);
	UpdateElfBall(3);
	UpdateElfBall(4);
	UpdateElfBall(5);
	UpdateElfBall(6);
	UpdateElfBall(7);
	UpdateElfBall(8);
	UpdateElfBall(9);

	delay(100);

	leds.show();

	/* Check for serial input.
	** If we get a number from 0-9 an elf will throw a snowball.
	** If we get a 'c' then clear the screen, otherwise we have too many dead Santas.
	*/
	if (Serial.available() > 0)
	{
		char foo = Serial.read();
		// Serial.println(foo);
		switch(foo)
		{
			case 'c':
				Clear();
				break;

			case '0':
				FireElfBall(0);
				break;
			case '1':
				FireElfBall(1);
				break;
			case '2':
				FireElfBall(2);
				break;
			case '3':
				FireElfBall(3);
				break;
			case '4':
				FireElfBall(4);
				break;
			case '5':
				FireElfBall(5);
				break;
			case '6':
				FireElfBall(6);
				break;
			case '7':
				FireElfBall(7);
				break;
			case '8':
				FireElfBall(8);
				break;
			case '9':
				FireElfBall(9);
				break;
		}
	}
}


/*********************************************************************************
** Convert X and Y to OctoWS2812 array.
** X -->
** Y ^
*/
int Conv(int x, int y)
{
	int z = (y * ledsPerStrip) + x;

	return(z);
}


/*********************************************************************************
** Update Santa's next position. Statistically Santa will move more in the X plane, (up/down).
** 2/10 - up
** 2/10 - down
** 1/10 - down
** 1/10 - down
** 4/10 - stay still
*/
void UpdateSanta()
{
	if (BoomDelay)
	{
		if (millis() - BoomDelay < 5000)
		{
			Serial.println("Santa Dead!");
			return;
		}
		else
		{
			BoomDelay = 0;
			// leds.setPixel(Conv(SantaXPos, SantaYPos), OFF);
			SantaXPos = random() % (ledsPerStrip - 1);
			SantaYPos = random() % (ledStrips - 1);
		}
	}

	switch(random(10))
	{
		case 1:		// Santa moves up.
		case 2:		// Santa moves up.
			if (SantaXPos > HowClose)
				SantaXPos--;
			else
				SantaXPos = ledsPerStrip - HowClose;
			break;

		case 3:		// Santa moves down.
		case 4:		// Santa moves down.
			if (SantaXPos < ledsPerStrip - HowClose)
				SantaXPos++;
			else
				SantaXPos = HowClose;
			break;

		case 5:		// Santa moves left.
			if (SantaYPos > 0)
				SantaYPos--;
			else
				SantaYPos = ledStrips - 1;
			break;

		case 6:		// Santa moves right.
			if (SantaYPos < ledStrips - 1)
				SantaYPos++;
			else
				SantaYPos = 0;
			break;
		// Drop through is to do nothing.
	}

	// Reset last position.
	if (SantaLast[2] == BALL)
	{
		// Special case - balls disappear.
		leds.setPixel(Conv(SantaLast[0], SantaLast[1]), OFF);
	}
	else
	{
		leds.setPixel(Conv(SantaLast[0], SantaLast[1]), SantaLast[2]);
	}

	// Move Santa.
	SantaLast[0] = SantaXPos;
	SantaLast[1] = SantaYPos;
	SantaLast[2] = leds.getPixel(Conv(SantaXPos, SantaYPos));
	leds.setPixel(Conv(SantaXPos, SantaYPos), SANTA);
}


/*********************************************************************************
** Move an elf snowball. It'll keep track of the LED colour underneath the current position so it doesn't wipe the screen.
** It won't travel further than the maximum throw distance and only one thrown at a time by an elf.
** When it hits Santa, he'll splat, but don't worry. He comes back to life again.
*/
void UpdateElfBall(int ElfNumber)
{
	signed int LastXPos = ElfBallX[ElfNumber];
	signed int LastYPos = ElfBallY[ElfNumber];

	if (!ElfFire[ElfNumber])
	{
		return;
	}

	// Reset last position.
	leds.setPixel(Conv(LastXPos, LastYPos), OFF);

	ElfBallX[ElfNumber]++;
	// leds.setPixel(Conv(LastXPos, LastYPos), OFF);
	leds.setPixel(Conv(ElfBallLast[ElfNumber][0], ElfBallLast[ElfNumber][1]), ElfBallLast[ElfNumber][2]);
	ElfBallLast[ElfNumber][0] = ElfBallX[ElfNumber];
	ElfBallLast[ElfNumber][1] = ElfBallY[ElfNumber];
	ElfBallLast[ElfNumber][2] = leds.getPixel(Conv(ElfBallX[ElfNumber], ElfBallY[ElfNumber]));
	if ((ElfBallX[ElfNumber] == SantaXPos) && (ElfBallY[ElfNumber] == SantaYPos))
	{
		// Explode.
		leds.setPixel(Conv(ElfBallX[ElfNumber], ElfBallY[ElfNumber]), BOOM);
		BoomDelay = millis();
		ElfFire[ElfNumber] = 0;
		Serial.println("A hit!");
		ElfBallLast[ElfNumber][2] = BOOM;
		SantaLast[2] = BOOM;
	}
	else if (abs(ElfHomeX[ElfNumber] - ElfBallX[ElfNumber]) > MaxElfThrow)
	{
		// Max snowball throw.
		// leds.setPixel(Conv(ElfBallX[ElfNumber], ElfBallY[ElfNumber]), OFF);
		leds.setPixel(Conv(ElfBallLast[ElfNumber][0], ElfBallLast[ElfNumber][1]), ElfBallLast[ElfNumber][2]);
		ElfFire[ElfNumber] = 0;
	}
	else
	{
		// Move snowball.
		leds.setPixel(Conv(ElfBallX[ElfNumber], ElfBallY[ElfNumber]), BALL);
	}
}


/*********************************************************************************
** An elf has thrown a snowball.
*/
void FireElfBall(int ElfNumber)
{
	Serial.print("ELf: ");
	Serial.println(ElfNumber);
	if (ElfFire[ElfNumber])
	{
		return;
	}

	ElfFire[ElfNumber] = 1;
	ElfBallX[ElfNumber] = ElfHomeX[ElfNumber] + 1;
	ElfBallY[ElfNumber] = ElfHomeY[ElfNumber];
}


/*********************************************************************************
** Clear the "screen" and reset everything to defaults.
** Good thing to have otherwise we have too many dead Santas.
*/
void Clear(void)
{
	int x, y;

	while(leds.busy()){}	// Just in case.

	for (x=0; x < ledsPerStrip; x++)
	{
		for (y=0; y < 8; y++)
		{
			leds.setPixel(x + y*ledsPerStrip, OFF);
		}
	}

	leds.setPixel(Conv(ElfHomeX[0], ElfHomeY[0]), ELF);
	leds.setPixel(Conv(ElfHomeX[1], ElfHomeY[1]), ELF);
	leds.setPixel(Conv(ElfHomeX[2], ElfHomeY[2]), ELF);
	leds.setPixel(Conv(ElfHomeX[3], ElfHomeY[3]), ELF);
	leds.setPixel(Conv(ElfHomeX[4], ElfHomeY[4]), ELF);
	leds.setPixel(Conv(ElfHomeX[5], ElfHomeY[5]), ELF);
	leds.setPixel(Conv(ElfHomeX[6], ElfHomeY[6]), ELF);
	leds.setPixel(Conv(ElfHomeX[7], ElfHomeY[7]), ELF);
	leds.setPixel(Conv(ElfHomeX[8], ElfHomeY[8]), ELF);
	leds.setPixel(Conv(ElfHomeX[9], ElfHomeY[9]), ELF);

	ElfBallLast[0][2] = OFF;
	ElfBallLast[1][2] = OFF;
	ElfBallLast[2][2] = OFF;
	ElfBallLast[3][2] = OFF;
	ElfBallLast[4][2] = OFF;
	ElfBallLast[5][2] = OFF;
	ElfBallLast[6][2] = OFF;
	ElfBallLast[7][2] = OFF;
	ElfBallLast[8][2] = OFF;
	ElfBallLast[9][2] = OFF;
	SantaLast[2] = OFF;

	leds.show();
}


