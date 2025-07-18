// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "GameFramework/GameState.h"
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

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bInLobby;

	bool bDoorClosing;

	UPROPERTY(Replicated, BlueprintReadWrite)
	TArray<ATunnelTerrorCharacter*> Players;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> deathCauses;

	UPROPERTY(BlueprintReadWrite)
	TArray<float> timesAlive;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> playerNames;
	
	bool bAllInfected;

	UPROPERTY(BlueprintReadWrite)
	int playerNo;

public:

	ATunnelTerrorGameState();

	virtual void Tick(float DeltaTime) override;

	void StartChaosTimer();

	UFUNCTION(Server, Reliable)
	void MulticastStartChaosTimer();

	void KillEveryone();

	UFUNCTION(Server, Reliable)
	void MulticastKillEveryone();

	void AddPlayer(ATunnelTerrorCharacter* Character);

	void RemovePlayer(ATunnelTerrorCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ResetTimers();

private:

	AElevatorEscape* ElevatorEscape;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
