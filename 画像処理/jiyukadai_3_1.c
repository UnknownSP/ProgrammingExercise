/*
 *  19T1698W_jiyukadai_3_1.c
 *  自由課題 3-1
 *  PSNRの誤差マップを出力するプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

int main(int argc, char *argv[]){
    
    if(argc != 3){
        printf("command error \n");
        printf("Usage : ./jiyukadai_3_1 [input filename] [output filename]\n");
        return 1;
    }

    char *originalIm_filename = argv[1];    //原画像のファイル名
    char *outputIm_filename = argv[2];      //出力画像のファイル名
    char *out_filename = "Result-j-3-1.jpg";//誤差画像の出力ファイル名
    int imageWidth = 0;                     //画像データの横幅の画素数
    int imageHeight = 0;                    //画像データの縦幅の画素数
    int imageBPP = 0;                       //画像データの1画素あたりのバイト数
    BYTE *data_originalIm = NULL;           //原画像データ領域のポインタ
    BYTE *data_outputIm = NULL;             //出力画像データ領域のポインタ
    BYTE *data_out = NULL;                  //出力誤差マップ画像データ領域のポインタ
    int imageSize = 0;                      //画像データのサイズ
    double MSE = 0.0;                       //MSE計算用
    double *PSNR;                           //PSNR計算用
    double PSNR_max,PSNR_min;               //PSNRの最大値最小値計算用
    double cal_pixel_value;                 //画素値計算用
    int i,j;

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*画像データの読み込み*/
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

    /*出力画像データ領域の確保*/
    data_out = (BYTE *)malloc(imageSize);

    /*出力誤差マップ画像データ領域を確保できなかった場合終了する*/
    if(data_out == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*PSNRを画素ごとに計算し、最大値最小値を求める*/
    PSNR = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
    for(i=0;i<imageSize;i+=3){
        MSE = 0.0;
        MSE += pow(data_originalIm[i]-data_outputIm[i],2);
        MSE += pow(data_originalIm[i+1]-data_outputIm[i+1],2);
        MSE += pow(data_originalIm[i+2]-data_outputIm[i+2],2);
        MSE *= 1.0/3.0;
        PSNR[i/3] = 10.0 * log10(255*255/MSE);
        if(i==0){
            PSNR_max = PSNR[0];
            PSNR_min = PSNR[0];
        }
        if(PSNR_max < PSNR[i/3]){
            PSNR_max = PSNR[i/3];
        }
        if(PSNR_min > PSNR[i/3]){
            PSNR_min = PSNR[i/3];
        }
    }
    printf("PSNR MIN = %3.3f [dB]\n",PSNR_min);
    printf("PSNR MAX = %3.3f [dB]\n",PSNR_max);

    /*PSNRが最大値であった場合、有限の値を最大値とする*/
    if(isinf(PSNR_max)){
        PSNR_max = 0.0;
        for(i=0;i<imageWidth*imageHeight;i++){
            if((PSNR_max < PSNR[i]) && !isinf(PSNR[i])){
                PSNR_max = PSNR[i];
            }
        }
    }
    printf("PSNR MAX = %3.3f [dB]\n",PSNR_max);

    /*PSNRが最小値のとき0、最大値のとき255として画素値を計算し誤差マップを出力*/
    for(i=0;i<imageSize;i+=3){
        cal_pixel_value = (PSNR[i/3]-PSNR_min)/(PSNR_max-PSNR_min) * 255;
        if(cal_pixel_value > 255){
            cal_pixel_value = 255;
        }
        data_out[i] = cal_pixel_value;
        data_out[i+1] = cal_pixel_value;
        data_out[i+2] = cal_pixel_value;
    }
    writeJpeg(out_filename,data_out,imageWidth,imageHeight,imageBPP,100);

    /*画像データ領域の開放*/
    free(data_originalIm);
    free(data_outputIm);
    free(data_out);

    return 0;
}