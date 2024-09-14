#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int readstaticfiles(char resp){
    FILE *file;
    char filename[] = "./index.html";
    //char ch;
    char *ch = &resp;

    file = fopen(filename, "r");
    if (file==NULL){
        perror("error opening file;");
        return 1;
    }

    while(fgets(ch, BUFFER_SIZE, file) != NULL){
        printf("%s\n",ch);
    }

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

        char resp[BUFFER_SIZE] = "";
        readstaticfiles(resp);

        write(newsockfd, resp, sizeof(resp));
        close(newsockfd);
    }
    return 0;
}
