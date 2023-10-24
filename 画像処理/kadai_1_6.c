/*
 *  19T1698W_kadai_1_6.c
 *  必須課題 1-6
 *  モノクロ画像の出力をするプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *In_filename = "image_3.jpg";              //読み込む画像のファイル名
    char *Out_filename_1 = "Result_1_6_mono1.jpg";  //出力画像1のファイル名
    char *Out_filename_2 = "Result_1_6_mono2.jpg";  //出力画像2のファイル名
    int imageWidth = 0;                             //画像データの横幅の画素数
    int imageHeight = 0;                            //画像データの縦幅の画素数
    int imageBPP = 0;                               //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                            //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                           //出力画像データ領域のポインタ
    int imageSize = 0;                              //画像データのサイズ
    int calc_value;                                 //計算値保存用
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

    /*変換式1を用いてモノクロ画像化*/
    for(i=0;i<imageSize;i+=3){
        calc_value = (1.0/3.0)*(dataIn[i]+dataIn[i+1]+dataIn[i+2]);
        dataOut[i] = calc_value;
        dataOut[i+1] = calc_value;
        dataOut[i+2] = calc_value;
    }
    /*変換式1を用いてモノクロ画像化したものをファイルに出力*/
    writeJpeg(Out_filename_1,dataOut,imageWidth,imageHeight,imageBPP,100);

    /*変換式2を用いてモノクロ画像化*/
    for(i=0;i<imageSize;i+=3){
        calc_value = 0.299 * dataIn[i] + 0.587 * dataIn[i+1] + 0.114 * dataIn[i+2];
        dataOut[i] = calc_value;
        dataOut[i+1] = calc_value;
        dataOut[i+2] = calc_value;
    }
    /*変換式2を用いてモノクロ画像化したものをファイルに出力*/
    writeJpeg(Out_filename_2,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}