#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<jpeglib.h>
#include<corecrt_math_defines.h>
#include<string.h>

void output_JPEG(JSAMPARRAY img) {

	//---------------------------------------------------------------------------------
	/* JPEGオブジェクト，エラーハンドラの確保*/
	struct jpeg_compress_struct cinfo2;
	struct jpeg_error_mgr jerr2;

	/* エラーハンドラにデフォルト値を設定*/
	cinfo2.err = jpeg_std_error(&jerr2);

	/* JPEGオブジェクトの追加*/
	jpeg_create_compress(&cinfo2);

	/*出力ファイルの設定*/

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

	/*圧縮開始*/
	jpeg_start_compress(&cinfo2, TRUE);

	/*書き込む*/
	jpeg_write_scanlines(&cinfo2, img, height);

	/*圧縮終了*/
	jpeg_finish_compress(&cinfo2);

	/*JPEGオブジェクトの破棄*/
	jpeg_destroy_compress(&cinfo2);

	fclose(fp2);

	//---------------------------------------------------------------------------------

}

void output_JPEGscan(JSAMPARRAY img, char *filename) {

	//---------------------------------------------------------------------------------
	/* JPEGオブジェクト，エラーハンドラの確保*/
	struct jpeg_compress_struct cinfo3;
	struct jpeg_error_mgr jerr3;

	/* エラーハンドラにデフォルト値を設定*/
	cinfo3.err = jpeg_std_error(&jerr3);

	/* JPEGオブジェクトの追加*/
	jpeg_create_compress(&cinfo3);

	/*出力ファイルの設定*/

	char *ptr, moji[64];//filenameを書き換えないようにmoji[]にコピーしておく
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

	/*圧縮開始*/
	jpeg_start_compress(&cinfo3, TRUE);

	/*書き込む*/
	jpeg_write_scanlines(&cinfo3, img, height);

	/*圧縮終了*/
	jpeg_finish_compress(&cinfo3);

	/*JPEGオブジェクトの破棄*/
	jpeg_destroy_compress(&cinfo3);

	fclose(fp3);

	//---------------------------------------------------------------------------------

}


#define n_g 9             //n_g-1次の多項式で近似（１以上 = 直線近似はできない）
#define N_g 9             //ガウスの消去法における未知数の数（上のn_gと同じ値にすること）
//#define S_g 5             //データの個数
#define CHECK_g 0 //ガウスの消去法における三角行列のチェック用(1にすればチェックできる)

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
	
	/*初期化*/
	for (i = 0; i<n_g; i++) {
		for (j = 0; j<n_g + 1; j++) {
			A[i][j] = 0.0;
		}
	}

	/*ガウスの消去法で解く行列の作成*/
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
	/*ガウスの消去法の実行（配列xxは解、すなわち多項式の係数を入れるためのもの）*/
	gauss(A, xx); // y = a + b * xi^1 + c * xi^2 + d * xi^3 + ...

	/* 出力用配列に代入*/
	for (int i = 0; i<N_g; i++) {
		SMg[i] = xx[i];
	}

	/*
	//GNUPLOTで表示するために最小２乗法による関数のデータをファイル保存
	for (X = x[0] - 10.0; X<x[S_g-1] + 10.0; X += 0.01) {
		Y = 0.0;
		for (i = 0; i<N_g; i++) {
			Y += xx[i] * pow(X, i);
		}
		fprintf(output1, "%f %f\n", X, Y);
	}

	//GNUPLOTで表示するために、最小２乗法に使われたデータを保存
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
			if (fabs(a[l][i])>m) {   //i列の中で一番値が大きい行を選ぶ
				m = fabs(a[l][i]);
				pivot = l;
			}
		}

		if (pivot != i) {                          //pivotがiと違えば、行の入れ替え
			for (j = 0; j<N_g + 1; j++) {
				b[0][j] = a[i][j];
				a[i][j] = a[pivot][j];
				a[pivot][j] = b[0][j];
			}
		}
	}

	for (k = 0; k<N_g; k++) {
		p = a[k][k];              //対角要素を保存
		a[k][k] = 1;              //対角要素は１になることがわかっているから

		for (j = k + 1; j<N_g + 1; j++) {
			a[k][j] /= p;
		}

		for (i = k + 1; i<N_g; i++) {
			q = a[i][k];

			for (j = k + 1; j<N_g + 1; j++) {
				a[i][j] -= q * a[k][j];
			}
			a[i][k] = 0;              //０となることがわかっているところ
		}
	}

	//解の計算
	for (i = N_g - 1; i >= 0; i--) {
		x[i] = a[i][N_g];
		for (j = N_g - 1; j>i; j--) {
			x[i] -= a[i][j] * x[j];
		}
	}

	//行列が最後どうなったか見たいときに実行
#if CHECK_g==1
	for (i = 0; i<N_g; i++) {
		for (j = 0; j<N_g + 1; j++) {
			printf("%10.3f", a[i][j]);
		}
		printf("\n");

	}
#endif
	
	//printf("解は\n");
	for (i = 0; i<N_g; i++) {
		//printf("%f\n", x[i]);
		xx[i] = x[i];
	}
	
}

/* 最小二乗法(直線近似用) */
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

	SM[0] = (N*Sxiyi - Sxi*Syi) / (N*Sxi2 - Sxi*Sxi); // aを求める
	SM[1] = (Sxi2*Syi - Sxi*Sxiyi) / (N*Sxi2 - Sxi*Sxi); // bを求める
	SM[2] = (180.0 / M_PI)*atan(SM[0]); // theta(°)を求める
	

	printf("a : %lf | b : %lf | theta : %lf | N : %d\n", SM[0], SM[1], SM[2], N);


}

