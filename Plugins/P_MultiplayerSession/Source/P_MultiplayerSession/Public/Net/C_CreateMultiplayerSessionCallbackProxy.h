// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MultiplayerBlueprintCallProxyBase.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "C_CreateMultiplayerSessionCallbackProxy.generated.h"

class UC_MultiplayerSessionSubsystem;
/**
 * 创建会话的代理
 */
UCLASS(MinimalAPI)
class UC_CreateMultiplayerSessionCallbackProxy : public UC_MultiplayerBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UC_CreateMultiplayerSessionCallbackProxy(const FObjectInitializer& ObjectInitializer);
	/**
	 * 创建成功时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnSuccess;

	/**
	 * 创建失败时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"), Category = "Multiplayer|Session")
	static UC_CreateMultiplayerSessionCallbackProxy* CreateMultiplayerSession(APlayerController* PlayerController);

	virtual void Activate() override;

private:
	FOnCreateMultiplayerSessionCompleteDelegate CreateCompleteDelegate;
	FDelegateHandle CreateCompleteDelegateHandle;
	void OnCreateMultiplayerSessionComplete(bool bSuccess) const;

	TWeakObjectPtr<APlayerController> PlayerController;

	/**
	 * 会话子系统
	 */
	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
