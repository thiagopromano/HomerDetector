//
// Created by Thiago on 3/4/2020.
//

#ifndef HOMER_IMAGE_H
#define HOMER_IMAGE_H
typedef unsigned char color[3];

struct Image {
    int code;
    int width;
    int height;
    color **bitmap;
};

void freeImage(Image image);
bool isALittleDarker(color c, color t, int distance);
bool isColorClose(color c, color t, int distance);
Image ReadBMP(int fileNumber);

#endif //HOMER_IMAGE_H
