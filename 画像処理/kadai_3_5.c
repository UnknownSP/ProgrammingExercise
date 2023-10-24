/*
 *  19T1698W_kadai_3_5.c
 *  必須課題 3-5
 *  ΔE_abを計算し出力するプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

#define X_N 95.05
#define Y_N 100.0
#define Z_N 108.89

double function_t(double value);

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("command error \n");
        printf("Usage : ./kadai_3_5 [input filename] [output filename]\n");
        return 1;
    }

    char *originalIm_filename = argv[1];//原画像のファイル名
    char *outputIm_filename = argv[2];  //出力画像のファイル名
    int imageWidth = 0;                 //画像データの横幅の画素数
    int imageHeight = 0;                //画像データの縦幅の画素数
    int imageBPP = 0;                   //画像データの1画素あたりのバイト数
    BYTE *data_Image[2] = {NULL};       //画像データ領域のポインタ配列
    double *CIEXYZ[2] = {NULL};         //CIEXYZ計算用
    double *L[2] = {NULL};              //L計算用
    double *a[2] = {NULL};              //a計算用
    double *b[2] = {NULL};              //b計算用
    double delta_E_ab = 0.0;            //ΔE_ab計算用
    const double calc_array[3][3] = {   //CIEXYZ計算用行列
        {0.4124, 0.3576, 0.1805},
        {0.2126, 0.7152, 0.0722},
        {0.0193, 0.1192, 0.9504},
    };
    int imageSize = 0;                  //画像データのサイズ
    int h,i,j,k;

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(originalIm_filename,&data_Image[0]);
    /*原画像データの読み込みを行い、出力画像とサイズが違った場合終了*/
    if( imageSize != readJpeg(outputIm_filename,&data_Image[1])
        || imageWidth != readHeader(outputIm_filename,IMAGE_WIDTH)
        || imageHeight != readHeader(outputIm_filename,IMAGE_HEIGHT)
        || imageBPP != readHeader(outputIm_filename,IMAGE_BPP)){
        printf("ImageSize Error\n");
        return 1;
    }
    
    /*腹画像と出力画像のL,a,bを計算*/
    for(h=0;h<2;h++){
        CIEXYZ[h] = (double *)malloc(sizeof(double)*imageSize);
        for(i=0;i<imageSize;i+=3){
            for(j=0;j<3;j++){
                CIEXYZ[h][i+j] = 0.0;
                for(k=0;k<3;k++){
                    CIEXYZ[h][i+j] += calc_array[j][k]*pow((data_Image[h][i+k]/255.0),2.2);
                }
                CIEXYZ[h][i+j] *= 100.0;
            }
        }
    
        L[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
        a[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
        b[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
        for(i=0;i<imageWidth*imageHeight;i++){
            L[h][i] = 116.0*function_t(CIEXYZ[h][i*3+1]/Y_N) - 16.0;
            a[h][i] = 500.0*(function_t(CIEXYZ[h][i*3]/X_N)-function_t(CIEXYZ[h][i*3+1]/Y_N));
            b[h][i] = 200.0*(function_t(CIEXYZ[h][i*3+1]/Y_N)-function_t(CIEXYZ[h][i*3+2]/Z_N));
        }
    }

    /*腹画像と出力画像のL,a,bからΔE_abを計算し表示*/
    for(h=0;h<imageWidth*imageHeight;h++){
        delta_E_ab += sqrt(pow(L[0][h]-L[1][h],2.0)+pow(a[0][h]-a[1][h],2.0)+pow(b[0][h]-b[1][h],2.0));
    }
    delta_E_ab *= 1.0/(imageWidth*imageHeight);
    printf("ΔE_ab : %3.3f\n",delta_E_ab);

    /*画像データ領域の開放*/
    free(data_Image[0]);
    free(data_Image[1]);

    return 0;
}

double function_t(double value){
    if(value > 0.008856){
        return pow(value,1.0/3.0);
    }else{
        return (7.7871*value+16.0/116.0);
    }
}