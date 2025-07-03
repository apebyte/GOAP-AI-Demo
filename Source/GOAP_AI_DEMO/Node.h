#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeTypes.h"
#include "Node.generated.h"

UCLASS()
class GOAP_AI_DEMO_API ANode : public AActor
{
    GENERATED_BODY()

public:
    ANode();

    // Linked nodes for movement or decision-making
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    TArray<ANode*> LinkedNodes;

    // Type of this node (e.g. Patrol, Cover)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    ENodeType NodeType;

    // Mesh component to be set in Blueprint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
    UStaticMeshComponent* MeshComponent;

public:
    // Call this to update the node's color based on its type
    void UpdateNodeColor();

    // Optionally, override PostEditChangeProperty for editor changes
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    virtual void OnConstruction(const FTransform& Transform) override;
};
