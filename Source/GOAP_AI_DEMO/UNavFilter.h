// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Node.h" // Include the Node header
#include "UNavFilter.generated.h"

/**
 * @class UUNavFilter
 * @brief Navigation filter class for validating nodes in navigation queries.
 *
 * UUNavFilter is a UObject-derived class that provides a method to check if a given node
 * is valid for navigation purposes. Extend this class to implement custom filtering logic.
 */
UCLASS()
class GOAP_AI_DEMO_API UUNavFilter : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Checks if the given node is valid for navigation filtering.
	 *
	 * Performs a basic validity check to determine if the provided node pointer is not null.
	 * Extend this function to include additional validation logic as needed.
	 *
	 * @param Node Pointer to the node to validate.
	 * @return true if the node is valid (not null), false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool IsNodeValid(ANode* Node) const;
};
