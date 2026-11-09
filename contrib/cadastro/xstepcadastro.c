#include <xstep.h>
#include <ctype.h>

/* temos q definir um tamanho maximo para listas
   ou entao alocar dinamicamente elas. acho mais facil
   definir um tamanho razoavel :) */

#define MAXLIST 100000

/* os dados! :D */

struct dados
{
        char nome[30];
        char rua[30];
        char numero[30];
        char bairro[30];
        char cidade[30];
        char estado[30];
        char telefone[30];
        char rg[30];
        char cpf[30];
        char escolaridade[30];
        char profissao[30];

} cliente;

/* variaveis auxiliares para disco, suporte a listas
   e pesquisa. as listas sao os objetos graficos mais
   complicados, entao tem uma serie de variaveis 
   auxiliares :) 
   
   listsize = tamanho da lista, muda dinamicamente
   listpointer = ponteiro da lista, eh a barra branca da 
                 lista q vc move com o mouse! :D
   
   dae tem as tres listas q sao os char *algo[] e uma
   struct q agrupa elas e identifica. isso vaih ser tudo
   usado por mscroll() e tudo funciona sozinho dae :D */

FILE 		*arquivo;
unsigned 	codigo,listsize,listpointer;
char 		status[64],nome_pesquisa[30];

char 		*codigo_list	[MAXLIST],
		*nome_list	[MAXLIST],
		*profissao_list	[MAXLIST];

struct mlist listao[]={

	{ "Codigo",	codigo_list,	100  },
	{ "Nome",	nome_list,	250  },
	{ "Profissao",	profissao_list,	200  }
};

void novo(struct xtree *wid) {

	/* localiza uma posicao para inserir novo registro
	   e limpa o buffer de edicao dos dados. nao tem muito
	   segredo, usa uma tecnica de reciclar registros
	   deletados */

	fseek(arquivo,0,SEEK_SET);

	for(codigo=0;fread(&cliente,sizeof(cliente),1,arquivo);codigo++)
		if(!cliente.nome[0])
			break;

	sprintf(status,"novo registro, codigo %d",codigo);                                                                

	memset(&cliente,0,sizeof(cliente));
	broadcast++;
}

void lista_mover(struct xtree *wid) {

	/* isso eh acionado pelo X quando vc mexe o mouse em cima
	da lista e muda o cursor dela de posicao. quando isso acontece,
	nos puxamos um novo registro da lista! :D */

	if(!listsize) return;

	codigo=atoi(codigo_list[listpointer]);

        fseek(arquivo,codigo*sizeof(cliente),SEEK_SET);
        fread(&cliente,sizeof(cliente),1,arquivo);

	sprintf(status,"editando registro %d!",codigo);
}

void lista_atualizar(struct xtree *wid) {

	int i,j;

	/* para listar, nao precisa ficar listando, pode
	   montar uma lista com scroll! a lista eh autoreciclavel,
	   ela primeiro se demonsta e entao se remonta a partir
	   do arquivo q tah no disco! */

	for(i=0;i!=listsize;i++) {
	
		free(codigo_list[i]);
		free(nome_list[i]);
		free(profissao_list[i]);
	}
	   
	fseek(arquivo,0,SEEK_SET);

	for(i=j=0;fread(&cliente,sizeof(cliente),1,arquivo);j++) {
	
		if(cliente.nome[0]) {
	
			if(codigo==j) listpointer=i;
	
			codigo_list[i]		=(char *)malloc(16);
			nome_list[i]		=(char *)malloc(1+strlen(cliente.nome));
			profissao_list[i]	=(char *)malloc(1+strlen(cliente.profissao));

			strcpy(nome_list[i],cliente.nome);
			strcpy(profissao_list[i],cliente.profissao);

			sprintf(codigo_list[i],"%d",j);
		
			i++;
		}
	}
	
	listsize	=i;
	broadcast++;

	lista_mover(wid);
}

void salvar(struct xtree *wid) {

	/* salva o registro atual q estiver na tela! */
	
	if(!cliente.nome[0]) {
	
		sprintf(status,"erro salvando registro %d: esta vazio!",codigo);
		broadcast++;
		
		return;
	}
	
	fseek(arquivo,codigo*sizeof(cliente),SEEK_SET);
	
	if(fwrite(&cliente,sizeof(cliente),1,arquivo)) {

		lista_atualizar(wid);
		sprintf(status,"registro %d salvo!",codigo);
	} else
		sprintf(status,"erro salvando registro %d!",codigo);
}

void excluir(struct xtree *wid) {

	/* exclui o registro atual! basicamente limpa ele */

	if(!cliente.nome[0]) {
	
		sprintf(status,"erro excluindo registro %d: ele jah esta vazio",codigo);

		broadcast++;
		return;
	}

	memset(&cliente,0,sizeof(cliente));

	fseek(arquivo,codigo*sizeof(cliente),SEEK_SET);
	
	if(fwrite(&cliente,sizeof(cliente),1,arquivo)) {

		lista_atualizar(wid);
		sprintf(status,"registro %d excluido!",codigo);
	} else
		sprintf(status,"erro excluindo registro %d!",codigo);
}

