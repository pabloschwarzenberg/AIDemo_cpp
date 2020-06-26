#include "MazeGenerator.h"
#include "Grafo.h"

MazeGenerator::MazeGenerator(int cells)
{
	int i, j;
	this->cells = cells;
	m = new char* [cells];
	if (m == nullptr)
		error_fatal("Error creando matriz");
	for (i = 0; i < cells; i++)
	{
		m[i] = new char[cells];
		if (m[i] == nullptr)
			error_fatal("Error creando matriz");
		for (j = 0; j < cells; j++)
			m[i][j] = '*';
	}
}

MazeGenerator::~MazeGenerator()
{
	int i;
	for (i = 0; i < cells; i++)
		delete m[i];
	delete m;
}

void MazeGenerator::build()
{
	int i,nodo;
	int f_o, f_d, c_o, c_d;
	Grafo* grafo;
	int nodos = (cells / 2) * (cells / 2);
	int nodos_grilla = cells / 2;

	grafo = new Grafo(nodos);
	if (grafo == nullptr)
		error_fatal("Imposible generar grafo");
	grafo->generarGrilla();
	grafo->prim(0);

	for (i = 0; i < nodos; i++)
	{
		nodo = grafo->getNodo(i).p;
		if (nodo != -1)
		{
			c_o = i % nodos_grilla;
			f_o = (i - c_o) / nodos_grilla;

			m[f_o * 2][c_o * 2]=' ';

			c_d = nodo % nodos_grilla;
			f_d = (nodo - c_d) / nodos_grilla;

			m[f_d * 2][c_d * 2]=' ';

			if (c_o == c_d)
			{
				m[f_d * 2 + 1][c_d * 2]=' ';
			}
			if (f_o == f_d)
			{
				m[f_d * 2][c_d * 2 + 1]=' ';
			}
		}
	}

	delete grafo;
}

bool MazeGenerator::isWall(int row, int col)
{
	if (m[row][col] == ' ')
		return false;
	else
		return true;
}

FString MazeGenerator::serialize()
{
	int i,j;
	FString buffer;

	for (i = 0; i < cells; i++)
		buffer+=FString::Printf(TEXT("* "));
	buffer += FString::Printf(TEXT("*\n"));

	for (i = 0; i < cells; i++)
	{
		buffer+=FString::Printf(TEXT("* "));
		for (j = 0; j < cells; j++)
		{
			buffer += FString::Printf(TEXT("%c "), m[i][j]);
		}
		buffer += FString::Printf(TEXT("\n"));
	}
	return buffer;
}