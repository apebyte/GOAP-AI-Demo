// Fill out your copyright notice in the Description page of Project Settings.


#include "UNavFilter.h"

/**
 * @brief Checks if the given node is valid for navigation filtering.
 *
 * This function performs a basic validity check to determine if the provided node pointer is not null.
 * You can extend this function to include additional validation logic as needed.
 *
 * @param Node Pointer to the node to validate.
 * @return true if the node is valid (not null), false otherwise.
 */
bool UUNavFilter::IsNodeValid(ANode* Node) const
{
	// Basic validity check: node pointer is not null
	return Node != nullptr;
}

