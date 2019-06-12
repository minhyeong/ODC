#include "../common.h"
#include "../constants.h"

static CLIENT clients[MAX_NUM_CLIENTS]; 
static int num_clients;
static int num_socks;
static fd_set mask;
static CONTAINER data;
static Airframe data2[4];
extern int score[4]={0,0,0,0};
void setup_server(int, u_short);
int control_requests();
void terminate_server();
//debrings init_debris();
static void send_data(int, void *, int);
static int receive_data(int cid, void *data, int size);
static void handle_error(char *);
static void send_data2(double data, int size);

#define xyzDF_MAX 491		// 491 デブリ配置範囲
#define xyzDF 256		// 256 範囲マイナス値
#define dely 2			// 重なり対策

typedef struct Debrings{
  	int x[Debri_MAX];
	int y[Debri_MAX];
	int z[Debri_MAX];
} debrings;

debrings init_debris();


/*
setup_server()
サーバのセットアップ関連
デブリの配置関連
*/
void setup_server(int num_cl, u_short port) {	// サーバのセットアップ
//	setup_server(クライアントの数,ポートの番号)
  	int rsock, sock = 0;
  	struct sockaddr_in sv_addr, cl_addr;
	
  	fprintf(stderr, "サーバのセットアップを開始\n");

  	num_clients = num_cl;//クライアント数

  	rsock = socket(AF_INET, SOCK_STREAM, 0);
  	if (rsock < 0) handle_error("socket()");
  	fprintf(stderr, "sock() for request socket is done successfully.\n");
/*	
**	同じマシンのプロセス間だけでなく,別のマシンのプロセスとも通信したいので,
**	socketシステムコールを使用している.
**	int socket(int domain, int type, int protocol);
**	TCP/IPで通信したいため int socket(AF_INET, SOCK_STREAM, 0);
*/

  	sv_addr.sin_family = AF_INET;
  	sv_addr.sin_port = htons(46728);
  	sv_addr.sin_addr.s_addr = INADDR_ANY;

  	int opt = 1;
  	setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  	if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) handle_error("bind()");
  	fprintf(stderr, "bind() is done successfully.\n");

  	if (listen(rsock, num_clients) != 0) handle_error("listen()");
  	fprintf(stderr, "listen() is started.\n");
/*
**	setsockopt関数でソケットのオプションの設定と取得を行う
**	成功時 0, エラー時 -1 が返される.
**	int setsockopt(
**		SOCKET s,	ソケットを識別する記述子。
**		int level,	オプションが定義されるレベル
**		int optname,	値がセットされることになっているソケットオプション 
**		const char* optval,	要求されたオプションのための値が
**					指定されるバッファへのポインター 
**		int optlen	バイト数による、optval バッファのサイズ
**	);
*/

  	int i, max_sock = 0;
  	socklen_t len;
  	char src[MAX_LEN_ADDR];
 	for (i = 0; i < num_clients; i++) {
    		len = sizeof(cl_addr);
    		sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
    		if (sock < 0) handle_error("accept()");

    		if (max_sock < sock) max_sock = sock;

    		if(read(sock, clients[i].name, MAX_LEN_NAME) == -1) handle_error("read()");
    		clients[i].cid = i;
    		clients[i].sock = sock;
    		clients[i].addr = cl_addr;
    		memset(src, 0, sizeof(src));
    		inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
    		fprintf(stderr, "Client %d is accepted (address=%s, port=%d).\n", i, src, ntohs(cl_addr.sin_port));
  	}
/*
**	クライアントの接続の確認
*/ 
	
  	close(rsock);

  	int j;
  	for (i = 0; i < num_clients; i++) {
    		send_data(i, &num_clients, sizeof(int));
    		send_data(i, &i, sizeof(int));
    		for (j = 0; j < num_clients; j++) {
      			send_data(i, &clients[j], sizeof(CLIENT));
	    	}
	}
	int debris[Debri_MAX][3];
	debrings debring;
	debring = init_debris();//配列リターンでいないので構造体で取得して随時代入するように変更すること！！
	
	for(i=0;i<Debri_MAX;i++){
		debris[i][0]=debring.x[i];
		debris[i][1]=debring.y[i];
		debris[i][2]=debring.z[i];
	}

	for (int i = 0; i < num_clients; i++) write(clients[i].sock, debris, sizeof(debris));
	
//	クライアントのID決定

  	num_socks = max_sock + 1;
    	FD_ZERO(&mask);
  	FD_SET(0, &mask);

  	for(i = 0; i < num_clients; i++) {
    		FD_SET(clients[i].sock, &mask);
  	}
  	fprintf(stderr, "サーバのセットアップが完了しました\n");
}

