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
/*
 * Include Files.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "agenda.c"


/*
 * Server Main.
 */
main(argc, argv)
int argc;
char **argv;
{

    /* Estrutura de Dados da Agenda */
    Cadastro agenda[LIMITE_AGENDA];
    char nome[10] = "";
    char telefone[10] = "";
    int i;
    int j;    
    int resultado;
    char resultadoProtocolo3[11] = "";

    unsigned short port;       /* port server binds to                  */
    char buf[25];              /* buffer for sending and receiving data */
    struct sockaddr_in client; /* client address information            */
    struct sockaddr_in server; /* server address information            */
    int s;                     /* socket for accepting connections      */
    int ns;                    /* socket connected to client            */
    int namelen;               /* length of client name                 */

    /*
     * Check arguments. Should be only one: the port number to bind to.
     */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
     //   exit(1);
    }

    /*
     * First argument should be the port.
     */
    port = (unsigned short) atoi(argv[1]);

    /*
     * Get a socket for accepting connections.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
       // exit(2);
    }

    /*
     * Bind the socket to the server address.
     */
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
       perror("Bind()");
     //  exit(3);
    }

    /*
     * Listen for connections. Specify the backlog as 1.
     */
    if (listen(s, 1) != 0)
    {
        perror("Listen()");
    //    exit(4);
    }
    printf("Servidor aguardando requisicoes... \n");
    /*
     * Accept a connection.
     */
    namelen = sizeof(client);
    if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) == -1)
    {
        perror("Accept()");
     //   exit(5);
    }

    while(1)
    {
    	/*
    	 * Receive the message on the newly connected socket.
    	 */
    	if (recv(ns, buf, sizeof(buf), 0) == -1)
    	{
        	perror("Recv()");
	    //    exit(6);
	}
 
        printf("Mensagem Recebida: %s",buf);
        printf("\n");
        printf("buf 0 %c",buf[0]);
        
        //Identifica a acao pelo protocolo
	/*
	Acao 1 é inserir/atualizar registro
	Mensagem esperada: 1|nome|telefone
	*/
        if (buf[0] == '1')
	{
		//Quebra a mensagem capturando os dois campos do protocolo
		i=0;
		j=2;
		while(buf[j] != '|')
		{
			nome[i] = buf[j];
			i++;
			j++;	
		}
		j++;
		i=0;
		while(buf[j] != '\0')
		{
			telefone[i] = buf[j];
			i++;
			j++;	
		}
		
		resultado = cadastrar(agenda,nome,telefone);
		strcpy(buf, "");		
		if (resultado){
			strcpy(buf, "OK");
		}
		else
		{
			strcpy(buf, "NAO-OK");
		}
		
		
		/*
		* Send the message back to the client.
	     	*/
	    	if (send(ns, buf, sizeof(buf), 0) < 0)
		{
       			perror("Send()");
		    //    exit(7);
 		}
		printf("\n Tupla de Registro:");
		printf("\n nome: %s ",nome);
		printf("\n telefone: %s ",telefone);
		printf("\n");
		//zera buffer
		strcpy(buf, "");
       }
       else 
       {
		/*
		Acao 2 é remover registro
		Mensagem esperada: 2|nome		
		*/
        	if (buf[0] == '2'){
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
			
			strcpy(buf, "");		
			if (resultado){
				strcpy(buf, "OK");
			}
			else
			{
				strcpy(buf, "REGISTRO NAO ENCONTRADO");
			}
		    	if (send(ns, buf, sizeof(buf), 0) < 0)
			{
       				perror("Send()");
			    //    exit(7);
 			}
		}
		else
		{
			/*
			Acao 3 é acessar registro e retornar seu telefone
			Mensagem esperada: 3|nome		
			*/
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
				
				strcpy(buf, "");		
				if (resultado){
					strcpy(buf, telefone);
				}
				else
				{
					strcpy(buf, "REGISTRO NAO ENCONTRADO");
				}
			    	if (send(ns, buf, sizeof(buf), 0) < 0)
				{
       					perror("Send()");
				    //    exit(7);
 				}	
			}
			else
			{
				/*
				A mensagem recebida não pode ser identificada, 
				portanto nenhuma ação poderá ser realizada
				*/
				strcpy(buf, "");					
				strcpy(buf, "ACAO NAO DETECTADA");
				if (send(ns, buf, sizeof(buf), 0) < 0)
				{
       					perror("Send()");
				    //    exit(7);
 				}
			}
		}
       }
	

       mostraTodos(agenda);

   }
    close(ns);
    close(s);

    printf("Server ended successfully\n");
  //  exit(0);
}


