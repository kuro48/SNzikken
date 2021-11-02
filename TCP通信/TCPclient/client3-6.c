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
  char buf[65536];
  int n, ret, e, err;
  char *hostname = "localhost";
  struct addrinfo hints, *res;
  struct in_addr addr;

  if (argc != 3)
  {
    fprintf(stderr, "Usage : %s filename\n", argv[2]);
    return 1;
  }

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

  /* ソケットの作成 */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("accept");
    printf("%d\n", errno);
    return 1;
  }
  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
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

  //データの送受信
  //n = read(fd, buf, sizeof(buf));

  ret = write(sock, argv[2], n);
  printf("%s", argv[2]);
  // ret = send(sock, argv[2], strlen(argv[2]), n);
  if (ret < 0)
  {
    perror("write");
    return 1;
  }

  fd = open(argv[2], O_WRONLY | O_CREAT, 0600);
  if (fd < 0)
  {
    perror("open");
    return 1;
  }
  while ((n = read(sock, buf, sizeof(buf))) > 0)
  {
    ret = write(fd, buf, n);
    if (ret < 0)
    {
      perror("write");
      break;
    }
  }

  /* socket の終了 */
  close(sock);
  return 0;
}