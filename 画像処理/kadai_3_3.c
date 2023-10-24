/*
 *  19T1698W_kadai_3_3.c
 *  必須課題 3-3
 *  PSNR値を計算し出力するプログラム
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

    char *originalIm_filename = argv[1];    //原画像のファイル名
    char *outputIm_filename = argv[2];      //出力画像のファイル名
    int imageWidth = 0;                     //画像データの横幅の画素数
    int imageHeight = 0;                    //画像データの縦幅の画素数
    int imageBPP = 0;                       //画像データの1画素あたりのバイト数
    BYTE *data_originalIm = NULL;           //原画像データ領域のポインタ
    BYTE *data_outputIm = NULL;             //出力画像データ領域のポインタ
    int imageSize = 0;                      //画像データのサイズ
    double MSE = 0.0;                       //MSE計算用
    double PSNR = 0.0;                      //PSNR計算用
    int i,color;

    if(argc != 3){
        printf("command error \n");
        printf("Usage : ./kadai_3_3 [input filename] [output filename]\n");
        return 1;
    }

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*原画像データの読み込み*/
    imageSize = readJpeg(originalIm_filename,&data_originalIm);
    /*原画像データの読み込みを行い、出力画像とサイズが違った場合終了*/
    if( imageSize != readJpeg(outputIm_filename,&data_outputIm)
        || imageWidth != readHeader(outputIm_filename,IMAGE_WIDTH)
        || imageHeight != readHeader(outputIm_filename,IMAGE_HEIGHT)
        || imageBPP != readHeader(outputIm_filename,IMAGE_BPP)){
        printf("ImageSize Error\n");
        return 1;
    }

    /*画像データの読み込みに失敗した場合終了する*/
    if(imageSize == 0){
        printf("Read Error\n");
        return 1;
    }
    
    /*MSEの計算*/
    for(color=0;color<3;color++){
        for(i=0;i<imageSize;i+=3){
            MSE += pow((double)(data_originalIm[i+color]-data_outputIm[i+color]),2.0);
        }
    }
    MSE *= 1.0/(3.0*imageWidth*imageHeight);
    
    /*PSNRを計算して表示*/
    PSNR = 10.0 * log10(255*255/MSE);
    printf("PSNR = %3.3f [dB]\n",PSNR);

    /*画像データ領域の開放*/
    free(data_originalIm);
    free(data_outputIm);

    return 0;
}