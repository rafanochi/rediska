#include "sys/types.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const size_t k_max_msg = 4096;

size_t int32_t one_request(int connfd){
  char rbuf[4+k_max_msg];
  errno = 0;
  int32_t err = read_full(connfd, rbuf, 4);
  if (err) {
    msg(errno==0? "EOF" : "read() err");
    return err;
  }


}

static int32_t read_full(int fd, char *buf, size_t n)
{
  while (n > 0) {
    ssize_t rv = read(fd, buf, n);

    if (rv < 0) {
      return -1;
    }

    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }

  return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n)
{
  while (n > 0) {
    size_t rv = write(fd, buf, n);
    if (rv <= 0) {
      return -1;
    }

    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }

  return 0;
}

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
