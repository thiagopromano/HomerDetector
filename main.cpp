#include <stdio.h>
#include <stdlib.h>
#include <config.h>
#include "Image.h"

color homerBeard = {209, 178, 113};
color white = {255, 255, 255};
color homerYellow = {248, 214, 3};
color black = {58, 45, 29};

bool hasHomer(Image image) {
    int cont = 0;
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            if (isColorClose(image.bitmap[i][j], homerBeard, 20) ||
                isALittleDarker(image.bitmap[i][j], homerBeard, 12)) {
                //return true;
                int notEyeCount = 0;
                for (int curline = i; curline > 0; curline--) {
                    if (isColorClose(image.bitmap[curline][j], white, 50)) {
                        cont++;
                        break;
                    } else notEyeCount++;
                    if (notEyeCount > 50) {
                        break;
                    }
                }
            }
        }
    }
    return cont > 5;
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

        float precision = tp / (tp + (float) fp);
        float recall = tp / (tp + (float) fn);

        printf("precision = %f\n", precision);
        printf("recall = %f\n", recall);

        float f1 = 2 * precision * recall / (precision + recall);
        printf("f1 = %f\n", f1);
    } catch (const char *err) {
        printf("%s", err);
    }
    return 0;
}
