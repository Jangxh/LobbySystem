// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "C_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class P_MULTIPLAYERSESSION_API AC_LobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AC_LobbyGameMode(const FObjectInitializer& ObjectInitializer);
};
