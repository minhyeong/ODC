#define DEFAULT_PORT 51000
#define MAX_LEN_NAME 10
#define MAX_NUM_CLIENTS 5
#define MAX_LEN_BUFFER 256
#define MAX_LEN_ADDR 32
#define BROADCAST -1

#define MESSAGE_COMMAND 'M'
#define QUIT_COMMAND 'Q'

typedef struct {
  	int cid;
  	int sock;
  	struct sockaddr_in addr;
  	char name[MAX_LEN_NAME];
} CLIENT;

typedef struct {//座標
  	int cid;
  	char command;
  	char message[MAX_LEN_BUFFER];
	double x;  //機体x座標
    	double y;  //機体y座標
    	double z;  //機体z座標
    	double angle_a;  //機体方位角
    	double angle_e;  //機体仰俯角
} CONTAINER;

typedef struct object2{
  	int type;
  
  	double x;
  	double y;
  	double z;
  
  	int status;
}Object_server;
//	debrings init_debris();	
/*
	Server内で下記のようなもを初期化
	Object debris[debris_num] = {
  		{1,0,0,-20,1},
  		{1,20,0,0,1},
  		{1,-20,0,0,1},
  		{1,0,0,20,1}
	};

*/