/*
クライアントからの情報の管理
座標、デブリの破壊情報、スコア情報の更新などを随時行う
*/
int control_requests() {
	int datasender[11];
	
  	fd_set read_flag = mask;
	int i;
	
  	if(select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1) handle_error("select()");
	  	
  	for (i = 0; i < num_clients; i++) {
    		if(FD_ISSET(clients[i].sock, &read_flag)) {
			
			read(clients[i].sock, &datasender, sizeof(datasender));
			score[datasender[0]] = datasender[datasender[0]+7];
			for(i=0;i<4;i++){
				datasender[7+i] = score[i];
			}
			
			for (int i = 0; i < num_clients; i++) {
      				if(datasender[6]!=i) write(clients[i].sock, &datasender, sizeof(datasender));
    			}
			
    		}
  	}
	
	
  	return 1;  
}

/*
サーバのセットアップ関連の送信
*/
static void send_data(int cid, void *data, int size) {
  	if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
    		fprintf(stderr, "send_data(): client id is illeagal.\n");
    		exit(1);
  	}
  	if ((data == NULL) || (size <= 0)) {
       		fprintf(stderr, "send_data(): data is illeagal.\n");
		exit(1);
  	}

  	if (cid == BROADCAST) {
    	int i;
    	for (i = 0; i < num_clients; i++) {
      		if (write(clients[i].sock, data, size) < 0) handle_error("write()");
    	}
  	} else {
    		if (write(clients[cid].sock, data, size) < 0) handle_error("write()");
  	}  
}

//ハンドルエラー
static void handle_error(char *message) {	//エラー出力
  	perror(message);
  	fprintf(stderr, "%d\n", errno);
  	exit(1);
}

//サーバの終了
void terminate_server(void) {
  	int i;
  	for (i = 0; i < num_clients; i++) {
    		close(clients[i].sock);
  	}
  	fprintf(stderr, "All connections are closed.\n");
  	exit(0);
}


/*
デブリの座標のランダム生成
ある程度離れるように調整している
*/

debrings init_debris(){//機体とデブリの乱数
	debrings debring;
	int i,j;
	int cont = 0, cnt = 0;
	int del = xyzDF_MAX;				// 最大値
	int x[Debri_MAX],y[Debri_MAX],z[Debri_MAX];	// 送信用
	int r[3][xyzDF_MAX];				// 範囲指定用
	double debris[Debri_MAX][3];
	
	srand(time(NULL));			// ランダム化
	
	for(i=0; i<Debri_MAX; i++){			// 初期化
		x[i] = 0;
		y[i] = 0;
		z[i] = 0;
	}
	
	for(j = 0; j < 3; j++){			// 範囲指定
		for(i = 0; i < xyzDF_MAX; i++){
			r[j][i] = i - xyzDF;
		}
	}
	//x
	del = xyzDF_MAX; 
	for(i = 0; i < Debri_MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		x[i] = r[0][cont];		// 乱数番目を格納
		del -= dely;			// 最大値を減らす
		
		// 配列から削除する
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < xyzDF_MAX; j++){
				r[0][i] = r[0][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < xyzDF_MAX; i++){	//ずらす
				r[0][i-4] = r[0][i+5];
			}
		}
	}
	//y
	del = xyzDF_MAX;
	for(i = 0; i < Debri_MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		y[i] = r[1][cont];		// 乱数番目を格納
		del -= dely;			// 最大値を減らす
		
		// 配列から削除する
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < xyzDF_MAX; j++){
				r[1][i] = r[1][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < xyzDF_MAX; i++){	//ずらす
				r[1][i-4] = r[1][i+5];
			}
		}
	}
	//z
	del = xyzDF_MAX;
	for(i = 0; i < Debri_MAX; i++){
		cont = rand() % del;		// 乱数番目 2回目以降は最大値を減らす
		z[i] = r[2][cont];		// 乱数番目を格納
		del -= dely;			// 最大値を減らす
		
		// 配列から削除するx
		if(0 <= cont || cont <= 5){	// 値が5以下の場合
			for(j = 0; j < xyzDF_MAX; j++){
				r[2][i] = r[2][cont+5+i];
			}
		}
		if((del-5) <= cont || cont <= del){cnt=0;}
		else{	
			for(i = cont; i < xyzDF_MAX; i++){	//ずらす
				r[2][i-4] = r[2][i+5];
			}
		}
	}
	
	for(j = 0; j < Debri_MAX-1; j++){		//xを軸にソート
		for(i = Debri_MAX-1; i > j; i--){
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
	
	for(i = 0; i < Debri_MAX; i++){
		debring.x[i] = x[i];
		debring.y[i] = y[i];
		debring.z[i] = z[i];
	}

		
	return debring;
	
	
}











