/*
 *  19T1698W_kadai_1_1.c
 *  必須課題 1-1
 *  画像情報を表示するプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *filename = "image.jpg";   //読み込む画像のファイル名
    int imageWidth = 0;             //画像データの横幅の画素数
    int imageHeight = 0;            //画像データの縦幅の画素数
    int imageBPP = 0;               //画像データの1画素あたりのバイト数
    BYTE *data = NULL;              //画像データ領域のポインタ
    int imageSize = 0;              //画像データのサイズ
    int calc_imageSize = 0;         //画像情報より計算した画像データのサイズ

    /*画像情報の読み込み*/
    imageWidth = readHeader(filename,IMAGE_WIDTH);
    imageHeight = readHeader(filename,IMAGE_HEIGHT);
    imageBPP = readHeader(filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(filename,&data);

    /*画像データの読み込みに失敗した場合終了する*/
    if(imageSize == 0){
        printf("Read Error\n");
        return 1;
    }

    /*画像情報より画像サイズを計算*/
    calc_imageSize = imageWidth * imageHeight * imageBPP;

    /*画像情報の表示*/
    printf("Width          : %8d\n",imageWidth);
    printf("Height         : %8d\n",imageHeight);
    printf("BPP            : %8d\n",imageBPP);
    printf("imageSize      : %8d\n",imageSize);
    printf("calc_imageSize : %8d\n",calc_imageSize);

    /*画像データ領域の開放*/
    free(data);

    return 0;
}