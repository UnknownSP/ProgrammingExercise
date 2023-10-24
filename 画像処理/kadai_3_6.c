/*
 *  19T1698W_kadai_3_6.c
 *  必須課題 3-6
 *  横軸に画質、縦軸にΔE_abのグラフを表示するプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "math.h"

#define X_N 95.05
#define Y_N 100.0
#define Z_N 108.89

double function_t(double value);

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
    BYTE *data_Image[2] = {NULL};                   //画像データ領域のポインタ配列
    double *CIEXYZ[2] = {NULL};                     //CIEXYZ計算用
    double *L[2] = {NULL};                          //L計算用
    double *a[2] = {NULL};                          //a計算用
    double *b[2] = {NULL};                          //b計算用
    double delta_E_ab[6] = {0.0};                   //ΔE_ab計算用
    const double calc_array[3][3] = {               //CIEXYZ計算用行列
        {0.4124, 0.3576, 0.1805},
        {0.2126, 0.7152, 0.0722},
        {0.0193, 0.1192, 0.9504},
    };
    int imageSize = 0;                  //画像データのサイズ
    FILE *gnuplot;                      //gnuplotを呼びだす際に用いるポインタ
    int g,h,i,j,k;

    /*画像情報の読み込み*/
    imageWidth = readHeader(originalIm_filename,IMAGE_WIDTH);
    imageHeight = readHeader(originalIm_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(originalIm_filename,IMAGE_BPP);
    /*画像データの読み込み*/
    imageSize = readJpeg(originalIm_filename,&data_Image[0]);
    
    /*画質数別にΔE_abを計算*/
    for(g=0;g<6;g++){

        if( imageSize != readJpeg(outputIm_filename[g],&data_Image[1])
            || imageWidth != readHeader(outputIm_filename[g],IMAGE_WIDTH)
            || imageHeight != readHeader(outputIm_filename[g],IMAGE_HEIGHT)
            || imageBPP != readHeader(outputIm_filename[g],IMAGE_BPP)){
            printf("ImageSize Error\n");
            return 1;
        }

        for(h=0;h<2;h++){
            CIEXYZ[h] = NULL;
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

            L[h] = NULL;
            a[h] = NULL;
            b[h] = NULL;
            L[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
            a[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
            b[h] = (double *)malloc(sizeof(double)*imageWidth*imageHeight);
            for(i=0;i<imageWidth*imageHeight;i++){
                L[h][i] = 116.0*function_t(CIEXYZ[h][i*3+1]/Y_N) - 16.0;
                a[h][i] = 500.0*(function_t(CIEXYZ[h][i*3]/X_N)-function_t(CIEXYZ[h][i*3+1]/Y_N));
                b[h][i] = 200.0*(function_t(CIEXYZ[h][i*3+1]/Y_N)-function_t(CIEXYZ[h][i*3+2]/Z_N));
            }
        }
        for(h=0;h<imageWidth*imageHeight;h++){
            delta_E_ab[g] += sqrt(pow(L[0][h]-L[1][h],2.0)+pow(a[0][h]-a[1][h],2.0)+pow(b[0][h]-b[1][h],2.0));
        }
        delta_E_ab[g] *= 1.0/(imageWidth*imageHeight);

        printf("ΔE_ab Quality%3d : %3.3f\n",quality_value[g],delta_E_ab[g]);
    }

    /*画像データ領域の開放*/
    free(data_Image[0]);
    free(data_Image[1]);

    /*gnuplotを呼び出し、グラフを表示*/
    gnuplot = popen("gnuplot -persist","w");
    fprintf(gnuplot,"set xrange [0:80]\n");
    fprintf(gnuplot,"set yrange [0:8]\n");
    fprintf(gnuplot,"plot '-' with lines\n");
    for(i=0;i<6;i++){
        fprintf(gnuplot,"%d\t%f\n",quality_value[i],delta_E_ab[i]);
    }
    fprintf(gnuplot,"e\n");

    /*領域の開放*/
    pclose(gnuplot);

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