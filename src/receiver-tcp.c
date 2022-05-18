#include "../include/set.h"

int main(void) {

    struct sockaddr_in servaddr, clientaddr;
    char buffer[MAXLINE];

    // create socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // init servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    ICHK(inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr));

    // assign an address to the socket
    CHK(bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)));

    // pass to a passive socket
    CHK(listen(socket_fd, 5));

    // create the connexion between the client and the server
    // on fera un tableau de client avec accept dans la boucle qui parcout
    // tout avec un for
    int connect_fd;
    socklen_t addrlen = sizeof(clientaddr);
    CHK(connect_fd =
            accept(socket_fd, (struct sockaddr *)&clientaddr, &addrlen));

    fd_set read_fds;
    FD_ZERO(&read_fds);

    buffer[0] = '\0';
    while (strcmp(buffer, "exit") != 0) {

        FD_SET(connect_fd, &read_fds);
        FD_SET(0, &read_fds);
        // choose the file descriptor (the client)
        select(connect_fd + 1, &read_fds, NULL, NULL, NULL);

        // if we receive an information, receive it
        if (FD_ISSET(connect_fd, &read_fds)) {
            CHK(recv(connect_fd, buffer, MAXLINE, 0));
            printf("%s", buffer);
        }

        // // if we receive an information, receive it
        // if (FD_ISSET(0, &read_fds)) {
        //     CHK(recv(0, buffer, MAXLINE, 0));
        //     printf("%s", buffer);
        // }
    }

    // close
    CHK(close(socket_fd));

    return 0;
}
