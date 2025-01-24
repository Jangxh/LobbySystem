// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "C_MultiplayerBlueprintCallProxyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmptyMultiplayersDelegate);

/**
 * 多人在线蓝图调用代理基类
 */
UCLASS(MinimalAPI)
class UC_MultiplayerBlueprintCallProxyBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UC_MultiplayerBlueprintCallProxyBase(const FObjectInitializer& InObjectInitializer);

	static FUniqueNetIdPtr QueryIDFromPlayerController(const APlayerController* InPlayerController);
};
