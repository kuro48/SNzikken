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
  int sock,e;
  

  //ソケットの作成
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if(sock0 < 0){
    perror("socket");
    printf("%d¥n", errno);
    return 1;
  }

  //ソケットの設定
  addr.sin_family = AF_INET; //アドレスファミリ
  addr.sin_port = htons(12345); //ポート番号
  addr.sin_addr.s_addr = INADDR_ANY; //IPアドレス
  e = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
  if (e < 0)
  {
    perror("bind");
    printf("%d¥n", errno);
    return 1;
  }

  e= listen(sock0, 100);
  if (e < 0)
  {
    perror("listen");
    printf("%d¥n", errno);
    return 1;
  }

  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);
  e = write(sock, "HELLO", 5);

  if (e < 0)
  {
    perror("write");
    printf("%d¥n", errno);
    return 1;
  }

  close(sock);

  close(sock0);

  return 0;
}
