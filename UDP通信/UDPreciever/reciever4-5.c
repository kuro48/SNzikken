#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <fcntl.h>

int main()
{
  int sock;
  struct addrinfo hints, *res;
  int err, n,fd;
  struct group_req greq;
  char buf[2048];
  struct sockaddr_in addr;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  err = getaddrinfo(NULL, "12345", &hints, &res);
  if (err != 0)
  {
    printf("getaddrinfo : %s¥n", gai_strerror(err));
    return 1;
  }

  sock = socket(res->ai_family, res->ai_socktype, 0);
  bind(sock, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  err = getaddrinfo("239.192.1.2", NULL, &hints, &res);
  if (err != 0)
  {
    printf("getaddrinfo : %s¥n", gai_strerror(err));
    return 1;
  }
  memset(&greq, 0, sizeof(greq));

  greq.gr_interface = if_nametoindex("en0"); /* 任意のネットワークインターフェースを利用 */

  /* getaddrinfo()の結果を group_req 構造体へコピー */
  memcpy(&greq.gr_group, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);

  /* MCAST_JOIN_GROUP を利用してマルチキャストグループへ JOIN */
  if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_GROUP, (char *)&greq, sizeof(greq)) !=
      0)
  {
    perror("setsockopt");
    return 1;
  }
  memset(buf, 0, sizeof(buf));

  n = recv(sock, buf, sizeof(buf), 0);

  char *a = strtok(buf,"\n");

  fd = open(a, O_WRONLY | O_CREAT, 0600);
  if (fd < 0)
  {
    perror("open");
    return 1;
  }

  while (n > 0)
  {
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    int ret = write(fd, buf, n);
    if (ret < 0)
    {
      perror("write");
    }
    // printf("%d\n", n);
  }

  close(fd);

  printf("%s\n", buf);
  close(sock);
  return 0;
}