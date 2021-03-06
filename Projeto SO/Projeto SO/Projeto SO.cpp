// Projeto SO.cpp : define o ponto de entrada para o aplicativo do console.
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include<stdio.h>

//Estrutura dos processos de entrada
struct processo {
	int entrada; // tempo que falta pra entrar na fila de prontos
	int pico; // tempo que demora a execução
	float espera; // tempo de espera 
	float retorno; // tempo de retorno 
	float resposta; // tempo de resposta
	int jaExecutou; // verifica se ja foi executado alguma vez
};
std::ifstream arquivo;
std::vector<processo> processos; // vetor com todos os processos
std::vector<processo> backup; // vetor que serve de backup para o vetor processos
std::vector<int> filaPronto; // vetor com os indices dos processos que estao na fila de espera(somente usado para o RR)
std::vector<int> auxiliar; // vetor que auxilia a construção do vetor filaPronto
std::vector<int> vazio; // vetor vazio para zerar auxiliar

void printar() {
	/*for(int i = 0 ; i<processos.size(); i++)
		printf("%d %d\n", processos[i].entrada, processos[i].pico);
	printf("Fila Pronto:");
	for(int i = 0 ; i < filaPronto.size(); i++)
		printf("%d", filaPronto[i]);
	printf("\n Auxiliar:");
	for (int i = 0; i < auxiliar.size(); i++) {
		printf("%d", auxiliar[i]);
	}*/
	for (int i = 0; i<processos.size(); i++)
		printf("%d %d %f %f %f\n", processos[i].entrada, processos[i].pico,processos[i].resposta,processos[i].retorno,processos[i].espera);

	printf("\n----------------------------------------------------------------------------\n");
}

// Inicia o vetor filaPronto com os processos que tem entrada 0 no inicio da execução
void filaDeProntos() {
	for (int i = 0; i < processos.size(); i++) {
		if (processos[i].entrada == 0)
			filaPronto.push_back(i);
	}
}

// Calcula o tempo de espera Médio
float esperaMedio() {
	float soma = 0;
	for (int i = 0; i < processos.size(); i++) {
		soma = soma + processos[i].espera;
	}
	return soma / processos.size();
}

// Calcula o tempo de retorno Médio
float retornoMedio() {
	float soma = 0;
	for (int i = 0; i < processos.size(); i++) {
		soma = soma + processos[i].retorno;
	}
	return soma / processos.size();
}


// Calcula o tempo de resposta Médio
float respostaMedio() {
	float soma = 0;
	for (int i = 0; i < processos.size(); i++) {
		soma = soma + processos[i].resposta;
	}
	return soma / processos.size();
}

// Confere o indice do proximo processo a ser executado
int confereSjf() {
	int i, j, menor;
	menor = 70000;// inicia com um valor muito alto para poder pegar o menor depois
	j = -1;
	for (i = 0; i < processos.size(); i++) {
		if (processos[i].entrada == 0 && processos[i].pico > 0) {
			if (menor > processos[i].pico) {
				menor = processos[i].pico;
				j = i;
			}
		}
	}
	return j;
}


// Confere se o indice do processo ja esta no vetor filaPronto
int confereFila(int num) {
	for (int i = 0; i < filaPronto.size(); i++) {
		if (filaPronto[i] == num) {
			return 0; // Significa que o indice ja esta no vetor.
		}
	}
	return 1; // Significa que o indice não esta no vetor.
}

// Confere se o indice do processo ja esta no vetor auxiliar
int confereAux(int num) {
	for (int i = 0; i < auxiliar.size(); i++) {
		if (auxiliar[i] == num) {
			return 0; // Significa que o indice ja esta no vetor.
		}
	}
	return 1; // Significa que o indice não esta no vetor.
}

// Passa um ciclo de execução para todos os processos
void passaTempo(int j) {
	for (int i = 0; i < processos.size(); i++) {
		if (processos[i].pico > 0) {										// verifica se ja terminou de ser executado
			if (j == i) {													// verifica se o indice passado é o mesmo que está sendo executado
				processos[i].pico--;										// decrementa o tempo que falta para terminar a execução
				processos[i].jaExecutou = 1;								// mostra que o processo ja entrou pelo menos uma vez em execução
				processos[i].retorno++;										// incrementa o tempo de retorno
			}
			else {
				if (processos[i].entrada > 0) {								// verifica se ainda falta tempo pra entrar na fila de prontos
					processos[i].entrada--;									// decrementa o tempo que falta para entrar na fila de prontos
				}
				else {
					processos[i].espera++;									// incrementa o tempo de espera
					processos[i].retorno++;									// incrementa o tempo de retorno
					if (processos[i].jaExecutou == 0) {						// verifica se o processo ainda nao foi executado
						processos[i].resposta++;							// incrementa o tempo de resposta
					}
				}
				if (processos[i].entrada == 0 && processos[i].jaExecutou == 0 && confereFila(i) && confereAux(i)) { /* verifica se ele esta na fila de prontos,se ainda nao foi executado
																									e se ainda nao se encontra no vetor filaProntos*/
					auxiliar.push_back(i);									// coloca o indice no vetor auxiliar que sera passado para o vetor filaProntos posteriormente
				}
			}
		}
	}
}

