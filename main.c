#include <stdio.h>

typedef struct _pixel {
    // Is a RGB (red, green, blue) type of image for an additive color system
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // width and height = 512, with is the size of the matrix of image
    // rgb = 3, with is the colors spectrum
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

//colocar o else
// These function is used only for define a color
int max (int firstColor, int secondColor) {
    if (firstColor > secondColor){
         return firstColor;
    } 
    else {
        return secondColor;
    }
}

int equal_pixel (Pixel firstPixel, Pixel secondPixel) {
    if (firstPixel.r == secondPixel.r && firstPixel.g == secondPixel.g && firstPixel.b == secondPixel.b){
        return 1;
    } 
    else {
        return 0;
    }
}


Image grey_scale (Image img) {

    for (unsigned int column1 = 0; column1 < img.height; ++column1) {
        for (unsigned int column2 = 0; column2 < img.width; ++column2) {
            int media = img.pixel[column1][column2][0] +
                        img.pixel[column1][column2][1] +
                        img.pixel[column1][column2][2];
            media /= 3;
            img.pixel[column1][column2][0] = media;
            img.pixel[column1][column2][1] = media;
            img.pixel[column1][column2][2] = media;
        }
    }

    return img;
}

void blur (unsigned int height, unsigned short int pixel[512][512][3], int aux, unsigned int width) {
    for (unsigned int firstColumn = 0; firstColumn < height; ++firstColumn) {
        for (unsigned int secondColumn = 0; secondColumn < width; ++secondColumn) {
            Pixel media = {0, 0, 0};
            
            int lowerHeight = (height - 1 > firstColumn + aux/2) ? firstColumn + aux/2 : height - 1;
            int minimumWidth = (width - 1 > secondColumn + aux/2) ? secondColumn + aux/2 : width - 1;
            
            for(int mark = (0 > firstColumn - aux/2 ? 0 : firstColumn - aux/2); mark <= lowerHeight; ++mark) {
                for(int label = (0 > secondColumn - aux/2 ? 0 : secondColumn - aux/2); label <= minimumWidth; ++label) {
                    media.r += pixel[mark][label][0];
                    media.g += pixel[mark][label][1];
                    media.b += pixel[mark][label][2];
                }
            }

            media.r /= aux * aux;
            media.g /= aux * aux;
            media.b /= aux * aux;

            pixel[firstColumn][secondColumn][0] = media.r;
            pixel[firstColumn][secondColumn][1] = media.g;
            pixel[firstColumn][secondColumn][2] = media.b;
        }
    }
}

Image rotate_90_right (Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int columnOne = 0, lineOne = 0; columnOne < rotated.height; ++columnOne, ++lineOne) {
        for (int columnTwo = rotated.width - 1, lineTwo = 0; columnTwo >= 0; --columnTwo, ++lineTwo) {
            rotated.pixel[columnOne][columnTwo][0] = img.pixel[lineOne][lineTwo][0];
            rotated.pixel[columnOne][columnTwo][1] = img.pixel[lineOne][lineTwo][1];
            rotated.pixel[columnOne][columnTwo][2] = img.pixel[lineOne][lineTwo][2];
        }
    }

    return rotated;
}

void invert_colors (unsigned short int pixel[512][512][3], unsigned int width, unsigned int height) {
    for (unsigned int column1 = 0; column1 < height; ++column1) {
        for (unsigned int column2 = 0; column2 < width; ++column2) {
            pixel[column1][column2][0] = 255 - pixel[column1][column2][0];
            pixel[column1][column2][1] = 255 - pixel[column1][column2][1];
            pixel[column1][column2][2] = 255 - pixel[column1][column2][2];
        }
    }
}

Image cut_image (Image img, int firtsRow, int secondRow, int w, int h) {
    Image cutting;

    cutting.width = w;
    cutting.height = h;

    for(int firstColumn = 0; firstColumn < h; ++firstColumn) {
        for(int secondColumn = 0; secondColumn < w; ++secondColumn) {
            cutting.pixel[firstColumn][secondColumn][0] = img.pixel[firstColumn + firtsRow][secondColumn + secondRow][0];
            cutting.pixel[firstColumn][secondColumn][1] = img.pixel[firstColumn + firtsRow][secondColumn + secondRow][1];
            cutting.pixel[firstColumn][secondColumn][2] = img.pixel[firstColumn + firtsRow][secondColumn + secondRow][2];
        }
    }

    return cutting;
}


