#include <stdio.h>
#include <stdlib.h>
#define n 10

struct cadastro
{
    char nome[80];
    int DDD;
    char telefone[8];
    char tipotel[15];
};
typedef struct cadastro Cadastro;

void cadastrar(Cadastro *agenda)
{
    int i;

    for (i=0;i<n;i++)
    {
        printf ("**AGENDA - CADASTRO DE PESSOAS**\n");
        printf ("Para sair digite 'acabou' !\n");
        printf ("Digite o nome:");
        fflush(stdin);
        scanf ("%[^\n]s", agenda[i].nome);
        if ((strcmp(agenda[i].nome, "acabou"))==0)
        {
            fflush(stdin);
            break;
        }
        fflush(stdin);
        printf ("Digite o DDD do telefone:");
        scanf ("%d", &agenda[i].DDD);
        fflush(stdin);
        printf ("Digite o numero do telefone:");
        scanf ("%[^\n]s", agenda[i].telefone);
        fflush(stdin);
        printf ("Digite o tipo do telefone(Fixo/Celular):");
        scanf ("%[^\n]s", agenda[i].tipotel);
        fflush(stdin);
        system("cls");
    }
    if (i==n)
        printf ("\n\nLimite de cadastro excedido !!\n\n");
}

void busca(Cadastro *agenda)
{
    int con, i;
    char aux[20];
    system("cls");
    do
    {
        printf ("**CONSULTA DE TELEFONES **\n");
        printf ("Para sair digite 'fim'.\n");
        printf ("\nDigite o nome da pessoa: ");
        fflush(stdin);
        scanf ("%[^\n]s",aux);
        fflush(stdin);
        if (strcmp(aux, "fim")==0)
            exit(1);

        con = 0;
        for (i=0;i<n;i++)
        {
            if (strcmp(aux, agenda[i].nome)==0)
            {
                con=1;
                break;
            }
        }
        if (con == 1)
        {
            system("cls");
            printf ("NOME: %s", agenda[i].nome);
            printf ("\nDDD: %d", agenda[i].DDD);
            printf ("\nTELEFONE: %s", agenda[i].telefone);
            printf ("\nTIPO\n: %s", agenda[i].tipotel);
        }
        else
        {
            system("cls");
            printf ("Registro nao encontrado !\n");
        }
        }while(aux != "fim");
}
/*
int main()
{
    Cadastro agenda[n];

    cadastrar(agenda);
    busca(agenda);

    return 0;
}*/
