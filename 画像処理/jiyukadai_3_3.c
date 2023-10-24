/*
 *  19T1698W_jiyukadai_3_3.c
 *  自由課題 3-3
 *  ΔE_abの色差マップを出力するプログラム
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
        printf("Usage : ./jiyukadai_3_3 [input filename] [output filename]\n");
        return 1;
    }

    char *originalIm_filename = argv[1];     //原画像のファイル名
    char *outputIm_filename = argv[2];    //出力画像のファイル名
    char *out_filename = "Result-j-3-3.jpg";    //
    int imageWidth = 0;                 //画像データの横幅の画素数
    int imageHeight = 0;                //画像データの縦幅の画素数
    int imageBPP = 0;                   //画像データの1画素あたりのバイト数
    BYTE *data_Image[2] = {NULL};       //画像データ領域のポインタ配列
    BYTE *data_out = NULL;           //出力色差マップ画像データ領域のポインタ
    double *CIEXYZ[2] = {NULL};
    double *L[2] = {NULL};
    double *a[2] = {NULL};
    double *b[2] = {NULL};
    double *delta_E_ab;
    const double calc_array[3][3] = {
        {0.4124, 0.3576, 0.1805},
        {0.2126, 0.7152, 0.0722},
        {0.0193, 0.1192, 0.9504},
    };
    int imageSize = 0;                  //画像データのサイズ
    double delta_E_ab_max,delta_E_ab_min;
    double cal_pixel_value;
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

    /*出力画像データ領域の確保*/
    data_out = (BYTE *)malloc(imageSize);

    /*出力色差マップ画像データ領域を確保できなかった場合終了する*/
    if(data_out == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }
    
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

    delta_E_ab = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
    for(h=0;h<imageWidth*imageHeight;h++){
        delta_E_ab[h] = sqrt(pow(L[0][h]-L[1][h],2.0)+pow(a[0][h]-a[1][h],2.0)+pow(b[0][h]-b[1][h],2.0));
        if(h==0){
            delta_E_ab_max = delta_E_ab[h];
            delta_E_ab_min = delta_E_ab[h];
        }
        if(delta_E_ab_max < delta_E_ab[h]){
            delta_E_ab_max = delta_E_ab[h];
        }
        if(delta_E_ab_min > delta_E_ab[h]){
            delta_E_ab_min = delta_E_ab[h];
        }
    }

    printf("ΔE_ab MIN = %3.3f [dB]\n",delta_E_ab_min);
    printf("ΔE_ab MAX = %3.3f [dB]\n",delta_E_ab_max);

    for(i=0;i<imageSize;i+=3){
        cal_pixel_value = (delta_E_ab[i/3]-delta_E_ab_min)/(delta_E_ab_max-delta_E_ab_min) * 255;
        if(cal_pixel_value > 255){
            cal_pixel_value = 255;
        }
        data_out[i] = cal_pixel_value;
        data_out[i+1] = cal_pixel_value;
        data_out[i+2] = cal_pixel_value;
    }

    writeJpeg(out_filename,data_out,imageWidth,imageHeight,imageBPP,100);

    /*画像データ領域の開放*/
    free(data_Image[0]);
    free(data_Image[1]);
    free(data_out);

    return 0;
}

double function_t(double value){
    if(value > 0.008856){
        return pow(value,1.0/3.0);
    }else{
        return (7.7871*value+16.0/116.0);
    }
}