/*

XSTEP 3.3 - Toolkit for X-Window System
Copyright (C) 1996,1997,1998,1999 by Marcelo Samsoniuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Please read the COPYING and README file!!!

*/

#include <mysql.h>
#include <xstep.h>

// define altura das widgets

#define ALTURA1 21
#define ALTURA2 24

// define tamanho maximo das listas

#define MAXALUNOS 100000
#define MAXCONTAS 1000

// descritores diversos p/ o mysql

MYSQL           mysql;
MYSQL_RES       *res;
MYSQL_ROW       row;
int 		rows,
		cols,
		news;
int		reloadtime;

// buffer de digitacao p/ widgets

struct {

	char documento	[15];
	char numero	[15];
	char valor	[15];
	char vencimento [11];
	char pagamento	[11];
	char observacao [15];
} conta;

struct {

	char contrato	[11];
	char nome	[40];
	char endereco	[40];
	char bairro	[20];
	char profissao	[20];
	char cidade	[20];
	char estado	[ 3];
	char cep	[11];
	char comercial	[15];
	char residencial[15];
	char rg		[15];
	char cpf	[15];
	char nascimento	[11];
	char civil	[11];
	char pai	[40];
	char mae	[40];
	char plano	[20];
	char marketing	[20];
	char atividade	[20];
	char vendedor	[20];
	char data	[11];
	char vencimento	[11];
	char avaliacao	[11];
	char reavaliacao[11];
	char sexo	[11];
	char foto	[20];
	char carteirinha[11];
} aluno;

// buffers das listas

char 	*alunolst[5][MAXALUNOS],
	*contalst[6][MAXCONTAS];

int 	alunoptr,alunomax=0,
	contaptr,contamax=0;

// labels das listas

struct mlist alunolabel[]={

	{ "Contrato",	 alunolst[0], 100 },
	{ "Nome",	 alunolst[1], 300 },
	{ "Vencimento",  alunolst[2], 100 },	
	{ "Reavaliacao", alunolst[3], 100 },
	{ "Atividades",	 alunolst[4], 100 },	
};

struct mlist contalabel[]={

	{ "Documento",	contalst[0], 100 },
	{ "Numero",	contalst[1], 100 },
	{ "Valor",     	contalst[2], 100 },
        { "Vencimento", contalst[3], 100 },
        { "Pagamento",  contalst[4], 100 },
	{ "Observacao", contalst[5], 100 },
};

// buffers p/ listas diversas

char 	*estados[]={

	"PR", "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", 
	"MA", "MG", "MS", "MT", "PA", "PB", "PI", "SE", "SC", "SP", 
	"RJ", "RN", "RO", "RR", "RS", "TO"
};

char	*civil[]={ "Solteiro", 	"Casado" };
char	*sexo []={ "Masculino",	"Feminino" };

// descritores e buffers p/ imagens

struct {

	struct xtree 	*widget;
	unsigned char 	*data;
	int 		x,y;
	int		check;
	char		visualized[20];

	struct {

		int	r,g,b,i;

	} color[32];

} image;

// funcoes p/ tratamento de imagens (formato pnm)

