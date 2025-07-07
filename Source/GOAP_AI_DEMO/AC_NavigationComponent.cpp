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
	AActor* Owner = GetOwner();
	if (!Owner) return;

	UWorld* World = GetWorld();
	if (!World) return;

	ANode* ClosestStartNode = nullptr;
	ANode* ClosestEndNode = nullptr;
	float ClosestStartDistSq = TNumericLimits<float>::Max();
	float ClosestEndDistSq = TNumericLimits<float>::Max();
	FVector OwnerLocation = Owner->GetActorLocation();

	// Iterate over all nodes in the world
	for (TActorIterator<ANode> It(World); It; ++It)
	{
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

	StartNode = ClosestStartNode;
	EndNode = ClosestEndNode;
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

void UAC_NavigationComponent::DebugDrawFinalPath(float Duration /*= 2.0f*/, FColor LineColor /*= FColor::Green*/) const
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
			DrawDebugLine(World, From, To, LineColor, false, Duration, 0, 8.0f);
		}
	}
}

