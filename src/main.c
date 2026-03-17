#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  socklen_t peer_adr_size;
  struct sockaddr_un my_addr, peer_adr;

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&my_addr, 0, sizeof(my_addr));

  my_addr.sun_family = AF_INET;

  int binding = bind(fd, (struct sockaddr *)&my_addr, sizeof(my_addr));

  listen(fd, 0);
  while (1) {
    peer_adr_size = sizeof(peer_adr);
    int cfd = accept(fd, (struct sockaddr *)&peer_adr, &peer_adr_size);

    printf("the connection: %d", cfd);

    close(fd);
  }

  return 0;
} // end of function main
