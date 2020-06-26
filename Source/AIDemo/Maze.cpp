// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"
#include "Engine.h"
#include "Misc/FileHelper.h"

AMaze::AMaze(const FObjectInitializer& oi)
{
	PrimaryActorTick.bCanEverTick = false;
	bbox = oi.CreateDefaultSubobject<UBoxComponent>(this, TEXT("bbox"));
	RootComponent = bbox;
	mesh = oi.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("mesh"));
	mesh->AttachTo(RootComponent);
	wall_template = nullptr;
	blockSpacing = 400.0f;
	rows = 8;
	cols = 8;
	free_cells = 0;
}

AMaze::~AMaze()
{
	int i;
	if (walls != nullptr)
	{
		for (i = 0; i < rows+1; i++)
			delete walls[i];
		delete walls;
	}
}

void AMaze::BeginPlay()
{
	InitWalls();

	Super::BeginPlay();
}

void AMaze::InitWalls()
{
	float XOffset, YOffset;
	const int32 NumBlocks = rows * cols;
	FVector location = GetActorLocation();
	FVector wall_location;
	int32 i, j, fila, columna;
	bool exito;
	MazeGenerator* generador;
	FVector min_size;
	FVector max_size;
	FVector escala;
	FString buffer;

	mesh->GetLocalBounds(min_size, max_size);
	escala = mesh->GetRelativeScale3D();
	escala = escala * max_size;

	buffer = FString::Printf(TEXT("generando nivel\n"));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, buffer);

	generador = new MazeGenerator(rows);
	if (generador == nullptr)
		UE_LOG(LogTemp, Fatal, TEXT("Fallo la creacion del generador de Laberintos"));

	generador->build();

	exito = FFileHelper::SaveStringToFile(generador->serialize(), *(FPaths::ProjectDir() + TEXT("level.txt")),
		FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail);
	if (!exito)
		UE_LOG(LogTemp, Warning, TEXT("No se pudo grabar el laberinto del nivel"));

/*
Se genera una fila y una columna más para cerrar los bordes
*/
	walls = new AWall * *[rows+1];
	if (walls == nullptr)
		UE_LOG(LogTemp, Fatal, TEXT("No se pudo crear arreglo de bloques"));
	for (i = 0; i < rows+1; i++)
	{
		walls[i] = new AWall * [cols+1];
		for (j = 0; j < cols+1; j++)
			walls[i][j] = nullptr;
	}

	i = 0;
	for (fila = -rows / 2; fila <= rows / 2; fila++, i++)
	{
		XOffset = fila * blockSpacing + blockSpacing / 2;
		j = 0;
		for (columna = -cols / 2; columna <= cols / 2; columna++, j++)
		{
			YOffset = columna * blockSpacing + blockSpacing / 2;
			wall_location = FVector(XOffset, YOffset, 150.0f) + location;
			if (i == 0 || j == 0)
			{
				/*
				La entrada del laberinto
				*/
				if (i == 0 && j == 1)
				{
					walls[i][j] = nullptr;
					start = wall_location;
					free_cells++;
				}
				else
					walls[i][j]=GetWorld()->SpawnActor<AWall>(wall_template, wall_location, FRotator(0, 0, 0));
			}
			else
			{
				if (generador->isWall(i-1, j-1))
					walls[i][j] = GetWorld()->SpawnActor<AWall>(wall_template, wall_location, FRotator(0, 0, 0));
				else
				{
					walls[i][j] = nullptr;
					free_cells++;
				}
			}
		}
	}

	delete generador;
}

FVector AMaze::getRandomFreeLocation()
{
	int stop;
	float XOffset, YOffset;
	FVector location = GetActorLocation();
	FVector wall_location;
	int32 i, j, k, fila, columna;

	if (free_cells == 0)
		return location;

	stop = rand() % free_cells;
	i = 0;
	k = 0;
	for (fila = -rows / 2; fila <= rows / 2; fila++, i++)
	{
		j = 0;
		for (columna = -cols / 2; columna <= cols / 2; columna++, j++)
		{
			if (walls[i][j] == nullptr)
			{
				if (k == stop)
				{
					XOffset = fila * blockSpacing + blockSpacing / 2;
					YOffset = columna * blockSpacing + blockSpacing / 2;
					wall_location = FVector(XOffset, YOffset, 150.0f) + location;
					return wall_location;
				}
				else
					k++;
			}
		}
	}
	return location;
}