void getimage(char *fimage) {

	FILE *f;
	char buffer[256];
	struct xtree *t;
	unsigned char *p;

	int x,y,i,r,g,b;
	unsigned long freecolor[32];

	t=image.widget;

	if(t->pix) {
	
		XFreeGC(display,t->gcpix);
		XFreePixmap(display,t->pix);
		free(image.data);
		t->pix=0;
	}

	// convert and read the image header.

	sprintf(buffer,"djpeg -scale 1/2 -colors 32 foto/%s.jpg",fimage);

	f=popen(buffer,"r");
	fscanf(f,"%s %d %d %s",buffer,&image.x,&image.y,buffer);
	i=fgetc(f);

	printf("detected image w/ %dx%d pixels (%d bytes). reading image 'foto/%s.jpg'...\n",
		image.x,
		image.y,
		image.x*image.y*3,
		fimage);

	// malloc() space for image.

	image.data=(char *)malloc(image.x*image.y*3);
	i=fread(image.data,image.x*3,image.y,f);
	pclose(f);

	printf("read from file %d bytes (pretended to be %d)\n",
		i,image.y);

	if(i) {

		// create a new pixmap for image

		t=image.widget;
	
		for(x=0,i=0;i!=32;i++) {
		
			if(image.color[i].i!=0) 
				freecolor[x++]=image.color[i].i;
		
			image.color[i].r=0;
			image.color[i].g=0;
			image.color[i].b=0;
			image.color[i].i=0;
		}

		printf("debug: trying to free %d colors\n",x);

		if(x) XFreeColors(display,
			DefaultColormap(display,screen),
			freecolor,
			x,0);

		t->wpix=image.x;
		t->hpix=image.y;

		t->pix=XCreatePixmap(display,t->win,t->wpix,t->hpix,DefaultDepth(display,screen));
	        t->gcpix=XCreateGC(display,t->pix,0,&values);
	        XSetLineAttributes(display,t->gcpix,0,LineSolid,CapRound,JoinRound);
	        XFillRectangle(display,t->pix,t->gcpix,0,0,t->wpix,t->hpix);

		for(p=image.data,y=0;y!=image.y;y++)
			for(x=0;x!=image.x;x++) {

				r	=(int)(*p++)<<8;
				g	=(int)(*p++)<<8;
				b	=(int)(*p++)<<8;

				/*printf("%dx%d: %x,%x,%x\n",x,y,r,g,b);*/

				for(i=0;i!=32;i++) {
		
					if(r == image.color[i].r &&
					   g == image.color[i].g &&
					   b == image.color[i].b) {
	
						XSetForeground(display,t->gcpix,image.color[i].i);
						break;
					}
				}
	
				if(i==32) {
	
					for(i=0;i!=32;i++) 
						if(image.color[i].i==0) {
	
							image.color[i].r=r;
							image.color[i].g=g;
							image.color[i].b=b;
	
							image.color[i].i=getcolor(r,g,b);
							XSetForeground(display,t->gcpix,image.color[i].i);
							break;
						}
				}
				XDrawPoint(display,t->pix,t->gcpix,x,y);
			}

	} else free(image.data);
	
	XClearArea(display,t->win,0,0,t->aw,t->ah,True);
}

// funcoes verificadoras diversas

void checkdate(struct xtree *t) {

	int 		formated,i;
	struct tm 	*tmdata;
	time_t		tmtime;

	if(!strcmp(t->b,"hoje")) {
	
		tmtime=time(0);
		tmdata=localtime(&tmtime);
	
		sprintf(t->b,"%02d/%02d/%04d",
			tmdata->tm_mday,
			tmdata->tm_mon,
			tmdata->tm_year+1900);
	}

	if(!strcmp(t->b,"ontem")) {
	
		tmtime=time(0);
		tmdata=localtime(&tmtime);
	
		sprintf(t->b,"%02d/%02d/%04d",
			tmdata->tm_mday-1,
			tmdata->tm_mon,
			tmdata->tm_year+1900);
	}

	if(!strcmp(t->b,"amanha")) {
	
		tmtime=time(0);
		tmdata=localtime(&tmtime);
	
		sprintf(t->b,"%02d/%02d/%04d",
			tmdata->tm_mday+1,
			tmdata->tm_mon,
			tmdata->tm_year+1900);
	}
	
	for(formated=i=0;t->b[i];i++) if(t->b[i]=='/') formated=1;

	if(!formated) {

		if(!(i=atoi(t->b))) return;

		if(i%10000>1900&&i%10000<2100)

			sprintf(t->b,"%02d/%02d/%04d",
				i/1000000,
				(i/10000)%100,
				i%10000);
		else
			sprintf(t->b,"%02d/%02d/%04d",
				i/10000,
				(i/100)%100,
				(i%100)<50?(i%100)+2000:(i%100)+1900);
	}
}

void checkcpf(struct xtree *t) {

	int i,soma,v1,v2;

	if(strlen(t->b)!=9) return;
	
	for(soma=i=0;i!=9;i++) soma+=(t->b[i]-'0')*(10-i);
	v1=11-(soma%11);

	for(soma=i=0;i!=9;i++) soma+=(t->b[i]-'0')*(11-i);
	soma+=(v1*2);
	v2=11-(soma%11);
	
	sprintf(t->b+9,"-%d%d",v1,v2);
}

// tratamento de redesenho da foto

