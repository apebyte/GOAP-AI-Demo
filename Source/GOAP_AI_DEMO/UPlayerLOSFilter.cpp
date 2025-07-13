// Fill out your copyright notice in the Description page of Project Settings.

#include "UPlayerLOSFilter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Node.h"

/**
 * @brief Overrides the IsNodeValid function from UNavFilter to use LOS filtering.
 *
 * This function checks if the given node is valid by determining if it is in the line of sight of any player.
 *
 * @param Node Pointer to the node to validate.
 * @return true if the node is in LOS of any player, false otherwise.
 */
bool UUPlayerLOSFilter::IsNodeValid(ANode* Node) const
{
	return !IsNodeInPlayerLOS(Node);
}

/**
 * @brief Checks if the given node is in the line of sight (LOS) of any player.
 *
 * This function iterates over all player controllers in the world, retrieves their camera location and forward vector,
 * and performs two tests for each player:
 * 1. Dot Product: Checks if the node is in front of the player's camera.
 * 2. Line Trace: Checks if there is a clear line of sight from the camera to a point slightly above the node.
 *    The trace must not hit any object to be considered valid (since nodes have no collision).
 *
 * @param Node Pointer to the node to check.
 * @return true if the node is in LOS of any player, false otherwise.
 */
bool UUPlayerLOSFilter::IsNodeInPlayerLOS(ANode* Node) const
{
	if (!Node)
		return false;

	UWorld* World = Node->GetWorld();
	if (!World)
		return false;

	const FVector NodeLocation = Node->GetActorLocation();
	const FVector NodeTarget = NodeLocation + FVector(0, 0, 0.1f); // Slightly above the node

	// Iterate over all player controllers
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC)
			continue;

		APawn* Pawn = PC->GetPawn();
		if (!Pawn)
			continue;

		// Try to get the camera component from the pawn
		FVector CameraLocation;
		FVector CameraForward;

		// If the pawn has a camera component, use it
		UCameraComponent* CameraComp = Pawn->FindComponentByClass<UCameraComponent>();
		if (CameraComp)
		{
			CameraLocation = CameraComp->GetComponentLocation();
			CameraForward = CameraComp->GetForwardVector();
		}
		else
		{
			// Fallback: use pawn's location and forward vector
			CameraLocation = Pawn->GetActorLocation();
			CameraForward = Pawn->GetActorForwardVector();
		}

		// Dot product test: is node in front of camera?
		FVector ToNode = (NodeLocation - CameraLocation).GetSafeNormal();
		float Dot = FVector::DotProduct(CameraForward, ToNode);
		const float DotThreshold = 0.0f; // Adjust as needed (0 = 90deg, 1 = 0deg)

		if (Dot < DotThreshold)
			continue; // Node is not sufficiently in front of camera

		// Line trace test: is there a clear line of sight?
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(PlayerLOS), true, Pawn);
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(Pawn);

		bool bHit = World->LineTraceSingleByChannel(
			HitResult,
			CameraLocation,
			NodeTarget,
			ECC_Visibility,
			TraceParams
		);

		// If nothing was hit, there is a clear line of sight to the node
		if (!bHit)
		{
			return true;
		}
	}

	// No player has LOS to this node
	return false;
}
