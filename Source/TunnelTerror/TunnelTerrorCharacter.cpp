// Copyright Epic Games, Inc. All Rights Reserved.

#include "TunnelTerrorCharacter.h"
#include "TunnelTerrorProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// ATunnelTerrorCharacter

ATunnelTerrorCharacter::ATunnelTerrorCharacter()
{
	bReplicates = true;
	
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	// Create Inventory Component
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	Inventory->SetIsReplicated(true);
}

void ATunnelTerrorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(InventoryMappingContext, 0);
		}

		if(IsLocallyControlled())
		{
			PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
			PlayerHUD->AddToPlayerScreen();
		}
	}
}

void ATunnelTerrorCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATunnelTerrorCharacter, Inventory);
}

//////////////////////////////////////////////////////////////////////////// Input

void ATunnelTerrorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::Look);

		//Inventory Selection
		EnhancedInputComponent->BindAction(Slot1, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot1);
		EnhancedInputComponent->BindAction(Slot2, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot2);
		EnhancedInputComponent->BindAction(Slot3, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot3);
		EnhancedInputComponent->BindAction(Slot4, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot4);
		EnhancedInputComponent->BindAction(Slot5, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot5);
	}
}

void ATunnelTerrorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATunnelTerrorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATunnelTerrorCharacter::SelectSlot1(const FInputActionValue& Value)
{
	Inventory->ChangeSelectedSlot(1);
	PlayerHUD->SetSlotSelection(1);
}

void ATunnelTerrorCharacter::SelectSlot2(const FInputActionValue& Value)
{
	Inventory->ChangeSelectedSlot(2);
	PlayerHUD->SetSlotSelection(2);
}

void ATunnelTerrorCharacter::SelectSlot3(const FInputActionValue& Value)
{
	Inventory->ChangeSelectedSlot(3);
	PlayerHUD->SetSlotSelection(3);
}

void ATunnelTerrorCharacter::SelectSlot4(const FInputActionValue& Value)
{
	Inventory->ChangeSelectedSlot(4);
	PlayerHUD->SetSlotSelection(4);
}

void ATunnelTerrorCharacter::SelectSlot5(const FInputActionValue& Value)
{
	Inventory->ChangeSelectedSlot(5);
	PlayerHUD->SetSlotSelection(5);
}

void ATunnelTerrorCharacter::EquipToInventory(AInventoryItem* NewItem)
{
	if (HasAuthority())
	{
		if (NewItem)
		{
			Inventory->AddItem(NewItem);

			ClientUpdateInventoryUI(NewItem);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to add item to inventory because NewItem is null."));
		}
	}
}

void ATunnelTerrorCharacter::ServerEquipToInventory_Implementation(AInventoryItem* InventoryItem)
{
	EquipToInventory(InventoryItem);
}

void ATunnelTerrorCharacter::ClientUpdateInventoryUI_Implementation(AInventoryItem* NewItem)
{
	if (!PlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD is null in ClientUpdateInventoryUI"));
		return;
	}

	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is null in ClientUpdateInventoryUI"));
		return;
	}

	if (NewItem)
	{
		PlayerHUD->SetSlotIcon(Inventory->GetNumOfItems(), NewItem->InventoryIcon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NewItem is null in ClientUpdateInventoryUI"));
	}
}

void ATunnelTerrorCharacter::UseSelectedItem()
{
	Inventory->GetSelectedItem()->UseItem();
}

void ATunnelTerrorCharacter::Ragdoll_Implementation()
{

}

void ATunnelTerrorCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATunnelTerrorCharacter::GetHasRifle()
{
	return bHasRifle;
}