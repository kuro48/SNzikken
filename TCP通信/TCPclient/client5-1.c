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
  int fd;
  char buf[65536],send[2048];
  int n, ret, e, err;
  char *hostname = "localhost";
  struct addrinfo hints, *res;
  struct in_addr addr;

  if (argc != 3)
  {
    fprintf(stderr, "Usage : %s filename\n", argv[2]);
    return 1;
  }

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
  /* 接続先指定用構造体の準備 */
  int port_num = atoi(argv[2]);
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  /* 127.0.0.1 は localhost */
  inet_pton(AF_INET, buf, &server.sin_addr.s_addr);

  /* サーバに接続 */
  e = connect(sock, (struct sockaddr *)&server, sizeof(server));
  if (e < 0)
  {
    perror("connect");
    printf("%d\n", errno);
    return 1;
  }
  fgets(send,1024,stdin);
  printf("\n");

  ret = write(sock, send, n);
  if (ret < 1)
  {
    perror("write");
    return 1;
  }

  n = read(sock, buf, sizeof(buf));

  /* socket の終了 */
  close(sock);
  return 0;
}