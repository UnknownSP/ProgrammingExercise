/*
 *  19T1698W_kadai_1_3.c
 *  必須課題 1-3
 *  画像の統計量を計算するプログラム
 */

#include "stdio.h"
#include "math.h"
#include "jpegio.h"

int main(void){

    char *filename = "noise1_image.jpg";   //読み込む画像のファイル名
    int imageWidth = 0;             //画像データの横幅の画素数
    int imageHeight = 0;            //画像データの縦幅の画素数
    int imageBPP = 0;               //画像データの1画素あたりのバイト数
    BYTE *data = NULL;              //画像データ領域のポインタ
    int imageSize = 0;              //画像データのサイズ
    int R_value = 0;                //R値の合計値を保存する変数
    int G_value = 0;                //G値の合計値を保存する変数
    int B_value = 0;                //B値の合計値を保存する変数
    int pixel_num = 0;              //画素数を保存する変数
    double R_ave = 0.0;             //R値の平均値を保存する変数
    double G_ave = 0.0;             //G値の平均値を保存する変数
    double B_ave = 0.0;             //B値の平均値を保存する変数
    double R_SD = 0.0;              //R値の標準偏差を保存する変数
    double G_SD = 0.0;              //G値の標準偏差を保存する変数
    double B_SD = 0.0;              //B値の標準偏差を保存する変数

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

    /*RGB値を変数に加算*/
    for(int i=0;i<imageSize;i+=3){
        R_value += data[i];
        G_value += data[i+1];
        B_value += data[i+2];
    }

    /*画素数を計算し、RGB値の平均を変数に格納*/
    pixel_num = imageWidth*imageHeight;
    R_ave = (double)R_value/(double)pixel_num;
    G_ave = (double)G_value/(double)pixel_num;
    B_ave = (double)B_value/(double)pixel_num;

    /*RGBの標準偏差を計算*/
    for(int i=0;i<imageSize;i+=3){
        R_SD += (data[i]-R_ave)*(data[i]-R_ave);
        G_SD += (data[i+1]-G_ave)*(data[i+1]-G_ave);
        B_SD += (data[i+2]-B_ave)*(data[i+2]-B_ave);
    }
    R_SD = sqrt(R_SD/pixel_num);
    G_SD = sqrt(G_SD/pixel_num);
    B_SD = sqrt(B_SD/pixel_num);

    /*各データを表示*/
    printf("R average            : %3.5f\n",R_ave);
    printf("G average            : %3.5f\n",G_ave);
    printf("B average            : %3.5f\n",B_ave);
    printf("R standard deviation : %3.5f\n",R_SD);
    printf("G standard deviation : %3.5f\n",G_SD);
    printf("B standard deviation : %3.5f\n",B_SD);

    /*画像データ領域の開放*/
    free(data);

    return 0;
}