void IcChecker_N50model(char *filename, double IPx, double IPy, double Vran, double Iran, double Th_max) {

	//JPEGファイルの読み込み---------------------------------------------------------------------------

	//オブジェクトの確保と初期化
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


	//原点を求める(imgオブジェクト座標系での計算)---------------------------------------------------------------------------
	//カラーのまま処理することで閾値をキツく設定できる(235 → 190)
	int i, j;
	//double io, jo, IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, OP[2];
	double io, jo, OP[2];

	// γスキャン
	// j != width の条件を付加することで画像データのフチの黒色が検出される不具合を防ぐ
	// 3回連続で原点位置を執拗に調べることで隣接した線の誤検出を防ぐ(3回目の位置を最終的な原点位置として定める)
	
	// 1回目
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
	
	// 2回目(前で見つけた原点位置よりも上側をさらに走査していく)
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

	// 3回目(前で見つけた原点位置よりも上側をさらに走査していく)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				OP[0] = (jo / IPx)*Vran; //xの値						(必要ないかも)
				OP[1] = (io / IPy)*Iran; //yの値						(必要ないかも)
				break;
			}
		}
		if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) break;
	}

	//printf("%f %f\n", jo, io+3.0); ←値を補正するためにもともとio-3.0としていた名残である

	//printf("%f %f\n", jo, io);

	double x[30000] = { 0 }, y[30000] = { 0 };
	int k = 0, S_g = 0;
	double SMg[n_g] = { 0 };
	//二値化する(データ点が黒=0になる)(imgオブジェクト座標系 → 第一象限計算座標系)---------------------------------------------------------------------------
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			if (img[i][j * 3 + 1] >= 190 && img[i][j * 3 + 2] >= 100) {
				img[i][j * 3 + 0] = 0;
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				x[k] = ((double)j - jo)/100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)(x/100)
				y[k] = (io - (double)i)/100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)(y/100)
				k++;
			}
			else {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}
	
	//近似曲線を求める
	S_g = k;
	sai(x, y, S_g, SMg);

	//output_JPEG(img);


	//閾値を求め，スキャンした値と比較してデータを選定する(近似直線の座標系とimgオブジェクトの座標系は異なる)---------------------------------------------------------------------------
	//近似直線が外側に行くほどどうなるかが予想できないので，あくまでも局所的な値として扱う必要がある
	//抵抗状態(jth_max以降の直線部分)のデータ群は消さないようにする

	double Y;
	for (int j = 0; j < (int)(jo + (Th_max*IPx) / Vran) && j < width; j++) {
		for (int i = 0; i < height; i++) {

			//横軸における各値に応じた　高さの閾値を算出する
			Y = 0.0;
			for (int k = 0; k<N_g; k++) {
				Y += SMg[k] * pow((j-jo)/100, k); //(第一象限計算座標系 → imgオブジェクト座標系 : こっちは横軸だけを考慮)
			}
			Y = io - Y*100; //(第一象限計算座標系 → imgオブジェクト座標系 : こっちで縦軸のみを考慮)
						//printf("%f\n",Y);
						//スキャンした値と閾値を比較してデータを選定する ( j < jo の条件によって原点よりマイナス軸側のデータを消す = ノイズを消せる) 
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 3 || j < jo) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//近似曲線描画確認用
			if (i == (int)Y) {
			//近似直線が外側に行くほど Yがどうなるかが予想できないので，外側にも赤く描画される可能性がある
			img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);


	//再度imgオブジェクトを読み込んで近似曲線を求める---------------------------------------------------------------------------

	//x[],y[]の初期化
	for (int i = 0; i < 30000; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	//SMg[]の初期化
	for (int i = 0; i < n_g; i++) {
		SMg[i] = 0;
	}

	//x[],y[]へimgオブジェクトのデータを格納し近似曲線を求める
	k = 0;
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {
				x[k] = ((double)j - jo)/100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)
				y[k] = (io - (double)i)/100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)
				k++;
			}
		}
	}

	S_g = k;
	sai(x, y, S_g, SMg);

	//再度，閾値を求め，スキャンした値と比較してデータを選定する(近似直線の座標系とimgオブジェクトの座標系は異なる)---------------------------------------------------------------------------

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			//横軸における各値に応じた　高さの閾値を算出する
			Y = 0.0;
			for (int k = 0; k < N_g; k++) {
				Y += SMg[k] * pow((j - jo)/100, k); //(第一象限計算座標系 → imgオブジェクト座標系 : こっちは横軸だけを考慮)
			}
			Y = io - Y*100; //(第一象限計算座標系 → imgオブジェクト座標系 : こっちで縦軸のみを考慮)

						//スキャンした値と閾値を比較してデータを選定する
						//二回目のほうが近似曲線の精度が上がるので，その分データ選定時のマージンを広くとることで，残したいデータ点を多く残せるようにする
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 9) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			//有効なデータ点の区間では近似線から100ピクセル上のデータを消す = ノイズを消せる
			if (img[i][j * 3 + 1] == 0 && j >= 0 && i < (int)Y - 100) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//近似曲線描画確認用
			if (i == (int)Y) {
			//近似直線が外側に行くほど Yがどうなるかが予想できないので，外側にも赤く描画される可能性がある
			img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}
	
	//output_JPEG(img);

	//抵抗状態の近似直線の傾きを求める(第一象限計算座標系での計算)---------------------------------------------------------------------------
	double **P; //動的配列の確保 (データ点を格納するため：第一象限計算座標系に変化したデータを格納する)

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

	//jth_max以降の突発的なノイズをカットする(2連続までを消去対象にする)
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

				P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
				P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
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

	SaishoJijo(P, SM); // SM[0]=a, SM[1]=b, SM[2]=theta(°) になっている

					   //中点 M(x,y) を求める(これが回転中心になる。double型なので注意)
	M_r[0] = (P[p - 1][0] + (jth_max - jo)) / 2.0; //中点Mのx座標(第一象限計算座標系)
	M_r[1] = ((SM[0] * P[p - 1][0] + SM[1]) + (SM[0] * (jth_max - jo) + SM[1])) / 2.0; //中点Mのy座標(第一象限計算座標系)

																					   //printf("a : %lf | b : %lf | theta : %lf\n", SM[0], SM[1], SM[2]);
																					   //printf("x : %lf | y : %lf\n", M_r[0], M_r[1]);
	
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			Y = 0.0;
			Y = io - (SM[0] * (j - jo) + SM[1]);

			//抵抗状態になった直後からのデータを削る(25→20)
			if (img[i][j * 3 + 1] == 0) {
				if (abs(i - (int)Y) <= 20) {
					img[i][j * 3 + 0] = 255;
					img[i][j * 3 + 1] = 255;
					img[i][j * 3 + 2] = 255;
				}

				/*
				//近似曲線描画確認用
				if (i == (int)Y) {
				img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				}
				*/
			}

		}
	}

	/*
	//jth_max位置描画確認用
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	*/

	/*
	//中点描画確認用
	int a, b;
	a = (int)M_r[0] + jo;
	b = io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//output_JPEG(img);

	//観測点における回転中心との角度差(°)を求める---------------------------------------------------------------------------
	//再度P[][]を初期化
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double theta_0, theta_scan, theta_max;
	p = 0;
	double jm, im; //atan2()に入れる都合上double型で定義する
	jm = M_r[0] + jo;
	im = io - M_r[1];

	theta_0 = SM[2] - 180.0; //SM[2]は(°)：degree表記 (大抵は負の値になると考えられる)

							 //この段階ではtheta_maxかどうかの判定は行わない
	for (int j = jth_max; j > 0; j--) {
		for (int i = 0; i < height; i++) {
			theta_scan = (180.0 / M_PI)*atan2(im - (double)i, (double)j - jm);

			it = (double)i;
			jt = (double)j;

			if (img[i][j * 3 + 1] == 0) {

				if (theta_scan < 0) {
					P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
					P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
					P[p][2] = fabs(theta_scan) - fabs(theta_0);
					p = p + 1;
				}
				else if (theta_scan >= 0) {
					P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
					P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
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

	
	//一度theta_maxを見つけた後，theta_maxから±0.5°を許容する---------------------------------------------------------------------------
	theta_max = P[i_max][2];

	for (i = end; i < 10000;i++) {
		if (fabs(theta_max - P[i][2]) < 0.5 && P[i][2] != 9999) {
			i_max = i;
		}
	}
	end = i_max;
	
	printf("start : %f | %f\n", jo + P[start][0], io - P[start][1]);
	printf("end : %f | %f\n", jo+P[end][0], io-P[end][1]);

	//P[][]の再配列(対象のデータ点が先頭に来るようにする)
	p = 0;
	for (i = start; i < end + 1; i++) {
		P[p][0] = P[i][0];
		P[p][1] = P[i][1];
		P[p][2] = P[i][2];
		p = p + 1;
	}

	//対象のデータ点以降のP[][]の初期化
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

	//臨界電流の区間の近似直線を求める

	double SM_Ic[3];

	SaishoJijo(P, SM_Ic);

	//中点を求める(最終的な結果は相対的なピクセル量になるのでimgオブジェクト座標系へ変換する必要はない)

	//臨界電流Icを求める
	double M_Ic[2];

	M_Ic[0] = (P[0][0] + P[end - start][0]) / 2.0; //中点Mのx座標(第一象限計算座標系)
	M_Ic[1] = ((SM_Ic[0] * P[0][0] + SM_Ic[1]) + (SM_Ic[0] * P[end - start][0] + SM_Ic[1])) / 2.0; //中点Mのy座標(第一象限計算座標系)

	printf("M_Ic : %f | %f\n", jo + M_Ic[0], io - M_Ic[1]);
	printf(" Ic : %f\n", M_Ic[1]);

	//dataファイルに書き出す------------------------------------------------------------------------------------------
	
	FILE *output_Ic_N50model;
	output_Ic_N50model = fopen("output_Ic_N50model.data", "a"); //再度，上書きする場合はこのファイルを消してから実行する必要がある
	
	char *ptr1, moji1[64];//filenameを書き換えないようにmoji1[]にコピーしておく
	strcpy(moji1, filename);
	ptr1 = moji1;
	strcpy(ptr1, filename);
	ptr1 = strtok(ptr1, "_");
	ptr1 = strtok(NULL, ".");

	fprintf(output_Ic_N50model, "%f %f %s\n", IPy, M_Ic[1], ptr1);
	//fprintf(output_Ic_N50model, "%c%c%c%c %f %f\n",filename[11], filename[12], filename[13], filename[14], IPy, M_Ic[1]);
	
	fclose(output_Ic_N50model);
	
	//Icの位置を画像中に描画して書き出す(各データごとに)------------------------------------------------------------------------------------------


	//jth_max位置描画確認用
	for (int i = 0; i < height; i++) {
		img[i][jth_max * 3 + 0] = 0;
		img[i][jth_max * 3 + 1] = 255;
		img[i][jth_max * 3 + 2] = 255;
	}

	//Icの位置を描画する(十字印に描画する)
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

	//原点の位置を描画する(十字印に描画する)
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


	//動的配列の破棄
	for (int i = 0; i < 10000; i++) {
		free(P[i]);
	}
	free(P);

	//printf("height : %d\n",cinfo.output_height);
	//printf("width : %d\n", cinfo.output_width);
	//printf("output_scanline : %d\n", cinfo.output_scanline);

	/* デバッグ用
	for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
	printf("(R)---img[%d][%d] = %d , (G)---img[%d][%d] = %d , (B)---img[%d][%d] = %d \n", i, j*3+0, img[i][j*3+0], i, j*3+1, img[i][j * 3 + 1], i, j*3+2, img[i][j * 3 + 2]);
	}
	}
	*/

	/* imgオブジェクトの破棄*/
	for (int i = 0; i<height; i++) {
		free(img[i]);
	}
	free(img);
	fclose(fp);


}

