// Copyright Epic Games, Inc. All Rights Reserved.

#include "GOAP_AI_DEMOPickUpComponent.h"

UGOAP_AI_DEMOPickUpComponent::UGOAP_AI_DEMOPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UGOAP_AI_DEMOPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UGOAP_AI_DEMOPickUpComponent::OnSphereBeginOverlap);
}

void UGOAP_AI_DEMOPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AGOAP_AI_DEMOCharacter* Character = Cast<AGOAP_AI_DEMOCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
