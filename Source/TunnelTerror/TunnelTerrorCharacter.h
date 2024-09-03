// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inventory/InventoryItem.h"
#include "PlayerHUD.h"
#include "TunnelTerrorCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
class UDrillMachine;

UCLASS(config=Game)
class ATunnelTerrorCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Inventory Input */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputMappingContext* InventoryMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Inventory Input Actions */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot1;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot2;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot3;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot4;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot5;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Scroll;
	
public:
	ATunnelTerrorCharacter();

protected:
	virtual void BeginPlay();

public:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Player Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	UInventoryComponent* Inventory;

	UFUNCTION()
	void EquipToInventory(AInventoryItem* NewItem);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipToInventory(AInventoryItem* InventoryItem);

	UFUNCTION(Client, Reliable)
	void ClientUpdateInventoryUI(AInventoryItem* NewItem);
	
	UFUNCTION()
	void UseSelectedItem();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInfected;

	UPROPERTY(VisibleAnywhere)
	float health;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();
	
	/// <summary>
	/// Ragdolls and infects the player
	/// </summary>
	void Die();

	/// <summary>
	/// Unragdolls them after they die
	/// </summary>
	void Reanimate();

	UFUNCTION(BlueprintGetter)
	bool IsRagdolled() const
	{
		return bIsRagdolled;
	}

	void SetIsRagdolled(const bool bNewRagdolled);

	UFUNCTION(BlueprintCallable)
	void SetIsInfected(bool bIsNowInfected);

	UFUNCTION(BlueprintCallable)
	bool GetIsInfected();

	UFUNCTION()
	void DecreaseHealth(float damage);

	UDrillMachine* DrillMachine;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for inventory input */
	void SelectSlot1(const FInputActionValue& Value);
	void SelectSlot2(const FInputActionValue& Value);
	void SelectSlot3(const FInputActionValue& Value);
	void SelectSlot4(const FInputActionValue& Value);
	void SelectSlot5(const FInputActionValue& Value);
	void ScrollSlots(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UFUNCTION(BlueprintNativeEvent)
	void OnRagdoll();
	/*UFUNCTION(BlueprintImplementableEvent)
	void OnRagdollBP(const bool bIsRagdolled);*/
	void Interact(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	UPROPERTY(ReplicatedUsing = OnRagdoll, BlueprintGetter = IsRagdolled, Replicated)
	bool bIsRagdolled = false;

};

