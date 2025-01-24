// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "C_MultiplayerSessionUtil.generated.h"

/**
 * 在线多人会话工具
 */
UCLASS()
class P_MULTIPLAYERSESSION_API UC_MultiplayerSessionUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 启动在线匹配系统
	 * @param WorldContextObject 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Session Util", meta=(WorldContext="WorldContextObject"))
	static bool StartOnlineMatchSystem(const UObject* WorldContextObject);
};
