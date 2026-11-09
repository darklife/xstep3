# 1) crie uma database ACADEMIA usando:
# 
#	mysqladmin create ACADEMIA
#
# utilize:
#
#	sql ACADEMIA < create.table.sql
# 
# se vc precisa limpar/atualizar as tabelas, descomente abaixo:
#
#	drop table conta;
#	drop table aluno;
# 
# as imagens nao sao armazenadas na database, ao inves disso, 
# ficam no diretorio 'foto' relativo ao home de execucao do
# programa. dada uma certa experiencia anterior c/ armazenamento
# de imagens em databases, pode ser altamente recomendavel 
# utilizar um outro mecanismo, seja um fs remoto montado, seja
# um 'image-server', na forma de um servidor web.

create table conta (
	
	contrato   	int not null,
	documento  	char(15),
	numero     	char(15),
	valor      	char(15),
	vencimento 	char(11),
	pagamento  	char(11),
	observacao 	char(15)
);

create table aluno (

        contrato   	int not null primary key,
        nome       	char(40) not null,
        endereco   	char(40),
        bairro     	char(20),
        profissao  	char(20),
        cidade     	char(20),
        estado     	char( 3),
        cep        	char(11),
        comercial  	char(15),
        residencial	char(15),
        rg         	char(15),
        cpf        	char(15),
        nascimento 	char(11),
        civil      	char(11),
        pai        	char(40),
        mae        	char(40),
        plano      	char(20),
        marketing  	char(20),
        atividade  	char(20),
        vendedor   	char(20),
        datas       	char(11),
        vencimento 	char(11),
        avaliacao  	char(11),
        reavaliacao	char(11),
        sexo       	char(11),
        foto       	char(20),
        carteirinha	char(11)
);
