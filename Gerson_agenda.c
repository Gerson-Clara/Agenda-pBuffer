#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_BUFFER 1024

void *pBuffer;

void Menu( int *opcao );
void AdicionarPessoa( void );
void RemoverPessoa( void );
void BuscarPessoa( void );
void Listar( void );

int main( void ) {
	pBuffer = malloc( TAM_BUFFER );
	if ( !pBuffer ) {
		printf( "Erro..." );
		exit( 1 );
	}
	int *opcao = ( int * )pBuffer;
	int *desloc = ( int * )( pBuffer + sizeof( int ) );
	void **head = ( void ** )( pBuffer + sizeof( int ) * 2 );
	void **tail = ( void ** )( pBuffer + sizeof( int ) * 2 + sizeof( void * ) );

	*desloc = 2 * sizeof( int ) + 2 * sizeof( void * );
	*head = NULL;
	*tail = NULL;

	for ( ;; ) {
		Menu( opcao );
		switch ( *opcao ) {
		case 1:
			AdicionarPessoa();
			break;
		case 2:
			RemoverPessoa();
			break;
		case 3:
			BuscarPessoa();
			break;
		case 4:
			Listar();
			break;
		case 5:
			printf( "Saindo do programa..." );
			free( pBuffer );
			exit( 0 );
			break;
		default:
			break;
		}
	}

	return 0;
}

/*
====================
Menu
 Mostra o menu de opções
====================
*/
void Menu( int *opcao ) {
	do {
		printf( "\n--| AGENDA |--\n" );
		printf( "\t1. Adicionar Pessoa\n" );
		printf( "\t2. Remover Pessoa\n" );
		printf( "\t3. Buscar Pessoa\n" );
		printf( "\t4. Listar Todos\n" );
		printf( "\t5. Sair\n" );
		printf( "\n--> " );
		scanf( "%d", opcao );
		getchar();
	} while ( *opcao < 1 || *opcao > 5 );
}

/*
====================
AdicionarPessoa
 Adiciona uma nova pessoa na agenda
====================
*/
void AdicionarPessoa( void ) {
	int *desloc = ( int * )( pBuffer + sizeof( int ) );
	void **head = ( void ** )( pBuffer + 2 * sizeof( int ) );
	void **tail = ( void ** )( pBuffer + 2 * sizeof( int ) + sizeof( void * ) );

	void *novoNodo = pBuffer + *desloc;
	void **next = ( void ** )novoNodo;
	void **prev = ( void ** )( novoNodo + sizeof( void * ) );
	char *nome = ( char * )( novoNodo + 2 * sizeof( void * ) );
	int *idade = ( int * )( nome + 50 );
	char *email = ( char * )( idade + 1 );

	printf( "Informe o nome: " );
	fgets( nome, 50, stdin );
	nome[strcspn( nome, "\n" )] = '\0';

	printf( "Informe a idade: " );
	scanf( "%d", idade );
	getchar();

	printf( "Informe o email: " );
	fgets( email, 50, stdin );
	email[strcspn( email, "\n" )] = '\0';

	*desloc += 2 * sizeof( void * ) + 50 + sizeof( int ) + 50;

	if ( *head == NULL ) {
		*head = novoNodo;
		*tail = novoNodo;
		*next = NULL;
		*prev = NULL;
	} else {
		void *atual = *head;
		void *anterior = NULL;

		while ( atual != NULL && strcmp( nome, ( char * )( atual + 2 * sizeof( void * ) ) ) > 0 ) {
			anterior = atual;
			atual = *( void ** )atual;
		}

		if ( anterior == NULL ) {
			*next = *head;
			*( void ** )( *head + sizeof( void * ) ) = novoNodo;
			*prev = NULL;
			*head = novoNodo;
		} else if ( atual == NULL ) {
			*next = NULL;
			*prev = *tail;
			*( void ** )( *tail ) = novoNodo;
			*tail = novoNodo;
		} else {
			*next = atual;
			*prev = anterior;
			*( void ** )anterior = novoNodo;
			*( void ** )( atual + sizeof( void * ) ) = novoNodo;
		}
	}
}

