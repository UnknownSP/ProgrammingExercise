/*
 *  19T1698W_kadai_1_5.c
 *  必須課題 1-5
 *  RGB各画像の出力をするプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *In_filename = "image_3.jpg";          //読み込む画像のファイル名
    char *Out_filename_R = "Result_1_5_R.jpg";  //出力するR成分のみの画像のファイル名
    char *Out_filename_G = "Result_1_5_G.jpg";  //出力するG成分のみの画像のファイル名
    char *Out_filename_B = "Result_1_5_B.jpg";  //出力するB成分のみの画像のファイル名
    int imageWidth = 0;                         //画像データの横幅の画素数
    int imageHeight = 0;                        //画像データの縦幅の画素数
    int imageBPP = 0;                           //画像データの1画素あたりのバイト数
    BYTE *dataIn = NULL;                        //入力画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int imageSize = 0;                          //画像データのサイズ
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

    /*
     *  入力画像サイズが大きい時、出力画像データ領域をRGBそれぞれ一つずつ用意すると
     *  使用メモリがとても大きくなるので、一つの出力画像データ領域を使い回す
     */

    /*R成分のみを出力画像データに保存*/
    for(i=0;i<imageSize;i++){
        if(i%3 == 1 || i%3 == 2){
            dataOut[i] = 0;
        }else{
            dataOut[i] = dataIn[i];
        }
    }
    /*R成分のみを抽出した画像をファイルに出力*/
    writeJpeg(Out_filename_R,dataOut,imageWidth,imageHeight,imageBPP,100);

    /*G成分のみを出力画像データに保存*/
    for(i=0;i<imageSize;i++){
        if(i%3 == 0 || i%3 == 2){
            dataOut[i] = 0;
        }else{
            dataOut[i] = dataIn[i];
        }
    }
    /*G成分のみを抽出した画像をファイルに出力*/
    writeJpeg(Out_filename_G,dataOut,imageWidth,imageHeight,imageBPP,100);

    /*B成分のみを出力画像データに保存*/
    for(i=0;i<imageSize;i++){
        if(i%3 == 0 || i%3 == 1){
            dataOut[i] = 0;
        }else{
            dataOut[i] = dataIn[i];
        }
    }
    /*B成分のみを抽出した画像をファイルに出力*/
    writeJpeg(Out_filename_B,dataOut,imageWidth,imageHeight,imageBPP,100);

    /*画像データ領域の開放*/
    free(dataIn);
    free(dataOut);

    return 0;
}