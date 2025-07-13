// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NodeEditorUtility.generated.h"

/**
 * @brief Utility class for editing and managing node links in the Unreal Editor.
 *
 * Provides static functions callable from the editor to link, unlink, auto-link, and auto-unlink nodes,
 * as well as clear all links for selected nodes or all nodes in the world.
 * These utilities are intended to assist with navigation graph editing in the editor.
 */
UCLASS()
class GOAP_AI_DEMO_API UNodeEditorUtility : public UObject
{
    GENERATED_BODY()

public:
    /**
     * @brief Links all selected nodes to each other (bidirectional).
     *
     * For each selected node, creates links to all other selected nodes if not already linked.
     * Updates link splines for visualization.
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void Link();

    /**
     * @brief Unlinks selected nodes from each other (bidirectional).
     *
     * For each pair of selected nodes, removes the link between them if it exists.
     * Updates link splines for visualization.
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void Unlink();

    /**
     * @brief Automatically links selected nodes that are within a specified distance of each other.
     *
     * Clears existing links for selected nodes, then links nodes that are within the given distance.
     * Updates link splines for visualization.
     * @param Distance The maximum distance between nodes to be linked (default: 250.0f).
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void AutoLink(float Distance = 250.0f);

    /**
     * @brief Automatically unlinks selected nodes that are within a specified distance of each other.
     *
     * For each selected node, removes links to other selected nodes that are within the given distance.
     * Updates link splines for visualization.
     * @param Distance The maximum distance between nodes to be unlinked (default: 250.0f).
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void AutoUnlink(float Distance = 250.0f);

    /**
     * @brief Unlinks all nodes in the world from each other.
     *
     * Clears the LinkedNodes array for every node in the world and updates link splines.
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void UnlinkAll();

    /**
     * @brief Clears all links for each selected node.
     *
     * For every selected node, removes all links (LinkedNodes) and updates link splines.
     */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Node Editor")
    static void ClearSelectedNodeLinks();
};
