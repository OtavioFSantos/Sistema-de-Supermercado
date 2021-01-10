#include"ProjetoBib.h"
int TAM;

struct prod{
    char nome[30];
	int codigo, qtdestoque;
	float preco;	
};

struct ListaProd{
	struct prod prod;
	struct ListaProd *prox;
};

struct prod* produto = NULL;



void crialista(void){
	int i;
	FILE *fp;
	fp = fopen("estoque.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Estoque.\n"); 
        exit(0); 
    } 
	for(i = 0; i < TAM; ++i){
		fscanf(fp, "%s%d%f%d", (produto + i)->nome, &(produto + i)->codigo, &(produto + i)->preco, &(produto + i)->qtdestoque);
	}
	fclose(fp);
}

int menu(void){	
	int escolha = 0;
	do{
		printf("\n\n\n===== Supermercado do Mariltão =====\n");
		printf("1. Realizar uma compra;\n");
        printf("2. Remover mercadoria do estoque;\n");
		printf("3. Cadastrar uma nova mercadoria;\n");
		printf("4. Verificar o estoque;\n");
		printf("5. Atualizar item no estoque;\n");
		printf("6. Fechar o programa.\n");
		printf("===== Por favor, escolha o que deseja fazer: "); scanf("%d", &escolha);
		if(escolha < 1 || escolha > 6)
			printf("\nPor favor, digite um valor válido.\n");
	}while(escolha < 1 || escolha > 6);
	getchar();
	return escolha;
}

void verifica(void){
	int c;
	FILE *fp;
	fp = fopen("estoque.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Estoque.\n"); 
        exit(0); 
    }
	printf("\n\n\n----- Estoque -----\n");
	for(c = 0; c < TAM; ++c){
        printf("Produto %d:\n\nNome: %s\nCódigo: %d\n", c+1, (produto + c)->nome, (produto + c)->codigo);
		printf("Preço: %.2fR$\nQuantidade no estoque: %d\n", (produto + c)->preco, (produto + c)->qtdestoque);
		printf("---------------------------------\n");
	}
	fclose(fp);
}

void cadastrar(void){
	struct ListaProd *inicio, *lista = criarCadeia();
	int escolha, testId = 0, i;
	inicio = lista;
	FILE *fp;
	fp = fopen("estoque.txt", "a");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Estoque.\n"); 
        exit(0); 
    }
	printf("\n\n\n----- Cadastrar -----\n");
	do{
		printf("Informe o nome do produto: "); scanf("%s", (produto)->nome);
        do{
			testId=0;
            printf("Informe o código do produto: "); scanf("%d", &(produto)->codigo);
            for(i=0,lista=inicio;i<TAM;i++,lista=lista->prox){
                if(lista->prod.codigo==(produto)->codigo){
                    printf("Codigo já esta em uso para. \"%s\". por favor ",lista->prod.nome);
                    i=TAM;
                    testId=1;
                }
            }
        }while(testId);

		printf("Informe o preço do produto: "); scanf("%f", &(produto)->preco);
		printf("Informe a quantidade no estoque: "); scanf("%d", &(produto)->qtdestoque);
		fprintf(fp,"%s %d %.2f %d\n", (produto)->nome, (produto)->codigo, (produto)->preco, (produto)->qtdestoque);
		TAM++;
		attTamanho();
		printf("\nDeseja cadastrar mais um produto(1.Sim 2.Não)? "); scanf("%d", &escolha);
		printf("---------------------------------\n");
	}while(escolha == 1);
	fclose(fp);
	limparMemoria(lista);
	crialista();
}

void compra(void){
	int cod, qnt, c, escolha;
	float dinheiro, precofinal = 0;
	FILE *fpnota, *fptemp;
	fpnota = fopen("nota_fiscal.txt", "w");
	if(fpnota == NULL){ 
        fprintf(stderr, "\nErro Nota Fiscal.\n"); 
        exit(0); 
    }
	fprintf(fpnota, "NOTA FISCAL\n");
	printf("\n\n\n----- Comprar -----\n");
	do{
		printf("\nInforme o código do produto a ser comprado: "); scanf("%d", &cod);
		fptemp = fopen("compra.tmp", "w");
		if(fptemp == NULL){ 
        	fprintf(stderr, "\nErro Compra.\n"); 
        	exit(0); 
    	}
		for(c = 0; c < TAM; ++c){
			if((produto+c)->codigo == cod){
				if((produto+c)->qtdestoque <= 0){
					printf("Produto em falta no estoque.\n");
					break;
				}else{
					do{
						printf("Informe a quantidade a ser comprada(0 - %d): ", (produto+c)->qtdestoque); scanf("%d", &qnt);
					}while(qnt > (produto+c)->qtdestoque || qnt < 0);
					printf("Preço: %.2fR$\n", qnt*(produto+c)->preco);
					precofinal += qnt*(produto+c)->preco;
					(produto+c)->qtdestoque = (produto+c)->qtdestoque-qnt;
				}
				fprintf(fpnota,"Produto comprado: %s\nCódigo: %d\nQuantidade: %d\n", (produto+c)->nome, (produto+c)->codigo, qnt);
				fprintf(fpnota,"Preço: %.2fR$\n-------------------\n", qnt*(produto+c)->preco);
			}
			fprintf(fptemp,"%s %d %.2f %d\n", (produto+c)->nome, (produto+c)->codigo, (produto+c)->preco, (produto+c)->qtdestoque);
		}
		fclose(fptemp);
		do{
			printf("\nDeseja comprar mais um produto(1.Sim 2.Não)? "); scanf("%d", &escolha);
		}while(escolha < 1 || escolha > 2);
	}while(escolha == 1);
	printf("Preço final: %.2fR$\nInforme o dinheiro recebido: ", precofinal); scanf("%f", &dinheiro);
	while(dinheiro < precofinal){
		printf("Dinheiro insuficiente! Faltou %.2fR$\nInforme o dinheiro recebido novamente: ", (dinheiro-precofinal)*-1); scanf("%f", &dinheiro);
	}
	printf("Troco: %.2fR$\n", dinheiro - precofinal);
	fprintf(fpnota, "Preço final: %.2fR$\nDinheiro Recebido: %.2fR$\nTroco: %.2fR$\n", precofinal, dinheiro, dinheiro - precofinal);
	fclose(fpnota);
	remove("estoque.txt");
	rename("compra.tmp", "estoque.txt");
}

