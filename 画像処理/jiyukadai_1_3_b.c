/*
 *  19T1698W_jiyukadai_1_3_b.c
 *  自由課題 1-3 (その2)
 *  画像の色補正・色変換を自動で行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *In_filename = "Input_j_1_3.jpg";      //読み込む画像のファイル名
    char *Out_filename = "Result_j_1_3_b.jpg";  //出力画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                        //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ
    int max_whitecolor = 0;                     //最も白に近い画素値の保存用
    int target_R = 0;                           //最も白に近い画素値におけるR成分の保存用
    int target_G = 0;                           //最も白に近い画素値におけるG成分の保存用
    int target_B = 0;                           //最も白に近い画素値におけるB成分の保存用
    double R_coeff = 0.0;                       //R成分の補正係数用
    double G_coeff = 0.0;                       //G成分の補正係数用
    double B_coeff = 0.0;                       //B成分の補正係数用
    double brightness = 0.0;                    //明るさ補正係数用
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

    /*出力画像データ領域の確保*/
    dataOut = (BYTE *)malloc(imageSize);
    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*明るさの補正係数を読み込み*/
    printf("Brightness => ");
    scanf("%lf",&brightness);
    /*明るさの補正係数が0より小さい場合終了*/
    if(brightness < 0.0){
        printf("Error: Brightness >= 0.0\n");
        return 1;
    }

    /*最も白に近い画素の画素値とRGB各成分の保存*/
    for(i=0;i<imageSize;i+=3){
        if(max_whitecolor <= (dataIn[i]+dataIn[i+1]+dataIn[i+2])){
            max_whitecolor = dataIn[i]+dataIn[i+1]+dataIn[i+2];
            target_R = dataIn[i];
            target_G = dataIn[i+1];
            target_B = dataIn[i+2];
        }
    }
    /*RGB各成分の係数を導出*/
    R_coeff = ((target_R+target_G+target_B)/3.0)/(double)target_R * brightness;
    G_coeff = ((target_R+target_G+target_B)/3.0)/(double)target_G * brightness;
    B_coeff = ((target_R+target_G+target_B)/3.0)/(double)target_B * brightness;
    /*画像全体にRGB各成分の係数をかける*/
    for(i=0;i<imageSize;i+=3){
        if(dataIn[i] * R_coeff > 255){
            dataOut[i] = 255;
        }else{
            dataOut[i] = dataIn[i] * R_coeff;
        }
        if(dataIn[i+1] * G_coeff > 255){
            dataOut[i+1] = 255;
        }else{
            dataOut[i+1] = dataIn[i+1] * G_coeff;
        }
        if(dataIn[i+2] * B_coeff > 255){
            dataOut[i+2] = 255;
        }else{
            dataOut[i+2] = dataIn[i+2] * B_coeff;
        }
    }
    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}