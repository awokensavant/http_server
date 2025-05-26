#include <stdio.h>
#include <stdbool.h>
#include <_stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
  char buffer[BUFFER_SIZE];
  const char response[] =
    "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/html\r\n\r\n"
    "<html>hello, world</html>\r\n";

  const int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file_descriptor == -1)
  {
    perror("webserver (method: socket)");
    return EXIT_FAILURE;
  }
  printf("socket created successfully\n");

  struct sockaddr_in host_addr;
  int host_address_length = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  struct sockaddr_in client_addr;
  int client_address_length = sizeof(client_addr);

  if (bind(socket_file_descriptor, (struct sockaddr *)&host_addr, host_address_length) != 0)
  {
    perror("webserver (method: bind)");
    return EXIT_FAILURE;
  }
  printf("socket successfully bound to address\n");

  if (listen(socket_file_descriptor, SOMAXCONN) != 0)
  {
    perror("webserver (method: listen)");
    return EXIT_FAILURE;
  }
  printf("server listening for connections\n");

  while (true)
  {
    const int new_socket_fd = accept(socket_file_descriptor, (struct sockaddr *)&host_addr,
                                     (socklen_t *)&host_address_length);
    if (new_socket_fd < 0)
    {
      perror("webserver (method: accept)");
      continue;
    }
    printf("connection accepted\n");

    int socket_name = getsockname(new_socket_fd, (struct sockaddr *)&client_addr,
                                  (socklen_t *)&client_address_length);
    if (socket_name < 0)
    {
      perror("webserver (method: getsockname)");
      continue;
    }

    const long read_value = read(new_socket_fd, buffer, BUFFER_SIZE);
    if (read_value < 0)
    {
      perror("webserver (method: read)");
      continue;
    }

    char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
    sscanf(buffer, "%s %s %s", method, uri, version);
    printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port), method, version, uri);

    const long write_value = write(new_socket_fd, response, strlen(response));
    if (write_value < 0)
    {
      perror("webserver (method: write)");
      continue;
    }

    close(new_socket_fd);
  }
}
