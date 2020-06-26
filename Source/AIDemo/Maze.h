// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Wall.h"
#include "MazeGenerator.h"
#include "Maze.generated.h"

UCLASS()
class AIDEMO_API AMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaze(const FObjectInitializer& oi);

	virtual ~AMaze();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = level)
		UBoxComponent* bbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = level)
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Content Generation")
		int rows;

		int cols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Content Generation")
		float blockSpacing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Procedural Content Generation")
		FVector start;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Procedural Content Generation")
		TSubclassOf<AWall> wall_template;

	UFUNCTION(BlueprintCallable, Category = "Procedural Content Generation")
		FVector getRandomFreeLocation();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitWalls();

private:
	class AWall*** walls;
	int free_cells;
};