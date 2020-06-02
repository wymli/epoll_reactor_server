#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 1024
int main()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    perror("\n");
    return 0;
  }
  // fcntl(fd, F_SETFL, O_NONBLOCK);
  struct sockaddr_in addr = {.sin_family = PF_INET,
                             .sin_addr.s_addr = inet_addr("127.0.0.1"),
                             .sin_port = htons(9999)};
  // inet_pton(AF_INET, servInetAddr, &servaddr.sin_addr);
  int err = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
  if (err < 0)
  {
    perror("\n connect");
    return 0;
  }

  char buf[BUFLEN];
  while (1)
  {
    memset(buf, 0, BUFLEN);
    // fread(buf, 1, BUFLEN, stdin);
    printf("请输入:\n");
    // fgets(buf, 1024, stdin);
    scanf("%s",buf);
    if (strncmp(buf, "q", 1) == 0)
    {
      break;
    }
    int n = send(fd, buf, strlen(buf), 0);
    if (n == -1)
    {
      perror("\n");
      return 1;
    }
    printf("[发送] len=%d data=%s\n", n, buf);
    memset(buf, 0, BUFLEN);
    recv(fd, buf, BUFLEN, 0);
    printf("[收到] len=%ld ,data=%s\n",strlen(buf), buf);
  }
}