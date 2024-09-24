// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeHazard.h"

#include "Components/BoxComponent.h"

ABridgeHazard::ABridgeHazard()
{
}

void ABridgeHazard::BeginPlay()
{
	Super::BeginPlay();
}

void ABridgeHazard::Fall_Implementation()
{
	FallVisual();
}

void ABridgeHazard::FallVisual_Implementation()
{
	
}
