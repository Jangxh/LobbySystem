// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MultiplayerBlueprintCallProxyBase.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "C_JoinMultiplayerSessionCallbackProxy.generated.h"

class UC_MultiplayerSessionSubsystem;
struct FBlueprintMultiplayerSessionResult;

/**
 * 加入会话的代理
 */
UCLASS(MinimalAPI)
class UC_JoinMultiplayerSessionCallbackProxy : public UC_MultiplayerBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UC_JoinMultiplayerSessionCallbackProxy(const FObjectInitializer& InObjectInitializer);

	/**
	 * 加入成功时的委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnSuccess;

	/**
	 * 加入失败时的委托
	 */
	UPROPERTY(BlueprintAssignable)
	FEmptyMultiplayersDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"), Category = "Multiplayers|Session")
	static UC_JoinMultiplayerSessionCallbackProxy* JoinMultiplayerSession(APlayerController* PlayerController, const FBlueprintMultiplayerSessionResult& SearchResult);

	virtual void Activate() override;

private:
	FOnJoinMultiplayerSessionCompleteDelegate JoinCompleteDelegate;
	FDelegateHandle JoinCompleteDelegateHandle;
	void OnJoinMultiplayerSessionComplete(EOnJoinSessionCompleteResult::Type Result) const;

	TWeakObjectPtr<APlayerController> PlayerController;
	FOnlineSessionSearchResult OnlineSearchResult;

	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
