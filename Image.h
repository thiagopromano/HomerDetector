//
// Created by Thiago on 3/4/2020.
//

#ifndef HOMER_IMAGE_H
#define HOMER_IMAGE_H
typedef unsigned char color[3];

struct Image {
    int width;
    int height;
    color **bitmap;
};

void freeImage(Image image);

#endif //HOMER_IMAGE_H
