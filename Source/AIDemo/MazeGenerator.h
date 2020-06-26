#ifndef __LABERINTO_H__
#define __LABERINTO_H__

#include "CoreMinimal.h"

void error_fatal(const char* mensaje);

class MazeGenerator
{
public:
	MazeGenerator(int cells);
	~MazeGenerator();
	void build();
	bool isWall(int row, int col);
	FString serialize();

private:
	int cells;
	char** m;
};

#endif