void draw(struct xtree *treeptr) {

	if(time(0)<(image.check+2)) return;

	if(strcmp(image.visualized,aluno.contrato)) {
	
		getimage(aluno.contrato);
		strcpy(image.visualized,aluno.contrato);
		// animate=0;
	}

	if(report.type!=Expose) return;

	if(treeptr->pix)	

		XCopyArea(display,treeptr->pix,treeptr->win,treeptr->gc,0,0,
			treeptr->wpix,treeptr->hpix,
			(treeptr->aw-treeptr->wpix)/2,(treeptr->ah-treeptr->hpix)/2);

	downbox(0,0,treeptr->aw,treeptr->ah,invisible,treeptr);
} 

// pesquisa

void searchalunof(struct xtree *t) {

	int i,j;

	for(j=0;j!=alunomax;j++) 
		for(i=0;i!=5;i++) 
			if(alunolst[i][j])
				free(alunolst[i][j]);

	alunomax=0;

	mysql_query(&mysql,
		"select contrato,nome,vencimento,reavaliacao,atividade from aluno order by nome;");

	if((res=mysql_store_result(&mysql))) {

		rows=mysql_num_rows(res);
		cols=mysql_num_fields(res);

		for(alunomax=0;alunomax!=rows;alunomax++) {

			row=mysql_fetch_row(res);

			for(i=0;i!=5;i++) { 

				printf("[%s] ",row[i]);
				alunolst[i][alunomax]=(char *)malloc(1+strlen(row[i]?row[i]:""));
				strcpy(alunolst[i][alunomax],row[i]?row[i]:"");
			}
			putchar('\n');
		}
		mysql_free_result(res);
	}
}

void searchcontaf(struct xtree *t) {

	int i;

	mysql_query(&mysql,"select * from conta;");

	if((res=mysql_store_result(&mysql))) {

		rows=mysql_num_rows(res);
		cols=mysql_num_fields(res);

		for(contamax=0;contamax!=rows;contamax++) {

			row=mysql_fetch_row(res);

			for(i=0;i!=7;i++) { 

				printf("[%s] ",row[i+1]);
				contalst[i][contamax]=(char *)malloc(1+strlen(row[i+1]));
				strcpy(contalst[i][contamax],row[i+1]);
			}
			putchar('\n');
		}
		mysql_free_result(res);
	}
}

void getcontaofalunof(struct xtree *t) {

	char buffer[256];
	int a,b;

	sprintf(buffer,
		"select "
		"documento,numero,valor,vencimento,pagamento,observacao "
		"from conta where contrato = '%s';",
		aluno.contrato);

	printf("debug: select "
		"documento,numero,valor,vencimento,pagamento,observacao "
		"from conta where contrato = '%s' = %d\n",
		aluno.contrato,
		mysql_query(&mysql,buffer));

	for(b=0;b!=contamax;b++) 
		for(a=0;a!=6;a++) 
			if(contalst[a][b]) 
				free(contalst[a][b]);

	contamax=0;

	if((res=mysql_store_result(&mysql))) {

		rows=mysql_num_rows(res);
		cols=mysql_num_fields(res);

                printf("debug: result is a table %dx%d\n",cols,rows);

		for(contamax=0;contamax!=rows;contamax++) {

			row=mysql_fetch_row(res);

			for(a=0;a!=6;a++) { 

				contalst[a][contamax]=(char *)malloc(1+strlen(row[a]));
				strcpy(contalst[a][contamax],row[a]);
			}
		}
		mysql_free_result(res);
	}
}

// tratamento do scroll das listas

void onscrollcontaf(struct xtree *t) {

	if(contamax) {

		strcpy(conta.documento		,contalst[0][contaptr]);
		strcpy(conta.numero		,contalst[1][contaptr]);
		strcpy(conta.valor		,contalst[2][contaptr]);
		strcpy(conta.vencimento 	,contalst[3][contaptr]);
		strcpy(conta.pagamento		,contalst[4][contaptr]);
		strcpy(conta.observacao 	,contalst[5][contaptr]);
		
		return;
	}
	
	memset(&conta,0,sizeof(conta));
}

