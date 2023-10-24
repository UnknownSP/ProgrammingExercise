/*
 *  19T1698W_jiyukadai_1_3_a.c
 *  自由課題 1-3 (その1)
 *  画像の色補正・色変換を手動で行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"

#define R_coeff 0.9 //R成分の補正係数
#define G_coeff 0.5 //G成分の補正係数
#define B_coeff 0.7 //B成分の補正係数

int main(void){

    char *In_filename = "image_3.jpg";          //読み込む画像のファイル名
    char *Out_filename = "Result_j_1_3_a.jpg";  //出力画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                        //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ
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
    /*RGB各成分に係数をかけて出力画像データ領域に保存*/
    for(i=0;i<imageSize;i+=3){
        dataOut[i] = dataIn[i] * R_coeff;
        dataOut[i+1] = dataIn[i +1] * G_coeff;
        dataOut[i+2] = dataIn[i+2] * B_coeff;
    }
    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}