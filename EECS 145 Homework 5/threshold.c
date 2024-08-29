#include <stdio.h>
#include <stdlib.h>

int main() {
    /* Load the raw image */
    FILE* fp = fopen("image.raw", "rb");
    unsigned char image[320][320];
    fread(image, sizeof(unsigned char), 320*320, fp);
    fclose(fp);

    /* Compute the histogram */
    int hist[256] = {0};
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 320; j++) {
            hist[image[i][j]]++;
        }
    }

    /* Compute Otsu's threshold */
    int total_pixels = 320*320;
    int sum = 0, sumB = 0, wB = 0, wF = 0;
    float mB, mF, between, max = 0.0, threshold = 0.0;
    for (int i = 0; i < 256; i++) {
        sum += i * hist[i];
    }
    for (int i = 0; i < 256; i++) {
        wB += hist[i];
        if (wB == 0) continue;
        wF = total_pixels - wB;
        if (wF == 0) break;
        sumB += i * hist[i];
        mB = (float)sumB / wB;
        mF = (float)(sum - sumB) / wF;
        between = (float)wB * wF * (mB - mF) * (mB - mF);
        if (between >= max) {
            threshold = i;
            max = between;
        }
    }

    printf("Computed threshold: %f\n", threshold);

    /* Threshold the image to create a binary image */
    unsigned char binary[320][320];
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 320; j++) {
            if (image[i][j] > threshold)
                binary[i][j] = 255; // white
            else
                binary[i][j] = 0; // black
        }
    }

    /* Save the binary image */
    fp = fopen("binary.raw", "wb");
    fwrite(binary, sizeof(unsigned char), 320*320, fp);
    fclose(fp);

    return 0;
}
