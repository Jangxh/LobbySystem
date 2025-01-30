// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MultiplayerBlueprintCallProxyBase.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "C_DestroyMultiplayerSessionCallbackProxy.generated.h"

class UC_MultiplayerSessionSubsystem;
/**
 * 销毁会话的代理
 */
UCLASS()
class P_MULTIPLAYERSESSION_API UC_DestroyMultiplayerSessionCallbackProxy : public UC_MultiplayerBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UC_DestroyMultiplayerSessionCallbackProxy(const FObjectInitializer& ObjectInitializer);

	/**
	 * 销毁成功时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnSuccess;

	/**
	 * 销毁失败时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Multiplayer|Session")
	UC_DestroyMultiplayerSessionCallbackProxy* DestroyMultiplayerSession(FName SessionName = NAME_None);

	virtual void Activate() override;

private:
	FOnDestroyMultiplayerSessionCompleteDelegate DestroyCompleteDelegate;
	FDelegateHandle DestroyCompleteDelegateHandle;

	void OnDestroyMultiplayerSessionComplete(bool bWasSuccessful);

	FName ToDestroySessionName;
	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
