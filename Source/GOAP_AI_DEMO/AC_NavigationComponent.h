#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Node.h"
#include "UNavFilter.h"
#include "AC_NavigationComponent.generated.h"

/**
 * @brief Navigation component for AI pathfinding using customizable node filters.
 *
 * This component finds navigation nodes, applies user-defined filters, and computes paths using the A* algorithm.
 * Filters can be selected in the editor as classes and are instantiated at runtime.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOAP_AI_DEMO_API UAC_NavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	* @brief Finds the nearest node to the owner and the target.
	* @param TargetLocation The world-space location to find the closest end node to.
	*/
	void FindStartAndEndNodes(const FVector& TargetLocation);

	/**
	 * @brief Array of navigation filter classes to be used for node validation.
	 * Editable in Blueprint so users can select which filters to apply.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	TArray<TSubclassOf<UUNavFilter>> NavFilterClasses;

protected:
	/**
	* @brief Default constructor. Sets default values for this component's properties.
	*/
	UAC_NavigationComponent();

	/**
	 * @brief Called when the game starts.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Called every frame.
	 * @param DeltaTime Time since last tick.
	 * @param TickType Type of tick.
	 * @param ThisTickFunction Tick function data.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Finds a path from StartNode to EndNode using A* search.
	 * @return true if a path was found, false otherwise.
	 */
	bool FindPathAStar();

	/**
	 * @brief Finalizes the path from the closed set and CameFrom map.
	 * @param ClosedSet Set of nodes that were evaluated during pathfinding.
	 * @param CameFrom Map of each node to its predecessor, used to reconstruct the path.
	 * @return true if a valid path was reconstructed, false otherwise.
	 */
	bool FinalizePath(const TSet<ANode*>& ClosedSet, const TMap<ANode*, ANode*>& CameFrom);

	/**
	 * @brief Checks if a node is valid for traversal by evaluating all navigation filters.
	 * @param Node The node to evaluate.
	 * @return true if the node passes all filters, false otherwise.
	 */
	bool IsNodeValidForTraversal(ANode* Node) const;

	/**
	 * @brief Instantiated filter objects (not exposed to editor, created at runtime from NavFilterClasses).
	 */
	TArray<UUNavFilter*> NavFilters;

	/**
	 * @brief The starting node for navigation (where the AI character begins).
	 * @note Not visible or accessible in Blueprints.
	 */
	ANode* StartNode = nullptr;

	/**
	 * @brief The end node for navigation (closest to the target).
	 * @note Not visible or accessible in Blueprints.
	 */
	ANode* EndNode = nullptr;

	/**
	 * @brief The finalized path from start to end node.
	 * @note Not visible or accessible in Blueprints.
	 */
	TArray<ANode*> FinalPath;
};
