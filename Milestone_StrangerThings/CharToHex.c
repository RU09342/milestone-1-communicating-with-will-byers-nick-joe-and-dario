#include <stdio.h>


int rDuty = 0, gDuty = 0, bDuty = 0;

void main(void) {
	char PWM[6] = {'0', 'F', 'F', 'F', 'F', 'F'};
	char *p;
	p = &PWM[0];
	int ledSelect = 0, i = 0, power = 0;

	switch (ledSelect) {
		case	0	:
			for (i = 0; i < 2; i++) {
				rDuty += (*p - '0' * (16^power));
				if(rDuty > 9){
					rDuty -= 7;
				}
				p++;
				power++;
			}
			if (i == 1){
				ledSelect = 1;
				power = 0;
			}
		break;
		case	1	:
			for (i = 0; i < 4; i++) {
				gDuty += (*p - '0');
				if(gDuty > 9){
					gDuty -= 7;
				}
				p++;
			}
			if (i == 3){
				ledSelect = 2;
			}
		break;
		case	2	:
			for (i = 0; i < 6; i++) {
				bDuty += (*p - '0');
				if(bDuty > 9){
					bDuty -= 7;
				}
				p++;
			}
		break;
	}

printf("Red = %d, Green = %d, Blue = %d", rDuty, gDuty, bDuty);
}

