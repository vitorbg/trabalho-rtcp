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
		  strcpy(agenda[i].telefone, telefone);
		return 1;	 	
        }
    }
    
    //Cadastra o registro novo
    strcpy(agenda[i].nome, nome);
    strcpy(agenda[i].telefone, telefone);
    fimAgenda++;
    
    return 1;
}

int removeRegistro(Cadastro *agenda, char *nome)
{
    //Verifica se registro existe
    int i,j;
    for (i=0;i<fimAgenda;i++)
    {
        //Se existe, atualiza telefone
        if ((strcmp(agenda[i].nome, nome))==0)
        {
		strcpy(agenda[i].nome, "");
		strcpy(agenda[i].telefone, "");
        for(j=i;j<fimAgenda;j++){
            strcpy(agenda[j].nome,agenda[j+1].nome);
            strcpy(agenda[j].telefone,agenda[j+1].telefone);				
        }
        fimAgenda--;
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
