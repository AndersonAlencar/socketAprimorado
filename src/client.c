#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <endian.h>
#include "protocol.h"


#define PORTA 3000


struct sockaddr_in remote;

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec;
    int env;
    int id = 0;
    int op;
    int num;
    int checker = TRUE;
    double valor;
    char ip[32];
    struct Cell *first_cell = NULL, *last_cell = NULL;
    struct Request *request = (struct Request *)malloc(sizeof(struct Request));
    struct Answer *answer = (struct Answer *)malloc(sizeof(struct Answer));
    socklen_t len = sizeof(remote);

    printf("Digite o Ip desejado para conexao: ");
    fgets(ip,14,stdin);// 192.168.1.120-> lucas

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("\nSocket criado!\n");
    }

    setup_socket(&remote,PORTA,ip);

    if(connect(sockfd, (struct sockaddr *)&remote, len) < 0){
        perror("connect ");
        exit(1);
    }

    printf("Conectado!\n\n");

    while(checker){
        //printf("\e[H\e[2J");
        //printf("\33[H\33[2J");  prints que limpam o console
        op = menu();
        switch(op){

            case MENU_OPERATION:
                op = submenu();
                if (op == RETURN_MENU)
                    break;
                
                last_cell = allocate_new_cell(first_cell,last_cell);
                if(first_cell == NULL)
                    first_cell = last_cell;

                reset_memory(request,answer);

                printf("Digite quantos numeros deseja: ");
                scanf("%d",&num);

                insert_array(request,num);

                id++;
                generate_head(request,num,id,op);
                save_package_req(request,last_cell);
                env = send(sockfd,htole64(request), sizeof(struct Request), FLAGS);
                if(env == -1){
                perror("\nError no envio do pacote: ");
                return EXIT_FAILURE;
                }

                rec = recv(sockfd, le64toh(answer), sizeof(struct Answer) , FLAGS);
                if(rec == -1){
                    perror("\nError na entrega do pacote: ");
                    return EXIT_FAILURE;
                }
                show_package_ans(answer);
                save_package_ans(answer,last_cell);

                break;

            case HISTORY:
                printf("\n-----------------------HISTORY-----------------------\n\n");
                show_history(first_cell);
                break;

            case FINISH:
                printf("Fim da aplicacao\n");
                checker = FALSE;
                break; 
        }
            
    }

    close(sockfd);
    printf("\nCliente Encerrado! \n");
    return 0;

}