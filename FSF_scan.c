#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<jpeglib.h>
#include<corecrt_math_defines.h>
#include<string.h>

void output_JPEG(JSAMPARRAY img) {

	//---------------------------------------------------------------------------------
	/* JPEG�I�u�W�F�N�g�C�G���[�n���h���̊m��*/
	struct jpeg_compress_struct cinfo2;
	struct jpeg_error_mgr jerr2;

	/* �G���[�n���h���Ƀf�t�H���g�l��ݒ�*/
	cinfo2.err = jpeg_std_error(&jerr2);

	/* JPEG�I�u�W�F�N�g�̒ǉ�*/
	jpeg_create_compress(&cinfo2);

	/*�o�̓t�@�C���̐ݒ�*/

	char *filename2 = "output.jpg";
	FILE *fp2 = fopen(filename2, "wb");

	jpeg_stdio_dest(&cinfo2, fp2);

	int width = 1600, height = 1200;
	cinfo2.image_width = width;
	cinfo2.image_height = height;
	cinfo2.input_components = 3;
	cinfo2.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo2);
	jpeg_set_quality(&cinfo2, 100, TRUE);

	/*���k�J�n*/
	jpeg_start_compress(&cinfo2, TRUE);

	/*��������*/
	jpeg_write_scanlines(&cinfo2, img, height);

	/*���k�I��*/
	jpeg_finish_compress(&cinfo2);

	/*JPEG�I�u�W�F�N�g�̔j��*/
	jpeg_destroy_compress(&cinfo2);

	fclose(fp2);

	//---------------------------------------------------------------------------------

}

void output_JPEGscan(JSAMPARRAY img, char *filename) {

	//---------------------------------------------------------------------------------
	/* JPEG�I�u�W�F�N�g�C�G���[�n���h���̊m��*/
	struct jpeg_compress_struct cinfo3;
	struct jpeg_error_mgr jerr3;

	/* �G���[�n���h���Ƀf�t�H���g�l��ݒ�*/
	cinfo3.err = jpeg_std_error(&jerr3);

	/* JPEG�I�u�W�F�N�g�̒ǉ�*/
	jpeg_create_compress(&cinfo3);

	/*�o�̓t�@�C���̐ݒ�*/

	char *ptr, moji[64];//filename�����������Ȃ��悤��moji[]�ɃR�s�[���Ă���
	strcpy(moji, filename);
	ptr = moji;
	strcpy(ptr, filename);
	ptr = strtok(ptr, "_");
	ptr = strtok(NULL, ".");

	char *filename3 = strcat(ptr, "out.jpg");
	FILE *fp3 = fopen(filename3, "wb");

	jpeg_stdio_dest(&cinfo3, fp3);

	int width = 1600, height = 1200;
	cinfo3.image_width = width;
	cinfo3.image_height = height;
	cinfo3.input_components = 3;
	cinfo3.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo3);
	jpeg_set_quality(&cinfo3, 100, TRUE);

	/*���k�J�n*/
	jpeg_start_compress(&cinfo3, TRUE);

	/*��������*/
	jpeg_write_scanlines(&cinfo3, img, height);

	/*���k�I��*/
	jpeg_finish_compress(&cinfo3);

	/*JPEG�I�u�W�F�N�g�̔j��*/
	jpeg_destroy_compress(&cinfo3);

	fclose(fp3);

	//---------------------------------------------------------------------------------

}


#define n_g 9             //n_g-1���̑������ŋߎ��i�P�ȏ� = �����ߎ��͂ł��Ȃ��j
#define N_g 9             //�K�E�X�̏����@�ɂ����関�m���̐��i���n_g�Ɠ����l�ɂ��邱�Ɓj
//#define S_g 5             //�f�[�^�̌�
#define CHECK_g 0 //�K�E�X�̏����@�ɂ�����O�p�s��̃`�F�b�N�p(1�ɂ���΃`�F�b�N�ł���)

void gauss(double a[N_g][N_g + 1], double p[N_g]);

void sai(double x[], double y[], int S_g, double SMg[])
{
	int i, j, k;
	double Xb=0, X_sum=0;
	double A[n_g][n_g + 1], xx[N_g];
	//double X, Y;


	/*
	FILE *output1;
	FILE *output2;
	output1 = fopen("output1.data", "w");
	output2 = fopen("output2.data", "w");
	*/
	
	/*������*/
	for (i = 0; i<n_g; i++) {
		for (j = 0; j<n_g + 1; j++) {
			A[i][j] = 0.0;
		}
	}

	/*�K�E�X�̏����@�ŉ����s��̍쐬*/
	for (i = 0; i<n_g; i++) {
		for (j = 0; j<n_g; j++) {
			for (k = 0; k<S_g; k++) {
				A[i][j] += pow(x[k], i + j);
			}
		}
	}
	for (i = 0; i<n_g; i++) {
		for (k = 0; k<S_g; k++) {
			A[i][n_g] += pow(x[k], i)*y[k];
		}
	}
	/*�K�E�X�̏����@�̎��s�i�z��xx�͉��A���Ȃ킿�������̌W�������邽�߂̂��́j*/
	gauss(A, xx); // y = a + b * xi^1 + c * xi^2 + d * xi^3 + ...

	/* �o�͗p�z��ɑ��*/
	for (int i = 0; i<N_g; i++) {
		SMg[i] = xx[i];
	}

	/*
	//GNUPLOT�ŕ\�����邽�߂ɍŏ��Q��@�ɂ��֐��̃f�[�^���t�@�C���ۑ�
	for (X = x[0] - 10.0; X<x[S_g-1] + 10.0; X += 0.01) {
		Y = 0.0;
		for (i = 0; i<N_g; i++) {
			Y += xx[i] * pow(X, i);
		}
		fprintf(output1, "%f %f\n", X, Y);
	}

	//GNUPLOT�ŕ\�����邽�߂ɁA�ŏ��Q��@�Ɏg��ꂽ�f�[�^��ۑ�
	for (i = 0; i<S_g; i++) {
		fprintf(output2, "%f %f\n", x[i], y[i]);
	}

	fclose(output1);
	fclose(output2);
	*/
}

