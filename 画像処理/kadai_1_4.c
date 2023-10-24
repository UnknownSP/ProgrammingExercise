/*
 *  19T1698W_kadai_1_4.c
 *  必須課題 1-4
 *  画像データコピーと出力をするプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *In_filename = "image_3.jpg";          //読み込む画像のファイル名
    char *Out_filename = "nulti_test.jpg";  //出力する画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                        //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ

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

    /*入力画像データを出力画像データに保存*/
    for(int i=0;i<imageSize;i++){
        //dataOut[i] = dataIn[i];
        dataOut[i] = 200;
    }

    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,80);

    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}