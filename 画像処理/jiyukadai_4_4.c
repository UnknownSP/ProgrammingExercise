/*
 *  19T1698W_jiyukadai_4_4.c
 *  自由課題 4-4
 *  Cannyエッジ検出法を用いてエッジ検出を行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

/*ガウシアンフィルタのサイズ*/
#define FILTER_SIZE 5

/*閾値*/
#define H_THRESHOLD 1
#define L_THRESHOLD 0

int gaussian(int *neighborhood,int filter_size);
void sobel(int *neighborhood, BYTE *strength, BYTE *tan);
void outputJpeg(char *outfilename,BYTE **outdata, int imageWidth, int imageHeight, int imageBPP);

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("command error \n");
        printf("Usage : ./jiyukadai_4_4 [input filename]\n");
        return 1;
    }

    char *In_filename = argv[1];    //読み込む画像のファイル名
    char *Out_filename[6] = {
        "result-j-4-4-mono.jpg",
        "result-j-4-4-gauss.jpg",
        "result-j-4-4-sobel.jpg",
        "result-j-4-4-thinning.jpg",
        "result-j-4-4-threshold.jpg",
        "result-j-4-4-Out.jpg",
    };                              //出力する画像のファイル名
    int imageWidth = 0;             //画像データの横幅の画素数
    int imageHeight = 0;            //画像データの縦幅の画素数
    int imageBPP = 0;               //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;            //入力画像データ領域のポインタ
    BYTE **input_data = NULL;       //入力データを変換し保存する配列
    BYTE **cal_data = NULL;         //計算データを保存する配列
    BYTE **sobel_data[2] = {NULL};  //Sobelフィルタによる計算データを保存する配列
    BYTE **out_data = NULL;         //出力データを保存する配列
    int *neihgborhood = NULL;       //nxnの画素を保存する配列
    int imageSize = 0;              //画像データのサイズ
    int c,i,j,x,y;
    double cal_value;               //モノクロ変換計算用
    int temp;                       //計算用
    bool adjacent = false;

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

    /*各画像データ領域を確保*/
    /*入力画像データを、RGB、x、y で分けて3次元配列とし、モノクロ画像に変換して保存*/
    neihgborhood = (int *)malloc(sizeof(int)*FILTER_SIZE*FILTER_SIZE);
    input_data = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
    cal_data = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
    sobel_data[0] = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
    sobel_data[1] = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
    out_data = (BYTE **)malloc(sizeof(BYTE *)*imageWidth);
    for(i=0;i<imageWidth;i++){
        input_data[i] = (BYTE *)malloc(sizeof(BYTE)*imageHeight);
        cal_data[i] = (BYTE *)malloc(sizeof(BYTE *)*imageHeight);
        sobel_data[0][i] = (BYTE *)malloc(sizeof(BYTE *)*imageHeight);
        sobel_data[1][i] = (BYTE *)malloc(sizeof(BYTE *)*imageHeight);
        out_data[i] = (BYTE *)malloc(sizeof(BYTE *)*imageHeight);
        for(j=0;j<imageHeight;j++){
            cal_value = 0.0;
            cal_value += dataIn[(i+j*imageWidth)*3]*0.299;
            cal_value += dataIn[(i+j*imageWidth)*3+1]*0.587;
            cal_value += dataIn[(i+j*imageWidth)*3+2]*0.114;
            input_data[i][j] = (int)cal_value;
            cal_data[i][j] = 0;
            sobel_data[0][i][j] = 0;
            sobel_data[1][i][j] = 0;
            out_data[i][j] = 0;
        }
    }

    /*モノクロに変換した画像を出力*/
    outputJpeg(Out_filename[0],input_data,imageWidth,imageHeight,1);

    /*各画素、各色ごとにガウシアンフィルタを行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            for(i=-((FILTER_SIZE-1)/2);i<((FILTER_SIZE-1)/2)+1;i++){
                for(j=-((FILTER_SIZE-1)/2);j<((FILTER_SIZE-1)/2)+1;j++){
                    if(x+i < 0 || x+i >= imageWidth || y+j < 0 || y+j >= imageHeight){
                        temp = input_data[x][y];
                    }else{
                        temp = input_data[x+i][y+j];
                    }
                    neihgborhood[(i+(FILTER_SIZE-1)/2)+(j+(FILTER_SIZE-1)/2)*FILTER_SIZE] = temp;
                }
            }
            cal_data[x][y] = gaussian(neihgborhood,FILTER_SIZE);
        }
    }

    /*ガウシアンフィルタを行った画像を出力*/
    outputJpeg(Out_filename[1],cal_data,imageWidth,imageHeight,1);
    
    /*各画素、各色ごとにソーべルフィルタを行い、エッジ強度と勾配方向を計算*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            for(i=-1;i<2;i++){
                for(j=-1;j<2;j++){
                    if(x+i < 0 || x+i >= imageWidth || y+j < 0 || y+j >= imageHeight){
                        temp = cal_data[x][y];
                    }else{
                        temp = cal_data[x+i][y+j];
                    }
                    neihgborhood[(i+1)+(j+1)*3] = temp;
                }
            }
            sobel(neihgborhood,&sobel_data[0][x][y],&sobel_data[1][x][y]);
        }
    }

    /*ソーべルフィルタを行った画像を出力*/
    outputJpeg(Out_filename[2],sobel_data[0],imageWidth,imageHeight,1);

    /*エッジの細線化を行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            cal_data[x][y] = sobel_data[0][x][y];
            switch (sobel_data[1][x][y]){
            case 0:
                if(x-1 < 0){
                    if(sobel_data[0][x][y]<sobel_data[0][x+1][y]){
                        cal_data[x][y] = 0;
                    }
                }else if(x+1 >= imageWidth){
                    if(sobel_data[0][x][y]<sobel_data[0][x-1][y]){
                        cal_data[x][y] = 0;
                    }
                }else{
                    if(sobel_data[0][x][y]<sobel_data[0][x-1][y] || sobel_data[0][x][y]<sobel_data[0][x+1][y]){
                        cal_data[x][y] = 0;
                    }
                }
                break;
            case 45:
                if((x-1 < 0 || y+1 >= imageHeight) && (x+1 >= imageWidth || y-1 < 0)){
                    break;
                }else if(x-1 < 0 || y+1 >= imageHeight){
                    if(sobel_data[0][x][y]<sobel_data[0][x+1][y-1]){
                        cal_data[x][y] = 0;
                    }
                }else if(x+1 >= imageWidth || y-1 < 0){
                    if(sobel_data[0][x][y]<sobel_data[0][x-1][y+1]){
                        cal_data[x][y] = 0;
                    }
                }else{
                    if(sobel_data[0][x][y]<sobel_data[0][x-1][y+1] || sobel_data[0][x][y]<sobel_data[0][x+1][y-1]){
                        cal_data[x][y] = 0;
                    }
                }
                break;
            case 90:
                if(y-1 < 0){
                    if(sobel_data[0][x][y]<sobel_data[0][x][y+1]){
                        cal_data[x][y] = 0;
                    }
                }else if(y+1 >= imageHeight){
                    if(sobel_data[0][x][y]<sobel_data[0][x][y-1]){
                        cal_data[x][y] = 0;
                    }
                }else{
                    if(sobel_data[0][x][y]<sobel_data[0][x][y-1] || sobel_data[0][x][y]<sobel_data[0][x][y+1]){
                        cal_data[x][y] = 0;
                    }
                }
                break;
            case 135:
                if((x-1 < 0 || y-1 < 0) && (x+1 >= imageWidth || y+1 >= imageHeight)){
                    break;
                }else if(x-1 < 0 || y-1 < 0){
                    if(sobel_data[0][x][y]<sobel_data[0][x+1][y+1]){
                        cal_data[x][y] = 0;
                    }
                }else if(x+1 >= imageWidth || y+1 >= imageHeight){
                    if(sobel_data[0][x][y]<sobel_data[0][x-1][y-1]){
                        cal_data[x][y] = 0;
                    }
                }else{
                    if(sobel_data[0][x][y]<sobel_data[0][x+1][y+1] || sobel_data[0][x][y]<sobel_data[0][x-1][y-1]){
                        cal_data[x][y] = 0;
                    }
                }
                break;
            default:
                printf("%d\nERROR\n",sobel_data[1][x][y]);
                return 1;
            }
        }
    }

    /*細線化を行った画像を出力*/
    outputJpeg(Out_filename[3],cal_data,imageWidth,imageHeight,1);

    /*閾値処理を行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            if(cal_data[x][y] >= H_THRESHOLD){
                cal_data[x][y] = 255;
            }else if(cal_data[x][y] < L_THRESHOLD){
                cal_data[x][y] = 0;
            }else{
                cal_data[x][y] = 1;
            }
        }
    }

    /*閾値処理を行った画像を出力*/
    outputJpeg(Out_filename[4],cal_data,imageWidth,imageHeight,1);

    /*ヒステリシス処理を行う*/
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            if(cal_data[x][y] == 1){
                for(i=-1;i<2;i++){
                    for(j=-1;j<2;j++){
                        if(x+i < 0 || x+i >= imageWidth || y+j < 0 || y+j >= imageHeight || (i==0&&j==0)){
                            continue;
                        }else{
                            if(cal_data[x+i][y+j] == 255){
                                out_data[x][y] = 255;
                                adjacent = true;
                            }
                        }
                    }
                }
                if(!adjacent){
                    out_data[x][y] = 0;
                }
                adjacent = false;
            }else{
                out_data[x][y] = cal_data[x][y];
            }
        }
    }

    /*ヒステリシス処理を行った画像を出力*/
    outputJpeg(Out_filename[5],out_data,imageWidth,imageHeight,1);

    /*画像データ領域の開放*/
    free(dataIn);
    for(i=0;i<imageWidth;i++){
        free(input_data[i]);
        free(cal_data[i]);
        free(sobel_data[0][i]);
        free(sobel_data[1][i]);
    }
    free(input_data);
    free(cal_data);
    free(sobel_data[0]);
    free(sobel_data[1]);
    free(out_data);

    return 0;
}

