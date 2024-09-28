// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelTerrorGameState.h"
#include "Kismet/GameplayStatics.h"

ATunnelTerrorGameState::ATunnelTerrorGameState()
{
	bGameTime = true;
	chaosTime = 30.0f;
	bChaosTime = false;
	gameTime = 300.0f;
}

void ATunnelTerrorGameState::BeginPlay()
{
	
}

void ATunnelTerrorGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bChaosTime == true && HasAuthority())
	{
		chaosTime -= DeltaTime;
		if (chaosTime <= 0.0f)
		{
			chaosTime = 0.0f;
			KillEveryone();
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

void ATunnelTerrorGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATunnelTerrorGameState, bGameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, gameTime);
	DOREPLIFETIME(ATunnelTerrorGameState, chaosTime);
	DOREPLIFETIME(ATunnelTerrorGameState, bChaosTime);
}