#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  int sock;
  char buf[32];
  int n,e,err;
  char *hostname = "localhost";
  struct addrinfo hints, *res;
  struct in_addr addr;

  argc = 2;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

//名前解決の取得エラー
  if ((err = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
  {
    printf("error %d\n", err);
    return 1;
  }

  addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
  inet_ntop(AF_INET, &addr, buf, sizeof(buf));
  printf("ip address : %s\n", buf);

  int protnum = atoi(argv[2]);

  //ソケットの作成
  sock = socket(AF_INET, SOCK_STREAM, 0);

  // socketのエラー
  if(sock < 0){
    perror("socket");
    printf("%d\n", errno);
    return 1;
  }

  //ソケットの設定
  server.sin_family = AF_INET;    //アドレスファミリ
  server.sin_port = htons(protnum); //ポート番号

  inet_pton(AF_INET, buf, &server.sin_addr.s_addr);

  e = connect(sock, (struct sockaddr *)&server, sizeof(server));

  //connectのエラー
  if (e < 0)
  {
    perror("connect");
    printf("%d\n", errno);
    return 1;
  }

  memset(buf, 0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));

  // readのエラー
  if (n < 0)
  {
    perror("read");
    printf("%d\n", errno);
    return 1;
  }

  printf("%d, %s\n", n, buf);

  freeaddrinfo(res);

  close(sock);

  return 0;
}
