#ifndef HOMEWORK_H
#define HOMEWORK_H


typedef struct{
    unsigned char a;
}blackwhite;

typedef struct {
    int type;
    int width;
    int height;
    int max;
    unsigned char *imag;
    blackwhite **imagbl;
}image;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */