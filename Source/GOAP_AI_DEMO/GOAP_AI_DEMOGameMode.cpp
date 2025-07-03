// Copyright Epic Games, Inc. All Rights Reserved.

#include "GOAP_AI_DEMOGameMode.h"
#include "GOAP_AI_DEMOCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGOAP_AI_DEMOGameMode::AGOAP_AI_DEMOGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
