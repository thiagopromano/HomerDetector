#include <stdio.h>
#include <stdlib.h>
#include <config.h>


bool isALittleLess(int color, int target, int distance) { return color > target - distance && color < target; }

bool isALittleDarker(int r, int g, int b, int tr, int tg, int tb, int distance) { return isALittleLess(r, tr, distance) && isALittleLess(g, tg, distance) && isALittleLess(b, tb, distance); }

bool isColorClose(int r, int g, int b, int tr, int tg, int tb, int distance) {
    if (isALittleDarker(r, g, b, tr, tg, tb, distance * 2))
        return true;
    return (abs(r - tr) + abs(g - tg) + abs(b - tb)) < distance;
}

bool ReadBMP(int fileNumber) {
    char filename[100];
    sprintf(filename, "%s\\%d.bmp", FILES_PATH, fileNumber);
    int i;
    int cont = 0;
    FILE *f = fopen(filename, "rb");

    if (f == NULL)
        return NULL;

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header


    int headerSize = *(int *) &info[14];
    //printf("Header size: %d\n", headerSize);

    int width = *(int *) &info[18];
    int height = *(int *) &info[22];

    //printf("Name: %s ", filename);
    //printf("Width: %d\n", width);
    //printf("Height: %d\n", height);

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char *data = new unsigned char[row_padded];
    unsigned char tmp;

    for (int i = 0; i < height; i++) {
        fread(data, sizeof(unsigned char), row_padded, f);
        for (int j = 0; j < width * 3; j += 3) {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;
            if (isColorClose((int) data[j], (int) data[j + 1], (int) data[j + 2], 209, 178, 113, 18)) {
                cont++;
            }
            //printf("R: %d - G: %d - B: %d\n", (int)data[j], (int)data[j+1], (int)data[j+2]);
        }
    }
    if (cont > 1) {
        return true;
    } else {
        return false;
    }
    delete data;
    fclose(f);
}

int main(int argc, char *argv[]) {

    int i;

    const char *hasHomerTruth =
            "ssssnnssssnsssnnssssnnsnsnsssnsssnssnsnnnnnnnnnnnnnnnnnnnn";

    int tp = 0;
    int fp = 0;
    int tn = 0;
    int fn = 0;
    for (i = 1; i <= 58; i++) {
        bool predicted = ReadBMP(i);
        if (predicted == true && hasHomerTruth[i - 1] == 's')
            tp++;
        if (predicted == true && hasHomerTruth[i - 1] == 'n') {
            fp++;
            printf("errou->");
        }
        if (predicted == false && hasHomerTruth[i - 1] == 'n')
            tn++;
        if (predicted == false && hasHomerTruth[i - 1] == 's') {
            printf("errou->");
            fn++;
        }
        printf("disse que a foto %d e %d e na real e %d\n", i, predicted, hasHomerTruth[i - 1] == 's');
    }
    printf("true positive = %d\n", tp);
    printf("false positive = %d\n", fp);
    printf("true negative = %d\n", tn);
    printf("false negative = %d\n", fn);

    return 0;
}