void gauss(double a[N_g][N_g + 1], double xx[N_g])
{
	int i, j, k, l, pivot;
	double x[N_g];
	double p, q, m, b[1][N_g + 1];

	for (i = 0; i<N_g; i++) {
		m = 0;
		pivot = i;

		for (l = i; l<N_g; l++) {
			if (fabs(a[l][i])>m) {   //i��̒��ň�Ԓl���傫���s��I��
				m = fabs(a[l][i]);
				pivot = l;
			}
		}

		if (pivot != i) {                          //pivot��i�ƈႦ�΁A�s�̓���ւ�
			for (j = 0; j<N_g + 1; j++) {
				b[0][j] = a[i][j];
				a[i][j] = a[pivot][j];
				a[pivot][j] = b[0][j];
			}
		}
	}

	for (k = 0; k<N_g; k++) {
		p = a[k][k];              //�Ίp�v�f��ۑ�
		a[k][k] = 1;              //�Ίp�v�f�͂P�ɂȂ邱�Ƃ��킩���Ă��邩��

		for (j = k + 1; j<N_g + 1; j++) {
			a[k][j] /= p;
		}

		for (i = k + 1; i<N_g; i++) {
			q = a[i][k];

			for (j = k + 1; j<N_g + 1; j++) {
				a[i][j] -= q * a[k][j];
			}
			a[i][k] = 0;              //�O�ƂȂ邱�Ƃ��킩���Ă���Ƃ���
		}
	}

	//���̌v�Z
	for (i = N_g - 1; i >= 0; i--) {
		x[i] = a[i][N_g];
		for (j = N_g - 1; j>i; j--) {
			x[i] -= a[i][j] * x[j];
		}
	}

	//�s�񂪍Ō�ǂ��Ȃ������������Ƃ��Ɏ��s
#if CHECK_g==1
	for (i = 0; i<N_g; i++) {
		for (j = 0; j<N_g + 1; j++) {
			printf("%10.3f", a[i][j]);
		}
		printf("\n");

	}
#endif
	
	//printf("����\n");
	for (i = 0; i<N_g; i++) {
		//printf("%f\n", x[i]);
		xx[i] = x[i];
	}
	
}

/* �ŏ����@(�����ߎ��p) */
void SaishoJijo(double **P, double SM[]) {

	double Sxiyi = 0, Sxi = 0, Syi = 0, Sxi2 = 0, M_r[2] = {0,0};
	//double a, b, theta;
	int N=0,i=0;

	while (P[i][1] != 9999) {
		N = N+1;
		i++;
	}

	for (int i = 0; i < N;i++) {
		Sxiyi = Sxiyi + P[i][0]*P[i][1];
		Sxi = Sxi + P[i][0];
		Syi = Syi + P[i][1];
		Sxi2 = Sxi2 + P[i][0]*P[i][0];
	}

	SM[0] = (N*Sxiyi - Sxi*Syi) / (N*Sxi2 - Sxi*Sxi); // a�����߂�
	SM[1] = (Sxi2*Syi - Sxi*Sxiyi) / (N*Sxi2 - Sxi*Sxi); // b�����߂�
	SM[2] = (180.0 / M_PI)*atan(SM[0]); // theta(��)�����߂�
	

	printf("a : %lf | b : %lf | theta : %lf | N : %d\n", SM[0], SM[1], SM[2], N);


}

