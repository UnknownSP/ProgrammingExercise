/*
 *  19T1698W_kadai_3_3.c
 *  必須課題 3-4
 *  横軸に画質、縦軸にPSNRのグラフを表示するプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

int main(void){

    char *originalIm_filename = "quality_100.jpg";  //原画像のファイル名
    char *outputIm_filename[6] = {
        "quality_1.jpg","quality_5.jpg",
        "quality_10.jpg","quality_20.jpg",
        "quality_40.jpg","quality_80.jpg"
    };                                              //出力画像のファイル名配列
    int quality_value[6] = {1,5,10,20,40,80};       //出力画像の画素数配列
    int imageWidth = 0;                             //画像データの横幅の画素数
    int imageHeight = 0;                            //画像データの縦幅の画素数
    int imageBPP = 0;                               //画像データの1画素あたりのバイト数
    BYTE *data_originalIm = NULL;                   //原画像データ領域のポインタ
    BYTE *data_outputIm = NULL;                     //出力画像データ領域のポインタ
    int imageSize = 0;                              //画像データのサイズ
    double MSE = 0.0;                               //MSE計算用
    double PSNR[6];                                 //PSNR計算用
    FILE *gnuplot;                                  //gnuplotを呼びだす際に用いるポインタ
    int i,j;

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(originalIm_filename,&data_originalIm);
    /*画像データの読み込みに失敗した場合終了する*/
    if(imageSize == 0){
        printf("Read Error\n");
        return 1;
    }

    /*画質数別にPSNRを計算*/
    for(i=0;i<6;i++){
        data_outputIm = NULL;
        readJpeg(outputIm_filename[i],&data_outputIm);
        MSE = 0.0;
        for(j=0;j<imageSize;j+=3){
            MSE += pow(data_originalIm[j]-data_outputIm[j],2);
            MSE += pow(data_originalIm[j+1]-data_outputIm[j+1],2);
            MSE += pow(data_originalIm[j+2]-data_outputIm[j+2],2);
        }
        MSE *= 1.0/(3.0*imageWidth*imageHeight);
        PSNR[i] = 10.0 * log10(255*255/MSE);
        PSNR[i] = MSE;
        printf("MSE  Quality%3d : %3.3f\n",quality_value[i],MSE);
        printf("PSNR Quality%3d : %3.3f\n",quality_value[i],PSNR[i]);
    }

    /*画像データ領域の開放*/
    free(data_originalIm);
    free(data_outputIm);

    /*gnuplotを呼び出し、グラフを表示*/
    gnuplot = popen("gnuplot -persist","w");
    fprintf(gnuplot,"set xrange [0:80]\n");
    //fprintf(gnuplot,"set yrange [20:45]\n");
    fprintf(gnuplot,"plot '-' with lines\n");
    for(i=0;i<6;i++){
        fprintf(gnuplot,"%d\t%f\n",quality_value[i],PSNR[i]);
    }
    fprintf(gnuplot,"e\n");

    /*領域の開放*/
    pclose(gnuplot);

    return 0;
}