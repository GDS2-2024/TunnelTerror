// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "TunnelTerrorGameState.generated.h"

/**
 * 
 */

class ATunnelTerrorCharacer;
class AElevatorEscape;

UCLASS()
class TUNNELTERROR_API ATunnelTerrorGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Replicated, BlueprintReadWrite)
	float chaosTime;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bChaosTime;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float gameTime;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bGameTime;

	bool bDoorClosing;

	UPROPERTY()
	TArray<ATunnelTerrorCharacter*> Players;

	bool bAllInfected;
	bool bAllReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReady;

public:

	ATunnelTerrorGameState();

	virtual void Tick(float DeltaTime) override;

	void StartChaosTimer();

	UFUNCTION(Server, Reliable)
	void MulticastStartChaosTimer();

	void KillEveryone();

	UFUNCTION(Server, Reliable)
	void MulticastKillEveryone();

	UFUNCTION()
	void EndGame();


private:

	TArray<ATunnelTerrorCharacter*> players;

	AElevatorEscape* ElevatorEscape;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
