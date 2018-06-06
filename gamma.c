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
	
	printf("�K���}�␳���s���t�@�C�����t���p�X�Ŏw�肵�Ă�������\n"
		"�Ȃ��␳��̃t�@�C���͌��t�@�C�����̖�����""2""�����������O�ŏo�͂���܂�\n\n");
	fgets(temp, NAMEMAX - 1, stdin);				/* �t�@�C�������󂯎�� */
	length = strlen(temp);							/* �t�@�C�������m�F */
	temp[length-1] = '\0';							/* ���s�R�[�h������ */
	
	if((temp[length - 2] != 'p' && temp[length - 2] != 'P') ||
		(temp[length - 3] != 'm' && temp[length - 3] != 'M') ||
		(temp[length - 4] != 'b' && temp[length - 4] != 'B') ||
		temp[length - 5] != '.')
	{
		printf("�g���q��bmp�łȂ��t�@�C���̓I�[�v���ł��܂���\n");
		rewind(stdin);
		getchar();
		exit(1);
	}												/* bmp�ȊO�̊g���q�͎󂯕t���Ȃ� */
	
	
	if((fp = fopen(temp, "rb")) == NULL){
		printf("�t�@�C��""%s""���I�[�v���ł��܂���", temp);
		rewind(stdin);
		getchar();
		exit(1);	
	}
	
	strcpy(outputfile,temp);						/* �ȉ��̏����ŏo�̓t�@�C���������߂� */
	outputfile[length]		= '\0';
	outputfile[length-1]	= 'p';
	outputfile[length-2]	= 'm';
	outputfile[length-3]	= 'b';
	outputfile[length-4]	= '.';
	outputfile[length-5]	= '2';

	if((fp2 = fopen(outputfile, "wb")) == NULL){	/* �o�̓t�@�C���I�[�v�� */
		printf("�t�@�C��""%s""���I�[�v���ł��܂���", outputfile);
		rewind(stdin);
		getchar();
		exit(1);	
	}
	printf("\n�K���}�l����͂��Ă�������\n");
	fgets(temp, NAMEMAX - 1, stdin);				/* �K���}�l���͂Ɛ��l�ϊ� */
	point = atof(temp);

	gamma(fp, fp2, point);							/* �K���}�֐����R�[�� */
	
	return(0);

}

void gamma(FILE * fp, FILE * fp2, double point){
	
	int  temp[4],i, flg, offset, ch;
	double a,b;

	flg = fseek(fp, 10, SEEK_SET);	/* �I�t�Z�b�g�l�m�F�̂��߃t�@�C���|�C���^�ړ� */
	
	if(flg != 0){
		printf("fseek�G���[");
		return;
	}

	for(i = 0; i < 4; i++){
		temp[i] = fgetc(fp);		/* �I�t�Z�b�g�l�擾 */
	}
	
	offset = temp[0] + 256 * (temp[1] + 256 * (temp[2] + 256 * temp[3]));
									/* �擾�����I�t�Z�b�g�l��int�l�ɕϊ� */

	rewind(fp);						/* ���t�@�C���̃|�C���^��擪�ɖ߂� */
	
	i = 0;
	b = 1.0 / point;				/* �搔���o���Ă��� */

	while((ch = fgetc(fp)) != EOF){	/* �P�o�C�g���擾���o�� */
		if(i < offset){				/* �I�t�Z�b�g�l�܂ł͂��̂܂܏o�͂��� */
			fputc(ch, fp2);			
		}
		else{						/* �␳��̒l���o�͂��� */
			a = ch / 255.0;
			a = pow(a , b);
			ch = (int)(255 * a);
			fputc(ch, fp2);
		}
	
		i++;
	}

}