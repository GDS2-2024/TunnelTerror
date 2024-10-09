// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelTerrorGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ElevatorEscape.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Net/UnrealNetwork.h"

ATunnelTerrorGameState::ATunnelTerrorGameState()
{
	bGameTime = true;
	chaosTime = 10.0f;
	bChaosTime = false;
	gameTime = 300.0f;
	bDoorClosing = false;
}

void ATunnelTerrorGameState::BeginPlay()
{
	ElevatorEscape = Cast<AElevatorEscape>(UGameplayStatics::GetActorOfClass(GetWorld(), AElevatorEscape::StaticClass()));
}

void ATunnelTerrorGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Players.Empty();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATunnelTerrorCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ATunnelTerrorCharacter* Character = Cast<ATunnelTerrorCharacter>(Actor))
		{
			Players.Add(Character);
		}
	}

	if (HasAuthority())
	{
		bAllInfected = false;

		for (ATunnelTerrorCharacter* Player : Players)
		{
			if (!Player->GetIsInfected())
			{
				break;
			}
			bAllInfected = true;
		}

		if (bAllInfected)
		{
			gameTime = 0.0f;
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
}

void ATunnelTerrorGameState::KillEveryone()
{
	MulticastKillEveryone();
}

void ATunnelTerrorGameState::MulticastKillEveryone_Implementation()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATunnelTerrorCharacter::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		ATunnelTerrorCharacter* player = Cast<ATunnelTerrorCharacter>(actor);
		if (player)
		{
			players.Add(player);
		}
	}

	for (ATunnelTerrorCharacter* player : players)
	{
		if (player)
		{
			player->DecreaseHealth(100.0f);
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

void ATunnelTerrorGameState::EndGame()
{
		
}

void ATunnelTerrorGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATunnelTerrorGameState, bGameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, gameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, chaosTime);
	DOREPLIFETIME(ATunnelTerrorGameState, bChaosTime);
}