void IcChecker_N200model(char *filename, double IPx, double IPy, double Vran, double Iran, double Th_max) {

	//JPEGファイルの読み込み---------------------------------------------------------------------------

	//オブジェクトの確保と初期化
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


	//原点を求める(imgオブジェクト座標系での計算)---------------------------------------------------------------------------
	//カラーのまま処理することで閾値をキツく設定できる(235 → 190にした)
	int i, j;
	//double io, jo, IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, OP[2];
	double io, jo, OP[2];

	// γスキャン
	// j != width の条件を付加することで画像データのフチの黒色が検出される不具合を防ぐ
	// 3回連続で原点位置を執拗に調べることで隣接した線の誤検出を防ぐ(3回目の位置を最終的な原点位置として定める)

	// 1回目
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

	// 2回目(前で見つけた原点位置よりも上側をさらに走査していく)
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

	// 3回目(前で見つけた原点位置よりも上側をさらに走査していく)
	for (i = (int)io - 1; i > 0; i--) {
		for (j = 0; j < width; j++) {
			if (img[i][j * 3 + 0] != 0 && img[i][j * 3 + 1] >= 190 && j != width) {
				io = (double)i;
				jo = (double)j;
				OP[0] = (jo / IPx)*Vran; //xの値						(必要ないかも)
				OP[1] = (io / IPy)*Iran; //yの値						(必要ないかも)
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
	//二値化する(データ点が黒=0になる)(imgオブジェクト座標系 → 第一象限計算座標系)---------------------------------------------------------------------------
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			if (img[i][j * 3 + 1] >= 190 && img[i][j * 3 + 2] >= 100) {
				img[i][j * 3 + 0] = 0;
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				x[k] = ((double)j - jo) / 100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)(x/100)
				y[k] = (io - (double)i) / 100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)(y/100)
				k++;
			}
			else {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}
		}
	}
	
	//近似曲線を求める
	S_g = k;
	sai(x, y, S_g, SMg);

	//output_JPEG(img);


	//閾値を求め，スキャンした値と比較してデータを選定する(近似直線の座標系とimgオブジェクトの座標系は異なる)---------------------------------------------------------------------------
	//近似直線が外側に行くほどどうなるかが予想できないので，あくまでも局所的な値として扱う必要がある
	//抵抗状態(jth_max以降の直線部分)のデータ群は消さないようにする

	double Y;
	for (int j = 0; j < (int)(jo + (Th_max*IPx) / Vran) && j < width; j++) {
		for (int i = 0; i < height; i++) {

			//横軸における各値に応じた　高さの閾値を算出する
			Y = 0.0;
			for (int k = 0; k<N_g; k++) {
				Y += SMg[k] * pow((j - jo) / 100, k); //(第一象限計算座標系 → imgオブジェクト座標系 : こっちは横軸だけを考慮)
			}
			Y = io - Y * 100; //(第一象限計算座標系 → imgオブジェクト座標系 : こっちで縦軸のみを考慮)
							  //printf("%f\n",Y);
							  //スキャンした値と閾値を比較してデータを選定する ( j < jo の条件によって原点よりマイナス軸側のデータを消す = ノイズを消せる) 
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 3 || j < jo) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//近似曲線描画確認用
			if (i == (int)Y) {
			//近似直線が外側に行くほど Yがどうなるかが予想できないので，外側にも赤く描画される可能性がある
			img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);


	//再度imgオブジェクトを読み込んで近似曲線を求める---------------------------------------------------------------------------

	//x[],y[]の初期化
	for (int i = 0; i < 30000; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	//SMg[]の初期化
	for (int i = 0; i < n_g; i++) {
		SMg[i] = 0;
	}

	//x[],y[]へimgオブジェクトのデータを格納し近似曲線を求める
	k = 0;
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (img[i][j * 3 + 1] == 0) {
				x[k] = ((double)j - jo) / 100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)
				y[k] = (io - (double)i) / 100; //第一象限計算座標系に変換(オーバーフローや桁落ちを防ぐ意図もある)
				k++;
			}
		}
	}

	S_g = k;
	sai(x, y, S_g, SMg);
	
	//再度，閾値を求め，スキャンした値と比較してデータを選定する(近似直線の座標系とimgオブジェクトの座標系は異なる)---------------------------------------------------------------------------

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {

			//横軸における各値に応じた　高さの閾値を算出する
			Y = 0.0;
			for (int k = 0; k < N_g; k++) {
				Y += SMg[k] * pow((j - jo) / 100, k); //(第一象限計算座標系 → imgオブジェクト座標系 : こっちは横軸だけを考慮)
			}
			Y = io - Y * 100; //(第一象限計算座標系 → imgオブジェクト座標系 : こっちで縦軸のみを考慮)

							  //スキャンした値と閾値を比較してデータを選定する
							  //二回目のほうが近似曲線の精度が上がるので，その分データ選定時のマージンを広くとることで，残したいデータ点を多く残せるようにする
			if (img[i][j * 3 + 1] == 0 && i >(int)Y + 9) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			//有効なデータ点の区間では近似線から100ピクセル上のデータを消す = ノイズを消せる
			if (img[i][j * 3 + 1] == 0 && j >= 0 && i < (int)Y - 100) {
				img[i][j * 3 + 0] = 255;
				img[i][j * 3 + 1] = 255;
				img[i][j * 3 + 2] = 255;
			}

			/*
			//近似曲線描画確認用
			if (i == (int)Y) {
			//近似直線が外側に行くほど Yがどうなるかが予想できないので，外側にも赤く描画される可能性がある
			img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
			img[i][j * 3 + 1] = 0;
			img[i][j * 3 + 2] = 0;
			}
			*/
		}
	}

	//output_JPEG(img);

	//抵抗状態の近似直線の傾きを求める(第一象限計算座標系での計算)---------------------------------------------------------------------------
	double **P; //動的配列の確保 (データ点を格納するため：第一象限計算座標系に変化したデータを格納する)

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

	//jth_max以降の突発的なノイズをカットする(2連続までを消去対象にする)
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

				P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
				P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
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

	SaishoJijo(P, SM); // SM[0]=a, SM[1]=b, SM[2]=theta(°) になっている

					   //中点 M(x,y) を求める(これが回転中心になる。double型なので注意)
	M_r[0] = (P[p - 1][0] + (jth_max - jo)) / 2.0; //中点Mのx座標(第一象限計算座標系)
	M_r[1] = ((SM[0] * P[p - 1][0] + SM[1]) + (SM[0] * (jth_max - jo) + SM[1])) / 2.0; //中点Mのy座標(第一象限計算座標系)

																					   //printf("a : %lf | b : %lf | theta : %lf\n", SM[0], SM[1], SM[2]);
																					   //printf("x : %lf | y : %lf\n", M_r[0], M_r[1]);

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			Y = 0.0;
			Y = io - (SM[0] * (j - jo) + SM[1]);

			//抵抗状態になった直後からのデータを削る
			if (img[i][j * 3 + 1] == 0) {
				if (abs(i - (int)Y) <= 25) {
					img[i][j * 3 + 0] = 255;
					img[i][j * 3 + 1] = 255;
					img[i][j * 3 + 2] = 255;
				}

				/*
				//近似曲線描画確認用
				if (i == (int)Y) {
				img[i][j * 3 + 0] = 255; //描画されるのは真っ赤ではなかった
				img[i][j * 3 + 1] = 0;
				img[i][j * 3 + 2] = 0;
				}
				*/
			}

		}
	}

	/*
	//jth_max位置描画確認用
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	*/

	/*
	//中点描画確認用
	int a, b;
	a = (int)M_r[0] + (int)jo;
	b = (int)io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//output_JPEG(img);

	//観測点における回転中心との角度差(°)を求める---------------------------------------------------------------------------
	//再度P[][]を初期化
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 3; j++) {
			P[i][j] = 9999;
			//printf("P[%d][%d] : %lf\n",i,j,P[i][j]);
		}
	}

	double theta_0, theta_scan, theta_max;
	p = 0;
	double jm, im; //atan2()に入れる都合上double型で定義する
	jm = M_r[0] + jo;
	im = io - M_r[1];

	theta_0 = SM[2] - 180.0; //SM[2]は(°)：degree表記 (大抵は負の値になると考えられる)

							 //この段階ではtheta_maxかどうかの判定は行わない
	for (int j = jth_max; j > 0; j--) {
		for (int i = 0; i < height; i++) {
			theta_scan = (180.0 / M_PI)*atan2(im - (double)i, (double)j - jm);

			it = (double)i;
			jt = (double)j;

			if (img[i][j * 3 + 1] == 0) {

				if (theta_scan < 0) {
					P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
					P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
					P[p][2] = fabs(theta_scan) - fabs(theta_0);
					p = p + 1;
				}
				else if (theta_scan >= 0) {
					P[p][0] = jt - jo; // こっちは元の向きのままで問題ない (データはピクセルデータのまま)
					P[p][1] = io - it; // 第一象限で +i(y軸側) および +j(x軸側) として扱えるように変換する。なお原点からの距離にする (データはピクセルデータのまま)
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
	//P[][]の再配列(対象のデータ点が先頭に来るようにする)
	p = 0;
	for (i = start; i < end + 1; i++) {
		P[p][0] = P[i][0];
		P[p][1] = P[i][1];
		P[p][2] = P[i][2];
		p = p + 1;
	}

	//対象のデータ点以降のP[][]の初期化
	for (i = end + 1; i < 10000; i++) {
		P[p][0] = 9999;
		P[p][1] = 9999;
		P[p][2] = 9999;
		p = p + 1;
	}
	*/

	//一度求めた theta_maxをさらに精査していく------------------------------------------------------------------------------------------
	//まず左側に向かってスキャンしていき，左側に最大値の箇所が他にないか調べる
	int i_before = i_max;
	double Ic_kouho;
	for (i = end; i < 10000; i++) {
		
		//Icの候補を選出
		if (P[i][1] < P[i_before][1] && P[i][2] != 9999) {
			Ic_kouho = P[i_before][1];

			if (Ic_kouho >= P[i_max][1] && P[i][0] != P[i_before][0]) {
				i_max = i_before;
			}

		}

		// 同じ j の所を無視できるようにするため(i!=0はエラーを防ぐため)
		if (P[i][0] != P[i-1][0] && i != 0) {
			i_before = i;
		}
		
	}

	//endを更新する
	end = i_max;
	//printf("end : %f | %f\n", jo + P[end][0], io - P[end][1]);
	//printf("end : %d\n",end);

	//次に右側に向かってスキャンしていき，右側に最大値の箇所が他にないか調べる
	//Icが抵抗状態まで徐々に上がっていくような形状の場合に，誤ってIcの最大値を更新する場合があるのでマージンを設けることで防ぐ
	//for (i = end; (int)(P[i][0]+jo) < jth_max; i--) {}
	i_before = i_max;
	int i_forward=1;

	for (i = end; i > 0; i--) {
		
		//printf("i_forward : %d\n", i_forward);
		//printf("P[%d][0] : %f\n", i, P[i][0]);

		//Icの候補を選出
		if (P[i][1] < P[i_before][1] && P[i][2] != 9999 && i==i_forward) {
			Ic_kouho = P[i_before][1];
			
			if (Ic_kouho > P[i_max][1] && P[i][0] != P[i_before][0]) {
				i_max = i_before;
			}
		}

		// 同じ j の所を無視できるようにするため
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

	//endを再度更新する
	end = i_max;
	//printf("end : %f | %f\n", jo + P[end][0], io - P[end][1]);
	//臨界電流の区間の近似直線を求める
	/*
	double SM_Ic[3];

	SaishoJijo(P, SM_Ic);

	//中点を求める(最終的な結果は相対的なピクセル量になるのでimgオブジェクト座標系へ変換する必要はない)
	
	//臨界電流Icを求める
	double M_Ic[2];

	M_Ic[0] = (P[0][0] + P[end - start][0]) / 2.0; //中点Mのx座標(第一象限計算座標系)
	M_Ic[1] = ((SM_Ic[0] * P[0][0] + SM_Ic[1]) + (SM_Ic[0] * P[end - start][0] + SM_Ic[1])) / 2.0; //中点Mのy座標(第一象限計算座標系)

	printf("M_Ic : %f | %f\n", jo + M_Ic[0], io - M_Ic[1]);
	printf(" Ic : %f\n", M_Ic[1]);
	*/


	//見つけたtheta_maxの地点の線幅を求める
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
	Ic[1] = P[end][1] - (LINE_wid / 2.0); //線幅の中心をIcにする

	printf(" Ic : %f\n", Ic[1]);

	//dataファイルに書き出す------------------------------------------------------------------------------------------
	
	FILE *output_Ic_N200model;
	output_Ic_N200model = fopen("output_Ic_N200model.data", "a"); //再度，上書きする場合はこのファイルを消してから実行する必要がある
	
	char *ptr1, moji1[64];//filenameを書き換えないようにmoji1[]にコピーしておく
	strcpy(moji1, filename);
	ptr1 = moji1;
	strcpy(ptr1, filename);
	ptr1 = strtok(ptr1, "_");
	ptr1 = strtok(NULL, ".");

	fprintf(output_Ic_N200model, "%f %f %s\n", IPy, Ic[1], ptr1);
	//fprintf(output_Ic_N200model, "%c%c%c%c %f %f\n", filename[11], filename[12], filename[13], filename[14], IPy, Ic[1]);

	fclose(output_Ic_N200model);

	//Icの位置を画像中に描画して書き出す(各データごとに)------------------------------------------------------------------------------------------

	
	//jth_max位置描画確認用
	for (int i = 0; i < height;i++) {
	img[i][jth_max * 3 + 0] = 0;
	img[i][jth_max * 3 + 1] = 255;
	img[i][jth_max * 3 + 2] = 255;
	}
	
	//Icの位置を描画する(十字印に描画する)
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
	
	//原点の位置を描画する(十字印に描画する)
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

	output_JPEGscan(img,filename);//これを実行する前にfilenameを変更するような処理を入れないこと

	/*
	//中点描画確認用
	int a, b;
	a = (int)M_r[0] + (int)jo;
	b = (int)io - (int)M_r[1];

	img[b][a * 3 + 0] = 255;
	img[b][a * 3 + 1] = 0;
	img[b][a * 3 + 2] = 0;

	printf("%d | %d\n",a,b);
	*/

	//動的配列の破棄
	for (int i = 0; i < 10000; i++) {
		free(P[i]);
	}
	free(P);

	//printf("height : %d\n",cinfo.output_height);
	//printf("width : %d\n", cinfo.output_width);
	//printf("output_scanline : %d\n", cinfo.output_scanline);

	/* デバッグ用
	for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
	printf("(R)---img[%d][%d] = %d , (G)---img[%d][%d] = %d , (B)---img[%d][%d] = %d \n", i, j*3+0, img[i][j*3+0], i, j*3+1, img[i][j * 3 + 1], i, j*3+2, img[i][j * 3 + 2]);
	}
	}
	*/

	/* imgオブジェクトの破棄*/
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

	/*データの横軸に値する数値を配列xに小さい順に入れ、
	それぞれの横軸の値における縦軸の値を配列yに入れ関数saiに渡す*/
	//sai(x, y, S_g, SMg);

	//Vran[V], Iran[A]
	//Ryo.05-9
	double IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, Th_max = 80e-3;

	//Ryo.2-3
	//double IPx = 114, IPy = 114, Vran = 50e-3, Iran = 0.5e-3, Th_max = 225e-3;

	//Ryo.3-1
	//double IPx = 114, IPy = 114, Vran = 100e-3, Iran = 0.1e-3, Th_max = 600e-3;

	//Ryo.05-9(平行磁場)
	//double IPx = 114, IPy = 114, Vran = 20e-3, Iran = 0.1e-3, Th_max = 80e-3;

	//Ryo.2-3(平行磁場)
	//double IPx = 114, IPy = 114, Vran = 50e-3, Iran = 0.5e-3, Th_max = 275e-3;
	
	//Ryo.2-2(平行磁場)
	//double IPx = 114, IPy = 114, Vran = 200e-3, Iran = 0.1e-3, Th_max = 1000e-3;
	
	//Ryo.2-2(平行磁場)
	//double IPx = 114, IPy = 114, Vran = 200e-3, Iran = 0.2e-3, Th_max = 800e-3;

	//Ryo.3-7
	//double IPx = 114, IPy = 114, Vran = 100e-3, Iran = 1e-3, Th_max = 500e-3;

	//画像データのパラメータを入力
	char mode = 'k';
	
	printf("mode 'a' or 'm' ( a:Average  m:Max ): ");
	scanf_s("%c",&mode,2);

	if (mode != 'a' && mode != 'm') {
		printf("Hen na atai wo nyuuryoku sinai de kudasai\n");
		exit(EXIT_FAILURE);
	}
	
	printf("複数値を入力のとき，各値の間はスペースを入れてください\n");
	printf("When the input, Please put a space between each value\n\n");
	printf("IPx[pixel] IPy[pixel] Vrange[mV] Irange[mA] Th_max[mV]: ");

	scanf_s("%lf %lf %lf %lf %lf",&IPx, &IPy, &Vran, &Iran, &Th_max);

	Vran = Vran * 1e-3;
	Iran = Iran * 1e-3;
	Th_max = Th_max * 1e-3;
	

	/*入力ファイルの設定*/

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

	//1つの画像データだけを処理する
	if (str4[0] == '0' && str4[1] == '\0') {
		printf("この一枚だけ処理します（´・ω・`）\n");
		
		strcat(filename, str1);
		strcat(filename, str2);
		strcat(filename, str3);

		if(mode == 'a')IcChecker_N50model(filename, IPx, IPy, Vran, Iran, Th_max);
		if(mode == 'm')IcChecker_N200model(filename, IPx, IPy, Vran, Iran, Th_max);
	}
	else if (str4[1] != '\0') {
		printf("複数枚を処理します（´・ω・`）\n");
		num_start = atoi(str2);
		num_end = atoi(str4);
		
		for (int i = num_start; i <= num_end;i++) {
			printf("---------------------------------------- Picture Number : %04d----------（´・ω・`）\n",i);
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