/*ガウシアンフィルタを行う際に用いる関数*/
int gaussian(int *neighborhood,int filter_size){
    double cal_value = 0;
    const int cal_array_3[9] = {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };
    const int cal_array_5[25] = {
        1,  4,  6,  4,  1,
        4, 16, 24, 16,  4,
        6, 24, 36, 24,  6,
        4, 16, 24, 16,  4,
        1,  4,  6,  4,  1,
    };
    switch (filter_size)
    {
    case 3:
        for(int i=0;i<filter_size*filter_size;i++){
            cal_value += cal_array_3[i]*neighborhood[i];
        }
        cal_value /= 16.0;
        break;
    case 5:
        for(int i=0;i<filter_size*filter_size;i++){
            cal_value += cal_array_5[i]*neighborhood[i];
        }
        cal_value /= 256.0;
        break;

    default:
        printf("FILTER SIZE ERROR\n");
        return 1;
    }

    return (int)cal_value;
}

/*ソーべルフィルタを行う際に用いる関数*/
void sobel(int *neighborhood, BYTE *strength, BYTE *tan){
    double fx = 0.0;
    double fy = 0.0;
    double cal_tan = 0.0;
    const int sobel_Horizontal[9] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    const int sobel_Vertical[9] = {
        -1,-2,-1,
        0, 0, 0,
        1, 2, 1
    };
    for(int i=0;i<9;i++){
        fx += sobel_Horizontal[i]*neighborhood[i];
        fy += sobel_Vertical[i]*neighborhood[i];
    }
    *strength = (int)sqrt(fx*fx + fy*fy);
    cal_tan = fy/fx;
    if(cal_tan > -0.4142 && cal_tan <= 0.4142){
        *tan = 0;
    }else if(cal_tan > 0.4142 && cal_tan < 2.4142){
        *tan = 45;
    }else if(cal_tan < -2.4142 || cal_tan > 2.4142){
        *tan = 90;
    }else if(cal_tan > -2.4142 && cal_tan <= -0.4142 ){
        *tan = 135;
    }
}

/*[x][y]の2次元配列からJPEG画像を出力する関数*/
void outputJpeg(char *outfilename,BYTE **outdata, int imageWidth, int imageHeight, int imageBPP){
    int x,y;
    BYTE *dataOut = NULL;

    dataOut = (BYTE *)malloc(sizeof(BYTE)*imageWidth*imageHeight*imageBPP);
    for(x=0;x<imageWidth;x++){
        for(y=0;y<imageHeight;y++){
            dataOut[x+y*imageWidth] = outdata[x][y];
        }
    }
    writeJpeg(outfilename,dataOut,imageWidth,imageHeight,imageBPP,100);

    free(dataOut);
}
