// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"
#include <TunnelTerror/TunnelTerrorCharacter.h>
#include "Components/BoxComponent.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	Killbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Killbox"));

	SetRootComponent(SceneRoot);
	Killbox->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		Killbox->OnComponentBeginOverlap.AddDynamic(this, &AHazard::OnHazardOverlap);
	}
}

void AHazard::OnHazardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	if (ATunnelTerrorCharacter* character = Cast<ATunnelTerrorCharacter>(OtherActor)) {
		OnKill(character);
		character->DecreaseHealth(100, DisplayName);
	}
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}