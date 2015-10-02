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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "agenda.c"
#define BUFSIZE 32
main(int argc, char *argv[])
{
  Cadastro agenda[LIMITE_AGENDA];
  char nome[10] = "";
  char telefone[10] = "";
  int i;
  int j;    
  int resultado;

   int sockint,sock, s,namelen, client_address_size,n;
   struct sockaddr_in client, server;
   char buf[BUFSIZE];

   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
  if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       perror("socket()");
       exit(1);
   }

   /*
    *
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network interface)
    */    
   server.sin_family      = AF_INET;   /* Server is in Internet Domain */
   server.sin_port        = htons(atoi(argv[1]));         /* Use any available port       */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address    */

   if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       perror("bind()");
       exit(2);
   }

   /* Find out what port was really assigned and print it */
   namelen = sizeof(server);
   if (getsockname(sock, (struct sockaddr *) &server, &namelen) < 0)
   {
       perror("getsockname()");
       exit(3);
   }

   printf("Servidor online, porta: %d\n", ntohs(server.sin_port));
   fflush(stdout);
   /*
    * Receive a message on socket s in buf  of maximum size 32
    * from a client. Because the last two paramters
    * are not null, the name of the client will be placed into the
    * client data structure and the size of the client address will
    * be placed into client_address_size.
    */
   client_address_size = sizeof(client);    
    while(1){
      memset(buf,'\0', BUFSIZE);
      if(s = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &client,
            &client_address_size) <0){
       perror("recvfrom()");
       exit(4);
      }
      /*
      * Print the message and the name of the client.
      * The domain should be the internet domain (AF_INET).
      * The port is received in network byte order, so we translate it to
      * host byte order before printing it.
      * The internet address is received as 32 bits in network byte order
      * so we use a utility that converts it to a string printed in
      * dotted decimal format for readability.
      */
      
      printf("Received message %s from domain %s port %d internet address %d\n",
       buf,
       (client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
       ntohs(client.sin_port),
       inet_ntoa(client.sin_addr));
      memset(nome,'\0', 10); //Limpa o vetor nome
      memset(telefone,'\0', 10); //Limpa o vetor telefone
      if (buf[0] == '1'){
        //Quebra a mensagem capturando os dois campos do protocolo
        i=0;
        j=2;
        while(buf[j] != '|'){
          nome[i] = buf[j];
          i++;
          j++;
        }
          j++;
          i=0;
        while(buf[j] != '\0'){
          telefone[i] = buf[j];
          i++;
          j++;  
        } 
        resultado = cadastrar(agenda,nome,telefone);
        memset(buf,'\0',31);

        if (resultado)
          strcpy(buf, "OK");        
        else
          strcpy(buf, "NAO-OK");
        printf("\nBuf: %s",buf);
        n = sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,client_address_size);
       if (n  < 0) error("sendto");
    }if (buf[0] == '2'){
      //Quebra a mensagem capturando o campo
      i=0;
      j=2;
      while(buf[j] != '\0')
      {
        nome[i] = buf[j];
        i++;
        j++;  
      }

      resultado = removeRegistro(agenda,nome);

      if (resultado)
        strcpy(buf, "OK");
      else
        strcpy(buf, "REGISTRO NAO ENCONTRADO");
      printf("\nBuf: %s",buf);
      n = sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,client_address_size);
       if (n  < 0) error("sendto");
    }
    if (buf[0] == '3'){
      //Quebra a mensagem capturando o campo
      i=0;
      j=2;
      while(buf[j] != '\0')
      {
        nome[i] = buf[j];
        i++;
        j++;  
      }

      resultado = retornaTelefone(agenda,nome,telefone);        
       bzero(buf, BUFSIZE);    

      if (resultado)
        strcpy(buf, telefone);
      else      
        strcpy(buf, "REGISTRO NAO ENCONTRADO");
       printf("\nBuf: %s",buf);
      n = sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,client_address_size);
       if (n  < 0) error("sendto");
    } 
    mostraTodos(agenda);
      /*
      * Deallocate the socket.
      */
    }
   close(sock);
}