void onscrollalunof(struct xtree *t) {

	char buffer[256];
	int a,b;

	news=0;

	if(reloadtime+30>time(0)) {

		searchalunof(0);
	        reloadtime=time(0);        
	}

	sprintf(buffer,
		"select * from aluno where contrato = '%s';",
		alunolst[0][alunoptr]);

	printf("debug: select * from aluno where contrato = '%s' = %d\n",
		alunolst[0][alunoptr],
		mysql_query(&mysql,buffer));

	if((res=mysql_store_result(&mysql))) {

	        rows=mysql_num_rows(res);  
	        cols=mysql_num_fields(res);

                printf("debug: result is a table %dx%d\n",cols,rows);

	        for(b=0;b!=rows;b++) {

	                row=mysql_fetch_row(res);

			a=0;

		        strcpy(aluno.contrato,		row[a++]);
			strcpy(aluno.nome,		row[a++]);
			strcpy(aluno.endereco,		row[a++]);
			strcpy(aluno.bairro,		row[a++]);
			strcpy(aluno.profissao,		row[a++]);
			strcpy(aluno.cidade,		row[a++]);
			strcpy(aluno.estado,		row[a++]);
			strcpy(aluno.cep,		row[a++]);
			strcpy(aluno.comercial,		row[a++]);
			strcpy(aluno.residencial,	row[a++]);
			strcpy(aluno.rg,		row[a++]);
			strcpy(aluno.cpf,		row[a++]);
			strcpy(aluno.nascimento,	row[a++]);
			strcpy(aluno.civil,		row[a++]);
			strcpy(aluno.pai,		row[a++]);
			strcpy(aluno.mae,		row[a++]);
			strcpy(aluno.plano,		row[a++]);
			strcpy(aluno.marketing,		row[a++]);
			strcpy(aluno.atividade,		row[a++]);
			strcpy(aluno.vendedor,		row[a++]);
			strcpy(aluno.data,		row[a++]);
			strcpy(aluno.vencimento,	row[a++]);
			strcpy(aluno.avaliacao,		row[a++]);
			strcpy(aluno.reavaliacao,	row[a++]);
			strcpy(aluno.sexo,		row[a++]);
			strcpy(aluno.foto,		row[a++]);
			strcpy(aluno.carteirinha,	row[a++]);
	        }	        
		mysql_free_result(res);
	}

	getcontaofalunof(t);
	
	// too slow! just mark the time and wait 3 seconds to get the picture.
	// getimage();

	image.check=time(0);
	// animate=1;

	broadcast++;
}

void savecontaf(struct xtree *t) {

	char buffer[sizeof(conta)+256];

	if(!conta.documento[0]) return;

	sprintf(buffer,
		"delete from conta where contrato = '%s' and documento = '%s';",
			aluno.contrato,
			conta.documento);

	printf("delete=%d\n",mysql_query(&mysql,buffer));
	
	sprintf(buffer,
		"insert into conta values ('%s','%s','%s','%s','%s','%s','%s')",
		        aluno.contrato,
        		conta.documento,
        		conta.numero,
        		conta.valor,
        		conta.vencimento,
        		conta.pagamento,
        		conta.observacao);

	printf("insert=%d\n",mysql_query(&mysql,buffer));

	getcontaofalunof(t);		
}

void delalunof(struct xtree *t) {

	char buffer[256];

	sprintf(buffer,
		"delete from aluno where contrato = '%s';",
		aluno.contrato);

	printf("delete=%d\n",mysql_query(&mysql,buffer));

	sprintf(buffer,
		"delete from conta where contrato = '%s';",
			aluno.contrato);

	printf("delete=%d\n",mysql_query(&mysql,buffer));

	searchalunof(0);
       	onscrollalunof(0);
}