void atualiza(void){
	int esc, cod, c;
	FILE *fp;
	printf("\n\n\n----- Atualizar -----\n");
	do{
		printf("\nInforme o código do produto a ser atualizado: "); scanf("%d", &cod);
		fp = fopen("atualiza.tmp", "w");
		if(fp == NULL){ 
			fprintf(stderr, "\nErro Atualizar.\n"); 
			exit(0); 
		}
		for(c = 0; c < TAM; ++c){
			if((produto+c)->codigo == cod){
				printf("Informe novo preço de %s: ", (produto+c)->nome); scanf("%f", &(produto+c)->preco);
				printf("Informe a nova quantidade no estoque: "); scanf("%d", &(produto+c)->qtdestoque);
			}
			fprintf(fp,"%s %d %.2f %d\n", (produto+c)->nome, (produto+c)->codigo, (produto+c)->preco, (produto+c)->qtdestoque);
		}
		fclose(fp);
		do{
			printf("\nDeseja atualizar mais um produto(1.Sim 2.Não)? "); scanf("%d", &esc);
		}while(esc < 1 || esc > 2);
	}while(esc == 1);
	remove("estoque.txt");
	rename("atualiza.tmp", "estoque.txt");
}

void remover(void){
	int i, r, aux = 0, testId = 1;
	struct ListaProd *temp, *lista, *inicio;
	FILE *fp;
	printf("\n\n\n----- Remover -----\n");
	printf("\nCódigo do produto a ser removido:");
	scanf("%d",&r);

	lista = criarCadeia();
	inicio = lista;

    for(i = 0, lista = inicio; i < TAM; i++, lista = lista->prox){
        if(lista->prod.codigo == r){
            testId = 0;
        }
    }
	if(testId){
		printf("Código invalido");
	}else{
		lista = inicio;

		for(i=0;i<TAM;i++,lista=lista->prox){
			if(lista->prod.codigo==r){
				aux = 1;
				if(i == 0){
					inicio = lista->prox;
				}else{
					temp->prox = lista->prox;
				}
				free(lista);
				i = TAM--;
			}else{
				temp = lista;
			}
		}
		attTamanho();
		fp = fopen("estoque.txt", "w");
		if(fp == NULL){ 
			fprintf(stderr, "\nErro Estoque.\n"); 
			exit(0);
		}
		lista = inicio;
		for(i = 0; i < TAM; i++, lista = lista->prox){
			fprintf(fp , "%s %d %.2f %d\n", lista->prod.nome, lista->prod.codigo, lista->prod.preco, lista->prod.qtdestoque);
		}
		if(aux)
			printf("\nProduto removido com sucesso.\n");
		fclose(fp);
		limparMemoria(lista);
		crialista();
	}
}

void tamanho(){
	FILE *fp;
	fp = fopen("tam.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Tam.\n"); 
        exit(0); 
    }
	fscanf(fp, "%d", &TAM);
	fclose(fp);
}

void attTamanho(){
	FILE *fp;
	fp = fopen("tam.txt", "w");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Tam.\n"); 
        exit(0); 
    }
	fprintf(fp,"%d",TAM);
	fclose(fp);
}

struct ListaProd *criarCadeia(){
    FILE *fp;
    int i;
	struct ListaProd *lista, *inicio;

    lista = (struct ListaProd *) malloc(sizeof(struct ListaProd));
    inicio = lista;

    fp = fopen("estoque.txt","r");
	if(fp == NULL){ 
        fprintf(stderr, "\nErro Estoque.\n"); 
        exit(0); 
    }

    for(i = 0; i < TAM; i++, lista = lista->prox){
		fscanf(fp,"%s",lista->prod.nome);
		fscanf(fp,"%d",&lista->prod.codigo);
		fscanf(fp,"%f",&lista->prod.preco);
		fscanf(fp,"%d",&lista->prod.qtdestoque);
		lista->prox = (struct ListaProd *) malloc(sizeof(struct ListaProd));
		lista->prox->prox = NULL;
	}
	
    fclose(fp);
	return (struct ListaProd *)inicio;
}

void limparMemoria (struct ListaProd *lista){
	struct ListaProd *temp;
	for(; lista->prox != NULL; lista = temp){
		temp = lista->prox;
		free(lista);
	}
}