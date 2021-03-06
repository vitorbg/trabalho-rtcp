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
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

/*
 * Client Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    unsigned short port;       /* port client will connect to              */
    char buf[25] = "";              /* data buffer for sending and receiving    */
    struct hostent *hostnm;    /* server host name information             */
    struct sockaddr_in server; /* server address                           */
    int s;                     /* client socket                            */

    char nome[10];
    char telefone[10];
    int op;
    struct timeval inicio, final;
    int tmili;
    float tempo;

    /*
     * Check Arguments Passed. Should be hostname and port.
     */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        //exit(1);
    }

    /*
     * The host name is the first argument. Get the server address.
     */
    hostnm = gethostbyname(argv[1]);
    if (hostnm == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
        //exit(2);
    }

    /*
     * The port is the second argument.
     */
    port = (unsigned short) atoi(argv[2]);

    /*
     * Put a message into the buffer.
     */
    //strcpy(buf, "the message");

    /*
     * Put the server information into the server structure.
     * The port must be put into network byte order.
     */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    /*
     * Get a stream socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
      //  exit(3);
    }

    /*
     * Connect to the server.
     */
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect()");
     //   exit(4);
    }

    do
    {
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
        switch(op)
        {
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
		printf("\nInfo Protocolo: %s",buf);

		//Inicio da contagem de tempo de resposta do servidor
		gettimeofday(&inicio, NULL);

   		if (send(s, buf, sizeof(buf), 0) < 0)
    		{
        		perror("Send()");
		       //    exit(5);
		}

		/*
		* The server sends back the same message. Receive it into the buffer.
		*/
		if (recv(s, buf, sizeof(buf), 0) < 0)
		{
		        perror("Recv()");
			//    exit(6);
	    	}
		

		//Fim da contagem de tempo de resposta do servidor
                gettimeofday(&final, NULL);
		tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
		printf("\nTempo para resposta do servidor: %f\n", tempo); 


    		printf("\nRESPOSTA DO SERVIDOR: %s",buf);

                getchar();
                getchar();

                break;
            case 2:
                printf ("Remover Registro\n");
                printf("Digite o nome: ");
                scanf("%s", nome);
		/*
		Prepara a mensagem no protocolo a ser enviado para o servidor.		
		*/
		strcat(buf, "2|");
		strcat(buf, nome);
		printf("\nInfo Protocolo: %s",buf);

		//Inicio da contagem de tempo de resposta do servidor
		gettimeofday(&inicio, NULL);

   		if (send(s, buf, sizeof(buf), 0) < 0)
    		{
        		perror("Send()");
		       //    exit(5);
		}

		/*
		* The server sends back the same message. Receive it into the buffer.
		*/
		if (recv(s, buf, sizeof(buf), 0) < 0)
		{
		        perror("Recv()");
			//    exit(6);
	    	}

		//Fim da contagem de tempo de resposta do servidor
                gettimeofday(&final, NULL);
		tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
		printf("\nTempo para resposta do servidor: %f\n", tempo); 

    		printf("\nRESPOSTA DO SERVIDOR: %s",buf);

                getchar();
                getchar();

                break;
            case 3:
		printf ("Acessar Registro\n");
                printf("Digite o nome: ");
                scanf("%s", nome);
		/*
		Prepara a mensagem no protocolo a ser enviado para o servidor.		
		*/
		strcat(buf, "3|");
		strcat(buf, nome);
		printf("\nInfo Protocolo: %s",buf);

		//Inicio da contagem de tempo de resposta do servidor
		gettimeofday(&inicio, NULL);

   		if (send(s, buf, sizeof(buf), 0) < 0)
    		{
        		perror("Send()");
		       //    exit(5);
		}

		/*
		* The server sends back the same message. Receive it into the buffer.
		*/
		if (recv(s, buf, sizeof(buf), 0) < 0)
		{
		        perror("Recv()");
			//    exit(6);
	    	}

		//Fim da contagem de tempo de resposta do servidor
                gettimeofday(&final, NULL);
		tempo = (float) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
		printf("\nTempo para resposta do servidor: %f\n", tempo); 

    		printf("\nRESPOSTA DO SERVIDOR: %s",buf);


                getchar();
                getchar();

                break;
            case 4:
                op =0;
                break;
            default:
                printf("Opcao invalida !\n");
                printf("Digite uma opcao correta !\n");
                op=5;
                getchar();
                getchar();
                system("cls");
                break;
        }

    }while(op);

    /*
     * Close the socket.
     */
    close(s);

    printf("Client Ended Successfully\n");
  //  exit(0);

}


