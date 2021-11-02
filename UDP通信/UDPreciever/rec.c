#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main()
{
  int sock, sock2;
  struct sockaddr_in addr, addr2;
  struct sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[2048];
  int n, s;
  char portnum[100] = "127.0.0.1";

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (struct sockaddr *)&addr, sizeof(addr));

  addrlen = sizeof(senderinfo);

  n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
  if (n < 1)
  {
    perror("recvfrom");
    return 1;
  }



  char *buf2 = buf;
    printf("%s\n", buf2);

  // sock2 = socket(AF_INET, SOCK_DGRAM, 0);
  addr2.sin_family = AF_INET;
  addr2.sin_port = htons(12346);
  inet_pton(AF_INET, portnum, &addr2.sin_addr.s_addr);

  s = sendto(sock, buf2, strlen(buf2), 0, (struct sockaddr *)&addr2, sizeof(addr2));
  if (s < 1)
  {
    perror("sendto");
    return 1;
  }

  close(sock);
  return 0;
}