void IcChecker_N50model(char *filename, double IPx, double IPy, double Vran, double Iran, double Th_max) {

	//JPEG�t�@�C���̓ǂݍ���---------------------------------------------------------------------------

	//�I�u�W�F�N�g�̊m�ۂƏ�����
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *fp = fopen(filename, "rb");

	if (fp == NULL) {
		fprintf(stderr, "cannat open %s\n", filename);
		exit(EXIT_FAILURE);
	}
	jpeg_stdio_src(&cinfo, fp);

	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	int width, height;
	width = cinfo.output_width;
	height = cinfo.output_height;

	JSAMPARRAY img = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);

	for (int i = 0; i < height; i++) {
		img[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * 3 * width);
	}

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, img + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
		//printf("scanline : %d\n",cinfo.output_scanline);
	}


	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);


	//���_�����߂�(img�I�u�W�F�N�g���W�n�ł̌v�Z)---------------------------------------------------------------------------
	//�J���[�̂܂܏������邱�Ƃ�臒l���L�c���ݒ�ł���(235 �� 190)
	int i, j;
	//double io, jo, IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, OP[2];
	double io, jo, OP[2];

	// ���X�L����
	// j != width �̏�����t�����邱�Ƃŉ摜�f�[�^�̃t�`�̍��F�����o�����s���h��
	// 3��A���Ō��_�ʒu�����X�ɒ��ׂ邱�Ƃŗאڂ������̌댟�o��h��(3��ڂ̈ʒu���ŏI�I�Ȍ��_�ʒu�Ƃ��Ē�߂�)
	
	// 1���
	for (i = height - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}
	
	// 2���(�O�Ō��������_�ʒu�����㑤������ɑ������Ă���)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	// 3���(�O�Ō��������_�ʒu�����㑤������ɑ������Ă���)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				OP[0] = (jo / IPx)*Vran; //x�̒l						(�K�v�Ȃ�����)
				OP[1] = (io / IPy)*Iran; //y�̒l						(�K�v�Ȃ�����)
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	//printf("%f %f\n", jo, io+3.0); ���l��␳���邽�߂ɂ��Ƃ���io-3.0�Ƃ��Ă������c�ł���

	//printf("%f %f\n", jo, io);

	double x[30000] = { 0 }, y[30000] = { 0 };
	int k = 0, S_g = 0;
	double SMg[n_g] = { 0 };
	//��l������(�f�[�^�_����=0�ɂȂ�)(img�I�u�W�F�N�g���W�n �� ���ی��v�Z���W�n)---------------------------------------------------------------------------
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			if (img[i][j * 3 + 1] >= 190 && img[i][j * 3 + 2] >= 100) {
				img[i][j * 3 + 0] = 0;
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				x[k] = ((double)j - jo)/100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)(x/100)
				y[k] = (io - (double)i)/100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)(y/100)
				k++;
			}
			else {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}
	
	//�ߎ��Ȑ������߂�
	S_g = k;
	sai(x, y, S_g, SMg);

	//output_JPEG(img);


	//臒l�����߁C�X�L���������l�Ɣ�r���ăf�[�^��I�肷��(�ߎ������̍��W�n��img�I�u�W�F�N�g�̍��W�n�͈قȂ�)---------------------------------------------------------------------------
	//�ߎ��������O���ɍs���قǂǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�����܂ł��Ǐ��I�Ȓl�Ƃ��Ĉ����K�v������
	//��R���(jth_max�ȍ~�̒�������)�̃f�[�^�Q�͏����Ȃ��悤�ɂ���

	double Y;
	for (int j = 0; j < (int)(jo + (Th_max*IPx) / Vran) && j < width; j++) {
		for (int i = 0; i < height; i++) {

			//�����ɂ�����e�l�ɉ������@������臒l���Z�o����
			Y = 0.0;
			for (int k = 0; k<N_g; k++) {
				Y += SMg[k] * pow((j-jo)/100, k); //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������͉����������l��)
			}
			Y = io - Y*100; //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������ŏc���݂̂��l��)
						//printf("%f\n",Y);
						//�X�L���������l��臒l���r���ăf�[�^��I�肷�� ( j < jo �̏����ɂ���Č��_���}�C�i�X�����̃f�[�^������ = �m�C�Y��������) 
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 3 || j < jo) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//�ߎ��Ȑ��`��m�F�p
			if (i == (int)Y) {
			//�ߎ��������O���ɍs���ق� Y���ǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�O���ɂ��Ԃ��`�悳���\��������
			img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);


	//�ēximg�I�u�W�F�N�g��ǂݍ���ŋߎ��Ȑ������߂�---------------------------------------------------------------------------

	//x[],y[]�̏�����
	for (int i = 0; i < 30000; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	//SMg[]�̏�����
	for (int i = 0; i < n_g; i++) {
		SMg[i] = 0;
	}

	//x[],y[]��img�I�u�W�F�N�g�̃f�[�^���i�[���ߎ��Ȑ������߂�
	k = 0;
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {
				x[k] = ((double)j - jo)/100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)
				y[k] = (io - (double)i)/100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)
				k++;
			}
		}
	}

	S_g = k;
	sai(x, y, S_g, SMg);

	//�ēx�C臒l�����߁C�X�L���������l�Ɣ�r���ăf�[�^��I�肷��(�ߎ������̍��W�n��img�I�u�W�F�N�g�̍��W�n�͈قȂ�)---------------------------------------------------------------------------

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			//�����ɂ�����e�l�ɉ������@������臒l���Z�o����
			Y = 0.0;
			for (int k = 0; k < N_g; k++) {
				Y += SMg[k] * pow((j - jo)/100, k); //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������͉����������l��)
			}
			Y = io - Y*100; //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������ŏc���݂̂��l��)

						//�X�L���������l��臒l���r���ăf�[�^��I�肷��
						//���ڂ̂ق����ߎ��Ȑ��̐��x���オ��̂ŁC���̕��f�[�^�I�莞�̃}�[�W�����L���Ƃ邱�ƂŁC�c�������f�[�^�_�𑽂��c����悤�ɂ���
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 9) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			//�L���ȃf�[�^�_�̋�Ԃł͋ߎ�������100�s�N�Z����̃f�[�^������ = �m�C�Y��������
			if (img[i][j * 3 + 1] == 0 && j >= 0 && i < (int)Y - 100) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//�ߎ��Ȑ��`��m�F�p
			if (i == (int)Y) {
			//�ߎ��������O���ɍs���ق� Y���ǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�O���ɂ��Ԃ��`�悳���\��������
			img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}
	
	//output_JPEG(img);

	//��R��Ԃ̋ߎ������̌X�������߂�(���ی��v�Z���W�n�ł̌v�Z)---------------------------------------------------------------------------
	double **P; //���I�z��̊m�� (�f�[�^�_���i�[���邽�߁F���ی��v�Z���W�n�ɕω������f�[�^���i�[����)

	P = malloc(sizeof(double*) * 10000);
	for (int i = 0; i < 10000; i++) {
		P[i] = malloc(sizeof(double) * 3);
	}

	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double it, jt, SM[3], M_r[2];
	int jth_max, p = 0, last_i = 0;

	jth_max = (int)(jo + (Th_max*IPx) / Vran);
	//printf("%d\n",ith_max);

	//printf(" width : %d | height : %d\n",width,height);

	//jth_max�ȍ~�̓˔��I�ȃm�C�Y���J�b�g����(2�A���܂ł������Ώۂɂ���)
	for (int j = jth_max; j < width - 1; j++) {
		for (int i = 1; i < height - 1; i++) {
			if (img[i][j * 3 + 1] == 0 && (img[i - 1][j * 3 + 1] + img[i + 1][j * 3 + 1] + img[i][(j - 1) * 3 + 1] + img[i][(j + 1) * 3 + 1]) >= 765) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}
	
	for (j = jth_max; j < width; j++) {
		for (i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {

				it = (double)i;
				jt = (double)j;

				P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
				P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
				p = p + 1;

			}
		}
	}
	//printf("%lf | %lf | %lf | %lf\n",P[0][0],P[0][1],it,jt);
	//printf("%d | %d | %lf | %lf\n",i,j,io,jo);

	/*
	for (int i = 0; i < 10000; i++) {
	for (int j = 0; j < 3; j++) {
	//printf("P[%d][0] : %lf  P[%d][1] : %lf\n",i,jo+P[i][0], i, io-P[i][1]);
	//printf("P[%d][0] : %lf  P[%d][1] : %lf\n", i, P[i][0], i, P[i][1]);
	}
	}
	*/

	SaishoJijo(P, SM); // SM[0]=a, SM[1]=b, SM[2]=theta(��) �ɂȂ��Ă���

					   //���_ M(x,y) �����߂�(���ꂪ��]���S�ɂȂ�Bdouble�^�Ȃ̂Œ���)
	M_r[0] = (P[p - 1][0] + (jth_max - jo)) / 2.0; //���_M��x���W(���ی��v�Z���W�n)
	M_r[1] = ((SM[0] * P[p - 1][0] + SM[1]) + (SM[0] * (jth_max - jo) + SM[1])) / 2.0; //���_M��y���W(���ی��v�Z���W�n)

																					   //printf("a : %lf | b : %lf | theta : %lf\n", SM[0], SM[1], SM[2]);
																					   //printf("x : %lf | y : %lf\n", M_r[0], M_r[1]);
	
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			Y = 0.0;
			Y = io - (SM[0] * (j - jo) + SM[1]);

			//��R��ԂɂȂ������ォ��̃f�[�^�����(25��20)
			if (img[i][j * 3 + 1] == 0) {
				if (abs(i - (int)Y) <= 20) {
					img[i][j * 3 + 0] = 255;
					img[i][j * 3 + 1] = 255;
					img[i][j * 3 + 2] = 255;
				}

				/*
				//�ߎ��Ȑ��`��m�F�p
				if (i == (int)Y) {
				img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				}
				*/
			}

		}
	}

	/*
	//jth_max�ʒu�`��m�F�p
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	*/

	/*
	//���_�`��m�F�p
	int a, b;
	a = (int)M_r[0] + jo;
	b = io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//output_JPEG(img);

	//�ϑ��_�ɂ������]���S�Ƃ̊p�x��(��)�����߂�---------------------------------------------------------------------------
	//�ēxP[][]��������
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double theta_0, theta_scan, theta_max;
	p = 0;
	double jm, im; //atan2()�ɓ����s����double�^�Œ�`����
	jm = M_r[0] + jo;
	im = io - M_r[1];

	theta_0 = SM[2] - 180.0; //SM[2]��(��)�Fdegree�\�L (���͕��̒l�ɂȂ�ƍl������)

							 //���̒i�K�ł�theta_max���ǂ����̔���͍s��Ȃ�
	for (int j = jth_max; j > 0; j--) {
		for (int i = 0; i < height; i++) {
			theta_scan = (180.0 / M_PI)*atan2(im - (double)i, (double)j - jm);

			it = (double)i;
			jt = (double)j;

			if (img[i][j * 3 + 1] == 0) {

				if (theta_scan < 0) {
					P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][2] = fabs(theta_scan) - fabs(theta_0);
					p = p + 1;
				}
				else if (theta_scan >= 0) {
					P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][2] = (180.0 - theta_scan) + (180.0 - fabs(theta_0));
					p = p + 1;
				}

			}
		}
	}

	/*
	for (int i = 0; i < 10000; i++) {
	printf("P[%d][2] : %lf\n",i,P[i][2]);
	}
	*/

	int start, end, i_max;
	theta_max = P[0][2];
	i = 0;
	i_max = i;
	while (P[i][2] <= 2.5) {
		i++;
	}
	start = i;

	for (i = 0; i < 10000; i++) {
		if (P[i][2] >= theta_max && P[i][2] != 9999) {
			theta_max = P[i][2];
			i_max = i;
		}
	}
	end = i_max;

	//printf("start : %d | end : %d\n", start, end);
	//printf("start : %f | %f\n", jo+P[start][0], io-P[start][1]);
	//printf("end : %f | %f\n", jo+P[end][0], io-P[end][1]);

	
	//��xtheta_max����������Ctheta_max����}0.5�������e����---------------------------------------------------------------------------
	theta_max = P[i_max][2];

	for (i = end; i < 10000;i++) {
		if (fabs(theta_max - P[i][2]) < 0.5 && P[i][2] != 9999) {
			i_max = i;
		}
	}
	end = i_max;
	
	printf("start : %f | %f\n", jo + P[start][0], io - P[start][1]);
	printf("end : %f | %f\n", jo+P[end][0], io-P[end][1]);

	//P[][]�̍Ĕz��(�Ώۂ̃f�[�^�_���擪�ɗ���悤�ɂ���)
	p = 0;
	for (i = start; i < end + 1; i++) {
		P[p][0] = P[i][0];
		P[p][1] = P[i][1];
		P[p][2] = P[i][2];
		p = p + 1;
	}

	//�Ώۂ̃f�[�^�_�ȍ~��P[][]�̏�����
	for (i = end + 1; i < 10000; i++) {
		P[p][0] = 9999;
		P[p][1] = 9999;
		P[p][2] = 9999;
		p = p + 1;
	}

	/*
	for (int i = 0; i < 10000; i++) {
	printf("%f | %f | %f\n", jo + P[i][0], io - P[i][1], P[i][2]);
	}
	*/

	//�ՊE�d���̋�Ԃ̋ߎ����������߂�

	double SM_Ic[3];

	SaishoJijo(P, SM_Ic);

	//���_�����߂�(�ŏI�I�Ȍ��ʂ͑��ΓI�ȃs�N�Z���ʂɂȂ�̂�img�I�u�W�F�N�g���W�n�֕ϊ�����K�v�͂Ȃ�)

	//�ՊE�d��Ic�����߂�
	double M_Ic[2];

	M_Ic[0] = (P[0][0] + P[end - start][0]) / 2.0; //���_M��x���W(���ی��v�Z���W�n)
	M_Ic[1] = ((SM_Ic[0] * P[0][0] + SM_Ic[1]) + (SM_Ic[0] * P[end - start][0] + SM_Ic[1])) / 2.0; //���_M��y���W(���ی��v�Z���W�n)

	printf("M_Ic : %f | %f\n", jo + M_Ic[0], io - M_Ic[1]);
	printf(" Ic : %f\n", M_Ic[1]);

	//data�t�@�C���ɏ����o��------------------------------------------------------------------------------------------
	
	FILE *output_Ic_N50model;
	output_Ic_N50model = fopen("output_Ic_N50model.data", "a"); //�ēx�C�㏑������ꍇ�͂��̃t�@�C���������Ă�����s����K�v������
	
	char *ptr1, moji1[64];//filename�����������Ȃ��悤��moji1[]�ɃR�s�[���Ă���
	strcpy(moji1, filename);
	ptr1 = moji1;
	strcpy(ptr1, filename);
	ptr1 = strtok(ptr1, "_");
	ptr1 = strtok(NULL, ".");

	fprintf(output_Ic_N50model, "%f %f %s\n", IPy, M_Ic[1], ptr1);
	//fprintf(output_Ic_N50model, "%c%c%c%c %f %f\n",filename[11], filename[12], filename[13], filename[14], IPy, M_Ic[1]);
	
	fclose(output_Ic_N50model);
	
	//Ic�̈ʒu���摜���ɕ`�悵�ď����o��(�e�f�[�^���Ƃ�)------------------------------------------------------------------------------------------


	//jth_max�ʒu�`��m�F�p
	for (int i = 0; i < height; i++) {
		img[i][jth_max * 3 + 0] = 0;
		img[i][jth_max * 3 + 1] = 255;
		img[i][jth_max * 3 + 2] = 255;
	}

	//Ic�̈ʒu��`�悷��(�\����ɕ`�悷��)
	int a, b;
	a = (int)M_Ic[0] + (int)jo;
	b = (int)io - (int)M_Ic[1];

	for (int j = a - 20; j < a + 20 && j<width; j++) {
		img[b][j * 3 + 0] = 255;
		img[b][j * 3 + 1] = 0;
		img[b][j * 3 + 2] = 0;
	}

	for (int i = b - 20; i < b + 20 && i<height; i++) {
		img[i][a * 3 + 0] = 255;
		img[i][a * 3 + 1] = 0;
		img[i][a * 3 + 2] = 0;
	}

	//���_�̈ʒu��`�悷��(�\����ɕ`�悷��)
	a = (int)jo;
	b = (int)io;

	for (int j = a - 20; j < a + 20 && j < width; j++) {
		img[b][j * 3 + 0] = 0;
		img[b][j * 3 + 1] = 255;
		img[b][j * 3 + 2] = 0;
	}

	for (int i = b - 20; i < b + 20 && i < height; i++) {
		img[i][a * 3 + 0] = 0;
		img[i][a * 3 + 1] = 255;
		img[i][a * 3 + 2] = 0;
	}

	output_JPEGscan(img, filename);


	//���I�z��̔j��
	for (int i = 0; i < 10000; i++) {
		free(P[i]);
	}
	free(P);

	//printf("height : %d\n",cinfo.output_height);
	//printf("width : %d\n", cinfo.output_width);
	//printf("output_scanline : %d\n", cinfo.output_scanline);

	/* �f�o�b�O�p
	for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
	printf("(R)---img[%d][%d] = %d , (G)---img[%d][%d] = %d , (B)---img[%d][%d] = %d \n", i, j*3+0, img[i][j*3+0], i, j*3+1, img[i][j * 3 + 1], i, j*3+2, img[i][j * 3 + 2]);
	}
	}
	*/

	/* img�I�u�W�F�N�g�̔j��*/
	for (int i = 0; i<height; i++) {
		free(img[i]);
	}
	free(img);
	fclose(fp);


}

