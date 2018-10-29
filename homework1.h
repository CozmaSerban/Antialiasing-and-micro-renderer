#ifndef HOMEWORK_H1
#define HOMEWORK_H1

typedef struct{
    unsigned char a;
}blackwhite;


typedef struct {
    int width;
    blackwhite** img;
}image;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */