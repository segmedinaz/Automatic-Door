#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "servo.h"

//funkcije za tastere
void initExternalPushButtons (void);
uint8_t readOpenDoorPushButton(void);
uint8_t readCloseDoorPushButton(void);
uint8_t readAutomaticModePushButton(void);
uint8_t readManualModePushButton(void);

//funkcije za led diode
void initLEDs(void);
void turnOnRedLED(void);
void turnOnGreenLED(void);
void turnOffLED(void);

// funkcije za senzore pokreta
void initMotionSensors(void);
uint8_t readExitMotionSensor(void);
uint8_t readEntranceMotionSensor(void);

//funkcije za kontrolu vrata
uint8_t openDoor(void);
uint8_t closeDoor(void);

int main(void)
{
	uint8_t stateManualModeButton = 0;
	uint8_t stateAutomaticModeButton = 0;
	uint8_t stateOpenDoorButton = 0;
	uint8_t stateCloseDoorButton = 0;
	
	uint8_t detectedEntranceMotion = 0;
	uint8_t detectedExitMotion = 0;
	
	uint8_t workingMode = 1; //0 -  automatski, 1 - manuelni
	uint8_t doorStatus = 0; //0 - closed, 1 - open
	
	
	uint32_t prescalar = (uint16_t) ( ( (SystemCoreClock / 2) / 1000000) - 1);	
	

	pwm(20000,prescalar);//GPIOB -> servo motor -> PB6 
	initUSART2(USART2_BAUDRATE_115200); //GPIOA ->  PA2 -> TX & PA3 -> RX
	initExternalPushButtons(); 			//GPIOE -> PE0, PE1, PE2 and PE3
	initLEDs(); 						//GPIOC -> PC0 and PC1
	initMotionSensors(); 				//GPIOD -> PD0 and PD1
	
	doorStatus = closeDoor();
	printUSART2("Pocetak: manuelni mod. Status: %d\n", workingMode);

	
	while(1)
	{ 
		
		
		
		stateManualModeButton = readManualModePushButton();
		stateAutomaticModeButton = readAutomaticModePushButton();		
		
		//odlucujemo nacin rad na osnovu tastera
		if (stateAutomaticModeButton == 1) 
		{
			if (workingMode == 1) 
			{
				//ako je pritisnut taster za automatski mod, prebacujemo se na automatski mod
				workingMode = 0;
				printUSART2("Automatski mod. Status: %d\n", workingMode);
			}
			
		}
		else if (stateManualModeButton == 1) 
		{
			if (workingMode == 0) 
			{
				//ako je pritisnut taster za manuelni mod, prebacujemo se na manuelni mod
				workingMode = 1;
				printUSART2("Manuelni mod. Status: %d\n", workingMode);
			}
		}
		
		//ako je odabran automatski mod
		if (workingMode == 0) 
		{
		
			detectedEntranceMotion = readEntranceMotionSensor();
			detectedExitMotion = readExitMotionSensor();
			
			if ((detectedEntranceMotion == 1) || (detectedExitMotion == 1)) 
			{
				if (doorStatus == 0) 
				{
					doorStatus = openDoor();
					printUSART2("A: Otvaramo vrata. Status: %d\n", doorStatus);
				}
			}
			else 
			{
				if (doorStatus == 1) 
				{
					doorStatus = closeDoor();
					printUSART2("A: Zatvaramo vrata. Status: %d\n", doorStatus);
				}
			}
		}
		
		//ili ako je odabran manuelni mod
		else if (workingMode == 1)
		{
			
			stateOpenDoorButton = readOpenDoorPushButton();
			stateCloseDoorButton = readCloseDoorPushButton();
			
			if (stateOpenDoorButton == 1) 
			{
				if (doorStatus == 0) 
				{
					doorStatus = openDoor();
					printUSART2("M: Otvaramo vrata. Status: %d\n", doorStatus);
				}
			}
			
			else if (stateCloseDoorButton == 1) 
			{
				if (doorStatus == 1) 
				{
					doorStatus = closeDoor();
					printUSART2("M: Zatvaramo vrata. Status: %d\n", doorStatus);
				}
			}
			
		}
		
	}
}


void initLEDs(void)
{
	//PC0 - crvena LED dioda
	//PC1 - zelena LED dioda
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 	
    GPIOC->MODER |= 0x00000005;				
    GPIOC->OTYPER |= 0x00000000;		
    GPIOC->OSPEEDR |= 0x0000000F; 			
	GPIOC->ODR &= 0xFFFC;	
}


void turnOnRedLED(void)
{
	GPIOC->ODR &= 0xFFFC;
	GPIOC->ODR |= 0x0001;
}

void turnOnGreenLED(void)
{
	GPIOC->ODR &= 0xFFFC;
	GPIOC->ODR |= 0x0002;
}

void turnOffLED(void)
{
	GPIOC->ODR &= 0xFFFC;
}


void initExternalPushButtons (void)
{
	//PE0 -> automatic mode taster
	//PE1 -> manual mode taster
	//PE2 -> open door taster
	//PE3 -> close door taster
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; 
	GPIOE->MODER &= ~(0x000000FF);       
	GPIOE->PUPDR |= 0x00000055;			 
}

uint8_t readManualModePushButton(void)
{
	uint8_t new_state = (GPIOE->IDR & 0x0002);
	
	if (new_state == 0x0002) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t readAutomaticModePushButton(void)
{
	uint8_t new_state = (GPIOE->IDR & 0x0001);
	
	if (new_state == 0x0001) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t readOpenDoorPushButton(void)
{
	uint8_t new_state = (GPIOE->IDR & 0x0004);
	
	if (new_state == 0x0004) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t readCloseDoorPushButton(void)
{
	uint8_t new_state = (GPIOE->IDR & 0x0008);
	
	if (new_state == 0x0008) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void initMotionSensors (void)
{
	// PD0 -> entrance motion sensor
	// PD2 -> exit motion sensor
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;	
	GPIOD->MODER &= ~(0x00000033);  	
	GPIOD->PUPDR |= 0x00000000;		
}

uint8_t readEntranceMotionSensor(void)
{
	uint8_t new_state = (GPIOD->IDR & 0x0001);
	
	if (new_state != 0x0001) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t readExitMotionSensor(void)
{
	uint8_t new_state = (GPIOD->IDR & 0x0004);
	
	if (new_state != 0x0004) 
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t openDoor(void)
{
	turnOffLED();
	turnOnGreenLED();
	SetServoPosition(180);
	return 1;
}

uint8_t closeDoor(void)
{
	turnOffLED();
	turnOnRedLED();
	SetServoPosition(0);
	return 0;
}
