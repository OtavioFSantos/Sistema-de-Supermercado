/*
Sistema de Supermercado
Autor: Otávio Fernandes dos Santos
*/

#include <stdio.h>
#include <stdlib.h>
#include"ProjetoBib.h"
#include"ProjetoBib.c"

int main(){
    int esc;
	tamanho();
    produto = (struct prod *)malloc(TAM * sizeof(struct prod));
	crialista();
	criarCadeia();
    for (;;){
		esc = menu();
		switch (esc){
		case 1:
			compra();
			break;
		case 2:
			remover();
			break;
		case 3:
			cadastrar();
			break;
        case 4:
			verifica();
			break;
		case 5:
			atualiza();
			break;
		case 6:
			exit(0);
			break;
		}
	}
	return 0;
}