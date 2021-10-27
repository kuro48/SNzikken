#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;

  //ソケットの作成
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  //ソケットの設定
  addr.sin_family = AF_INET; //アドレスファミリ
  addr.sin_port = htons(12345); //ポート番号
  addr.sin_addr.s_addr = INADDR_ANY; //IPアドレス
  bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

  listen(sock0, 5);

  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);
  write(sock, "HELLO", 5);
  
  close(sock);

  close(sock0);

  return 0;
}
