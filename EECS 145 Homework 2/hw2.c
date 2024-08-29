#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ROWS		100
#define COLUMNS		100

int main( int argc, char **argv )
{

	int			i;
	FILE			*fp;
	float			u, var;
	unsigned char	image[ROWS][COLUMNS];
  	char			*ifile, *ofile, ch;
	char			filename[4][50];
	int N = 100;

	strcpy(filename[0], "image1.raw");
	strcpy(filename[1], "image2.raw");
	strcpy(filename[2], "image3.raw");
	strcpy(filename[3], "image4.raw");

	//Assign each image name in filename to ifile here	
	/* example: ifile = filename[k]; k=0,1,2,3; a loop might be needed*/

	for(i = 0; i < 4; i++)
	{
		ifile = filename[i];

		if (( fp = fopen( ifile, "rb" )) == NULL )
		{
		fprintf( stderr, "error: couldn't open %s\n", ifile );
		exit( 1 );
		}			

		for (int u = 0; u < ROWS ; u++ )
		if ( fread( image[u], 1, COLUMNS, fp ) != COLUMNS )
		{
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		}

		fclose( fp );

		//Calculate Mean for each image here

		//Calculate Variance for each image here

		//Print mean and variance for each image

		double mean = 0;
		double variance = 0;
		double num = 0;

		//Iterate through all the pixels of the image
		for (int x = 0; x < ROWS; x++) 
		{
			for (int y = 0; y < COLUMNS; y++) 
			{
				num += image[x][y];
			}
		}

		mean = num / (ROWS*COLUMNS);

		double total = 0.0;

		for (int x = 0; x < ROWS; x++) 
		{
			for (int y = 0; y < COLUMNS; y++) 
			{
				total += (image[x][y] - mean) * (image[x][y] - mean);
			}
		}

		double num3 = (ROWS * COLUMNS) - 1;
		variance = total / num3;
		
		printf("%s: %f %f\n", ifile, mean, variance);
	}


	printf("Press any key to exit: ");
	

	return 0;
}


