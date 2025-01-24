// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MultiplayerBlueprintCallProxyBase.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "C_StartMultiplayerSessionCallbackProxy.generated.h"

class UC_MultiplayerSessionSubsystem;
/**
 * 启动会话的代理
 */
UCLASS(MinimalAPI)
class UC_StartMultiplayerSessionCallbackProxy : public UC_MultiplayerBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UC_StartMultiplayerSessionCallbackProxy(const FObjectInitializer& ObjectInitializer);

	/**
	 * 启动成功时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnSuccess;

	/**
	 * 启动失败时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="Multiplayer|Session")
	UC_StartMultiplayerSessionCallbackProxy* StartMultiplayerSession(FName SessionName=NAME_None);

	virtual void Activate() override;

private:
	FOnStartMultiplayerSessionCompleteDelegate StartCompleteDelegate;
	FDelegateHandle StartCompleteDelegateHandle;
	void OnStartMultiplayerSessionComplete(bool Success) const;

	FName ToStartSessionName;
	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