void savealunof(struct xtree *t) {

	char buffer[sizeof(aluno)+256];
	int a,b;

	if(!aluno.contrato[0]) return;

	if(!news) {

		sprintf(buffer,
			"delete from aluno where contrato = '%s';",
			aluno.contrato);

		printf("delete=%d\n",mysql_query(&mysql,buffer));
	}
	sprintf(buffer,	
		"insert into aluno values ("
		"'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s','%s','%s')",

		        aluno.contrato,
			aluno.nome,
			aluno.endereco,
			aluno.bairro,
			aluno.profissao,
			aluno.cidade,
			aluno.estado,
			aluno.cep,
			aluno.comercial,
			aluno.residencial,
			aluno.rg,
			aluno.cpf,
			aluno.nascimento,
			aluno.civil,
			aluno.pai,
			aluno.mae,
			aluno.plano,
			aluno.marketing,
			aluno.atividade,
			aluno.vendedor,
			aluno.data,
			aluno.vencimento,
			aluno.avaliacao,
			aluno.reavaliacao,
			aluno.sexo,
			aluno.foto,
			aluno.carteirinha);

	printf("insert=%d, news=%d\n",a=mysql_query(&mysql,buffer),news);

	if(a==-1) {

        	dialog_create(400,180);

	        defaultfont=helvetica24b;
	        label_create(0,0,0,-90,"Falha de Base de Dados!",gray,up);
	        defaultfont=helvetica12m;

	        label_create(0,-90,0,0,"",gray,up);
	        label_create(0,-(24+16),0,40,
	                "Voce nao pode gravar um novo contrato encima de outro existente.",
	                invisible,center);

	        button_create( -8,-8,72,24,"OK",window_close);
	
		return;
	}

        if((res=mysql_store_result(&mysql))) {

	        rows=mysql_num_rows(res);  
	        cols=mysql_num_fields(res);

	        for(b=0;b!=rows;b++) {

	                row=mysql_fetch_row(res);

	                for(a=0;a!=cols;a++) 
	                        printf("%20s ",row[a]);
                              
	                putchar('\n');
	        }
	        
	        mysql_free_result(res);
	}

	searchalunof(0);
       	onscrollalunof(0);	
}

void novof(struct xtree *t) {

	int a,b;

	memset(&aluno,0,sizeof(aluno));
	memset(&conta,0,sizeof(conta));
	news=1;

        for(b=0;b!=contamax;b++)
                for(a=0;a!=6;a++) 
                        if(contalst[a][b])
                                free(contalst[a][b]);
        contamax=0;
	getimage("");
 
	broadcast++;
}

void pesquisaf(struct xtree *t) {

}

