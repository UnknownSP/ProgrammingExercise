/*
 *  19T1698W_jiyukadai_1_2.c
 *  自由課題 1-2
 *  画像のヒストグラムを表示するプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *In_filename = "image_3.jpg";  //読み込む画像のファイル名
    int imageWidth = 0;                 //画像データの横幅の画素数
    int imageHeight = 0;                //画像データの縦幅の画素数
    int imageBPP = 0;                   //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                //入力画像データ領域のポインタ
    int imageSize = 0;                  //画像データのサイズ
    int calc_value;                     //計算値保存用
    int density[256] = {0};             //画素値保存用
    FILE *gnuplot;                      //gnuplotを呼びだす際に用いるポインタ
    int max_value = 0;                  //最も多い濃さの値の保存用
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

    /*変換式1を用いてモノクロ画像化した際の画素値を求め、その値に対応する配列の場所に出現回数を加算する*/
    for(i=0;i<imageSize;i+=3){
        calc_value = 0.299 * dataIn[i] + 0.587 * dataIn[i+1] + 0.114 * dataIn[i+2];
        density[calc_value]++;
    }
    /*画像データ領域の開放*/
    free(dataIn);

    /*画素値の出現回数の最大値を求める*/
    for(i=0;i<256;i++){
        if(max_value < density[i]){
            max_value = density[i];
        }
    }

    /*gnuplotを呼び出し、ヒストグラムを表示する*/
    gnuplot = popen("gnuplot -persist","w");
    fprintf(gnuplot,"set xrange [0:255]\n");
    fprintf(gnuplot,"set yrange [0:%d]\n",max_value);
    fprintf(gnuplot,"plot '-' with lines\n");
    for(i=0;i<256;i++){
        fprintf(gnuplot,"%d\t%d\n",i,density[i]);
    }
    fprintf(gnuplot,"e\n");

    /*領域の開放*/
    pclose(gnuplot);

    return 0;
}