#ifndef _Main_
#define _Main_

#define pdbg 	printf														   /* Print for debug */
#define pdbga(n) printf("Addr: %p\n",n)									 /* Print address - Debug */
#define null	NULL													   /* Lowercase is better */

#define ScH 480																		/* Map Height */
#define ScW 640																		 /* Map Width */

#define Pi 3.1415926535

void Dessiner(double aX, double aY, double aZ);

#define Alpha 0.05
#define Time 0.15
#define hBuffLen 64
#define usbTimeout 200 // ms

#endif
