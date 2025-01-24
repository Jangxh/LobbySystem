// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_MultiplayerBlueprintCallProxyBase.h"

#include "GameFramework/PlayerState.h"

UC_MultiplayerBlueprintCallProxyBase::UC_MultiplayerBlueprintCallProxyBase(
	const FObjectInitializer& InObjectInitializer): Super(InObjectInitializer)
{
}

FUniqueNetIdPtr UC_MultiplayerBlueprintCallProxyBase::QueryIDFromPlayerController(const APlayerController* InPlayerController)
{
	FUniqueNetIdPtr UniqueId;
	UniqueId.Reset();
	
	TObjectPtr<APlayerState> PlayerState = nullptr;
	if (InPlayerController != nullptr)
	{
		PlayerState = InPlayerController->PlayerState;
	}

	if (PlayerState != nullptr)
	{
		UniqueId = PlayerState->GetUniqueId().GetUniqueNetId();
	}

	return UniqueId;
}
