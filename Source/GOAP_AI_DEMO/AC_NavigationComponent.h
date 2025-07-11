#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Node.h"
#include "AC_NavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOAP_AI_DEMO_API UAC_NavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_NavigationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The starting node for navigation (where the AI character begins)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	ANode* StartNode = nullptr;

	// The end node for navigation (closest to the target)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	ANode* EndNode = nullptr;

	// The finalized path from start to end node
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	TArray<ANode*> FinalPath;

	// Finds the nearest node to the owner and the target
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void FindStartAndEndNodes(const FVector& TargetLocation);

	// Finds a path from StartNode to EndNode using A* search
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool FindPathAStar(TArray<ANode*>& OutPath);

	// Finalizes the path from the closed set and CameFrom map
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool FinalizePath(const TSet<ANode*>& ClosedSet, const TMap<ANode*, ANode*>& CameFrom);
};
