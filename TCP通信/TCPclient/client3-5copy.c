#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
/*----------追加したコード--------*/
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
/*------------終了----------------*/

int main(int argc, char *argv[])
{
  int sock0;
  struct sockaddr_in addr2;
  struct sockaddr_in client;
  socklen_t len;
  int sock;

  struct sockaddr_in server;
  int n;

  struct stat statBuf;

  if (stat(argv[1], &statBuf) == 0)
    printf("ファイル%sは存在します。\n", argv[1]);
  else
    printf("ファイル%sは存在しません。\n。", argv[1]);

  /* ソケットの作成 */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  //エラー処理
  // sock = -3232;

  if (sock < 0)
  {
    perror("socket");
    printf("%d¥n", errno);
    return 1;
  }

  /* 接続先指定用構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(12348);

  /* 127.0.0.1はlocalhost */
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  /* サーバに接続 */
  connect(sock, (struct sockaddr *)&server, sizeof(server));
  //エラー処理
  if (n < 0)
  {
    perror("connect");
    printf("%d¥n", errno);
    return 1;
  }

  /* 5文字送信 */
  sock = write(sock, argv[1], strlen(argv[1]));
  if (sock < 0)
  {
    perror("write");
    printf("%d¥n", errno);
    return 1;
  }

  /* socketの終了 */
  close(sock);
  /////////--------------------------------/////////////////////////////////////////

  return 0;
}