int main() {
    Image img;

    // read type of image
    char readImage[4];
    scanf("%s", readImage);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int columnOne = 0; columnOne < img.height; ++columnOne) {
        for (unsigned int columnTwo = 0; columnTwo < img.width; ++columnTwo) {
            scanf("%hu %hu %hu", &img.pixel[columnOne][columnTwo][0],
                                 &img.pixel[columnOne][columnTwo][1],
                                 &img.pixel[columnOne][columnTwo][2]);

        }
    }

    int number_options;
    scanf("%d", &number_options);

    for(int generic = 0; generic < number_options; ++generic) {
        int option;
        scanf("%d", &option);

        switch(option) {

            case 1: { 
                img = grey_scale(img);
                break;
            }

            case 2: { 
                //Sepia filter
                for (unsigned int column1 = 0; column1 < img.height; ++column1) {
                    for (unsigned int column2 = 0; column2 < img.width; ++column2) {
                        unsigned short int pixel[3];

                        pixel[0] = img.pixel[column1][column2][0];
                        pixel[1] = img.pixel[column1][column2][1];
                        pixel[2] = img.pixel[column1][column2][2];
                        
                        // p is for pixel
                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int minorRange = (255 >  p) ? p : 255;

                        img.pixel[column1][column2][0] = minorRange;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        minorRange = (255 >  p) ? p : 255;
                        img.pixel[column1][column2][1] = minorRange;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        minorRange = (255 >  p) ? p : 255;
                        img.pixel[column1][column2][2] = minorRange;
                    }
                }

                break;
            }

            case 3: { 
                // Blur
                int size = 0;
                scanf("%d", &size);
                blur(img.height, img.pixel, size, img.width);
                break;
            }
            
            case 4: { 
                // Rotation
                int how_many  = 0;
                scanf("%d", &how_many);
                how_many %= 4;
                for (int j = 0; j < how_many; ++j) {
                    img = rotate_90_right(img);
                }
                break;
            }

            case 5: { // Mirroring
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.width;
                int h = img.height;

                if (horizontal == 1) {
                    w /= 2;
                } else {
                    h /= 2;
                }

                for (int column1 = 0; column1 < h; ++column1) {
                    for (int column2 = 0; column2 < w; ++column2) {
                        
                        int x = column1;
                        int y = column2;

                        if (horizontal == 1) {
                            y = img.width - 1 - column2;
                        } else {
                            x = img.height - 1 - column1;
                        }
                        

                        Pixel aux1;
                        aux1.r = img.pixel[column1][column2][0];
                        aux1.g = img.pixel[column1][column2][1];
                        aux1.b = img.pixel[column1][column2][2];

                        img.pixel[column1][column2][0] = img.pixel[x][y][0];
                        img.pixel[column1][column2][1] = img.pixel[x][y][1];
                        img.pixel[column1][column2][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.r;
                        img.pixel[x][y][1] = aux1.g;
                        img.pixel[x][y][2] = aux1.b;
                    }
                }
                break;
            }

            case 6: { 
                invert_colors (img.pixel, img.width, img.height);
                break;
            }

            case 7: { 
                // Cut Imagem
                int firstLine;
                int secondLine;
                scanf("%d %d", &firstLine, &secondLine);
                
                int width;
                int height;
                scanf("%d %d", &width, &height);

                img = cut_image (img, firstLine, secondLine, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("readImage\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int column1 = 0; column1 < img.height; ++column1) {
        for (unsigned int column2 = 0; column2 < img.width; ++column2) {
            printf("%hu %hu %hu ", img.pixel[column1][column2][0],
                                   img.pixel[column1][column2][1],
                                   img.pixel[column1][column2][2]);

        }
        printf("\n");
    }
    return 0;
}