void pesquisaf(struct xtree *wid) {

	int i,j;

	/* funcao auxiliar de pesquisa: efetivamente faz a 
	pesquisa, selecionando sozinho se eh codigo ou nome,
	acionada pela outra funcao da caixa de dialogo! */

	if(!nome_pesquisa[0]) return;

	if(isdigit(nome_pesquisa[0])) {
	
		codigo=atoi(nome_pesquisa);
		fseek(arquivo,codigo*sizeof(cliente),SEEK_SET);
	
		if(fread(&cliente,sizeof(cliente),1,arquivo)) {

			lista_atualizar(wid);
			window_close(wid);
			return;
		}
	
	} else {

		for(i=0,j=strlen(nome_pesquisa);i!=j;i++)
			nome_pesquisa[i]=tolower(nome_pesquisa[i]);

		fseek(arquivo,0,SEEK_SET);
	
		for(codigo=0;fread(&cliente,sizeof(cliente),1,arquivo);codigo++) {

			for(i=0,j=strlen(cliente.nome);i!=j;i++)
				cliente.nome[i]=tolower(cliente.nome[i]);

			if(strstr(cliente.nome,nome_pesquisa)) {

				lista_atualizar(wid);
				window_close(wid);
				return;
			}
		}
	}
	sprintf(status,"nao encontrado registro %d!",codigo);
	broadcast++;
}

void pesquisar(struct xtree *wid) {

	/* faz questoes sobre a pesquisa */

	dialog_create(400,180);

	defaultfont=helvetica24b;
	label_create(0,0,0,-90,"Pesquisar Dados!",gray,up);
	defaultfont=helvetica12m;

	label_create(0,90,0,0,"",gray,up);
	label_create(0,100,120,21,"Codigo ou nome: ",invisible,right);
	edit_create(120,100,-8,21,nome_pesquisa,30,pesquisaf);	

	button_create( -8, -8,72,24,"Cancelar",      window_close); 		
	button_create(-88, -8,72,24,"Pesquisar",      pesquisaf);
}

void sair(struct xtree *wid) {

	/* podemos fechar o arquivo somente quando
	   o usuario clicar para sair! */

	fclose(arquivo);
	window_close(wid);
}

void xmain(int argc,char **argv) {

	int x,y;

	/* podemos abrir o arquivo uma vez na inicializacao
	   e fechar somente na funcao sair. dah ateh para melhorar
	   isso mais tarde, mas por hora tah suficiente */

	arquivo = fopen("dados.dat","r+");

	if(!arquivo) {
	
		fclose(fopen("dados.dat","w"));
		arquivo = fopen("dados.dat","r+");
	}

	window_create(0,0,640,480,"Cadastro de Clientes");

	/* uma lista multicoluna fica no topo! tirado dos docs: */

	mscroll_create(8,8,-88,156,      /* position and size */
        	&listpointer,       	/* list pointer */
        	&listsize,       	/* list size */
                listao,        		/* list struct */
                lista_mover,       	/* list pointer function */
                NULL,         		/* doubleclick function */
                NULL,           	/* right button menu */
                3);             	/* lists inside list struct */	

	y=186; x=108; 	/* vamos posicionar na vertical usando o valor de
			y, assim podemos colocar os textos a 24 pixels
			um do outro simplesmente somando 24 a y :)

			o valor x fixamos como 44, deve dar. as caixas
			nos dimensionamos para ir ateh o lado direito da
			janela, mas a 88 pixels de distancia da borda,
			assim sobra espaco para os botoes lah. se o cara
			mudar o tamanho da janela, ele redimensiona
			automaticamente tb. */

	edit_create(x,y,-88,21,cliente.nome,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.rua,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.numero,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.bairro,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.cidade,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.estado,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.telefone,30,NULL);	y+=24;
	edit_create(x,y,-88,21,cliente.rg,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.cpf,30,NULL);		y+=24;
	edit_create(x,y,-88,21,cliente.escolaridade,30,NULL);	y+=24;
	edit_create(x,y,-88,21,cliente.profissao,30,NULL);

	y=186; x=8;	/* agora vamos colocar labels ao lado das 
			caixas de edicao! como usamos 40, vamos tentar
			alinhas certinho ao lado! */

	label_create(x,y,100,21,"Nome:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Rua:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Numero:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Bairro:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Cidade:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Estado:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Telefone:",invisible,right);		y+=24;
	label_create(x,y,100,21,"RG:",invisible,right);			y+=24;
	label_create(x,y,100,21,"CPF:",invisible,right);		y+=24;
	label_create(x,y,100,21,"Escolaridade:",invisible,right);	y+=24;
	label_create(x,y,100,21,"Profissao:",invisible,right);

	label_create(0,-1,0,21,status,darkgray,center);

	y=8; x=-8; 	/* vamos posicionar na vertical usando o valor de
			y, assim podemos colocar os botoes a 32 pixels
			um do outro simplesmente somando 32 a y :)

			o valor de x fixamos como -8, assim ele prende
			ao lado direito da janela. isso eh feito automaticamente
			pelo X, conforme vc aumenta e diminui a janela :) */

	button_create(x,y,72,24,"Novo",		novo); 		y+=32;
	button_create(x,y,72,24,"Salvar",	salvar); 	y+=32;
	button_create(x,y,72,24,"Excluir",	excluir); 	y+=32;
	button_create(x,y,72,24,"Pesquisar",	pesquisar); 	y+=32;
	button_create(x,y,72,24,"Sair",		sair); 

	/* jah q tamos abrindo, aproveitamos e fazemos um
	   novo() implicito, assim ele jah posiciona um registro
	   limpo para edicao */

	lista_atualizar(NULL);
	novo(NULL);
}
