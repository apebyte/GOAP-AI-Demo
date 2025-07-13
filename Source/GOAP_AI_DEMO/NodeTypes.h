/**
 * @file NodeTypes.h
 * @brief Defines enums for node types and connection types used in the navigation and AI systems.
 */

#pragma once

#include "NodeTypes.generated.h"

/**
 * @brief Enumerates the types of nodes that can exist in the navigation graph.
 *
 * These types describe the purpose or role of a node in the world, such as a walkable point,
 * a cover location, a flanking position, a turret spot, or a danger zone.
 */
UENUM(BlueprintType)
enum class ENodeType : uint8
{
    Walk    UMETA(DisplayName = "Walk"),      /**< Standard walkable node. */
    Cover   UMETA(DisplayName = "Cover"),     /**< Node representing a cover position. */
    Flank   UMETA(DisplayName = "Flank"),     /**< Node used for flanking maneuvers. */
    Turret  UMETA(DisplayName = "Turret"),    /**< Node for turret or stationary AI. */
    Danger  UMETA(DisplayName = "Danger")     /**< Node marking a dangerous area. */
};

/**
 * @brief Enumerates the types of connections between nodes.
 *
 * These types describe how an agent can traverse from one node to another, such as walking, jumping, or climbing.
 * Used to inform pathfinding and AI decision-making about available traversal methods.
 */
UENUM(BlueprintType)
enum class ENodeConnectionType : uint8
{
    Walking     UMETA(DisplayName = "Walking"),    /**< Standard walking connection. */
    Jumping     UMETA(DisplayName = "Jumping"),    /**< Connection that requires jumping. */
    Climbing    UMETA(DisplayName = "Climbing"),   /**< Connection that requires climbing. */
    // Add more as needed
};
