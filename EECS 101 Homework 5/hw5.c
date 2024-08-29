#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	480
#define COLUMNS	640
#define PI 3.14159265358979323846

#define sqr(x)	((x)*(x))

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char** argv )
{
	int i, j, sgmmax;
    int dedx, dedy, sgm, max[3] = {0, 0, 0}, index[3][2] = { 0, 0, 0, 0, 0, 0 };
    int sgm_threshold, hough_threshold, voting[180][400];
    FILE* fp;
    unsigned char image[ROWS][COLUMNS], simage[ROWS][COLUMNS], head[32];
    char filename[50], ifilename[50];
    float theta, rho;

    clear(simage);
    strcpy(filename, "image.raw");
    memset(voting, 0, sizeof(int) * 180 * 400);
    header(COLUMNS, ROWS, head);
	 
	/* Read in the image */
	if (!( fp = fopen(filename, "rb" ) ))
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1]);
		exit(1);
	}

	for ( i = 0 ; i < ROWS ; i++ )
		if (!( COLUMNS == fread( image[i], sizeof(char), COLUMNS, fp ) ))
		{
			fprintf( stderr, "error: couldn't read %s\n", argv[1] );
			exit(1);
		}
	fclose(fp);

	/* Compute SGM */
	for ( i = 1 ; i < ROWS - 1 ; i++ )
	{
		for ( j = 1 ; j < COLUMNS - 1 ; j++ )
		{
	
			dedx = -image[i-1][j-1] + image[i-1][j+1] 
				- 2*image[i][j-1] + 2*image[i][j+1] 
				- image[i+1][j-1] + image[i+1][j+1];
			dedy = -image[i-1][j-1] - 2*image[i-1][j] - image[i-1][j+1] 
				+ image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1];

			sgm = (int) sqrt((double) sqr(dedx) + sqr(dedy));

			simage[i][j] = (unsigned char) sgm;
		}
	}

	sgm_threshold = 50;
	hough_threshold = 250;

	/* build up voting array */
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			if (simage[i][j] > sgm_threshold)
			{
				for (int t = 0; t < 180; t++)
				{
					theta = t * PI / 180;
					rho = j * cos(theta) + i * sin(theta);
					voting[t][ (int) (rho + 200) ]++;
				}
			}
		}
	}

	/* Save SGM to an image */
	strcpy(filename, "image");
	if (!(fp = fopen(strcat(filename, "-sgm.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);

	/* Compute the binary image */
	for ( i = 0 ; i < ROWS ; i++ )
	{
		for ( j = 0 ; j < COLUMNS ; j++ )
		{
			if ( simage[i][j] >= sgm_threshold )
				simage[i][j] = 255;
			else
				simage[i][j] = 0;
		}
	}

	/* Save the thresholded SGM to an image */
	strcpy(filename, "image");
	if (!(fp = fopen(strcat(filename, "-binary.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);

	/* Save original voting array to an image */
	strcpy(filename, "image");
	header(180, 400, head);
	if (!(fp = fopen(strcat(filename, "-voting_array.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);

	for (i = 0; i < 180; i++)
		fwrite(voting[i], sizeof(int), 400, fp);
	fclose(fp);


	/* Threshold the voting array */
	for (i = 0; i < 180; i++) {
		for (j = 0; j < 400; j++) {
			if (voting[i][j] > hough_threshold) {
				simage[i][j] = 255;
			} else {
				simage[i][j] = 0;
			}
		}
	}

	/* Write the thresholded voting array to a new image */
	strcpy(filename, "image");
	header(180, 400, head);
	if (!(fp = fopen(strcat(filename, "-voting_array.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}

	// Convert the 2D voting array to a 1D array of 8-bit values
	unsigned char voting_1d[180 * 400];
	for (i = 0; i < 180; i++) {
		for (j = 0; j < 400; j++) {
			voting_1d[i * 400 + j] = (unsigned char) voting[i][j];
		}
	}

	fwrite(head, 4, 8, fp);
	fwrite(voting_1d, sizeof(unsigned char), 180*400, fp);
	fclose(fp);


	printf("Hough threshold: %d\n", hough_threshold);
	printf("%d %d %d\n%d %d %d\n%d %d %d\n", index[0][0], index[0][1], voting[0][0],
										 index[1][0], index[1][1], voting[1][0],
										 index[2][0], index[2][1], voting[2][0]);


	// Reconstruct an image from the voting array
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			int pixel_value = 0;
			for (int k = 0; k < 3; k++)
			{
				rho = (float)(index[k][0] - 199) * 0.25;
				theta = (float)(index[k][1] - 90);
				theta *= PI / 180.0;
		
				float distance = fabs((float)j * cos(theta) + (float)i * sin(theta) - rho);
			
				if (distance <= hough_threshold)
				{
					int rho_index = (int)(rho + 200);
					if (rho_index >= 0 && rho_index < 400)
					{
						pixel_value += voting[(int)theta][rho_index];
					}
				}
			}
			
			if (pixel_value >= sgm_threshold)
			{
				simage[i][j] = 255;
			}
			else
			{
				simage[i][j] = 0;
			}

		}
	}
	/* Write the reconstructed figure to an image */
	strcpy(filename, "image");
	header(ROWS, COLUMNS, head);
	if (!(fp = fopen(strcat(filename, "-reconstructed_image.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);

	printf("Press any key to exit: ");
	char ch;
	fgets(&ch, sizeof(ch), stdin);

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
	*(p + 7) = 0xf8000000;

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

