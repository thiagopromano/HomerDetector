#include "Image.h"

void freeImage(Image image) {
    for (int i = 0; i < image.height; i++) {
        delete[] image.bitmap[i];
    }
    delete image.bitmap;
}
