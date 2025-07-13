#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeTypes.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Node.generated.h"

UCLASS()
class GOAP_AI_DEMO_API ANode : public AActor
{
    GENERATED_BODY()

public:
    ANode();

    // Linked nodes for movement or decision-making
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    TMap<ANode*, ENodeConnectionType> LinkedNodes;

    // Type of this node (e.g. Patrol, Cover)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    ENodeType NodeType;

    // Mesh component to be set in Blueprint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
    UStaticMeshComponent* MeshComponent;

    // Debug splines for visualization
    UPROPERTY(Transient)
    TArray<USplineComponent*> DebugSplines;

    // A* Search costs
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AStar")
    float HCost = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AStar")
    float GCost = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AStar")
    float FCost = 0.0f;

public:
    // Call this to update the node's color based on its type
    void UpdateNodeColor();

    // Call this to update the spline visualization for linked nodes
    void UpdateLinkSplines();

    // Optionally, override PostEditChangeProperty for editor changes
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void BeginPlay() override;
};
