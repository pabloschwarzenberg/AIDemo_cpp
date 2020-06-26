// Fill out your copyright notice in the Description page of Project Settings.

#include "Wall.h"

// Sets default values
AWall::AWall(const FObjectInitializer& oi)
{
	PrimaryActorTick.bCanEverTick = false;
	bbox = oi.CreateDefaultSubobject<UBoxComponent>(this, TEXT("bbox"));
	RootComponent = bbox;
	mesh = oi.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("mesh"));
	mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}
