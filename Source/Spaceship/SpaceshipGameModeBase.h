// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceshipGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHIP_API ASpaceshipGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly)
	int Score;

	ASpaceshipGameModeBase();
	
public:

	void CountScore();
	
};
