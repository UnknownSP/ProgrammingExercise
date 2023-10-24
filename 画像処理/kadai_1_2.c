/*
 *  19T1698W_kadai_1_2.c
 *  必須課題 1-2
 *  指定された座標の画素値を表示するプログラム
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
    int x,y;                        //指定座標
    int target_addr = 0;            //表示する画素の場所

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

    while(1){
        /*指定座標の値(x,y)の読み込み*/
        printf("If you want to quit, please enter (x,y)=(-1,-1)\n\n");
        printf("x coordinate => ");
        scanf("%d",&x);
        printf("y coordinate => ");
        scanf("%d",&y);

        /*指定座標が範囲外だった場合終了*/
        if(x >= imageWidth || y >= imageHeight || x < 0 || y < 0){
            if(x == -1 && y== -1){
                break;
            }
            printf("Error : Over range\n");
            printf("x range : 0 - %d\n",imageWidth-1);
            printf("y range : 0 - %d\n",imageHeight-1);
            return 1;
        }

        /*指定座標から、指定座標の画素値が入っている配列内の場所を計算*/
        target_addr = (x + imageWidth * y) * imageBPP;
        printf("\n(x,y) = (%4d,%4d)\n",x,y);
        printf("R : %3d\n",data[target_addr]);
        printf("G : %3d\n",data[target_addr+1]);
        printf("B : %3d\n\n",data[target_addr+2]);
    }

    /*画像データ領域の開放*/
    free(data);

    return 0;
}