void IcChecker_N200model(char *filename, double IPx, double IPy, double Vran, double Iran, double Th_max) {

	//JPEG�t�@�C���̓ǂݍ���---------------------------------------------------------------------------

	//�I�u�W�F�N�g�̊m�ۂƏ�����
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *fp = fopen(filename, "rb");

	if (fp == NULL) {
		fprintf(stderr, "cannat open %s\n", filename);
		exit(EXIT_FAILURE);
	}
	jpeg_stdio_src(&cinfo, fp);

	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	int width, height;
	width = cinfo.output_width;
	height = cinfo.output_height;

	JSAMPARRAY img = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);

	for (int i = 0; i < height; i++) {
		img[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * 3 * width);
	}

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, img + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
		//printf("scanline : %d\n",cinfo.output_scanline);
	}


	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);


	//���_�����߂�(img�I�u�W�F�N�g���W�n�ł̌v�Z)---------------------------------------------------------------------------
	//�J���[�̂܂܏������邱�Ƃ�臒l���L�c���ݒ�ł���(235 �� 190�ɂ���)
	int i, j;
	//double io, jo, IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, OP[2];
	double io, jo, OP[2];

	// ���X�L����
	// j != width �̏�����t�����邱�Ƃŉ摜�f�[�^�̃t�`�̍��F�����o�����s���h��
	// 3��A���Ō��_�ʒu�����X�ɒ��ׂ邱�Ƃŗאڂ������̌댟�o��h��(3��ڂ̈ʒu���ŏI�I�Ȍ��_�ʒu�Ƃ��Ē�߂�)

	// 1���
	for (i = height - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	// 2���(�O�Ō��������_�ʒu�����㑤������ɑ������Ă���)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	// 3���(�O�Ō��������_�ʒu�����㑤������ɑ������Ă���)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				OP[0] = (jo / IPx)*Vran; //x�̒l						(�K�v�Ȃ�����)
				OP[1] = (io / IPy)*Iran; //y�̒l						(�K�v�Ȃ�����)
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	//printf("%f %f\n", jo, io+3.0);
	
	//printf("%f %f\n", jo, io);

	double x[30000] = { 0 }, y[30000] = { 0 };
	int k = 0, S_g = 0;
	double SMg[n_g] = { 0 };
	//��l������(�f�[�^�_����=0�ɂȂ�)(img�I�u�W�F�N�g���W�n �� ���ی��v�Z���W�n)---------------------------------------------------------------------------
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			if (img[i][j * 3 + 1] >= 190 && img[i][j * 3 + 2] >= 100) {
				img[i][j * 3 + 0] = 0;
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				x[k] = ((double)j - jo) / 100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)(x/100)
				y[k] = (io - (double)i) / 100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)(y/100)
				k++;
			}
			else {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}
	
	//�ߎ��Ȑ������߂�
	S_g = k;
	sai(x, y, S_g, SMg);

	//output_JPEG(img);


	//臒l�����߁C�X�L���������l�Ɣ�r���ăf�[�^��I�肷��(�ߎ������̍��W�n��img�I�u�W�F�N�g�̍��W�n�͈قȂ�)---------------------------------------------------------------------------
	//�ߎ��������O���ɍs���قǂǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�����܂ł��Ǐ��I�Ȓl�Ƃ��Ĉ����K�v������
	//��R���(jth_max�ȍ~�̒�������)�̃f�[�^�Q�͏����Ȃ��悤�ɂ���

	double Y;
	for (int j = 0; j < (int)(jo + (Th_max*IPx) / Vran) && j < width; j++) {
		for (int i = 0; i < height; i++) {

			//�����ɂ�����e�l�ɉ������@������臒l���Z�o����
			Y = 0.0;
			for (int k = 0; k<N_g; k++) {
				Y += SMg[k] * pow((j - jo) / 100, k); //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������͉����������l��)
			}
			Y = io - Y * 100; //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������ŏc���݂̂��l��)
							  //printf("%f\n",Y);
							  //�X�L���������l��臒l���r���ăf�[�^��I�肷�� ( j < jo �̏����ɂ���Č��_���}�C�i�X�����̃f�[�^������ = �m�C�Y��������) 
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 3 || j < jo) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//�ߎ��Ȑ��`��m�F�p
			if (i == (int)Y) {
			//�ߎ��������O���ɍs���ق� Y���ǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�O���ɂ��Ԃ��`�悳���\��������
			img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);


	//�ēximg�I�u�W�F�N�g��ǂݍ���ŋߎ��Ȑ������߂�---------------------------------------------------------------------------

	//x[],y[]�̏�����
	for (int i = 0; i < 30000; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	//SMg[]�̏�����
	for (int i = 0; i < n_g; i++) {
		SMg[i] = 0;
	}

	//x[],y[]��img�I�u�W�F�N�g�̃f�[�^���i�[���ߎ��Ȑ������߂�
	k = 0;
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {
				x[k] = ((double)j - jo) / 100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)
				y[k] = (io - (double)i) / 100; //���ی��v�Z���W�n�ɕϊ�(�I�[�o�[�t���[�⌅������h���Ӑ}������)
				k++;
			}
		}
	}

	S_g = k;
	sai(x, y, S_g, SMg);
	
	//�ēx�C臒l�����߁C�X�L���������l�Ɣ�r���ăf�[�^��I�肷��(�ߎ������̍��W�n��img�I�u�W�F�N�g�̍��W�n�͈قȂ�)---------------------------------------------------------------------------

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			//�����ɂ�����e�l�ɉ������@������臒l���Z�o����
			Y = 0.0;
			for (int k = 0; k < N_g; k++) {
				Y += SMg[k] * pow((j - jo) / 100, k); //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������͉����������l��)
			}
			Y = io - Y * 100; //(���ی��v�Z���W�n �� img�I�u�W�F�N�g���W�n : �������ŏc���݂̂��l��)

							  //�X�L���������l��臒l���r���ăf�[�^��I�肷��
							  //���ڂ̂ق����ߎ��Ȑ��̐��x���オ��̂ŁC���̕��f�[�^�I�莞�̃}�[�W�����L���Ƃ邱�ƂŁC�c�������f�[�^�_�𑽂��c����悤�ɂ���
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 9) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			//�L���ȃf�[�^�_�̋�Ԃł͋ߎ�������100�s�N�Z����̃f�[�^������ = �m�C�Y��������
			if (img[i][j * 3 + 1] == 0 && j >= 0 && i < (int)Y - 100) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//�ߎ��Ȑ��`��m�F�p
			if (i == (int)Y) {
			//�ߎ��������O���ɍs���ق� Y���ǂ��Ȃ邩���\�z�ł��Ȃ��̂ŁC�O���ɂ��Ԃ��`�悳���\��������
			img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);

	//��R��Ԃ̋ߎ������̌X�������߂�(���ی��v�Z���W�n�ł̌v�Z)---------------------------------------------------------------------------
	double **P; //���I�z��̊m�� (�f�[�^�_���i�[���邽�߁F���ی��v�Z���W�n�ɕω������f�[�^���i�[����)

	P = malloc(sizeof(double*) * 10000);
	for (int i = 0; i < 10000; i++) {
		P[i] = malloc(sizeof(double) * 3);
	}

	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double it, jt, SM[3], M_r[2];
	int jth_max, p = 0, last_i = 0;

	jth_max = (int)(jo + (Th_max*IPx) / Vran);
	//printf("%d\n",jth_max);

	//printf(" width : %d | height : %d\n",width,height);

	//jth_max�ȍ~�̓˔��I�ȃm�C�Y���J�b�g����(2�A���܂ł������Ώۂɂ���)
	for (int j = jth_max; j < width-1;j++) {
		for (int i = 1; i < height-1;i++) {
			if (img[i][j*3+1]==0 && (img[i - 1][j * 3 + 1] + img[i + 1][j * 3 + 1] + img[i][(j - 1) * 3 + 1] + img[i][(j + 1) * 3 + 1]) >= 765) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}


	for (j = jth_max; j < width; j++) {
		for (i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {

				it = (double)i;
				jt = (double)j;

				P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
				P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
				p = p + 1;

			}
		}
	}
	//printf("%lf | %lf | %lf | %lf\n",P[0][0],P[0][1],it,jt);
	//printf("%d | %d | %lf | %lf\n",i,j,io,jo);
	
	/*
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
		printf("P[%d][0] : %lf  P[%d][1] : %lf\n",i,jo+P[i][0], i, io-P[i][1]);
		//printf("P[%d][0] : %lf  P[%d][1] : %lf\n", i, P[i][0], i, P[i][1]);
		}
	}
	*/

	SaishoJijo(P, SM); // SM[0]=a, SM[1]=b, SM[2]=theta(��) �ɂȂ��Ă���

					   //���_ M(x,y) �����߂�(���ꂪ��]���S�ɂȂ�Bdouble�^�Ȃ̂Œ���)
	M_r[0] = (P[p - 1][0] + (jth_max - jo)) / 2.0; //���_M��x���W(���ی��v�Z���W�n)
	M_r[1] = ((SM[0] * P[p - 1][0] + SM[1]) + (SM[0] * (jth_max - jo) + SM[1])) / 2.0; //���_M��y���W(���ی��v�Z���W�n)

																					   //printf("a : %lf | b : %lf | theta : %lf\n", SM[0], SM[1], SM[2]);
																					   //printf("x : %lf | y : %lf\n", M_r[0], M_r[1]);

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			Y = 0.0;
			Y = io - (SM[0] * (j - jo) + SM[1]);

			//��R��ԂɂȂ������ォ��̃f�[�^�����
			if (img[i][j * 3 + 1] == 0) {
				if (abs(i - (int)Y) <= 25) {
					img[i][j * 3 + 0] = 255;
					img[i][j * 3 + 1] = 255;
					img[i][j * 3 + 2] = 255;
				}

				/*
				//�ߎ��Ȑ��`��m�F�p
				if (i == (int)Y) {
				img[i][j * 3 + 0] = 255; //�`�悳���̂͐^���Ԃł͂Ȃ�����
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				}
				*/
			}

		}
	}

	/*
	//jth_max�ʒu�`��m�F�p
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	*/

	/*
	//���_�`��m�F�p
	int a, b;
	a = (int)M_r[0] + (int)jo;
	b = (int)io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//output_JPEG(img);

	//�ϑ��_�ɂ������]���S�Ƃ̊p�x��(��)�����߂�---------------------------------------------------------------------------
	//�ēxP[][]��������
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double theta_0, theta_scan, theta_max;
	p = 0;
	double jm, im; //atan2()�ɓ����s����double�^�Œ�`����
	jm = M_r[0] + jo;
	im = io - M_r[1];

	theta_0 = SM[2] - 180.0; //SM[2]��(��)�Fdegree�\�L (���͕��̒l�ɂȂ�ƍl������)

							 //���̒i�K�ł�theta_max���ǂ����̔���͍s��Ȃ�
	for (int j = jth_max; j > 0; j--) {
		for (int i = 0; i < height; i++) {
			theta_scan = (180.0 / M_PI)*atan2(im - (double)i, (double)j - jm);

			it = (double)i;
			jt = (double)j;

			if (img[i][j * 3 + 1] == 0) {

				if (theta_scan < 0) {
					P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][2] = fabs(theta_scan) - fabs(theta_0);
					p = p + 1;
				}
				else if (theta_scan >= 0) {
					P[p][0] = jt - jo; // �������͌��̌����̂܂܂Ŗ��Ȃ� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][1] = io - it; // ���ی��� +i(y����) ����� +j(x����) �Ƃ��Ĉ�����悤�ɕϊ�����B�Ȃ����_����̋����ɂ��� (�f�[�^�̓s�N�Z���f�[�^�̂܂�)
					P[p][2] = (180.0 - theta_scan) + (180.0 - fabs(theta_0));
					p = p + 1;
				}

			}
		}
	}

	/*
	for (int i = 0; i < 10000; i++) {
	printf("P[%d][2] : %lf\n",i,P[i][2]);
	}
	*/

	int start, end, i_max;
	theta_max = P[0][2];
	i = 0;
	i_max = i;
	while (P[i][2] <= 4) {
		i++;
	}
	start = i;

	for (i = 0; i < 10000; i++) {
		if (P[i][2] >= theta_max && P[i][2] != 9999) {
			theta_max = P[i][2];
			i_max = i;
		}
	}
	end = i_max;

	//printf("start : %d | end : %d\n", start, end);
	//printf("start : %f | %f\n", jo+P[start][0], io-P[start][1]);
	//printf("end : %f | %f\n", jo+P[end][0], io-P[end][1]);

	/*
	//P[][]�̍Ĕz��(�Ώۂ̃f�[�^�_���擪�ɗ���悤�ɂ���)
	p = 0;
	for (i = start; i < end + 1; i++) {
		P[p][0] = P[i][0];
		P[p][1] = P[i][1];
		P[p][2] = P[i][2];
		p = p + 1;
	}

	//�Ώۂ̃f�[�^�_�ȍ~��P[][]�̏�����
	for (i = end + 1; i < 10000; i++) {
		P[p][0] = 9999;
		P[p][1] = 9999;
		P[p][2] = 9999;
		p = p + 1;
	}
	*/

	//��x���߂� theta_max������ɐ������Ă���------------------------------------------------------------------------------------------
	//�܂������Ɍ������ăX�L�������Ă����C�����ɍő�l�̉ӏ������ɂȂ������ׂ�
	int i_before = i_max;
	double Ic_kouho;
	for (i = end; i < 10000; i++) {
		
		//Ic�̌���I�o
		if (P[i][1] < P[i_before][1] && P[i][2] != 9999) {
			Ic_kouho = P[i_before][1];

			if (Ic_kouho >= P[i_max][1] && P[i][0] != P[i_before][0]) {
				i_max = i_before;
			}

		}

		// ���� j �̏��𖳎��ł���悤�ɂ��邽��(i!=0�̓G���[��h������)
		if (P[i][0] != P[i-1][0] && i != 0) {
			i_before = i;
		}
		
	}

	//end���X�V����
	end = i_max;
	//printf("end : %f | %f\n", jo + P[end][0], io - P[end][1]);
	//printf("end : %d\n",end);

	//���ɉE���Ɍ������ăX�L�������Ă����C�E���ɍő�l�̉ӏ������ɂȂ������ׂ�
	//Ic����R��Ԃ܂ŏ��X�ɏオ���Ă����悤�Ȍ`��̏ꍇ�ɁC�����Ic�̍ő�l���X�V����ꍇ������̂Ń}�[�W����݂��邱�ƂŖh��
	//for (i = end; (int)(P[i][0]+jo) < jth_max; i--) {}
	i_before = i_max;
	int i_forward=1;

	for (i = end; i > 0; i--) {
		
		//printf("i_forward : %d\n", i_forward);
		//printf("P[%d][0] : %f\n", i, P[i][0]);

		//Ic�̌���I�o
		if (P[i][1] < P[i_before][1] && P[i][2] != 9999 && i==i_forward) {
			Ic_kouho = P[i_before][1];
			
			if (Ic_kouho > P[i_max][1] && P[i][0] != P[i_before][0]) {
				i_max = i_before;
			}
		}

		// ���� j �̏��𖳎��ł���悤�ɂ��邽��
		if (P[i][0] != P[i - 1][0] && i != 0) {
			i_forward = i - 1;

			for (k = i_forward; k>0; k--) {
				if (P[k][0] == P[k - 1][0] && k != 0) {
					i_forward = k - 1;
				}
				else break;
			}
			
			//printf("i_forward : %d\n", i_forward);
			i_before = i;
		}
		
	}

	//end���ēx�X�V����
	end = i_max;
	//printf("end : %f | %f\n", jo + P[end][0], io - P[end][1]);
	//�ՊE�d���̋�Ԃ̋ߎ����������߂�
	/*
	double SM_Ic[3];

	SaishoJijo(P, SM_Ic);

	//���_�����߂�(�ŏI�I�Ȍ��ʂ͑��ΓI�ȃs�N�Z���ʂɂȂ�̂�img�I�u�W�F�N�g���W�n�֕ϊ�����K�v�͂Ȃ�)
	
	//�ՊE�d��Ic�����߂�
	double M_Ic[2];

	M_Ic[0] = (P[0][0] + P[end - start][0]) / 2.0; //���_M��x���W(���ی��v�Z���W�n)
	M_Ic[1] = ((SM_Ic[0] * P[0][0] + SM_Ic[1]) + (SM_Ic[0] * P[end - start][0] + SM_Ic[1])) / 2.0; //���_M��y���W(���ی��v�Z���W�n)

	printf("M_Ic : %f | %f\n", jo + M_Ic[0], io - M_Ic[1]);
	printf(" Ic : %f\n", M_Ic[1]);
	*/


	//������theta_max�̒n�_�̐��������߂�
	int i_LINE_wid_end=0;
	double LINE_wid;

	j = (int)(jo + P[end][0]);

	for (int i = (int)(io - P[end][1]); i < (int)(io - P[end][1]) + 17 && i < height; i++) {
		if (img[i][j * 3 + 1] == 0) {
			i_LINE_wid_end = i;
		}
	}
	LINE_wid = (double)(i_LINE_wid_end - (int)(io - P[end][1]));

	printf("LINEwid : %f\n",LINE_wid);
	//printf("i_LINEwid_end : %d\n", i_LINE_wid_end);
	//printf("P[end][1] : %f\n", P[end][1]);
	printf("Point_Ic : %f | %f\n", jo + P[end][0], io - (P[end][1] - LINE_wid / 2.0));

	double Ic[2];

	Ic[0] = P[end][0];
	Ic[1] = P[end][1] - (LINE_wid / 2.0); //�����̒��S��Ic�ɂ���

	printf(" Ic : %f\n", Ic[1]);

	//data�t�@�C���ɏ����o��------------------------------------------------------------------------------------------
	
	FILE *output_Ic_N200model;
	output_Ic_N200model = fopen("output_Ic_N200model.data", "a"); //�ēx�C�㏑������ꍇ�͂��̃t�@�C���������Ă�����s����K�v������
	
	char *ptr1, moji1[64];//filename�����������Ȃ��悤��moji1[]�ɃR�s�[���Ă���
	strcpy(moji1, filename);
	ptr1 = moji1;
	strcpy(ptr1, filename);
	ptr1 = strtok(ptr1, "_");
	ptr1 = strtok(NULL, ".");

	fprintf(output_Ic_N200model, "%f %f %s\n", IPy, Ic[1], ptr1);
	//fprintf(output_Ic_N200model, "%c%c%c%c %f %f\n", filename[11], filename[12], filename[13], filename[14], IPy, Ic[1]);

	fclose(output_Ic_N200model);

	//Ic�̈ʒu���摜���ɕ`�悵�ď����o��(�e�f�[�^���Ƃ�)------------------------------------------------------------------------------------------

	
	//jth_max�ʒu�`��m�F�p
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	
	//Ic�̈ʒu��`�悷��(�\����ɕ`�悷��)
	int a, b;
	a = (int)Ic[0] + (int)jo;
	b = (int)io - (int)Ic[1];

	for (int j = a-20; j < a+20 && j<width; j++) {
		img[b][j * 3 + 0] = 255;
		img[b][j * 3 + 1] = 0;
		img[b][j * 3 + 2] = 0;
	}

	for (int i = b-20; i < b+20 && i<height; i++) {
		img[i][a * 3 + 0] = 255;
		img[i][a * 3 + 1] = 0;
		img[i][a * 3 + 2] = 0;
	}
	
	//���_�̈ʒu��`�悷��(�\����ɕ`�悷��)
	a = (int)jo;
	b = (int)io;

	for (int j = a - 20; j < a + 20 && j < width; j++) {
		img[b][j * 3 + 0] = 0;
		img[b][j * 3 + 1] = 255;
		img[b][j * 3 + 2] = 0;
	}

	for (int i = b - 20; i < b + 20 && i < height; i++) {
		img[i][a * 3 + 0] = 0;
		img[i][a * 3 + 1] = 255;
		img[i][a * 3 + 2] = 0;
	}

	output_JPEGscan(img,filename);//��������s����O��filename��ύX����悤�ȏ��������Ȃ�����

	/*
	//���_�`��m�F�p
	int a, b;
	a = (int)M_r[0] + (int)jo;
	b = (int)io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//���I�z��̔j��
	for (int i = 0; i < 10000; i++) {
		free(P[i]);
	}
	free(P);

	//printf("height : %d\n",cinfo.output_height);
	//printf("width : %d\n", cinfo.output_width);
	//printf("output_scanline : %d\n", cinfo.output_scanline);

	/* �f�o�b�O�p
	for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
	printf("(R)---img[%d][%d] = %d , (G)---img[%d][%d] = %d , (B)---img[%d][%d] = %d \n", i, j*3+0, img[i][j*3+0], i, j*3+1, img[i][j * 3 + 1], i, j*3+2, img[i][j * 3 + 2]);
	}
	}
	*/

	/* img�I�u�W�F�N�g�̔j��*/
	for (int i = 0; i<height; i++) {
		free(img[i]);
	}
	free(img);
	fclose(fp);


}


