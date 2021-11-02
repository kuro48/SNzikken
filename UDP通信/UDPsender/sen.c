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
  int sock, sock2;
  struct sockaddr_in addr;
  struct sockaddr_in addr3;
  struct sockaddr_in senderinfo;
  struct addrinfo hints, *res;
  struct in_addr addr2;
  socklen_t addrlen;
  char buf[2048], buf2[2048];
  int n, r, err, b;

  if (argc != 3)
  {
    fprintf(stderr, "Usage : %s dstipaddr¥n", argv[0]);
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

  addr2.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
  inet_ntop(AF_INET, &addr2, buf, sizeof(buf));
  // printf("ip address : %s\n", buf);

  /* 実行例： ./simpleUDPsender 127.0.0.1 */

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  int port_num = atoi(argv[2]);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_num);
  inet_pton(AF_INET, buf, &addr.sin_addr.s_addr);

  n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
  if (n < 1)
  {
    perror("sendto");
    return 1;
  }

   sock2 = socket(AF_INET, SOCK_DGRAM, 0);
  addr3.sin_family = AF_INET;
  addr3.sin_port = htons(12346);
  addr3.sin_addr.s_addr = INADDR_ANY;

  b = bind(sock2, (struct sockaddr *)&addr3, sizeof(addr3));
  if (b < 0)
  {
    perror("bind");
    return 1;
  }

  addrlen = sizeof(senderinfo);

  r = recvfrom(sock2, buf2, sizeof(buf2) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
  if (r < 1)
  {
    perror("recvfrom");
    return 1;
  }

  write(fileno(stdout), buf2, r);

  close(sock);
  close(sock2);
  return 0;
}