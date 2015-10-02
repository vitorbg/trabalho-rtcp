/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This product contains "Restricted Materials of IBM":              */
/*      Program Number:   5798RXW                                      */
/*      Program Name:     IBM TCP/IP Version 1.2 for OS/2              */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   All rights reserved.                                              */
/*                                                                     */
/*   US Government Users Restricted Rights -                           */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule     */
/*   Contract with IBM Corp.                                           */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xic***/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 32
main(argc, argv)
int argc;
char **argv;
{
   
   unsigned short port;
   struct sockaddr_in server;
   int s,op,sendsize = sizeof(server);
   char buf[BUFSIZE];
   char nome[10],telefone[10];
   struct hostent *hp;

   struct timeval inicio, final;
   int tmili;
   float tempo;

   /* argv[1] is internet address of server argv[2] is port of server.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
   if(argc != 3)
   {
      printf("Usage: %s <host address> <port> \n",argv[0]);
      exit(1);
   }  
  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       perror("socket()");
       exit(1);
   } 
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");
   port = htons(atoi(argv[2]));
 
   /* Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */

   /* Set up the server name */
   server.sin_family      = AF_INET;            /* Internet Domain    */
   server.sin_port        = port;               /* Server Port        */
   server.sin_addr.s_addr = *((unsigned long *)hp->h_addr); /* Server's Address   */

  fflush(stdin);
  printf ("**Cliente de cadastro da Agenda Telefonica Distribuida**\n");
  printf ("Selecione uma opcao.\n");
  printf ("1- Inserir/Atualiza registro\n");
  printf ("2- Apagar registro\n");
  printf ("3- Acessar um registro\n");
  printf ("4- Sair\n");
  printf ("\nOpcao:");
  scanf("%d", &op);
  //Limpa o buffer para montar a mensagem do protocolo.
  strcpy(buf, "");
  switch(op){
    case 1:
      printf ("Inserir/Atualizar Registro\n");
      printf("Digite o nome: ");
      scanf("%s", nome);
      printf("Digite o telefone: ");
      scanf("%s", telefone);
      printf("\nEnviando para o servidor. ");
      printf("\nNome: %s  Telefone: %s",nome,telefone);
    /*
    Prepara a mensagem no protocolo a ser enviado para o servidor.    
    */
      strcat(buf, "1|");
      strcat(buf, nome);
      strcat(buf, "|");
      strcat(buf, telefone);
      printf("\nInfo Protocolo: %s\n",buf);
      
      //Inicio da contagem de tempo de resposta do servidor
      gettimeofday(&inicio, NULL);
      
      //Aqui deve ser enviado o pacote para o servidor
      if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("sendto()");
        exit(2);
      }
      //Cliente recebe resposta do servidor
      if(recvfrom(s, buf, strlen(buf), 0,(struct sockaddr *)&server, &sendsize)<0){
        perror("recvfrom()");
      }
      printf("\nRESPOSTA DO SERVIDOR: %s\n",buf);

      //Fim da contagem de tempo de resposta do servidor
      gettimeofday(&final, NULL);
      tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
      printf("\nTempo para resposta do servidor: %f\n", tempo); 	

      break;
    case 2:
      printf ("Remover Registro\n");
      printf("Digite o nome: ");
      scanf("%s", nome);
      /*Prepara a mensagem no protocolo a ser enviado para o servidor.*/
      strcat(buf, "2|");
      strcat(buf, nome);
      printf("\nInfo Protocolo: %s",buf);
    
      //Inicio da contagem de tempo de resposta do servidor
      gettimeofday(&inicio, NULL);
      
      //Envia pacote para servidor
      if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("sendto()");
        exit(2);
      }
      //Cliente recebe resposta do servidor
      if(recvfrom(s, buf, strlen(buf), 0,(struct sockaddr *)&server, &sendsize)<0){
        perror("recvfrom()");
      }

      //Fim da contagem de tempo de resposta do servidor
      gettimeofday(&final, NULL);
      tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
      printf("\nTempo para resposta do servidor: %f\n", tempo); 	

      printf("\nRESPOSTA DO SERVIDOR: %s\n",buf);
      break;
    case 3:
        printf ("Acessar Registro\n");
        printf("Digite o nome: ");
        scanf("%s", nome);
        /*Prepara a mensagem no protocolo a ser enviado para o servidor.*/
        strcat(buf, "3|");
        strcat(buf, nome);
        printf("\nInfo Protocolo: %s",buf);
        
       //Inicio da contagem de tempo de resposta do servidor
       gettimeofday(&inicio, NULL);

       if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server)) < 0){
          perror("sendto()");
          exit(2);
        }//Cliente recebe resposta do servidor
        if(recvfrom(s, buf, strlen(buf), 0,(struct sockaddr *)&server, &sendsize)<0){
        perror("recvfrom()");
      }
      //Fim da contagem de tempo de resposta do servidor
      gettimeofday(&final, NULL);
      tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
      printf("\nTempo para resposta do servidor: %f\n", tempo); 	
      
      printf("\nRESPOSTA DO SERVIDOR: %s\n",buf);
      break;
      case 4:
        op =0;
        break;
      default:
        printf("Opcao invalida !\n");
        printf("Digite uma opcao correta !\n");
        getchar();
        system("cls");
        break;
  }

   /* Deallocate the socket */
   close(s);
}
