// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Escape.h"
#include "EscapeGameMode.h"
#include "EscapeCharacter.h"

AEscapeGameMode::AEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
