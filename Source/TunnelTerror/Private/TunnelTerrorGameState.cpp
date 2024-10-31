// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelTerrorGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ElevatorEscape.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Net/UnrealNetwork.h"

ATunnelTerrorGameState::ATunnelTerrorGameState()
{
	bGameTime = true;
	chaosTime = 30.0f;
	bChaosTime = false;
	gameTime = 600.0f;
	bDoorClosing = false;
}

void ATunnelTerrorGameState::BeginPlay()
{
	ElevatorEscape = Cast<AElevatorEscape>(UGameplayStatics::GetActorOfClass(GetWorld(), AElevatorEscape::StaticClass()));

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATunnelTerrorCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ATunnelTerrorCharacter* Character = Cast<ATunnelTerrorCharacter>(Actor))
		{
			Players.Add(Character);
		}
	}
}

void ATunnelTerrorGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && Players.Num() > 0)
	{
		bAllInfected = true;

		for (ATunnelTerrorCharacter* Player : Players)
		{
			if (Player && !Player->GetIsInfected())
			{
				bAllInfected = false;
				break;
			}
		}

		if (bAllInfected)
		{
			gameTime = 0.0f;
			bChaosTime = false;
		}
	}

	if (bChaosTime == true && HasAuthority())
	{
		chaosTime -= DeltaTime;
		gameTime = chaosTime;
		if (chaosTime <= 0.0f)
		{
			chaosTime = 0.0f;
			bChaosTime = false;
			KillEveryone();
			if (!bDoorClosing)
			{
				ElevatorEscape->ServerPlayDoorCloseAnimation();
				bDoorClosing = true;
			}
		}
	}

	if (gameTime <= 0.0f)
	{
		playerNo = 0;
		for (ATunnelTerrorCharacter* Player : Players)
		{
			deathCauses.Add(Player->causeOfDeath);
			timesAlive.Add(Player->timeAlive);
			playerNo++;
		}
	}
}

void ATunnelTerrorGameState::KillEveryone()
{
	MulticastKillEveryone();
}

void ATunnelTerrorGameState::MulticastKillEveryone_Implementation()
{
	for (ATunnelTerrorCharacter* player : Players)
	{
		if (player)
		{
			player->DecreaseHealth(100.0f, "The entity");
		}
	}
}

void ATunnelTerrorGameState::StartChaosTimer()
{
	MulticastStartChaosTimer();
}

void ATunnelTerrorGameState::MulticastStartChaosTimer_Implementation()
{
	bChaosTime = true;
	bGameTime = false;
	UE_LOG(LogTemp, Log, TEXT("Chaos Timer Started"));
}

void ATunnelTerrorGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATunnelTerrorGameState, bGameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, gameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, chaosTime);
	DOREPLIFETIME(ATunnelTerrorGameState, bChaosTime);
}

void ATunnelTerrorGameState::AddPlayer(ATunnelTerrorCharacter* Character)
{
	if (Character && !Players.Contains(Character))
	{
		Players.Add(Character);

		if (ElevatorEscape)
		{
			ElevatorEscape->samplesNeeded = Players.Num() * 2;
		}
	}
}

void ATunnelTerrorGameState::RemovePlayer(ATunnelTerrorCharacter* Character)
{
	if (Character)
	{
		Players.Remove(Character);

		if (ElevatorEscape)
		{
			ElevatorEscape->samplesNeeded = Players.Num() * 2;
		}
	}
}