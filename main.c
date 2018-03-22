#include <stdio.h>

unsigned int column1;
unsigned int column2;

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

int equal_pixel (Pixel firstPixel, Pixel secondPixel) {
    if (firstPixel.r == secondPixel.r && firstPixel.g == secondPixel.g && firstPixel.b == secondPixel.b){
        return 1;
    } 
    else {
        return 0;
    }
}

int mean_matrix (Image img) {
    int average;
    int divide = 3;
    average = img.pixel[column1][column2][0] + img.pixel[column1][column2][1] +
              img.pixel[column1][column2][2];

    average/= divide;
}

void data_matrix (Image img, int average) {
    img.pixel[column1][column2][0] = average;
    img.pixel[column1][column2][1] = average;
    img.pixel[column1][column2][2] = average;
 }

void blur (unsigned int height, unsigned short int pixel[512][512][3], int aux, unsigned int width) {
    for (column1 = 0; column1 < height; ++column1) {
        for (column2 = 0; column2 < width; ++column2) {
            Pixel average = {0, 0, 0};
            
            int lowerHeight;
            int minimumWidth;

            if (height - 1 > column1 + aux/2) {
                lowerHeight = column1 + aux/2;
            } else {
                lowerHeight = height - 1;
            } 
            
            if (width - 1 > column2 + aux/2) {
                minimumWidth = column2 + aux/2;
            } else {
                minimumWidth = width - 1;
            }
            
            int markEquality;            ;

            if (0 > column1 - aux/2){
                markEquality = 0;
            } else {
                markEquality = column1 - aux/2;
            }

            for(int mark = markEquality; mark <= lowerHeight; ++mark) {
                int aux1;

                if( 0 > column2 - aux/2 ){
                    aux1 = 0;
                } else {
                    aux1 = column2 - aux/2;
                }

                for(int label = aux1; label <= minimumWidth; ++label) {
                    average.r += pixel[mark][label][0];
                    average.g += pixel[mark][label][1];
                    average.b += pixel[mark][label][2];
                }
            }

            average.r /= aux * aux;
            average.g /= aux * aux;
            average.b /= aux * aux;

            pixel[column1][column2][0] = average.r;
            pixel[column1][column2][1] = average.g;
            pixel[column1][column2][2] = average.b;
        }
    }
}

void invert_colors (unsigned short int pixel[512][512][3], unsigned int width, unsigned int height) {
    for (column1 = 0; column1 < height; ++column1) {
        for (column2 = 0; column2 < width; ++column2) {
            pixel[column1][column2][0] = 255 - pixel[column1][column2][0];
            pixel[column1][column2][1] = 255 - pixel[column1][column2][1];
            pixel[column1][column2][2] = 255 - pixel[column1][column2][2];
        }
    }
}

// This part will group all filters used in the project
Image grey_scale (Image img) {
    
        for (column1 = 0; column1 < img.height; ++column1) {
            for (column2 = 0; column2 < img.width; ++column2) {
                int average = mean_matrix (img);
    
                data_matrix (img, average);
            }
        }
    
        return img;
    }

Image sepia_filter (Image img){
    for (column1 = 0; column1 < img.height; ++column1) {
        for (column2 = 0; column2 < img.width; ++column2) {
            unsigned short int pixel[3];

            pixel[0] = img.pixel[column1][column2][0];
            pixel[1] = img.pixel[column1][column2][1];
            pixel[2] = img.pixel[column1][column2][2];
            
            // p is for pixel
            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;

            int minorRange; 

            if ( 255 > p) {
               minorRange = p;
            } else {
               minorRange =255;
            }

            img.pixel[column1][column2][0] = minorRange;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;

            img.pixel[column1][column2][1] = minorRange;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          
            img.pixel[column1][column2][2] = minorRange;
        }
    }
}

Image rotate_90_right (Image img) {
    Image rotated;

    int lineOne;
    int lineTwo;

    rotated.width = img.height;
    rotated.height = img.width;

    for (column1 = 0, lineOne = 0; column1 < rotated.height; ++column1, ++lineOne) {
        for (column2 = rotated.width - 1, lineTwo = 0; column2 >= 0; --column2, ++lineTwo) {
            rotated.pixel[column1][column2][0] = img.pixel[lineOne][lineTwo][0];
            rotated.pixel[column1][column2][1] = img.pixel[lineOne][lineTwo][1];
            rotated.pixel[column1][column2][2] = img.pixel[lineOne][lineTwo][2];
        }
    }

    return rotated;
}

Image mirror_filter (Image img){
    int horizontal = 0;
    scanf("%d", &horizontal);

    int w = img.width;
    int h = img.height;

    if (horizontal == 1) {
        w /= 2;
    } else {
        h /= 2;
    }

    for (column1 = 0; column1 < h; ++column1) {
        for (column2 = 0; column2 < w; ++column2) {
            
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
}

Image cut_image (Image img, int firtsRow, int secondRow, int w, int h) {
    Image cutting;

    cutting.width = w;
    cutting.height = h;

    for(column1 = 0; column1 < h; ++column1) {
        for(column2 = 0; column2 < w; ++column2) {
            cutting.pixel[column1][column2][0] = img.pixel[column1 + firtsRow][column2 + secondRow][0];
            cutting.pixel[column1][column2][1] = img.pixel[column1 + firtsRow][column2 + secondRow][1];
            cutting.pixel[column1][column2][2] = img.pixel[column1 + firtsRow][column2 + secondRow][2];
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
    for (unsigned int column1 = 0; column1 < img.height; ++column1) {
        for (unsigned int column2 = 0; column2 < img.width; ++column2) {
            scanf("%hu %hu %hu", &img.pixel[column1][column2][0],
                                 &img.pixel[column1][column2][1],
                                 &img.pixel[column1][column2][2]);

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
                img = sepia_filter (img);
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
                img = mirror_filter (img);
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
