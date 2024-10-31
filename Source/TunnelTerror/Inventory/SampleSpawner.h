// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPickup.h"
#include "GameFramework/Actor.h"
#include "SampleSpawner.generated.h"

UCLASS()
class TUNNELTERROR_API ASampleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASampleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Propeties
	UPROPERTY(EditAnywhere)
	const TSubclassOf<AItemPickup> SampleClass;

	UPROPERTY(VisibleAnywhere, Replicated)
	AItemPickup* CurrentSample = nullptr;

	FTimerHandle SpawnTimerHandle;
	
	bool bTimerActive;

	// Functions
	UFUNCTION(Server, Reliable)
	void SpawnNewPickup();
	
	UFUNCTION()
	void OnSampleDestroyed(AActor* DestroyedActor);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
