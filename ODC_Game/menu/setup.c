#include "common.h"
#include "constants.h"

//********************** client ************************//
static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask;
static CLIENT clients[MAX_NUM_CLIENTS];
Object debris[Debri_MAX];
int setup_client(char *, u_short);
Airframe control_requests_client();
void terminate_client();

static int input_command(void);
//static void send_data_client(double data,int size);
static void send_data_client2(void *, int);
static int receive_data_client(void *, int);
static void handle_error(char *);
int receive_debris(void *r,int size);
int score_all[5]={0,0,0,0,0};

Object object[Debri_MAX];


/*
クライアントのセットアップと
サーバとの接続
*/
int setup_client(char *server_name, u_short port) {	// クライアントのセットアップ
  	struct hostent *server;
  	struct sockaddr_in sv_addr;
	
  	fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
  	if ((server = gethostbyname(server_name)) == NULL) {
    		handle_error("gethostbyname()");
  	}
/*
**	ホスト名をgethostbyname()関数で探す.
**	失敗した場合NULLを返される.
*/

  	sock = socket(AF_INET, SOCK_STREAM, 0);	
  	if (sock < 0) {
    		handle_error("socket()");
  	}
/*
**	同じマシンのプロセス間だけでなく,別のマシンのプロセスとも通信したいので,
**	socketシステムコールを使用している.
**	int socket(int domain, int type, int protocol);
**	TCP/IPで通信したいため int socket(AF_INET, SOCK_STREAM, 0);
*/

  	sv_addr.sin_family = AF_INET;
  	sv_addr.sin_port = htons(46728);
  	sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0];

  	if(connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) {
    		handle_error("connect()");
  	}
/*
**	connect関数で,TCP通信においてクライアントからサーバ側へ
**	コネクションの接続確立要求を行う.
*/

  	fprintf(stderr, "エンターを押して下さい");
  	char user_name[MAX_LEN_NAME];
  	if(fgets(user_name, sizeof(user_name), stdin) == NULL) {
    		handle_error("fgets()");
  	}
  	user_name[strlen(user_name) - 1] = '\0';
  	send_data_client2(user_name, MAX_LEN_NAME);
/*
**	自分（クライアント）の名前の設定
*/

  	fprintf(stderr, "ゲームを開始致します\n");
  	receive_data_client(&num_clients, sizeof(int));
  	fprintf(stderr, "Number of clients = %d.\n", num_clients);
  	receive_data_client(&myid, sizeof(int));
  	fprintf(stderr, "Your ID = %d.\n", myid);
  	int i;
  	for(i = 0; i < num_clients; i++) {
    		receive_data_client(&clients[i], sizeof(CLIENT));
  	}
	//デブリ情報取得
	int debris[Debri_MAX][3];
	
	read(sock, debris, sizeof(debris));
	for(i = 0; i < Debri_MAX; i++) {
	    //printf("%3d : (%4d, %4d, %4d)\n",i+1,debris[i][0],debris[i][1],debris[i][2]);
	    object[i].x = (double)debris[i][0];
	    object[i].y = (double)debris[i][1];
	    object[i].z = (double)debris[i][2];
	    object[i].status = 1;
	    if(i % 2) object[i].type = 1;
	    else object[i].type = 2;
	    //printf("%3d : (%4f, %4f, %4f)\n",i+1,object[i].x,object[i].y,object[i].z);
        }
	//printf("debris=%d\n",debris[0]);
/*
**	名前の取得後,clientsのnumberとIDを決定する.
*/

  	num_sock = sock + 1;
  	FD_ZERO(&mask);
  	FD_SET(0, &mask);
 	FD_SET(sock, &mask);
 	fprintf(stderr, "他の方の接続を待っています\n少々おまちください\n");
	//init_object(debris);
	
	return myid;
}

/*
データの送信
*/
static void send_data_client(double data,int size) { 	//座標データ取得
	//送信は id x y z a e の順番
	write(sock, &data, size);
}

/*
データの受信
*/
static int receive_data_client(void *data, int size) {
	//送信は id x y z a e の順番
  	if ((data == NULL) || (size <= 0)) {
        	fprintf(stderr, "receive_data_client(): data is illeagal.\n");
		exit(1);
  	}
	
  	return(read(sock, data, size));
}

//ハンドルエラー
static void handle_error(char *message) {	// エラー出力
  	perror(message);
  	fprintf(stderr, "%d\n", errno);
  	exit(1);
}

// 終了のための開放
void terminate_client() {
  	fprintf(stderr, "Connection is closed.\n");
  	close(sock);
  	exit(0);
}

// 名前送信用
static void send_data_client2(void *data, int size) {
	
  	if ((data == NULL) || (size <= 0)) {
        	fprintf(stderr, "send_data_client(): data is illeagal.\n");
		exit(1);
  	}
	write(sock, data, size);
  	
}

/*
リクエスト管理
機体関連の座標のやり取り
スコアの管理と更新
デブリの破壊情報の管理の共有
*/
Airframe control_requests_client(Airframe data/*myself*/,double move, int no){	//リクエストの管理
	int i;
	int datasender[11]={data.id, data.x, data.y, data.z, data.angle_a, data.angle_e,no,score_all[0],score_all[1],score_all[2],score_all[3]};
  	fd_set read_flag = mask;
	
  	struct timeval timeout;
  	timeout.tv_sec = 0;
  	timeout.tv_usec = 1;
	if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) handle_error("select()");
	
	if(move) write(sock, &datasender, sizeof(datasender));
	if(move == -10) {
		datasender[0]=-10;
		write(sock, &datasender, sizeof(datasender));
		return data;	
	}
	if(FD_ISSET(sock, &read_flag)) {
		read(sock, &datasender, sizeof(datasender));
		if(datasender[0]==-10){
			data.id=-10;
			return data;
		}
	}
	data.id = (double)datasender[0];
	data.x = (double)datasender[1];
	data.y = (double)datasender[2];
	data.z = (double)datasender[3];
	data.angle_a = (double)datasender[4];
	data.angle_e = (double)datasender[5];
	
	score_all[datasender[0]] = datasender[datasender[0]+7];
	
	return data;
}

/*
デブリの受信
*/
int receive_debris(void *r,int size){
	if ((r == NULL) || (size <= 0)) {
        	fprintf(stderr, "receive_debris(): data is illeagal.\n");
		exit(1);
  	}
	return read(sock, r, sizeof(size));
}