int terminou() {															// Verifica se todos os processos terminaram
	int teste = 0;															/* variavel que indica se todos processos terminaram, 1 = ainda existem processos a serem
																			executados , 0 = todos os processos foram executados*/
	for (int i = 0; i < processos.size(); i++) {
		if (processos[i].pico > 0) {
			teste = 1;
			return teste;
		}
	}
	return teste;
}

void fcfs() {																//algoritmo de escalonamento First-Come,First-Served
	if (processos[0].entrada > 0) {											// verifica se ja existe um processo na fila de prontos
		for (int i = processos[0].entrada; i < 0; i--) {
			passaTempo(-1);													// se nao houver, passa o tempo para todos os processos ate que um possa ser executado
		}
	}
	while (terminou()) {
		for (int i = 0; i < processos.size(); i++) {
			while (processos[i].entrada > 0) {								// se nao houver um processo pra ser executado, passa o tempo para todos os processos
				passaTempo(-1);
			}
			for (int j = processos[i].pico; j > 0; j--) {					/*quando se achar um processo que possa ser executado, executamos ele ate ele terminar*/
				passaTempo(i);

			}
		}
	}
}

void sjf() {																//algoritmo de escalonamento Shortest Job First
	if (processos[0].entrada > 0) {											// verifica se ja existe um processo na fila de prontos
		for (int i = processos[0].entrada; i < 0; i--) {
			passaTempo(-1);													// se nao houver, passa o tempo para todos os processos ate que um possa ser executado
		}
	}
	while (terminou()) {
		int j = confereSjf();												/*pega o proximo processo a ser executado*/
		while (j == -1) {													/*se nao houver nenhum para ser executado passa o tempo e tenta novamente até um ser encontrado*/
			passaTempo(j);
			j = confereSjf();
		}
		for (int i = processos[j].pico; i > 0; i--) {						/*executa o menor processo ate acabar*/
			passaTempo(j);
		}
	}
}
void rr() {																	/*algoritmo de escalonamento Round Robin(com quantum 2)*/
	while (processos[0].entrada > 0) {											// verifica se ja existe um processo na fila de prontos
			passaTempo(-1);		
	}
	filaDeProntos();														/*inicia o vetor filaProntos com o processos que ja podem ser executados*/
	auxiliar = filaPronto;
	while (terminou()) {
		filaPronto = auxiliar;
		auxiliar = vazio;													// zera o vetor auxiliar
		for (int i = 0; i < filaPronto.size(); i++) {
			if (processos[filaPronto[i]].entrada == 0 && processos[filaPronto[i]].pico > 0) {
				passaTempo(filaPronto[i]);
				if (processos[filaPronto[i]].entrada == 0 && processos[filaPronto[i]].pico > 0) {
					passaTempo(filaPronto[i]);
				}
			}
			if (processos[filaPronto[i]].pico > 0) {
				auxiliar.push_back(filaPronto[i]);
			}
			while (terminou() == 1  && auxiliar.size() == 0 && i == filaPronto.size() - 1) {
					passaTempo(-1);													// se nao houver, passa o tempo para todos os processos ate que um possa ser executado
			}
		}
	}
}

int main()
{
	std::string arquivoname;
	std::cout << "Digite o nome do arquivo(ex. exemplo.txt): ";
	std::cin >> arquivoname;
	arquivo.open(arquivoname.data());
	if (!arquivo.is_open()) {
		system("pause");
		return -1;
	}
	std::string stringer;
	std::string sEntrada;
	std::string sPico;
	while (!arquivo.eof()) {												// constroi o vetor de processos baseado no arquivo recebido
		processo aux;
		std::getline(arquivo, stringer);									// pega a linha do arquivo
		std::stringstream a;
		a << stringer;
		a >> sEntrada >> sPico;												// separa as duas strings separadas pelo espaço em branco
		int entrada, pico, ultimo;
		entrada = std::stoi(sEntrada);										// tranforma a string em int e atribui a entrada
		pico = std::stoi(sPico);											// tranforma a string em int e atribui a pico
		aux.entrada = entrada;												// inicializa o processo com o valor de entrada passado pelo arquivo
		aux.pico = pico;													// inicializa o processo com o valor de pico passado pelo arquivo
		aux.espera = 0;														// zera os outros elementos do processo
		aux.resposta = 0;
		aux.retorno = 0;
		aux.jaExecutou = 0;
		processos.push_back(aux);											// adiciona o processo ao vetor processos
		// ordena o vetor processos com base no tempo de entrada
		ultimo = processos.size() - 1;
		while (ultimo != 0 && processos[ultimo].entrada < processos[ultimo - 1].entrada) {
			processos[ultimo] = processos[ultimo - 1];
			processos[ultimo - 1] = aux;
			ultimo--;
		}

	}
	backup = processos;
	fcfs();
	printf("FCFS %.1f %.1f %.1f\n", retornoMedio(), respostaMedio(), esperaMedio());
	processos = backup;
	sjf();
	printf("SJF %.1f %.1f %.1f\n", retornoMedio(), respostaMedio(), esperaMedio());	
	processos = backup;
	rr();
	printf("RR %.1f %.1f %.1f\n", retornoMedio(), respostaMedio(), esperaMedio());



	system("pause");
	return 0;
}

