#include <stdio.h>
#include <stdlib.h>

#define ROWS 512
#define COLS 512
#define THRESHOLD 128
#define OBJECT 255
#define BACKGROUND 0
#define CROSS 128

int main()
{
    int i, j, row_sum, col_sum, count;
    int center_row, center_col;
    int gray[ROWS][COLS];
    int binary[ROWS][COLS];
    FILE *fp;
    fp = fopen("image1.raw", "rb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            gray[i][j] = fgetc(fp);
            if (gray[i][j] >= THRESHOLD)
                binary[i][j] = OBJECT;
            else
                binary[i][j] = BACKGROUND;
        }
    }
    fclose(fp);
    row_sum = 0;
    col_sum = 0;
    count = 0;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (binary[i][j] == OBJECT) {
                row_sum += i;
                col_sum += j;
                count++;
            }
        }
    }
    center_row = row_sum / count;
    center_col = col_sum / count;
    for (i = center_row - 5; i <= center_row + 5; i++) {
        if (i >= 0 && i < ROWS) {
            binary[i][center_col] = CROSS;
        }
    }
    for (j = center_col - 5; j <= center_col + 5; j++) {
        if (j >= 0 && j < COLS) {
            binary[center_row][j] = CROSS;
        }
    }
    printf("Area of object: %d\n", count);
    printf("Center of area: (%d, %d)\n", center_row, center_col);
    fp = fopen("binary.raw", "wb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            fputc(binary[i][j], fp);
        }
    }
    fclose(fp);
    return 0;
}
