#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void do_something(int connfd)
{
  // read buffer
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);

  if (n < 0) {
    printf("read() error");
    return;
  }

  printf("client says: %s\n", rbuf);

  char wbuf[] = "Hello  World";
  write(connfd, &wbuf, strlen(wbuf));
}

int main()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    printf("socket is not initialized\n");
  }

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1234);
  addr.sin_addr.s_addr = htonl(0);

  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    printf("Bind from main: %d", rv);
  }

  // listen
  rv = listen(fd, SOMAXCONN);
  if (rv) {
    printf("listen()");
  }

  while (1) {
    struct sockaddr_in clietn_addr = {};
    socklen_t addr_len = sizeof(clietn_addr);
    int connfd = accept(fd, (struct sockaddr *)&clietn_addr, &addr_len);
    if (connfd < 0) {
      continue;
    }

    do_something(connfd);

    close(connfd);
  }

  return 0;
}
