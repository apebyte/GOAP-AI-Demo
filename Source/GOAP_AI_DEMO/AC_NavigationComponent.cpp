// Fill out your copyright notice in the Description page of Project Settings.

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

void UAC_NavigationComponent::FindStartAndEndNodes(const FVector& TargetLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("FindStartAndEndNodes CALLED"));

    // Get the owner actor of this component (the AI character)
    AActor* Owner = GetOwner();
    if (!Owner) {
        UE_LOG(LogTemp, Warning, TEXT("FindStartAndEndNodes: Owner is nullptr"));
        return;
    }

    // Get the world context
    UWorld* World = GetWorld();
    if (!World) {
        UE_LOG(LogTemp, Warning, TEXT("FindStartAndEndNodes: World is nullptr"));
        return;
    }
    
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
			float StartDistSq = FVector::DistSquared(OwnerLocation, Node->GetActorLocation());
			if (StartDistSq < ClosestStartDistSq)
			{
				ClosestStartDistSq = StartDistSq;
				ClosestStartNode = Node;
			}

			float EndDistSq = FVector::DistSquared(TargetLocation, Node->GetActorLocation());
			if (EndDistSq < ClosestEndDistSq)
			{
				ClosestEndDistSq = EndDistSq;
				ClosestEndNode = Node;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total nodes found: %d"), NodeCount);
	UE_LOG(LogTemp, Warning, TEXT("Node search loop finished"));

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

	UE_LOG(LogTemp, Warning, TEXT("StartNode: %s, EndNode: %s"), 
		StartNode ? *StartNode->GetName() : TEXT("None"), 
		EndNode ? *EndNode->GetName() : TEXT("None"));

	// If both nodes are found and are not the same, call FindPathAStar
	if (StartNode && EndNode && StartNode != EndNode)
	{
		TArray<ANode*> OutPath;
		//FindPathAStar(OutPath);
	}
}

bool UAC_NavigationComponent::FindPathAStar(TArray<ANode*>& OutPath)
{
	OutPath.Empty();

	if (!StartNode || !EndNode || StartNode == EndNode)
		return false;

	auto Heuristic = [](ANode* A, ANode* B) {
		return FVector::Dist(A->GetActorLocation(), B->GetActorLocation());
	};

	TArray<ANode*> OpenSet;
	TSet<ANode*> ClosedSet;
	TMap<ANode*, ANode*> CameFrom;

	for (TActorIterator<ANode> It(GetWorld()); It; ++It)
	{
		It->GCost = TNumericLimits<float>::Max();
		It->HCost = 0.0f;
		It->FCost = TNumericLimits<float>::Max();
	}

	StartNode->GCost = 0.0f;
	StartNode->HCost = Heuristic(StartNode, EndNode);
	StartNode->FCost = StartNode->HCost;

	OpenSet.Add(StartNode);

	while (OpenSet.Num() > 0)
	{
		ANode* Current = OpenSet[0];
		for (ANode* Node : OpenSet)
		{
			if (Node->FCost < Current->FCost ||
				(Node->FCost == Current->FCost && Node->HCost < Current->HCost))
			{
				Current = Node;
			}
		}

		if (Current == EndNode)
		{
			 // Call FinalizePath to fill FinalPath
			bool bPathFound = FinalizePath(ClosedSet, CameFrom);
			OutPath = FinalPath;
			return bPathFound;
		}

		OpenSet.Remove(Current);
		ClosedSet.Add(Current);

		for (ANode* Neighbor : Current->LinkedNodes)
		{
			if (!Neighbor || ClosedSet.Contains(Neighbor))
				continue;

			float TentativeG = Current->GCost + FVector::Dist(Current->GetActorLocation(), Neighbor->GetActorLocation());

			if (TentativeG < Neighbor->GCost)
			{
				CameFrom.Add(Neighbor, Current);
				Neighbor->GCost = TentativeG;
				Neighbor->HCost = Heuristic(Neighbor, EndNode);
				Neighbor->FCost = Neighbor->GCost + Neighbor->HCost;

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.Add(Neighbor);
				}
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

	// If the end node is not in the closed set, no path was found
	if (!ClosedSet.Contains(EndNode))
		return false;

	// Reconstruct the path from EndNode to StartNode using the CameFrom map
	ANode* PathNode = EndNode;
	while (PathNode)
	{
		FinalPath.Add(PathNode);
		if (PathNode == StartNode)
			break;
		PathNode = CameFrom.Contains(PathNode) ? CameFrom[PathNode] : nullptr;
	}

	// If we didn't reach the start node, the path is incomplete
	if (FinalPath.Last() != StartNode)
		return false;

	Algo::Reverse(FinalPath);

	// Draw debug lines for the final path if valid
	DebugDrawFinalPath();

	return true;
}

void UAC_NavigationComponent::DebugDrawFinalPath() const
{
	if (FinalPath.Num() < 2)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 i = 0; i < FinalPath.Num() - 1; ++i)
	{
		const ANode* FromNode = FinalPath[i];
		const ANode* ToNode = FinalPath[i + 1];
		if (FromNode && ToNode)
		{
			FVector From = FromNode->GetActorLocation();
			FVector To = ToNode->GetActorLocation();
			DrawDebugLine(World, From, To, FColor::White, false, 100.0f, 0, 8.0f);
		}
	}
}

