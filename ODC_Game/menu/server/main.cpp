#include <stdio.h>
#include <stdlib.h>
#include "../constants.h"  

extern void setup_server(int, u_short);  
extern int control_requests();
extern void terminate_server();  
extern int control_server_result();
/*
	Server内で下記のようなもを初期化
	Object debris[debris_num] = {
  		{1,0,0,-20,1},
  		{1,20,0,0,1},
  		{1,-20,0,0,1},
  		{1,0,0,20,1}
	};

*/
 
int main(int argc, char *argv[]) {//クライアント数　ポート番号
  	int num_cl = 1;
  	u_short port = DEFAULT_PORT;

  	switch(argc) {
  	case 1:
    		break;
  	case 2:
    		num_cl = atoi(argv[1]);
    		break;
  	case 3:
    		num_cl = atoi(argv[1]);
    		port = atoi(argv[2]);
    		break;
  	default:
    		fprintf(stderr, "Usage: %s [number of clients] [port number]\n", argv[0]);
    		return 1;
  	}

  	if(num_cl < 0 || num_cl > MAX_NUM_CLIENTS) {
    		fprintf(stderr, "Max number of clients is %d\n", MAX_NUM_CLIENTS);
    		return 1;
  	}

  	fprintf(stderr, "Number of clients = %d\n", num_cl);//クライアントの数を表示
 	fprintf(stderr, "Port number = %d\n", port);//ポートナンバーを表示

  	setup_server(num_cl, port);
	
  	int cond = 1;
  	while(cond) {
      		cond = control_requests();
  	}

  	terminate_server();
  	return 0;
}