/*
====================
RemoverPessoa
 Remove uma pessoa da agenda
====================
*/
void RemoverPessoa( void ) {
	int *desloc = ( int * )( pBuffer + sizeof( int ) );
	void **head = ( void ** )( pBuffer + 2 * sizeof( int ) );
	void **tail = ( void ** )( pBuffer + 2 * sizeof( int ) + sizeof( void * ) );

	if ( *head == NULL ) {
		printf( "Lista vazia. Nada para remover.\n" );
		return;
	}

	void *nomeParaRemover = pBuffer + *desloc;
	*desloc += 50;

	printf( "Informe o nome da pessoa a ser removida: " );
	fgets( ( char * )nomeParaRemover, 50, stdin );
	( ( char * )nomeParaRemover )[strcspn( ( char * )nomeParaRemover, "\n" )] = '\0';

	void *atual = *head;
	void *anterior = NULL;

	while ( atual != NULL && strcmp( ( char * )nomeParaRemover, ( char * )( atual + 2 * sizeof( void * ) ) ) != 0 ) {
		anterior = atual;
		atual = *( void ** )atual;
	}

	if ( atual == NULL ) {
		printf( "Pessoa nao encontrada.\n" );
		return;
	}

	void *proximo = *( void ** )atual;

	if ( anterior == NULL ) {
		*head = proximo;
		if ( proximo != NULL ) {
			*( void ** )( proximo + sizeof( void * ) ) = NULL;
		} else {
			*tail = NULL;
		}
	} else {
		*( void ** )anterior = proximo;
		if ( proximo != NULL ) {
			*( void ** )( proximo + sizeof( void * ) ) = anterior;
		} else {
			*tail = anterior;
		}
	}

	printf( "\nPessoa removida com sucesso.\n" );
	*desloc -= 50;  
}

/*
====================
BuscarPessoa
 Busca uma pessoa na agenda
====================
*/
void BuscarPessoa( void ) {
	void **head = ( void ** )( pBuffer + 2 * sizeof( int ) );
	if ( *head == NULL ) {
		printf( "\nLista vazia.\n" );
		return;
	}

	int *desloc = ( int * )( pBuffer + sizeof( int ) );
	void *nomeParaBuscar = pBuffer + *desloc;
	*desloc += 50;

	printf( "Informe o nome da pessoa a ser buscada: " );
	fgets( ( char * )nomeParaBuscar, 50, stdin );
	( ( char * )nomeParaBuscar )[strcspn( ( char * )nomeParaBuscar, "\n" )] = '\0';

	void *atual = *head;

	while ( atual != NULL ) {
		char *nome = ( char * )( atual + 2 * sizeof( void * ) );
		if ( strcmp( ( char * )nomeParaBuscar, nome ) == 0 ) {
			int *idade = ( int * )( nome + 50 );
			char *email = ( char * )( idade + 1 );

			printf( "--------------------\n" );
			printf( "\tNome: %s\n", nome );
			printf( "\tIdade: %d\n", *idade );
			printf( "\tEmail: %s\n", email );
			printf( "--------------------\n" );

			*desloc -= 50;  
			return;
		}
		atual = *( void ** )atual;
	}
	printf( "Pessoa nao encontrada.\n" );
	*desloc -= 50;  
}

/*
====================
Listar
 Lista todas as pessoas na agenda
====================
*/
void Listar( void ) {
	void **head = ( void ** )( pBuffer + 2 * sizeof( int ) );
	if ( *head == NULL ) {
		printf( "\nLista vazia.\n" );
		return;
	}
	void *atual = *head;

	printf( "\n" );

	while ( atual != NULL ) {
		char *nome = ( char * )( atual + 2 * sizeof( void * ) );
		int *idade = ( int * )( nome + 50 );
		char *email = ( char * )( idade + 1 );

		printf( "--------------------\n" );
		printf( "\tNome: %s\n", nome );
		printf( "\tIdade: %d\n", *idade );
		printf( "\tEmail: %s\n", email );

		atual = *( void ** )atual;
	}
	printf( "--------------------\n" );
}