void xmain(int n,char *p[]) {

	int x,y,i;

	window_create(0,0,800,600,"Cadastro de Alunos");

	i=0;

	mscroll_create(8,8,-8,100,&alunoptr,&alunomax,alunolabel,onscrollalunof,0,0,5);
	
	x=8; y=108; i=0;

	y=116+ALTURA1;

	// to support big images, you can use scrollbars!
	/* image.widget=xyscroll_create(608,y,-8,10*ALTURA2+ALTURA1,draw,white,1024,1024);*/
	image.widget=box_create(608,y,-8,10*ALTURA2+ALTURA1,draw,white);
	
	label_create(0,y,100,ALTURA1,"Contrato",		invisible,right);

	check_create(208,y,100,ALTURA1,"Impresso",		aluno.carteirinha); 

	label_create(400,y,100,ALTURA1,"Data",			invisible,right); y+=ALTURA2;
	
	label_create(0,y,100,ALTURA1,"Nome",			invisible,right);
	label_create(400,y,100,ALTURA1,"Profissao",		invisible,right); y+=ALTURA2;
	
	label_create(0,y,100,ALTURA1,"Endereco",		invisible,right);
	label_create(400,y,100,ALTURA1,"Bairro",		invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"Cidade",			invisible,right);
	label_create(400,y,100,ALTURA1,"CEP",			invisible,right); 
	label_create(300,y,50,ALTURA1,"UF",			invisible,right); 

	popup_create(350,y,100,ALTURA1,aluno.estado,estados,26*21); y+=ALTURA2; 

	label_create(0,y,100,ALTURA1,"Nascimento",		invisible,right);
	label_create(250,y,100,ALTURA1,"Estado Civil",		invisible,right); 

	popup_create(350,y,100,ALTURA1,aluno.civil,civil,	2*21);  	

	label_create(450,y,50,ALTURA1,"Sexo",			invisible,right); 

	popup_create(500,y,100,ALTURA1,aluno.sexo,sexo,	2*21); y+=ALTURA2; 
	
	label_create(0,y,100,ALTURA1,"Comercial",		invisible,right);
	label_create(300,y,100,ALTURA1,"Residencial",		invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"RG",			invisible,right);
	label_create(300,y,100,ALTURA1,"CPF",			invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"Pai",			invisible,right); 
	label_create(300,y,100,ALTURA1,"Mae",			invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"Plano",			invisible,right);
	label_create(300,y,100,ALTURA1,"Marketing",		invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"Atividade",		invisible,right);
	label_create(300,y,100,ALTURA1,"Vendedor",		invisible,right); y+=ALTURA2;

	label_create(0,y,100,ALTURA1,"Vencimento",		invisible,right);
	label_create(200,y,100,ALTURA1,"Avaliacao",		invisible,right);
	label_create(400,y,100,ALTURA1,"Reavaliacao",		invisible,right); y+=ALTURA1+8;

	mscroll_create(8,y,-8,-(8+24+8+ALTURA1),&contaptr,&contamax,contalabel,onscrollcontaf,0,0,6);

	y=116+ALTURA1;

	edit_create(100,y,100,ALTURA1,aluno.contrato,		11,0);
	edit_create(500,y,100,ALTURA1,aluno.data,		11,checkdate); y+=ALTURA2;
	
	edit_create(100,y,300,ALTURA1,aluno.nome,		40,0);
	
	edit_create(500,y,100,ALTURA1,aluno.profissao,		20,0); y+=ALTURA2;
	
	edit_create(100,y,300,ALTURA1,aluno.endereco,		40,0);
	edit_create(500,y,100,ALTURA1,aluno.bairro,		20,0); y+=ALTURA2;

	edit_create(100,y,200,ALTURA1,aluno.cidade,		20,0);
	edit_create(500,y,100,ALTURA1,aluno.cep,		11,0); y+=ALTURA2;

	edit_create(100,y,100,ALTURA1,aluno.nascimento,		11,checkdate); y+=ALTURA2;
	
	edit_create(100,y,200,ALTURA1,aluno.comercial,		15,0);
	edit_create(400,y,200,ALTURA1,aluno.residencial,	15,0); y+=ALTURA2;

	edit_create(100,y,200,ALTURA1,aluno.rg,			15,0);
	edit_create(400,y,200,ALTURA1,aluno.cpf,		15,checkcpf); y+=ALTURA2;

	edit_create(100,y,200,ALTURA1,aluno.pai,		40,0); 
	edit_create(400,y,200,ALTURA1,aluno.mae,		40,0); y+=ALTURA2;

	edit_create(100,y,200,ALTURA1,aluno.plano,		20,0);
	edit_create(400,y,200,ALTURA1,aluno.marketing,		20,0); y+=ALTURA2;

	edit_create(100,y,200,ALTURA1,aluno.atividade,		20,0);
	edit_create(400,y,200,ALTURA1,aluno.vendedor,		20,0); y+=ALTURA2;

	edit_create(100,y,100,ALTURA1,aluno.vencimento,		11,checkdate);
	edit_create(300,y,100,ALTURA1,aluno.avaliacao,		11,checkdate);
	edit_create(500,y,100,ALTURA1,aluno.reavaliacao,	11,checkdate); y+=29; 

	/*y+=100+ALTURA1;*/

	edit_create(28,-(8+24+8),100,ALTURA1,conta.documento,	11,0); 
	edit_create(128,-(8+24+8),100,ALTURA1,conta.numero,	15,0); 
	edit_create(228,-(8+24+8),100,ALTURA1,conta.valor,	15,0); 
	edit_create(328,-(8+24+8),100,ALTURA1,conta.vencimento,	11,checkdate);
	edit_create(428,-(8+24+8),100,ALTURA1,conta.pagamento,	11,checkdate);
	edit_create(528,-(8+24+8),-8,ALTURA1,conta.observacao,	15,savecontaf);

	button_create(-(8+80*0),-8,72,ALTURA2,	"Sair",		window_close);
	button_create(-(8+80*1),-8,72,ALTURA2,	"Deletar",	delalunof);
	button_create(-(8+80*2),-8,72,ALTURA2,	"Salvar",	savealunof);
	button_create(-(8+80*3),-8,72,ALTURA2,	"Pesquisar",	pesquisaf);
	button_create(-(8+80*4),-8,72,ALTURA2,	"Novo",		novof);

        mysql_init(&mysql);

        mysql_options(&mysql,MYSQL_OPT_COMPRESS,0);
        mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"odbc");

        if (!mysql_real_connect(&mysql,"localhost","root","","ACADEMIA",0,NULL,0))
        {
        	dialog_create(400,180);

	        defaultfont=helvetica24b;
	        label_create(0,0,0,-90,"MySQL error!",gray,up);
	        defaultfont=helvetica12m;

	        label_create(0,-90,0,0,"",gray,up);
	        label_create(0,-(24+16),0,40,
	                "Can't connect to database server.",
	                invisible,center);

	        button_create( -8,-8,72,24,"Quit",exit);

        } else {

		searchalunof(0);
        	onscrollalunof(0);
        }
	animate=1;
}
