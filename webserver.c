#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 6969
#define BUFFER_SIZE 4096

int authorizeConnection(char *buffer){
    printf("%s\n", buffer);
    printf("%lu\n", strlen(buffer));
    return 0;



}

int readstaticfiles(char *resp, size_t resp_size){
    FILE *file;
    char filename[] = "./data.json";
    size_t bytes_read;

    file = fopen(filename, "r");
    if (file==NULL){
        perror("error opening file;");
        return 1;
    }

    bytes_read = fread(resp, sizeof(char), resp_size-1, file);
    if(bytes_read==0 && ferror(file)){
        perror("Error reading file");
        fclose(file);
        return 1;
    }

    resp[bytes_read] = '\0';
    fclose(file);
    return 0;
}

int main(){

    char buffer[BUFFER_SIZE];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        perror("webserver (socket)");
        return 1;
    }
    printf("socket created succesfully\n");

    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen)!=0){
        perror("webserver (bind)");
        return 1;
    }
    printf("socket succesfully bound to address\n");

    if(listen(sockfd, SOMAXCONN)!=0){
        perror("webserver (listen)");
        return 1;
    }
    printf("socket succesfully listening on address: http://localhost:%d\n", PORT);

    for(;;){
        int newsockfd = accept(sockfd, (struct sockaddr*)&host_addr, (socklen_t *)&host_addrlen);
        if(newsockfd<0){
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\n");

        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if(valread<0){
            perror("webserver (read)");
            continue;
        }

        printf("%s\n", buffer);
        authorizeConnection(buffer);

        char resp[BUFFER_SIZE];
        readstaticfiles(resp, BUFFER_SIZE);

        char header[BUFFER_SIZE];
        snprintf(header, BUFFER_SIZE,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Length: %zu\r\n"
                 "Content-Type: text/json\r\n"
                 "\r\n",
                 strlen(resp));
        
        write(newsockfd, header, strlen(header));
        write(newsockfd, resp, strlen(resp));
        close(newsockfd);
    }
    return 0;
}
