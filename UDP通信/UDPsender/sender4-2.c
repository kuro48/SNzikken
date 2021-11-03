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
  int sock;
  struct sockaddr_in addr1;
  struct sockaddr_in senderinfo;
  struct addrinfo hints, *res;
  struct in_addr addr2;
  socklen_t addrlen;
  char buf[2048];
  int n, err;

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
  addr1.sin_family = AF_INET;
  addr1.sin_port = htons(port_num);
  inet_pton(AF_INET, buf, &addr1.sin_addr.s_addr);
  n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr1, sizeof(addr1));
  if (n < 0)
  {
    perror("sendto");
    return 1;
  }
  close(sock);
  return 0;
}