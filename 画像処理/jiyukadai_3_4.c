/*
 *  19T1698W_jiyukadai_3_4.c
 *  自由課題 3-4
 *  L,a,bの値を計算しその値を画素値としたモノクロ画像を出力するプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

#define X_N 95.05
#define Y_N 100.0
#define Z_N 108.89

double function_t(double value);

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("command error \n");
        printf("Usage : ./jiyukadai_3_4 [input filename]\n");
        return 1;
    }

    char *originalIm_filename = argv[1];    //入力画像のファイル名
    char *outputIm_filename[3] = {
        "result-j-3-4-L.jpg","result-j-3-4-a.jpg","result-j-3-4-b.jpg"
    };                                      //出力画像のファイル名配列
    int imageWidth = 0;                     //画像データの横幅の画素数
    int imageHeight = 0;                    //画像データの縦幅の画素数
    int imageBPP = 0;                       //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                    //入力画像データ領域のポインタ配列
    BYTE *dataOut = NULL;                   //出力画像データ領域のポインタ
    double *CIEXYZ = NULL;                  //CIEXYZ計算用
    double *CIELAB[3] = {NULL};             //L,a,b計算用
    const double calc_array[3][3] = {       //CIEXYZ計算用行列
        {0.4124, 0.3576, 0.1805},
        {0.2126, 0.7152, 0.0722},
        {0.0193, 0.1192, 0.9504},
    };
    int imageSize = 0;                      //画像データのサイズ
    double CIELAB_max[3],CIELAB_min[3];     //L,a,bの最大値最小値計算用
    double cal_pixel_value;                 //画素値計算用
    int h,i,j,k;

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(originalIm_filename,&dataIn);

    /*出力画像データ領域の確保*/
    dataOut = (BYTE *)malloc(imageSize);

    /*出力色差マップ画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }
    
    /*CIEXYZを画素毎に計算*/
    CIEXYZ = (double *)malloc(sizeof(double)*imageSize);
    for(i=0;i<imageSize;i+=3){
        for(j=0;j<3;j++){
            CIEXYZ[i+j] = 0.0;
            for(k=0;k<3;k++){
                CIEXYZ[i+j] += calc_array[j][k]*pow((dataIn[i+k]/255.0),2.2);
            }
            CIEXYZ[i+j] *= 100.0;
        }
    }

    /*L,a,bを画素毎に計算し最大値と最小値を求める*/
    for(i=0;i<3;i++){
        CIELAB[i] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
    }
    for(i=0;i<imageWidth*imageHeight;i++){
        CIELAB[0][i] = 116.0*function_t(CIEXYZ[i*3+1]/Y_N) - 16.0;
        CIELAB[1][i] = 500.0*(function_t(CIEXYZ[i*3]/X_N)-function_t(CIEXYZ[i*3+1]/Y_N));
        CIELAB[2][i] = 200.0*(function_t(CIEXYZ[i*3+1]/Y_N)-function_t(CIEXYZ[i*3+2]/Z_N));

        for(j=0;j<3;j++){
            if(i==0){
                CIELAB_max[j] = CIELAB[j][0];
                CIELAB_min[j] = CIELAB[j][0];
            }
            if(CIELAB_max[j] < CIELAB[j][i]){
                CIELAB_max[j] = CIELAB[j][i];
            }
            if(CIELAB_min[j] > CIELAB[j][i]){
                CIELAB_min[j] = CIELAB[j][i];
            }
        }
    }

    /*L,a,bが最小値のとき0、最大値のとき255として画素値を計算し各画像を出力*/
    for(i=0;i<3;i++){
        for(j=0;j<imageSize;j+=3){
            cal_pixel_value = (CIELAB[i][j/3]-CIELAB_min[i])/(CIELAB_max[i]-CIELAB_min[i]) * 255;
            if(cal_pixel_value > 255){
                cal_pixel_value = 255;
            }
            dataOut[j] = cal_pixel_value;
            dataOut[j+1] = cal_pixel_value;
            dataOut[j+2] = cal_pixel_value;
        }
        writeJpeg(outputIm_filename[i],dataOut,imageWidth,imageHeight,imageBPP,100);
    }

    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}

/*f(t)を計算する関数*/
double function_t(double value){
    if(value > 0.008856){
        return pow(value,1.0/3.0);
    }else{
        return (7.7871*value+16.0/116.0);
    }
}