#include "Grafo.h"
#include "Cola.h"
#include "Engine.h"
#include <ctime>

using namespace std;

void error_fatal(const char* s)
{
	UE_LOG(LogTemp, Fatal, TEXT("%s\n"), s);
}

bool operator<(Nodo a, Nodo b)
{
	if (a.d < b.d)
		return true;
	else
		return false;
}

bool operator==(Nodo a, Nodo b)
{
	if (a.id == b.id)
		return true;
	else
		return false;
}

Grafo::Grafo(int nodos)
{
	int i, j;

	this->nodos = nodos;
	this->V = new Nodo[nodos];
	if (this->V == nullptr)
		error_fatal("Error creando grafo");
	inicializarNodos();
	ady = new int*[nodos];
	if (ady == nullptr)
		error_fatal("Error creando matriz de adyacencia");
	for (i = 0; i < nodos; i++)
	{
		ady[i] = new int[nodos];
		if (ady[i] == nullptr)
			error_fatal("Error creando matriz de adyacencia");
		for (j = 0; j < nodos; j++)
			ady[i][j] = -1;
	}
}

Grafo::~Grafo()
{
	int i;

	if (V != nullptr)
		delete V;
	if (ady != nullptr)
	{
		for (i = 0; i < nodos; i++)
			delete ady[i];
		delete ady;
	}
}

void Grafo::generarVecino(int nodo, int fila, int columna)
{
	int vecino;
	int nodos_grilla = (int)sqrt(nodos);

	if (fila < 0 || fila >= nodos_grilla || columna < 0 || columna >= nodos_grilla)
		return;

	vecino = fila * nodos_grilla + columna;
	if (ady[nodo][vecino] == -1 && ady[vecino][nodo] == -1)
	{
		ady[nodo][vecino] = rand() % 100;
		ady[vecino][nodo] = ady[nodo][vecino];
	}
}

void Grafo::generarGrilla()
{
	int i;
	int fila;
	int columna;
	int nodos_grilla = (int)sqrt(nodos);

	srand((int)time(NULL));
	for (i = 0; i < nodos; i++)
	{
		columna = i % nodos_grilla;
		fila = (i - columna) / nodos_grilla;

		generarVecino(i, fila, columna + 1);
		generarVecino(i, fila, columna - 1);
		generarVecino(i, fila + 1, columna);
		generarVecino(i, fila - 1, columna);
	}
}

void Grafo::inicializarNodos()
{
	int i;
	for (i = 0; i < nodos; i++)
	{
		V[i].id = i;
		V[i].d = Grafo::COSTO_INFINITO;
		V[i].p = -1;
	}
}

bool Grafo::existeArco(int u, int v)
{
	if (ady[u][v] == -1)
		return false;
	else
		return true;
}

int Grafo::costo(int u, int v)
{
	return ady[u][v];
}

void Grafo::prim(int s)
{
	int v;
	Nodo u;
	Cola<Nodo> Q;
	inicializarNodos();
	V[s].d = 0;

	for (v = 0; v < nodos; v++)
		Q.encolar(V[v]);

	while (!Q.vacia())
	{
		u = Q.minimo();
		Q.extraer(u);
		for (v = 0; v < nodos; v++)
		{
			if (existeArco(u.id, v))
			{
				if (Q.contiene(V[v]) && costo(u.id, v) < V[v].d)
				{
					V[v].d = costo(u.id, v);
					V[v].p = u.id;
				}
			}
		}
	}
}

FString Grafo::serializar()
{
	int i;
	FString buffer;
	for (i = 0; i < nodos; i++)
	{
		buffer+=FString::Printf(TEXT("%d, p=%d, c=%d\n"),i,V[i].p,V[i].d);
	}
	return buffer;
}

const Nodo& Grafo::getNodo(int i)
{
	return V[i];
}