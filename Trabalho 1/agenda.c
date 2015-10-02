#include <stdio.h>
#include <stdlib.h>
#define LIMITE_AGENDA 10

struct cadastro
{
    char nome[10];
    char telefone[10];
};

typedef struct cadastro Cadastro;

int fimAgenda = 0;

int cadastrar(Cadastro *agenda, char *nome, char *telefone)
{
    //Agenda está cheia
    if (fimAgenda == LIMITE_AGENDA)
    {
        return 0;
    }

    //Verifica se registro existe
    int i;
    for (i=0;i<fimAgenda;i++)
    {
        //Se existe, atualiza telefone
        if ((strcmp(agenda[i].nome, nome))==0)
        {
		strcpy(agenda[fimAgenda].telefone, telefone);
		return 1;	 	
        }
    }
    
    //Cadastra o registro novo
    strcpy(agenda[fimAgenda].nome, nome);
    strcpy(agenda[fimAgenda].telefone, telefone);
    fimAgenda++;
    
    return 1;
}

int removeRegistro(Cadastro *agenda, char *nome)
{
    //Verifica se registro existe
    int i;
    for (i=0;i<fimAgenda;i++)
    {
        //Se existe, atualiza telefone
        if ((strcmp(agenda[i].nome, nome))==0)
        {
		strcpy(agenda[i].nome, "");
		strcpy(agenda[i].telefone, "");				
		return 1;	 	
        }
    }
    //Registro não encontrado, retorna 0
    return 0;
}

int retornaTelefone(Cadastro *agenda, char *nome, char *telefone)
{
    //Verifica se registro existe
    int i;
    for (i=0;i<fimAgenda;i++)
    {
        //Se existe, copia telefone para variavel auxiliar telefone
        if ((strcmp(agenda[i].nome, nome))==0)
        {
		strcpy(telefone, agenda[i].telefone);	 		
		return 1;
        }
    }
    //Registro não encontrado, retorna 0
    return 0;

}

void mostraTodos(Cadastro *agenda)
{
    //Mostra todos os registros da Agenda
    int i;
    printf("\nRegistros da Agenda\n");
    for (i=0;i<fimAgenda;i++)
    {
    	printf("Registro %d ",i);	
    	printf("%s - ", agenda[i].nome);	
   	printf("%s ", agenda[i].telefone);	
	printf("\n");
    }
}

void busca(Cadastro *agenda)
{
   
}
/*
int main()
{
    Cadastro agenda[n];

    cadastrar(agenda);
    busca(agenda);

    return 0;
}*/
