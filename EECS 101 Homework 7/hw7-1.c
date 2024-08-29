#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	240
#define COLUMNS	240
#define PI 3.14159265358979323846

float dot_product(float a[][3], float b[]);
void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

/*
Unit surface normal Nc(x, y) to the sphere with center (0, 0z0) and radius r:
Nc(x,y) = (-2x/(r^2 - x^2 - y^2)^(1/2))i + (-2y/(r^2 - x^2 - y^2)^(1/2))j + (1 - q/(r^2 - x^2 - y^2))k
*/

int main( int argc, char** argv )
{
	int				i, j, k;
	FILE*			fp;
	unsigned char	image[ROWS][COLUMNS], head[32];
	char			filename[9][50], ch;
	char 			*ofile;
	float           S[3][3] = { { 0, 0, 1 }, { 1/sqrt(3), 1/sqrt(3), 1/sqrt(3) }, { 1, 0, 0 } }, V[] = { 0, 0, 1 }, H[3], N[3], E[ROWS][COLUMNS];
	float			R[3] = { 50, 10, 100 }, M[3] = { 1, 0.1, 10000 }, A[3] = { 0.5, 0.1, 1 };
	float			r, m, a, s[3], alpha, x, y, costheta, len;
	float			Nc[3] = {0.0, 0.0, 0.0};
	
	header ( ROWS, COLUMNS, head );

	strcpy(filename[0], "image1.ras");
	strcpy(filename[1], "image2.ras");
	strcpy(filename[2], "image3.ras");
	strcpy(filename[3], "image4.ras");
	strcpy(filename[4], "image5.ras");
	strcpy(filename[5], "image6.ras");
	strcpy(filename[6], "image7.ras");
	strcpy(filename[7], "image8.ras");
	strcpy(filename[8], "image9.ras");

	for(int k = 0; k < 9; k++)
	{
		float L = 0.0;
		float L_i = 0.0;

		// set the source vector S for this configuration
		if(k == 0) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
		} else if(k == 1) {
			s[0] = 1/sqrt(3);
			s[1] = 1/sqrt(3);
			s[2] = 1/sqrt(3);
		} else if(k == 2) {
			s[0] = 1;
			s[1] = 0;
			s[2] = 0;
		} else if(k == 3) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 10;
		} else if(k == 4) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 100;
		} else if(k == 5) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			a = 0.1;
		} else if(k == 6) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			a = 1;
		} else if(k == 7) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			m = 0.1;
		} else if(k == 8) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			m = 10000;
		}
		// Define sphere center and radius
		float r = 50.0;
		float z0 = 0.0;
		float q = m / (4 * PI);

		// Define unit vector bV in direction of camera
		float bV[3] = {0.0, 0.0, 1.0};

		// Define unit vector bS in direction of light source
		float bS[3] = {0.0, 0.0, 1.0};

		// Loop through each pixel in the image
		for (i = 0; i < ROWS; i++)
		{
			y = (float)i - ROWS / 2.0;
			for (j = 0; j < COLUMNS; j++)
			{
				x = (float)j - COLUMNS / 2.0;

				// Calculate surface normal Nc(x,y)
				if (x*x + y*y <= r*r)
				{
					Nc[0] = -2.0 * x / sqrt(r*r - x*x - y*y);
					Nc[1] = -2.0 * y / sqrt(r*r - x*x - y*y);
					Nc[2] = 1.0 - q / sqrt(r*r - x*x - y*y);
					len = sqrt(Nc[0]*Nc[0] + Nc[1]*Nc[1] + Nc[2]*Nc[2]);
					Nc[0] /= len;
					Nc[1] /= len;
					Nc[2] /= len;
				}

				// Calculate the angle alpha between Nc and the angular bisector Hc
				float Hc[3] = {(bV[0]+bS[0])/sqrt(pow(bV[0]+bS[0], 2) + pow(bV[1]+bS[1], 2) + pow(bV[2]+bS[2], 2)),
							(bV[1]+bS[1])/sqrt(pow(bV[0]+bS[0], 2) + pow(bV[1]+bS[1], 2) + pow(bV[2]+bS[2], 2)),
							(bV[2]+bS[2])/sqrt(pow(bV[0]+bS[0], 2) + pow(bV[1]+bS[1], 2) + pow(bV[2]+bS[2], 2))};
				float alpha = acos(dot_product(&Nc[i][j][0], &Hc[0]));


				// Calculate the Lambertian and specular components of the reflectance model
				float L_lambert = dot_product(Nc, bS);
				float L_specular = exp(-pow(alpha/m, 2));

				// Calculate the overall reflectance value L using the linear combination of Lambertian and specular components
				L = a*L_lambert + (1-a)*L_specular;

				// Scale L to an appropriate range for an 8-bit pixel value and save to the image array
				E[i][j] = (unsigned char)(255.0 * L);
				float nx = -2 * x / sqrt(r * r - x * x - y * y);
				float ny = -2 * y / sqrt(r * r - x * x - y * y);
				float nz = 1 - m / sqrt(r * r - x * x - y * y);
				float Nc_mag = sqrt(nx * nx + ny * ny + nz * nz);
				float Nc[3] = {nx / Nc_mag, ny / Nc_mag, nz / Nc_mag};

				float theta = dot_product(S, Nc);
				theta = acos(theta);
				// compute Hc, the angular bisector of S and V
				float H[3];
				float V[] = {0, 0, 1}; // assume V is the z-axis
				float s_norm = sqrt(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
				float v_norm = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
				float S_norm[3] = {s[0]/s_norm, s[1]/s_norm, s[2]/s_norm};
				float V_norm[3] = {V[0]/v_norm, V[1]/v_norm, V[2]/v_norm};
				float sum[3] = {S_norm[0] + V_norm[0], S_norm[1] + V_norm[1], S_norm[2] + V_norm[2]};
				float sum_norm = sqrt(sum[0]*sum[0] + sum[1]*sum[1] + sum[2]*sum[2]);
				H[0] = sum[0] / sum_norm;
				H[1] = sum[1] / sum_norm;
				H[2] = sum[2] / sum_norm;

				// Compute L_lambert and L_specular
				L_lambert = dot_product(S, Nc);
				L_specular = 0.0;
				if (L_lambert > 0.0) {
					float R[3];
					reflect(R, S, Nc);
					float V[3] = {0.0, 0.0, 1.0};
					normalize(V);
					float H[3];
					bisector(H, S, V);
					normalize(H);
					float alpha = acos(dot_product(Nc, H));
					float costheta = dot_product(R, V);
					if (costheta < 0.0) costheta = 0.0;
					float phong_exp = 50.0;
					L_specular = pow(costheta, phong_exp);
				}
				float L = A[0] * L_lambert + A[1] * L_specular + A[2];

				// Scale L and store it in image array
				L *= 255.0;
				if (L > 255.0) L = 255.0;
				if (L < 0.0) L = 0.0;
				image[i][j] = (unsigned char)L;
				L = A[0] * L_lambert + A[1] * L_specular + A[2];
				image[i][j] = (unsigned char) (L * 255);
			}
		}

		
		ofile = filename[k];
		if (!( fp = fopen(ofile, "wb" ) ))
		{
			fprintf( stderr, "error: could not open %s\n", filename);
			exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ )
		{
			fwrite( image[i], sizeof(char), COLUMNS, fp );
		}
		fclose( fp );
	}


	printf("Press any key to exit: ");
	gets ( &ch );
	return 0;
}

float dot_product(float a[][3], float b[])
{
    float result = 0.0;
    int i;
    for (i = 0; i < 3; i++)
    {
        result += a[0][i] * b[i];
    }
    return result;
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