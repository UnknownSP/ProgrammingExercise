/*
 *  19T1698W_jiyukadai_4_2.c
 *  自由課題 4-2
 *  3x3の移動平均フィルタ、ガウシアンフィルタを行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "stdlib.h"

#define FILTER_SIZE 5

int median(int *neighborhood,int filter_size);
int moving_average(int *neighborhood,int filter_size);
int gaussian(int *neighborhood,int filter_size);
int compare_int(const void *a, const void *b);

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("command error \n");
        printf("Usage : ./jiyukadai_4_2 [input filename]\n");
        return 1;
    }

    char *In_filename = argv[1];    //読み込む画像のファイル名
    char *Out_filename[2] = {
        "result-j-4-2-ma.jpg","result-j-4-2-gau.jpg"
    };                              //出力する画像のファイル名
    int imageWidth = 0;             //画像データの横幅の画素数
    int imageHeight = 0;            //画像データの縦幅の画素数
    int imageBPP = 0;               //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;            //入力画像データ領域のポインタ
    BYTE *dataOut[2] = {NULL};      //出力画像データ領域のポインタ
    BYTE **input_data[3] = {NULL};  //入力データを変換し保存する配列
    int *neihgborhood[3] = {NULL};  //nxnの画素を保存する配列
    int imageSize = 0;              //画像データのサイズ
    int c,i,j,x,y;

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

    /*出力画像データ領域の確保*/
    dataOut[0] = (BYTE *)malloc(sizeof(BYTE)*imageSize);
    dataOut[1] = (BYTE *)malloc(sizeof(BYTE)*imageSize);

    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut[0] == NULL || dataOut[1] == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*入力画像データを、RGB、x、y で分けて3次元配列として保存*/
    for(c=0;c<3;c++){
        neihgborhood[c] = (int *)malloc(sizeof(int)*FILTER_SIZE*FILTER_SIZE);
        input_data[c] = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
        for(i=0;i<imageWidth;i++){
            input_data[c][i] = (BYTE *)malloc(sizeof(BYTE)*imageHeight);
            for(j=0;j<imageHeight;j++){
                input_data[c][i][j] = dataIn[(i+j*imageWidth)*3+c];
            }
        }
    }

    /*各画素、各色ごとにFILTER_SIZExFILTER_SIZEの移動平均フィルタ、ガウシアンフィルタを行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            for(c=0;c<3;c++){
                for(i=-((FILTER_SIZE-1)/2);i<((FILTER_SIZE-1)/2)+1;i++){
                    for(j=-((FILTER_SIZE-1)/2);j<((FILTER_SIZE-1)/2)+1;j++){
                        if(x+i < 0 || x+i >= imageWidth || y+j < 0 || y+j >= imageHeight){
                            neihgborhood[c][(i+(FILTER_SIZE-1)/2)+(j+(FILTER_SIZE-1)/2)*FILTER_SIZE] = 0;
                        }else{
                            neihgborhood[c][(i+(FILTER_SIZE-1)/2)+(j+(FILTER_SIZE-1)/2)*FILTER_SIZE] = input_data[c][x+i][y+j];
                        }
                    }
                }
                dataOut[0][(x+y*imageWidth)*3+c] = moving_average(neihgborhood[c],FILTER_SIZE);
                dataOut[1][(x+y*imageWidth)*3+c] = gaussian(neihgborhood[c],FILTER_SIZE);
            }
        }
    }

    /*ファイルに出力*/
    writeJpeg(Out_filename[0],dataOut[0],imageWidth,imageHeight,imageBPP,100);
    writeJpeg(Out_filename[1],dataOut[1],imageWidth,imageHeight,imageBPP,100);

    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut[0]);
    free(dataOut[1]);
    for(c=0;c<3;c++){
        for(i=0;i<imageWidth;i++){
            free(input_data[c][i]);
        }
    }

    return 0;
}

/*移動平均フィルタを行う際に用いる関数*/
int moving_average(int *neighborhood,int filter_size){
    double cal_value = 0;
    for(int i=0;i<filter_size*filter_size;i++){
        cal_value += neighborhood[i];
    }
    cal_value /= filter_size*filter_size;
    return (int)cal_value;
}

/*ガウシアンフィルタを行う際に用いる関数*/
int gaussian(int *neighborhood,int filter_size){
    double cal_value = 0;
    const int cal_array[25] = {
        1,  4,  6,  4,  1,
        4, 16, 24, 16,  4,
        6, 24, 36, 24,  6,
        4, 16, 24, 16,  4,
        1,  4,  6,  4,  1,
    };
    for(int i=0;i<filter_size*filter_size;i++){
        cal_value += cal_array[i]*neighborhood[i];
    }
    cal_value /= 256.0;
    return (int)cal_value;
}