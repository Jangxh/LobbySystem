// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class P_MULTIPLAYERSESSION_API AC_LobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
