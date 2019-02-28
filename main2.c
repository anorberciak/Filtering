#include "stdio.h"
#include "usbstk5505.h"
#include "aic3204.h"
#include "PLL.h"
#include "FIR_Filters_asm.h"
#include "LEDFlasher.h"
#include "hamming.h"
#include "hanning.h"
#include "kaiser.h"
#include "rectangular.h"
#include "stereo.h"
#include "dsplib.h"



Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;
Int16 mono_input;

DATA left[303];
DATA input[303];
int right[303];

#define SAMPLES_PER_SECOND 48000
#define GAIN_IN_dB         10
#define N 303

int bufor[N];
int tab[N];

const int wsp[303] = {        0,      0,      0,      0,      0,      0,      0,      0,      0,
	        0,      1,      1,      0,      0,     -1,     -1,     -1,      0,
	        1,      2,      2,      1,      0,     -2,     -3,     -3,     -1,
	        2,      4,      4,      3,      0,     -3,     -6,     -6,     -3,
	        2,      6,      8,      6,      1,     -5,    -10,    -10,     -6,
	        1,      9,     14,     12,      5,     -6,    -15,    -18,    -13,
	       -1,     12,     21,     21,     11,     -5,    -20,    -28,    -22,
	       -6,     14,     30,     33,     21,     -2,    -26,    -41,    -37,
	      -16,     14,     41,     51,     38,      6,    -31,    -57,    -58,
	      -32,     11,     52,     73,     61,     20,    -33,    -76,    -86,
	      -58,      0,     62,    100,     95,     45,    -30,    -96,   -123,
	      -95,    -21,     69,    134,    141,     84,    -17,   -117,   -172,
	     -150,    -57,     69,    174,    206,    145,     12,   -138,   -236,
	     -232,   -120,     57,    224,    300,    244,     68,   -156,   -328,
	     -364,   -233,     22,    293,    457,    423,    186,   -170,   -491,
	     -622,   -474,    -75,    425,    808,    868,    513,   -179,   -959,
	    -1476,  -1398,   -538,   1058,   3093,   5098,   6563,   7100,   6563,
	     5098,   3093,   1058,   -538,  -1398,  -1476,   -959,   -179,    513,
	      868,    808,    425,    -75,   -474,   -622,   -491,   -170,    186,
	      423,    457,    293,     22,   -233,   -364,   -328,   -156,     68,
	      244,    300,    224,     57,   -120,   -232,   -236,   -138,     12,
	      145,    206,    174,     69,    -57,   -150,   -172,   -117,    -17,
	       84,    141,    134,     69,    -21,    -95,   -123,    -96,    -30,
	       45,     95,    100,     62,      0,    -58,    -86,    -76,    -33,
	       20,     61,     73,     52,     11,    -32,    -58,    -57,    -31,
	        6,     38,     51,     41,     14,    -16,    -37,    -41,    -26,
	       -2,     21,     33,     30,     14,     -6,    -22,    -28,    -20,
	       -5,     11,     21,     21,     12,     -1,    -13,    -18,    -15,
	       -6,      5,     12,     14,      9,      1,     -6,    -10,    -10,
	       -5,      1,      6,      8,      6,      2,     -3,     -6,     -6,
	       -3,      0,      3,      4,      4,      2,     -1,     -3,     -3,
	       -2,      0,      1,      2,      2,      1,      0,     -1,     -1,
	       -1,      0,      0,      1,      1,      0,      0,      0,      0,
	        0,      0,      0,      0,      0,      0
	};



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( void )
{
    /* Initialize BSL */
    USBSTK5505_init( );

    /* Initialize the Phase Locked Loop in EEPROM */
    pll_frequency_setup(100);

	/* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();

    /* Initialise the AIC3204 codec */
	aic3204_init();


	/* Set sampling frequency in Hz and ADC gain in dB */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN_IN_dB);

   asm(" bclr XF");


   	   int idx = 0;
       int pos=idx;
       long result=0;
       int i, j;

       for(idx=0; idx<N; idx++)
       {
          bufor[idx]=0;
       }

       for(j=0;j<SAMPLES_PER_SECOND * 600L;j++)
       {

    	   aic3204_codec_read(&left_input, &right_input); // Configured for one interrupt per two channels.

    	   mono_input = stereo_to_mono(left_input, right_input);


    	   bufor[idx]=left_input;
    	   pos=idx;
    	   result=0;

    	   for (i=0; i<N; i++)
    	   {

    		   result += (long)bufor[poz] * wsp[i];

    		   poz += 1;
    		   if(poz == N)
               	   {
                   	   poz-=N;
               	   }

    	   }

    	   int output = (int)((result+16384)>>15);

    	   aic3204_codec_write(left_input,output);

    	   idx +=1;
    	   if(idx>=N)
    	   {
    		   idx -=N;
    	   }



       }

   /* Disable I2S and put codec into reset */
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
    SW_BREAKPOINT;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of main.c                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */


