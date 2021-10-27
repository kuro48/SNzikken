#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<errno.h>

int main()
{
  struct sockaddr_in server;
  int sock;
  char buf[32];
  int n,e;

  //ソケットの作成
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0){
    perror("socket");
    printf("%d¥n", errno);
    return 1;
  }

  //ソケットの設定
  server.sin_family = AF_INET;    //アドレスファミリ
  server.sin_port = htons(12345); //ポート番号

  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  e = connect(-1, (struct sockaddr *)&server, sizeof(server));

  if (e < 0)
  {
    perror("connect");
    printf("%d¥n", errno);
    return 1;
  }

  memset(buf, 0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));

  if (n < 0)
  {
    perror("read");
    printf("%d¥n", errno);
    return 1;
  }

  printf("%d, %s¥n", n, buf);

  close(sock);

  return 0;
}
