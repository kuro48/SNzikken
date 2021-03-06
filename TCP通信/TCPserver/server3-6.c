#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  int fd;
  int n, ret, e, b;
  char buf[65536];
  char get[65536];

  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  if (sock0 < 0)
  {
    perror("socket");
    printf("%d\n", errno);
    return 1;
  }

  /* ソケットの設定 */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  b = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
  if (b < 0)
  {
    perror("bind");
    printf("%d\n", errno);
    return 1;
  }

  /* TCP クライアントからの接続要求を待てる状態にする */
  e = listen(sock0, 5);
  if (e < 0)
  {
    perror("listen");
    printf("%d\n", errno);
    return 1;
  }
  /* TCP クライアントからの接続要求を受け付ける */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);
  if (sock < 0)
  {
    perror("accept");
    printf("%d\n", errno);
    return 1;
  }
  else
  {
    printf("connected");
  }

  //データの送受信
  n = read(sock, get, sizeof(get));
  printf("%s", get);
  fd = open(get, O_RDONLY);
  if (fd < 0)
  {
    perror("open");
    write(sock, "CANNOT FIND THE FILE", 1024);
    return 1;
  }

  while ((n = read(fd, buf, sizeof(buf))) > 0)
  {
    ret = write(sock, buf, n);
    if (ret < 0)
    {
      perror("write");
      break;
    }
  }

  /* TCP セッションの終了 */
  close(sock);
  /* listen する socket の終了 */
  close(sock0);
  return 0;
}