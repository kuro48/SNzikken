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
  printf("ip address : %s\n", buf);

  int protnum = atoi(argv[2]);

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr1.sin_family = AF_INET;
  addr1.sin_port = htons(12345);
  addr1.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (struct sockaddr *)&addr1, sizeof(addr1));
  addrlen = sizeof(senderinfo);
  n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
               (struct sockaddr *)&senderinfo, &addrlen);
  write(fileno(stdout), buf, n);
  close(sock);
  return 0;
}