#include "AC_NavigationComponent.h"
#include "EngineUtils.h" // For TActorIterator
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "Algo/Reverse.h"
#include "DrawDebugHelpers.h" // Add this include for debug drawing

// Sets default values for this component's properties
UAC_NavigationComponent::UAC_NavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAC_NavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UAC_NavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/**
 * @brief Finds the closest navigation nodes to the AI character (start) and the target location (end).
 *
 * Iterates over all ANode actors in the world to determine which node is closest to the AI character's current location
 * (StartNode) and which is closest to the specified target location (EndNode). Sets StartNode and EndNode accordingly.
 * If both nodes are valid and distinct, initiates pathfinding using the A* algorithm.
 *
 * @param TargetLocation The world-space location to find the closest end node to.
 */
void UAC_NavigationComponent::FindStartAndEndNodes(const FVector& TargetLocation)
{
    // Get the owner actor of this component (the AI character)
    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    // Get the world context
    UWorld* World = GetWorld();
    if (!World)
        return;
    
    // Variables to track the closest nodes to the owner (start) and the target (end)
    ANode* ClosestStartNode = nullptr;
    ANode* ClosestEndNode = nullptr;
    float ClosestStartDistSq = TNumericLimits<float>::Max();
    float ClosestEndDistSq = TNumericLimits<float>::Max();
    FVector OwnerLocation = Owner->GetActorLocation();

    int32 NodeCount = 0;
    // Iterate over all nodes in the world
    for (TActorIterator<ANode> It(World); It; ++It)
    {
        NodeCount++;
        ANode* Node = *It;
        if (Node)
        {
            // Calculate squared distance from owner to node
            float StartDistSq = FVector::DistSquared(OwnerLocation, Node->GetActorLocation());
            // Update closest start node if this node is closer
            if (StartDistSq < ClosestStartDistSq)
            {
                ClosestStartDistSq = StartDistSq;
                ClosestStartNode = Node;
            }

            // Calculate squared distance from target to node
            float EndDistSq = FVector::DistSquared(TargetLocation, Node->GetActorLocation());
            // Update closest end node if this node is closer
            if (EndDistSq < ClosestEndDistSq)
            {
                ClosestEndDistSq = EndDistSq;
                ClosestEndNode = Node;
            }
        }
    }

    // Set the found nodes as start and end nodes
    StartNode = ClosestStartNode;
    EndNode = ClosestEndNode;

    // Debug draw spheres for start (red) and end (blue) nodes
    if (StartNode)
    {
        DrawDebugSphere(World, StartNode->GetActorLocation(), 50.0f, 16, FColor::Red, false, 200.0f);
    }
    if (EndNode)
    {
        DrawDebugSphere(World, EndNode->GetActorLocation(), 50.0f, 16, FColor::Blue, false, 200.0f);
    }

    // If both nodes are found and are not the same, call FindPathAStar
    if (StartNode && EndNode && StartNode != EndNode)
        FindPathAStar();
}

/**
 * Attempts to find a path from StartNode to EndNode using the A* search algorithm.
 * The resulting path is stored in OutPath if found.
 *
 * @param OutPath Reference to an array that will be filled with the path nodes (from start to end) if a path is found.
 * @return true if a path was found, false otherwise.
 *
 * The function uses the following logic:
 * - Initializes all node costs.
 * - Uses a heuristic (Euclidean distance) to estimate cost to the goal.
 * - Maintains an open set (nodes to be evaluated) and a closed set (already evaluated).
 * - For each node, checks all linked neighbors and updates their costs if a better path is found.
 * - When the EndNode is reached, reconstructs the path using the CameFrom map.
 */
bool UAC_NavigationComponent::FindPathAStar()
{
    // Heuristic function: straight-line distance between two nodes
    auto Heuristic = [](ANode* A, ANode* B) {
        return FVector::Dist(A->GetActorLocation(), B->GetActorLocation());
    };

    // OpenSet: nodes to be evaluated; ClosedSet: nodes already evaluated
    TArray<ANode*> OpenSet;
    TSet<ANode*> ClosedSet;
    TMap<ANode*, ANode*> CameFrom;

    // Initialize all node costs to "infinity"
    for (TActorIterator<ANode> It(GetWorld()); It; ++It)
    {
        It->GCost = TNumericLimits<float>::Max();
        It->HCost = 0.0f;
        It->FCost = TNumericLimits<float>::Max();
    }

	// Initialize start node costs and add it to the OpenSet
    StartNode->GCost = 0.0f;
    StartNode->HCost = Heuristic(StartNode, EndNode);
    StartNode->FCost = StartNode->HCost;
    OpenSet.Add(StartNode);

    // Main A* loop
    while (OpenSet.Num() > 0)
    {
        ANode* Current = OpenSet[0];

        // Find node in OpenSet with lowest FCost (and lowest HCost as tiebreaker)
        for (ANode* Node : OpenSet)
        {
           if (Node->FCost < Current->FCost || (Node->FCost == Current->FCost && Node->HCost < Current->HCost))
                Current = Node;
        }

        // If we've reached the goal, reconstruct the path and return
        if (Current == EndNode)
        {
            bool bPathFound = FinalizePath(ClosedSet, CameFrom);
            return bPathFound;
        }

        // Move current node from open to closed set
        OpenSet.Remove(Current);
        ClosedSet.Add(Current);

        // Check all neighbors of the current node
        for (ANode* Neighbor : Current->LinkedNodes)
        {
            if (!Neighbor || ClosedSet.Contains(Neighbor))
                continue;

            // Calculate tentative G cost for neighbor
            float TentativeG = Current->GCost + FVector::Dist(Current->GetActorLocation(), Neighbor->GetActorLocation());

            // If this path to neighbor is better, record it
            if (TentativeG < Neighbor->GCost)
            {
                CameFrom.Add(Neighbor, Current);
                Neighbor->GCost = TentativeG;
                Neighbor->HCost = Heuristic(Neighbor, EndNode);
                Neighbor->FCost = Neighbor->GCost + Neighbor->HCost;

                if (!OpenSet.Contains(Neighbor))
                    OpenSet.Add(Neighbor);
            }
        }
    }

    // No path found
    FinalPath.Empty();
    return false;
}

// Finalizes the path from the closed set and CameFrom map, storing it in FinalPath
bool UAC_NavigationComponent::FinalizePath(const TSet<ANode*>& ClosedSet, const TMap<ANode*, ANode*>& CameFrom)
{
	FinalPath.Empty();

	if (!StartNode || !EndNode)
		return false;

	// Reconstruct the path from EndNode to StartNode using the CameFrom map
	ANode* PathNode = EndNode;
	while (PathNode)
	{
		FinalPath.Add(PathNode);
		if (PathNode == StartNode)
			break;
		// Use PathNode as the key to get the previous node
		PathNode = CameFrom.Contains(PathNode) ? CameFrom[PathNode] : nullptr;
	}

	Algo::Reverse(FinalPath);

	// Draw debug spheres for the final path (green)
	UWorld* World = GetWorld();
	if (World)
	{
		for (ANode* Node : FinalPath)
		{
			DrawDebugSphere(
				World,
				Node->GetActorLocation(),
				50.0f,
				16,
				FColor::Green,
				false,
				10.0f
			);
		}
	}

	return true;
}

