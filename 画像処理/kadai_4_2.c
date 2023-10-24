/*
 *  19T1698W_kadai_4_2.c
 *  必須課題 4-2
 *  5x5のメディアンフィルタを行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "stdlib.h"

int median(int *neighborhood,int filter_size);
int compare_int(const void *a, const void *b);

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("command error \n");
        printf("Usage : ./kadai_4_2 [input filename]\n");
        return 1;
    }

    char *In_filename = argv[1];            //読み込む画像のファイル名
    char *Out_filename = "result-4-2.jpg";  //出力する画像のファイル名
    int imageWidth = 0;                     //画像データの横幅の画素数
    int imageHeight = 0;                    //画像データの縦幅の画素数
    int imageBPP = 0;                       //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                    //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                   //出力画像データ領域のポインタ
    BYTE **input_data[3] = {NULL};          //入力データを変換し保存する配列
    int *neihgborhood[3] = {NULL};          //5x5の画素を保存する配列
    int imageSize = 0;                      //画像データのサイズ
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
    dataOut = (BYTE *)malloc(imageSize);

    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*入力画像データを、RGB、x、y で分けて3次元配列として保存*/
    for(c=0;c<3;c++){
        neihgborhood[c] = (int *)malloc(sizeof(int)*25);
        input_data[c] = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
        for(i=0;i<imageWidth;i++){
            input_data[c][i] = (BYTE *)malloc(sizeof(BYTE)*imageHeight);
            for(j=0;j<imageHeight;j++){
                input_data[c][i][j] = dataIn[(i+j*imageWidth)*3+c];
            }
        }
    }

    /*各画素、各色ごとに5x5のメディアンフィルタを行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            for(c=0;c<3;c++){
                for(i=-2;i<3;i++){
                    for(j=-2;j<3;j++){
                        if(x+i < 0 || x+i >= imageWidth || y+j < 0 || y+j >= imageHeight){
                            neihgborhood[c][(i+2)+(j+2)*5] = 0;
                        }else{
                            neihgborhood[c][(i+2)+(j+2)*5] = input_data[c][x+i][y+j];
                        }
                    }
                }
                dataOut[(x+y*imageWidth)*3+c] = median(neihgborhood[c],5);
            }
        }
    }

    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);

    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);
    for(c=0;c<3;c++){
        for(i=0;i<imageWidth;i++){
            free(input_data[c][i]);
        }
    }

    return 0;
}

/*メディアンフィルタを行う際に用いる関数*/
int median(int *neighborhood,int filter_size){
    qsort(neighborhood,filter_size*filter_size,sizeof(int),compare_int);
    return neighborhood[(filter_size*filter_size-1)/2];
}

/*ソートをする際に用いる関数*/
int compare_int(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}