/*
 *  19T1698W_kadai_2_3.c
 *  必須課題 2-3
 *  クロマキー合成を行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *person_filename = "person.jpg";       //読み込む人物画像のファイル名
    char *chromakey_filename = "chromakey.jpg"; //読み込むクロマキー画像のファイル名
    char *back_filename = "background.jpg";     //読み込む背景画像のファイル名
    char *Out_filename = "result2-3.jpg";       //出力画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn_person = NULL;                 //入力人物画像データ領域のポインタ
    BYTE *dataIn_chromakey = NULL;              //入力クロマキー画像データ領域のポインタ
    BYTE *dataIn_back = NULL;                   //入力背景画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ
    int R_value = 0;                            //R値の合計値を保存する変数
    int G_value = 0;                            //G値の合計値を保存する変数
    int B_value = 0;                            //B値の合計値を保存する変数
    int pixel_num = 0;                          //画素数を保存する変数
    double R_ave = 0.0;                         //R値の平均値を保存する変数
    double G_ave = 0.0;                         //G値の平均値を保存する変数
    double B_ave = 0.0;                         //B値の平均値を保存する変数
    int range = 0;                              //マスク画像を黒色にする際の平均値からの許容値
    int i;

    /*画像情報の読み込み*/
    imageWidth = readHeader(person_filename,IMAGE_WIDTH);
    imageHeight = readHeader(person_filename,IMAGE_HEIGHT);
    imageBPP = readHeader(person_filename,IMAGE_BPP);
    /*人物画像データの読み込み*/
    imageSize = readJpeg(person_filename,&dataIn_person);
    /*クロマキー画像と背景画像の読み込みを行い、人物画像とサイズが違った場合終了*/
    if( imageSize != readJpeg(chromakey_filename,&dataIn_chromakey) 
        || imageSize != readJpeg(back_filename,&dataIn_back) ){
        printf("ImageSize Error\n");
        return 1;
    }   

    /*画像データの読み込みに失敗した場合終了する*/
    if(imageSize == 0){
        printf("Read Error\n");
        return 1;
    }

    //マスク画像を黒色にする際の平均値からの許容値を受け取る
    printf("range => ");
    scanf("%d",&range);

    /*出力画像データ領域の確保*/
    dataOut = (BYTE *)malloc(imageSize);
    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*RGB値を変数に加算し、出力画像データを255で初期化する*/
    for(i=0;i<imageSize;i+=3){
        R_value += dataIn_chromakey[i];
        G_value += dataIn_chromakey[i+1];
        B_value += dataIn_chromakey[i+2];
        dataOut[i] = 255;
        dataOut[i+1] = 255;
        dataOut[i+2] = 255;
    }
    /*画素数を計算し、RGB値の平均を変数に格納*/
    pixel_num = imageWidth*imageHeight;
    R_ave = (double)R_value/(double)pixel_num;
    G_ave = (double)G_value/(double)pixel_num;
    B_ave = (double)B_value/(double)pixel_num;

    /*人物画像のRGB値がクロマキー画像のRGBの平均値±rangeであった場合出力画像データをRGB各0にする*/
    for(i=0;i<imageSize;i+=3){
        if( dataIn_person[i] <= R_ave+range && dataIn_person[i] >= R_ave-range
            && dataIn_person[i+1] <= G_ave+range && dataIn_person[i+1] >= G_ave-range
            && dataIn_person[i+2] <= B_ave+range && dataIn_person[i+2] >= B_ave-range ){
            dataOut[i] = 0;
            dataOut[i+1] = 0;
            dataOut[i+2] = 0;
        }
    }
    /*出力画像データのRGBが0の場合、背景画像に置き換え、255の場合人物画像に置き換える*/
    for(i=0;i<imageSize;i+=3){
        if(dataOut[i] == 0){
            dataOut[i] = dataIn_back[i];
            dataOut[i+1] = dataIn_back[i+1];
            dataOut[i+2] = dataIn_back[i+2];
        }else{
            dataOut[i] = dataIn_person[i];
            dataOut[i+1] = dataIn_person[i+1];
            dataOut[i+2] = dataIn_person[i+2];
        }
    }
    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,imageWidth,imageHeight,imageBPP,100);
    
    /*画像データ領域の開放*/
    free(dataIn_person);
    free(dataIn_chromakey);
    free(dataIn_back);
    free(dataOut);

    return 0;
}