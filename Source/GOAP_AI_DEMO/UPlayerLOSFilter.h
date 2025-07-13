// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UNavFilter.h"
#include "Node.h" // Ensure Node is included
#include "UPlayerLOSFilter.generated.h"

/**
 * @class UUPlayerLOSFilter
 * @brief Navigation filter that checks if a node is in the line of sight (LOS) of a player.
 */
UCLASS()
class GOAP_AI_DEMO_API UUPlayerLOSFilter : public UUNavFilter
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Validates if the given node is valid.
	 * @param Node Pointer to the node to validate.
	 * @return true if the node is valid, false otherwise.
	 */
	virtual bool IsNodeValid(ANode* Node) const override;

	/**
	 * @brief Checks if the given node is in the line of sight (LOS) of any player.
	 * @param Node Pointer to the node to check.
	 * @return true if the node is in LOS of a player, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool IsNodeInPlayerLOS(ANode* Node) const;
};
