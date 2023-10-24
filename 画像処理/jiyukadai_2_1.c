/*
 *  19T1698W_jiyukadai_2_1.c
 *  自由課題 2-1
 *  背景画像のサイズが自由に指定でき、
 *  人物画像の位置を指定しクロマキー合成を行うプログラム
 */

#include "stdio.h"
#include "jpegio.h"

int main(void){

    char *person_filename = "person.jpg";       //読み込む人物画像のファイル名
    char *chromakey_filename = "chromakey.jpg"; //読み込むクロマキー画像のファイル名
    char *back_filename = "background-3.jpg";   //読み込む背景画像のファイル名
    char *Out_filename = "result-j-2-1.jpg";    //出力画像のファイル名
    int Width_person = 0;                       //人物画像の横幅の画素数
    int Height_person = 0;                      //人物画像の縦幅の画素数
    int BPP_person = 0;                         //人物画像の1画素あたりのバイト数
    int Width_back = 0;                         //背景画像の横幅の画素数
    int Height_back = 0;                        //背景画像の縦幅の画素数
    int BPP_back = 0;                           //背景画像の1画素あたりのバイト数
    BYTE *dataIn_person = NULL;                 //入力人物画像データ領域のポインタ
    BYTE *dataIn_chromakey = NULL;              //入力クロマキー画像データ領域のポインタ
    BYTE *dataIn_back = NULL;                   //入力背景画像データ領域のポインタ
    BYTE *dataOut = NULL;                       //出力画像データ領域のポインタ
    int ImSize_person = 0;                      //人物画像データのサイズ
    int ImSize_back = 0;                        //背景画像データのサイズ
    int R_value = 0;                            //R値の合計値を保存する変数
    int G_value = 0;                            //G値の合計値を保存する変数
    int B_value = 0;                            //B値の合計値を保存する変数
    int pixel_num = 0;                          //画素数を保存する変数
    double R_ave = 0.0;                         //R値の平均値を保存する変数
    double G_ave = 0.0;                         //G値の平均値を保存する変数
    double B_ave = 0.0;                         //B値の平均値を保存する変数
    int range = 0;                              //マスク画像を黒色にする際の平均値からの許容値
    int move_x = 0;                             //人物画像をx方向に移動する量
    int move_y = 0;                             //人物画像をy方向に移動する量
    int range_width_min = 0;        //背景画像における人物画像位置の横方向の最小値
    int range_width_max = 0;        //背景画像における人物画像位置の横方向の最大値
    int range_height_min = 0;       //背景画像における人物画像位置の縦方向の最小値
    int range_height_max = 0;       //背景画像における人物画像位置の縦方向の最大値
    int target_posi_person = 0;     //人物画像データ配列における指定画素の場所
    int target_posi = 0;            //画像データ配列における指定画素の場所
    int i,j;

    /*画像情報の読み込み*/
    Width_person = readHeader(person_filename,IMAGE_WIDTH);
    Height_person = readHeader(person_filename,IMAGE_HEIGHT);
    BPP_person = readHeader(person_filename,IMAGE_BPP);

    Width_back = readHeader(back_filename,IMAGE_WIDTH);
    Height_back = readHeader(back_filename,IMAGE_HEIGHT);
    BPP_back = readHeader(back_filename,IMAGE_BPP);

    /*画像データの読み込み*/
    ImSize_person = readJpeg(person_filename,&dataIn_person);
    ImSize_back = readJpeg(back_filename,&dataIn_back);

    /*クロマキー画像の読み込みを行い、人物画像とサイズが違った場合終了*/
    if(ImSize_person != readJpeg(chromakey_filename,&dataIn_chromakey)){
        printf("ImageSize Error\n");
        return 1;
    }

    /*画像データの読み込みに失敗した場合終了する*/
    if(ImSize_person == 0 || ImSize_back == 0){
        printf("Read Error\n");
        return 1;
    }

    //マスク画像を黒色にする際の平均値からの許容値を受け取る
    printf("range => ");
    scanf("%d",&range);
    //人物画像をx方向に移動する量を受け取る
    printf("move x => ");
    scanf("%d",&move_x);
    //人物画像をy方向に移動する量を受け取る
    printf("move y => ");
    scanf("%d",&move_y);

    /*出力画像データ領域の確保*/
    dataOut = (BYTE *)malloc(ImSize_back);
    /*出力画像データ領域を確保できなかった場合終了する*/
    if(dataOut == NULL){
        fprintf(stderr, "Can't allocate memory\n");
        return 1;
    }

    /*RGB値を変数に加算する*/
    for(i=0;i<ImSize_person;i+=3){
        R_value += dataIn_chromakey[i];
        G_value += dataIn_chromakey[i+1];
        B_value += dataIn_chromakey[i+2];
    }
    /*画素数を計算し、RGB値の平均を変数に格納*/
    pixel_num = Width_person*Height_person;
    R_ave = (double)R_value/(double)pixel_num;
    G_ave = (double)G_value/(double)pixel_num;
    B_ave = (double)B_value/(double)pixel_num;
    /*出力画像データを255で初期化する*/
    for(i=0;i<ImSize_back;i++){
        dataOut[i] = 255;
    }

    /* 
     * 背景画像の領域外にある人物画像の領域は切り捨てると考え、
     * 背景画像における人物画像の縦横方向の位置を求める
     */
    if(move_x < 0){
        range_width_min = 0;
        range_width_max = Width_person + move_x;
    }else{
        range_width_min = move_x;
        range_width_max = Width_person + move_x;
    }
    if(move_y < 0){
        range_height_min = 0;
        range_height_max = Height_person + move_y;
    }else{
        range_height_min = move_y;
        range_height_max = Height_person + move_y;
    }

    /* 
     * 縦横の画素の位置から配列の場所を求め、それぞれクロマキー画像のRGBの
     * 平均値±rangeであった場合出力画像データを背景画像にする
     */
    for(i=0;i<Height_back;i++){
        for(j=0;j<Width_back;j++){
            target_posi = i*Width_back*3+j*3;
            target_posi_person = Width_person*(-move_y+i)*3 + (-move_x+j)*3;
            if(j>=range_width_min && j<range_width_max && i>=range_height_min && i<range_height_max){
                if( 
                    dataIn_person[target_posi_person] <= R_ave+range 
                    && dataIn_person[target_posi_person] >= R_ave-range
                    && dataIn_person[target_posi_person+1] <= G_ave+range
                    && dataIn_person[target_posi_person+1] >= G_ave-range
                    && dataIn_person[target_posi_person+2] <= B_ave+range
                    && dataIn_person[target_posi_person+2] >= B_ave-range
                ){
                    dataOut[target_posi] = dataIn_back[target_posi];
                    dataOut[target_posi+1] = dataIn_back[target_posi+1];
                    dataOut[target_posi+2] = dataIn_back[target_posi+2];
                    continue;
                }
                dataOut[target_posi] = dataIn_person[target_posi_person];
                dataOut[target_posi+1] = dataIn_person[target_posi_person+1];
                dataOut[target_posi+2] = dataIn_person[target_posi_person+2];
            }else{
                dataOut[target_posi] = dataIn_back[target_posi];
                dataOut[target_posi+1] = dataIn_back[target_posi+1];
                dataOut[target_posi+2] = dataIn_back[target_posi+2];
            }
        }
    }

    /*ファイルに出力*/
    writeJpeg(Out_filename,dataOut,Width_back,Height_back,BPP_back,100);
    
    /*画像データ領域の開放*/
    free(dataIn_person);
    free(dataIn_chromakey);
    free(dataIn_back);
    free(dataOut);

    return 0;
}