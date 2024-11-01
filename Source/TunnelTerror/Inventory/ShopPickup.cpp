// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopPickup.h"

#include "TunnelTerror/TunnelTerrorCharacter.h"

void AShopPickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	if(GetLocalRole() == ROLE_Authority) // Only the server should handle the pickup
	{
		if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		{
			if (Player->CollidedShopPickup)
			{
				Player->CollidedShopPickup = nullptr;
				if (Player->IsLocallyControlled())
				{
					Player->OnRep_CollidedShopPickup();
				}
			}
			Player->CollidedShopPickup = this;
			if (Player->IsLocallyControlled())
			{
				Player->OnRep_CollidedShopPickup();
			}
		}
	}
}

void AShopPickup::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(GetLocalRole() == ROLE_Authority) // Only the server should handle the pickup
	{
		if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		{
			if (Player->CollidedShopPickup == this)
			{
				Player->CollidedShopPickup = nullptr;
				if (Player->IsLocallyControlled())
				{
					Player->OnRep_CollidedShopPickup();
				}
			}
		}
	}
}
