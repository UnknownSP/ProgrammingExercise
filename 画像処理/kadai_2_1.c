/*
 *  19T1698W_kadai_2_1.c
 *  必須課題 2-1
 *  画像合成の基礎を行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"

#define PERSON_COEFF 0.25    //人物画像のRGB値の係数
#define BACK_COEFF 0.75      //背景画像のRGB値の係数

int main(void){

    char *person_filename = "person.jpg";   //読み込む人物画像のファイル名
    char *back_filename = "background.jpg"; //読み込む背景画像のファイル名
    char *Out_filename = "result2-1_25_75.jpg";   //出力画像のファイル名
    int imageWidth = 0;                     //画像データの横幅の画素数
    int imageHeight = 0;                    //画像データの縦幅の画素数
    int imageBPP = 0;                       //画像データの1画素あたりのバイト数
    BYTE *dataIn_person = NULL;             //入力人物画像データ領域のポインタ
    BYTE *dataIn_back = NULL;               //入力背景画像データ領域のポインタ
    BYTE *dataOut = NULL;                   //出力画像データ領域のポインタ
    int imageSize = 0;                      //画像データのサイズ
    int i;

    /*画像情報の読み込み*/
    imageWidth = readHeader(person_filename,IMAGE_WIDTH);
    imageHeight = readHeader(person_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(person_filename,IMAGE_BPP);
    /*人物画像データの読み込み*/
    imageSize = readJpeg(person_filename,&dataIn_person);
    /*背景画像データの読み込みを行い、人物画像とサイズが違った場合終了*/
    if(imageSize != readJpeg(back_filename,&dataIn_back)){
        printf("ImageSize Error\n");
        return 1;
    }

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
    /*人物画像と背景画像にそれぞれRGB値の係数をかけて足し合わせたものを、出力画像データ領域に保存*/
    for(i=0;i<imageSize;i+=3){
        dataOut[i] = dataIn_back[i] * BACK_COEFF + dataIn_person[i] * PERSON_COEFF;
        dataOut[i+1] = dataIn_back[i+1] * BACK_COEFF + dataIn_person[i+1] * PERSON_COEFF;
        dataOut[i+2] = dataIn_back[i+2] * BACK_COEFF + dataIn_person[i+2] * PERSON_COEFF;
    }
    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn_person);
    free(dataIn_back);
    free(dataOut);

    return 0;
}