#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS		512
#define COLUMNS		512

#define sqr(x)		((x)*(x))

void header( int row, int col, unsigned char head[32] );
void clear( unsigned char image[][COLUMNS] );

int main( int argc, char **argv )
{

	int				i, j, k;
	int				A, threshold[3] = {137, 163, 47};
	//int 			A, threshold[3] = {100, 100, 100}; ^^ it was this initially
	FILE			*fp;
	char			*ifile, *ofile;
	unsigned char	image[ROWS][COLUMNS], bimage[ROWS][COLUMNS], head[32];
	char			filename[9][50], ch;
	float			xbar, ybar;

	strcpy(filename[0], "image1.raw");
	strcpy(filename[1], "image1-b.ras");
	strcpy(filename[2], "image1.ras");
	strcpy(filename[3], "image2.raw");
	strcpy(filename[4], "image2-b.ras");
	strcpy(filename[5], "image2.ras");
	strcpy(filename[6], "image3.raw");
	strcpy(filename[7], "image3-b.ras");
	strcpy(filename[8], "image3.ras");
	header ( ROWS, COLUMNS, head );

	printf("Filename: Threshold Area X Y\n");

	for ( k = 0; k < 3; k ++)
	{
		clear( bimage );
		A = 0;
		xbar = 0;
		ybar = 0;
		ifile = filename[3*k];

		/* Read the image */
		if (( fp = fopen( ifile, "rb" )) == NULL )
		{
		  fprintf( stderr, "error: couldn't open %s\n", ifile );
		  exit( 1 );
		}			

		for ( i = 0; i < ROWS ; i++ )
		  if ( fread( image[i], 1, COLUMNS, fp ) != COLUMNS )
		  {
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		  }
		fclose( fp );

		/* Convert each image into binary image with its respective threshold value，
		compute the center of same image，
		and mark the center of same image*/

		// This part of the code will compute each image into a binary image with its respective threshold value
		int areaCounter = 0;
		for(int i = 0; i < ROWS; i++)
		{
			for(int j = 0; j < COLUMNS; j++)
			{
				//printf("%d ", image[i][j]);
				if(image[i][j] > threshold[k])
				{
					bimage[i][j] = 1;
					areaCounter++;
				}
				else
				{
					bimage[i][j] = 0;
				}
			}
			//printf("\n");
		}

		A = areaCounter;

		int xTotal = 0;
		int yTotal = 0;

		for(int i = 0; i < ROWS; i++)
		{
			for(int j = 0; j < COLUMNS; j++)
			{
				if(bimage[i][j] == 1)
				{
					xTotal += i * bimage[i][j];
					yTotal += j * bimage[i][j];
				}
				
			}
		}

		xbar = xTotal / A;
		ybar = yTotal / A;
		
		/* Save the binary image */		

		for(int i = 0; i < ROWS; i++)
		{
			for(int j = 0; j < COLUMNS; j++)
			{
				if(bimage[i][j] == 1)
				{
					bimage[i][j] = 255;
				}
			}
		}
		for (i = xbar - 100; i <= xbar + 100; i++) 
		{
			if (i >= 0 && i < ROWS) 
			{
				bimage[i][(int)ybar] = 128;
        	}
    	}
		for (j = xbar - 100; j <= xbar + 100; j++) 
		{
			if (j >= 0 && j < COLUMNS) 
			{
				bimage[(int)xbar][j] = 128;
        	}
    	}


		ofile = filename[3*k+1];
		if (( fp = fopen( ofile, "wb" )) == NULL )
		{
		  fprintf( stderr, "error: could not open %s\n", ofile );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( bimage[i], 1, COLUMNS, fp );
		fclose( fp );

		printf("%s: %d %d %d %d\n", ifile, threshold[k], A, (int)xbar, (int)ybar);

		/* Save the original image as ras */
		ofile = filename[3*k+2];
		if (( fp = fopen(ofile, "wb" )) == NULL )
		{
		  fprintf( stderr, "error: could not open %s\n", ofile );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( image[i], 1, COLUMNS, fp );
		fclose( fp );

	}

	printf("Press any key to exit: ");
	gets ( &ch );

	return 0;
}

void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0x00000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}


