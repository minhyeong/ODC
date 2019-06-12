#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
int main(void){					//最後に-246
	
	int i,j;
	int cont = 0, cnt = 0;
	int del = 491;				// 最大値
	int x[MAX],y[MAX],z[MAX];		// 送信用
	int r[3][491];				// 範囲指定用
	
	srand(time(NULL));			// ランダム化
	
	for(i=0; i<MAX; i++){			// 初期化
		x[i] = 0;
		y[i] = 0;
		z[i] = 0;
	}
	
	for(j = 0; j < 3; j++){			// 範囲指定
		for(i = 0; i < 491; i++){
			r[j][i] = i - 246;
		}
	}
	//x
	del = 491;
	for(i = 0; i < MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		x[i] = r[0][cont];		// 乱数番目を格納
		del -= 11;			// 最大値を減らす
		
		// 配列から削除する
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < 491; j++){
				r[0][i] = r[0][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < 491; i++){	//ずらす
				r[0][i-4] = r[0][i+5];
			}
		}
	}
	//y
	del = 491;
	for(i = 0; i < MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		y[i] = r[1][cont];		// 乱数番目を格納
		del -= 11;			// 最大値を減らす
		
		// 配列から削除する
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < 491; j++){
				r[1][i] = r[1][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < 491; i++){	//ずらす
				r[1][i-4] = r[1][i+5];
			}
		}
	}
	//z
	del = 491;
	for(i = 0; i < MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		z[i] = r[2][cont];		// 乱数番目を格納
		del -= 11;			// 最大値を減らす
		
		// 配列から削除するx
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < 491; j++){
				r[2][i] = r[2][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < 491; i++){	//ずらす
				r[2][i-4] = r[2][i+5];
			}
		}
	}
	
	for(j = 0; j < MAX-1; j++){		//xを軸にソート
		for(i = MAX-1; i > j; i--){
			if(x[i-1] > x[i]){
				cnt = x[i];
				x[i] = x[i-1];
				x[i-1] = cnt;

				cnt = y[i];
				y[i] = y[i-1];
				y[i-1] = cnt;

				cnt = z[i];
				z[i] = z[i-1];
				z[i-1] = cnt;
			}
		}
	}
	
	for(i = 0; i < MAX; i++) printf("%3d : (%4d, %4d, %4d)\n",i+1,x[i],y[i],z[i]);
	
	return 0;
}


