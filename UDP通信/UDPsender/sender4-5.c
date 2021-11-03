#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in addr;
  int n, fd;
  char buf[2048];

  fd = open(argv[1], O_RDONLY);

  if (fd < 0)
  {
    perror("open");
    return 1;
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  /* 宛先マルチキャストアドレスの設定 */
  inet_pton(AF_INET, "239.192.1.2", &addr.sin_addr.s_addr);

  n = sendto(sock, argv[1], sizeof(argv[1]), 0, (struct sockaddr *)&addr, sizeof(addr));

  while ((n = read(fd, buf, sizeof(buf))) > 0)
  {

    n = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n < 1)
    {
      perror("sendto");
      return 1;
    }
    printf("%s", buf);
  }

  if (n < 0)
  {
    perror("sendto");
    return 1;
  }

  close(sock);
  return 0;
}