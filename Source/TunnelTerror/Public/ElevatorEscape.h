// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorEscape.generated.h"

class ATunnelTerrorCharacter;
class USphereComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TUNNELTERROR_API AElevatorEscape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorEscape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddSample(int newSample);

	void AddSampleImplementation(int newSamples);

	UFUNCTION(Server, Reliable)
	void ServerAddSample(int newSamples);

	UFUNCTION(Server, Reliable)
	void MulticastAddSample(int newSamples);

	UFUNCTION(BlueprintImplementableEvent, Category = "Elevator")
	void PlayDoorOpenAnimationImplementation();

	UFUNCTION(Server, Reliable)
	void ServerPlayDoorOpenAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDoorOpenAnimation();

	UPROPERTY(Replicated)
	int samplesNeeded;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int currentSamples;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	ATunnelTerrorCharacter* Character;
};
