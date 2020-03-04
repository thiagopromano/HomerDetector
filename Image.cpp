#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#include "Image.h"

void freeImage(Image image) {
    for (int i = 0; i < image.height; i++) {
        delete[] image.bitmap[i];
    }
    delete image.bitmap;
}

bool isALittleLess(int color, int target, int distance) { return color > target - distance && color < target; }

bool isALittleDarker(unsigned char *c, unsigned char *t, int distance) {
    return isALittleLess(c[0], t[0], distance) &&
           isALittleLess(c[1], t[1], distance) &&
           isALittleLess(c[2], t[2], distance);
}

bool isColorClose(unsigned char *c, unsigned char *t, int distance) {
    return (abs(c[0] - t[0]) + abs(c[1] - t[1]) + abs(c[2] - t[2])) < distance;
}

Image ReadBMP(int fileNumber) {
    char filename[100];
    sprintf(filename, "%s/%d.bmp", FILES_PATH, fileNumber);
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        throw "could not read file";
    }

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    int width = *(int *) &info[18];
    int height = *(int *) &info[22];
    Image image = {.code=fileNumber, .width=width, .height=height};
    image.bitmap = new color *[height];
    for (int i = 0; i < image.height; i++) {
        image.bitmap[i] = new color[width];
    }

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char *data = new unsigned char[row_padded];
    unsigned char tmp;


    for (int i = 0; i < height; i++) {
        fread(data, sizeof(unsigned char), row_padded, f);
        for (int j = 0; j < width; j++) {
            image.bitmap[i][j][0] = data[j * 3 + 2];
            image.bitmap[i][j][1] = data[j * 3 + 1];
            image.bitmap[i][j][2] = data[j * 3];
        }
    }
    delete[] data;
    fclose(f);
    return image;
}