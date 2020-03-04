#include <stdio.h>
#include <stdlib.h>
#include <config.h>
#include "Image.h"


bool isALittleLess(int color, int target, int distance) { return color > target - distance && color < target; }

bool isALittleDarker(color c, color t, int distance) {
    return isALittleLess(c[0], t[0], distance) &&
           isALittleLess(c[1], t[1], distance) &&
           isALittleLess(c[2], t[2], distance);
}

bool isColorClose(color c, color t, int distance) {
    return (abs(c[0] - t[0]) + abs(c[1] - t[1]) + abs(c[2] - t[2])) < distance;
}

bool hasHomer(Image image) {
    int cont = 0;
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            color target = {209, 178, 113};
            if (isColorClose(image.bitmap[i][j], target, 15) || isALittleDarker(image.bitmap[i][j], target, 30))
                cont++;
        }
    }
    return cont > 5;
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
    Image image = {.width=width, .height=height};
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

int main(int argc, char *argv[]) {

    try {
        const char *hasHomerTruth =
                "ssssnnssssnsssnnssssnnsnsnsssnsssnssnsnnnnnnnnnnnnnnnnnnnn";

        int tp = 0;
        int fp = 0;
        int tn = 0;
        int fn = 0;

        for (int i = 1; i <= 58; i++) {
            Image image = ReadBMP(i);
            bool predicted = hasHomer(image);
            freeImage(image);
            if (predicted && hasHomerTruth[i - 1] == 's')
                tp++;
            if (predicted && hasHomerTruth[i - 1] == 'n') {
                fp++;
                printf("errou->");
            }
            if (!predicted && hasHomerTruth[i - 1] == 'n')
                tn++;
            if (!predicted && hasHomerTruth[i - 1] == 's') {
                printf("errou->");
                fn++;
            }
            printf("disse que a foto %d e %d e na real e %d\n", i, predicted, hasHomerTruth[i - 1] == 's');
        }
        printf("true positive = %d\n", tp);
        printf("false positive = %d\n", fp);
        printf("true negative = %d\n", tn);
        printf("false negative = %d\n", fn);
    } catch (const char *err) {
        printf("%s", err);
    }
    return 0;
}
