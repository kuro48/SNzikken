#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

int main()
{
  int sock0;
  struct sockaddr_in addr2;
  struct sockaddr_in client;
  socklen_t len;
  int sock;
  struct sockaddr_in server;

  /* ソケットの作成 */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if (sock0 < 0)
  {
    perror("socket");
    printf("%d¥n", errno);
    return 1;
  }

  /* ソケットの設定 */
  addr2.sin_family = AF_INET;
  addr2.sin_port = htons(12348);
  addr2.sin_addr.s_addr = INADDR_ANY;
  sock = bind(sock0, (struct sockaddr *)&addr2, sizeof(addr2));
  if (sock < 0)
  {
    perror("bind");
    printf("%d¥n", errno);
    return 1;
  }
  /* TCPクライアントからの接続要求を待てる状態にする */
  sock = listen(sock0, 5);
  if (sock < 0)
  {
    perror("listen");
    printf("%d¥n", errno);
    return 1;
  }
  /* TCPクライアントからの接続要求を受け付ける */
  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr *)&client, &len);
  if (sock < 0)
  {
    perror("accept");
    printf("%d¥n", errno);
    return 1;
  }
  char buf[300];
  memset(buf, 0, sizeof(buf));
  int n = read(sock, buf, sizeof(buf));
  //エラー処理
  if (n < 0)
  {
    perror("read");
    printf("%d¥n", errno);
    return 1;
  }

  printf("%s", buf);

  /* TCPセッションの終了 */
  close(sock);

  /* listen するsocketの終了 */
  close(sock0);

  //ここから、ファイルの送受信

  // /*----------追加したコード--------*/
  // struct addrinfo hints, *res;
  // struct in_addr addr;
  // char buf2[16];
  // int err;
  // /*------------終了----------------*/

  // int fd;
  // char buf3[65536];
  // int ret, err_set;

  // //引数の指定数が定義外の場合の例外処理

  // int openerr = 0;

  // fd = open(buf, O_RDONLY);
  // if (fd < 0)
  // {
  //   perror("open");
  //   openerr = 1;
  // }

  // /* ソケットの作成 */

  // /*ソケットのエラー作成用*/
  // // sock = socket(3000, 4000, 5000);

  // sock = socket(AF_INET, SOCK_STREAM, 0);
  // if (sock < 0)
  // {
  //   perror("socket");
  //   printf("%d¥n", errno);
  //   return 1;
  // }

  // /* 接続先指定用構造体の準備 */
  // server.sin_family = AF_INET;
  // server.sin_port = htons(12456);

  // /*----------追加したコード--------*/

  // /*------------終了----------------*/

  // /* 127.0.0.1はlocalhost */
  // /* 127.0.0.1をbuf2に変更*/
  // inet_pton(AF_INET, "localhost", &server.sin_addr.s_addr);

  // char *str_e = "file not exit";

  // /* サーバに接続 */
  // err_set = connect(sock, (struct sockaddr *)&server, sizeof(server));
  // if (err_set < 0)
  // {
  //   perror("connect");
  //   printf("%d¥n", errno);
  //   return 1;
  // }
  // if (openerr == 0)
  // {
  //   while ((n = read(fd, buf, sizeof(buf))) > 0)
  //   {
  //     ret = write(sock, buf, n);
  //     if (ret < 1)
  //     {
  //       perror("write");
  //       break;
  //     }
  //   }
  // }
  // else
  // {
  //   ret = write(sock, str_e, strlen(str_e));
  //   if (ret < 1)
  //   {
  //     perror("write");
  //     return 1;
  //   }
  // }
  // /* socketの終了 */
  // close(sock);
}