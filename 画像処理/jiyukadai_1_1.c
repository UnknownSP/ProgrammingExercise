/*
 *  19T1698W_jiyukadai_1_1.c
 *  自由課題 1-1
 *  画像の2値化をするプログラム
 */

#include "stdio.h"
#include "jpegio.h"

#define Threshold 165  //2値化する際の閾値

int main(void){

    char *In_filename = "image_3.jpg";          //読み込む画像のファイル名
    char *Out_filename = "Result_j_1_1_165.jpg";    //出力画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                        //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ
    int calc_value;                             //計算値保存用
    int i;

    /*画像情報の読み込み*/
    imageWidth = readHeader(In_filename,IMAGE_WIDTH);
    imageHeight = readHeader(In_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(In_filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(In_filename,&dataIn);

    /*画像データの読み込みに失敗した場合終了する*/
    if(imageSize == 0){
        printf("Read Error\n");
        return 1;
    }

    /*出力画像データ領域の確保*/
    dataOut = (BYTE *)malloc(imageSize);
    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*変換式1を用いてモノクロ画像化した際の画素値を求め、閾値未満なら黒、以上なら白にする*/
    for(i=0;i<imageSize;i+=3){
        calc_value = 0.299 * dataIn[i] + 0.587 * dataIn[i+1] + 0.114 * dataIn[i+2];
        if(calc_value < Threshold){
            dataOut[i] = 0;
            dataOut[i+1] = 0;
            dataOut[i+2] = 0;
        }else{
            dataOut[i] = 255;
            dataOut[i+1] = 255;
            dataOut[i+2] = 255;
        }
    }
    /*2値化したものをファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}