int main() {

	//int S_g = 5;
	//double x[] = { 1.0,2.0,3.0,4.0,5.0 }, y[] = { 7.987,2.986,1.998,2.224,5.678 };
	//double SMg[4] = {0};

	/*�f�[�^�̉����ɒl���鐔�l��z��x�ɏ��������ɓ���A
	���ꂼ��̉����̒l�ɂ�����c���̒l��z��y�ɓ���֐�sai�ɓn��*/
	//sai(x, y, S_g, SMg);

	//Vran[V], Iran[A]
	//Ryo.05-9
	double IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, Th_max = 80e-3;

	//Ryo.2-3
	//double IPx = 114, IPy = 114, Vran = 50e-3, Iran = 0.5e-3, Th_max = 225e-3;

	//Ryo.3-1
	//double IPx = 114, IPy = 114, Vran = 100e-3, Iran = 0.1e-3, Th_max = 600e-3;

	//Ryo.05-9(���s����)
	//double IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, Th_max = 80e-3;

	//Ryo.2-3(���s����)
	//double IPx = 114, IPy = 114, Vran = 50e-3, Iran = 0.5e-3, Th_max = 275e-3;
	
	//Ryo.2-2(���s����)
	//double IPx = 114, IPy = 114, Vran = 200e-3, Iran = 0.1e-3, Th_max = 1000e-3;
	
	//Ryo.2-2(���s����)
	//double IPx = 114, IPy = 114, Vran = 200e-3, Iran = 0.2e-3, Th_max = 800e-3;

	//Ryo.3-7
	//double IPx = 114, IPy = 114, Vran = 100e-3, Iran = 1e-3, Th_max = 500e-3;

	//�摜�f�[�^�̃p�����[�^�����
	char mode = 'k';
	
	printf("mode 'a' or 'm' ( a:Average  m:Max ): ");
	scanf_s("%c",&mode,2);

	if (mode != 'a' && mode != 'm') {
		printf("Hen na atai wo nyuuryoku sinai de kudasai\n");
		exit(EXIT_FAILURE);
	}
	
	printf("�����l����͂̂Ƃ��C�e�l�̊Ԃ̓X�y�[�X�����Ă�������\n");
	printf("When the input, Please put a space between each value\n\n");
	printf("IPx[pixel] IPy[pixel] Vrange[mV] Irange[mA] Th_max[mV]: ");

	scanf_s("%lf %lf %lf %lf %lf",&IPx, &IPy, &Vran, &Iran, &Th_max);

	Vran = Vran * 1e-3;
	Iran = Iran * 1e-3;
	Th_max = Th_max * 1e-3;
	

	/*���̓t�@�C���̐ݒ�*/

	char str1[24] = "./pics/IMG_";
	//char str1[24] = "IMG_";
	char str2[24] = "\0";
	char str3[24] = ".jpg";

	char str4[24] = "\0";
	
	printf("Picture Number(start):");
	scanf_s("%s",str2,24);

	printf("Picture Number(end):");
	scanf_s("%s", str4, 24);

	char filename[48] = {'\0'}, str_num[24];
	int num_start, num_end;

	//1�̉摜�f�[�^��������������
	if (str4[0] == '0' && str4[1] == '\0') {
		printf("���̈ꖇ�����������܂��i�L�E�ցE`�j\n");
		
		strcat(filename, str1);
		strcat(filename, str2);
		strcat(filename, str3);

		if(mode == 'a')IcChecker_N50model(filename, IPx, IPy, Vran, Iran, Th_max);
		if(mode == 'm')IcChecker_N200model(filename, IPx, IPy, Vran, Iran, Th_max);
	}
	else if (str4[1] != '\0') {
		printf("���������������܂��i�L�E�ցE`�j\n");
		num_start = atoi(str2);
		num_end = atoi(str4);
		
		for (int i = num_start; i <= num_end;i++) {
			printf("---------------------------------------- Picture Number : %04d----------�i�L�E�ցE`�j\n",i);
			char filename2[48] = { '\0' };

			sprintf_s(str_num,24,"%04d",i);
			strcat(filename2, str1);
			strcat(filename2, str_num);
			strcat(filename2, str3);
						
			if(mode == 'a')IcChecker_N50model(filename2, IPx, IPy, Vran, Iran, Th_max);
			if(mode == 'm')IcChecker_N200model(filename2, IPx, IPy, Vran, Iran, Th_max);
			printf("\n\n");
		}

	}
	
	printf("\a");
	//char *filename = "IMG_6939.jpg";
	
}