/*
 *  19T1698W_kadai_3_3.c
 *  必須課題 3-3
 *  画像合成の基礎を行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

int main(int argc, char *argv[]){
    
    if(argc != 3){
        printf("command error \n");
        printf("Usage : ./kadai_3_3 [input filename] [output filename]\n");
        return 1;
    }

    char *originalIm_filename = argv[1];
    char *noiseIm_filename = argv[2];
    int imageWidth = 0;
    int imageHeight = 0;
    int imageBPP = 0;
    BYTE *data_originalIm = NULL;
    BYTE *data_noiseIm = NULL;
    int imageSize = 0;
    double mse = 0.0;
    double psnr = 0.0;
    int i,j;

    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    imageSize = readJpeg(originalIm_filename,&data_originalIm);
    
    if(imageSize != readJpeg(noiseIm_filename,&data_noiseIm)){
        return 1;
    }


    for(j=0;j<3;j++){
        for(i=0;i<imageSize;i+=3){
            mse += pow((double)(data_originalIm[i+j]-data_noiseIm[i+j]),2.0);
        }
    }
    mse = mse/(3.0*imageWidth*imageHeight);
    psnr = 10.0 * log10(255*255/mse);
    printf("MSE  = %3.3f \n",mse);
    printf("PSNR  = %3.3f \n",psnr);
    for(i=0;i<10;i++){
        printf("%d %d\n",data_originalIm[i],data_noiseIm[i]);
    }
}