// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassItem.h"
#include "ElevatorEscape.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ACompassItem::ACompassItem()
{
	CompassNeedle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Compass Needle"));
	CompassNeedle->SetupAttachment(GetRootComponent());
	CompassNeedle->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

void ACompassItem::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AElevatorEscape> It(GetWorld()); It; ++It)
	{
		AElevatorEscape* Elevator = *It;

		if (Elevator)
		{
			ElevatorPosition = Elevator->GetActorLocation();
			UE_LOG(LogTexture, Log, TEXT("Set elevator position in Compass"))
		}
	}

}

void ACompassItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Points the compass towards the Elevator Position
	if (Player)
	{
		FVector PlayerPosition = Player->GetActorLocation();
		FVector PlayerForwardVector = Player->GetActorForwardVector();
		FVector DirectionToElevator = ElevatorPosition - PlayerPosition;
		DirectionToElevator.Normalize();
		FRotator PlayerRotation = PlayerForwardVector.Rotation();
		FRotator TargetRotation = DirectionToElevator.Rotation();
		FRotator CompassRotation = TargetRotation - PlayerRotation;
		FRotator ZAxisOnlyRotation = FRotator(0.0f, CompassRotation.Yaw, 0.0f);
		CompassNeedle->SetRelativeRotation(ZAxisOnlyRotation);
	}
}

void ACompassItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACompassItem, CompassNeedle)
	DOREPLIFETIME(ACompassItem, ElevatorPosition)
}
