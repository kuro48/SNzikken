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
#include <fcntl.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  int sock;
  char buf[100], buf2[100], word[100];
  int n, err, ret, e;
  struct addrinfo hints, *res;
  struct in_addr addr;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  if ((err = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
  {
    printf("error %d\n", err);
    return 1;
  }

  addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
  inet_ntop(AF_INET, &addr, buf, sizeof(buf));
  printf("ip address : %s\n", buf);

  /* ソケットの作成 */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("connect");
    printf("%d\n", errno);
    return 1;
  }

  int port_num = atoi(argv[2]);
  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(port_num);
  /* 127.0.0.1 は localhost */
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
  /* サーバに接続 */
  e = connect(sock, (struct sockaddr *)&server, sizeof(server));
  if (e < 0)
  {
    perror("connect");
    printf("%d\n", errno);
    return 1;
  }

  /* サーバからデータを受信 */
  printf(">");
  fgets(word, 100, stdin);

  char *word2 = word;

  ret = send(sock, word2, strlen(word2), n);

  // ret = write(sock, send, n);
  if (ret < 0)
  {
    perror("send");
    return 1;
  }

  n = read(sock, buf2, sizeof(buf2));
  if (n < 0)
  {
    perror("read");
    printf("%d\n", errno);
    return 1;
  }

  printf("%d, %s\n", n, buf2);
  /* socket の終了 */
  close(sock);
  return 0;
}