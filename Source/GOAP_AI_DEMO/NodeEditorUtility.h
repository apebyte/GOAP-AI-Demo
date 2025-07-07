#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NodeEditorUtility.generated.h"

UCLASS()
class GOAP_AI_DEMO_API UNodeEditorUtility : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void Link();

    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void Unlink();

    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void AutoLink(float Distance = 250.0f);

    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void AutoUnlink(float Distance = 250.0f);

    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void UnlinkAll();
};
