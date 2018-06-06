#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define NAMEMAX 256
#define INPUTMAX 256

void gamma(FILE * fp, FILE * fp2, double point);

int main(void){
	
	size_t length;
	char temp[NAMEMAX], outputfile[NAMEMAX + 2];
	double point;
	FILE * fp, * fp2;
	
	printf("ガンマ補正を行うファイルをフルパスで指定してください\n"
		"なお補正後のファイルは元ファイル名の末尾に""2""を加えた名前で出力されます\n\n");
	fgets(temp, NAMEMAX - 1, stdin);				/* ファイル名を受け取る */
	length = strlen(temp);							/* ファイル長を確認 */
	temp[length-1] = '\0';							/* 改行コードを消す */
	
	if((temp[length - 2] != 'p' && temp[length - 2] != 'P') ||
		(temp[length - 3] != 'm' && temp[length - 3] != 'M') ||
		(temp[length - 4] != 'b' && temp[length - 4] != 'B') ||
		temp[length - 5] != '.')
	{
		printf("拡張子がbmpでないファイルはオープンできません\n");
		rewind(stdin);
		getchar();
		exit(1);
	}												/* bmp以外の拡張子は受け付けない */
	
	
	if((fp = fopen(temp, "rb")) == NULL){
		printf("ファイル""%s""がオープンできません", temp);
		rewind(stdin);
		getchar();
		exit(1);	
	}
	
	strcpy(outputfile,temp);						/* 以下の処理で出力ファイル名を決める */
	outputfile[length]		= '\0';
	outputfile[length-1]	= 'p';
	outputfile[length-2]	= 'm';
	outputfile[length-3]	= 'b';
	outputfile[length-4]	= '.';
	outputfile[length-5]	= '2';

	if((fp2 = fopen(outputfile, "wb")) == NULL){	/* 出力ファイルオープン */
		printf("ファイル""%s""がオープンできません", outputfile);
		rewind(stdin);
		getchar();
		exit(1);	
	}
	printf("\nガンマ値を入力してください\n");
	fgets(temp, NAMEMAX - 1, stdin);				/* ガンマ値入力と数値変換 */
	point = atof(temp);

	gamma(fp, fp2, point);							/* ガンマ関数をコール */
	
	return(0);

}

void gamma(FILE * fp, FILE * fp2, double point){
	
	int  temp[4],i, flg, offset, ch;
	double a,b;

	flg = fseek(fp, 10, SEEK_SET);	/* オフセット値確認のためファイルポインタ移動 */
	
	if(flg != 0){
		printf("fseekエラー");
		return;
	}

	for(i = 0; i < 4; i++){
		temp[i] = fgetc(fp);		/* オフセット値取得 */
	}
	
	offset = temp[0] + 256 * (temp[1] + 256 * (temp[2] + 256 * temp[3]));
									/* 取得したオフセット値をint値に変換 */

	rewind(fp);						/* 元ファイルのポインタを先頭に戻す */
	
	i = 0;
	b = 1.0 / point;				/* 乗数を出しておく */

	while((ch = fgetc(fp)) != EOF){	/* １バイトずつ取得し出力 */
		if(i < offset){				/* オフセット値まではそのまま出力する */
			fputc(ch, fp2);			
		}
		else{						/* 補正後の値を出力する */
			a = ch / 255.0;
			a = pow(a , b);
			ch = (int)(255 * a);
			fputc(ch, fp2);
		}
	
		i++;
	}

}