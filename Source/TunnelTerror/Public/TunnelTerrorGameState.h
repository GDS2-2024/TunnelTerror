// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TunnelTerrorGameState.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ATunnelTerrorGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	float chaosTime;

	UPROPERTY(BlueprintReadWrite)
	bool bChaosTime;

	UPROPERTY(BlueprintReadWrite)
	bool bGameTime;

public:

	ATunnelTerrorGameState();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartChaosTimer();

};
