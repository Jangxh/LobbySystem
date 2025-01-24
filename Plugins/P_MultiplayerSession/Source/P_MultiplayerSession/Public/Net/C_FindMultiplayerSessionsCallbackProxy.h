// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MultiplayerBlueprintCallProxyBase.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "C_FindMultiplayerSessionsCallbackProxy.generated.h"

class UC_MultiplayerSessionSubsystem;

USTRUCT(BlueprintType)
struct FBlueprintMultiplayerSessionResult
{
	GENERATED_BODY()

	FOnlineSessionSearchResult MultiplayerSessionResult;

	/**
	 * 会话显示的名称
	 */
	UPROPERTY(BlueprintReadOnly, Category = "MultiplayerSessionResult")
	FString SessionDisplayName;

	/**
	 * 会话实际的名称
	 */
	UPROPERTY(BlueprintReadOnly, Category = "MultiplayerSessionResult")
	FString SessionName;
	
	void ConstructSessionName();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintFindMultiplayerSessionResultDelegate, const TArray<FBlueprintMultiplayerSessionResult>&, Results);

/**
 * 查找会话的代理
 */
UCLASS(MinimalAPI)
class UC_FindMultiplayerSessionsCallbackProxy : public UC_MultiplayerBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UC_FindMultiplayerSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer);
	/**
	 * 查找成功时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindMultiplayerSessionResultDelegate OnSuccess;

	/**
	 * 查找失败时的多播委托
	 */
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindMultiplayerSessionResultDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"), Category = "Multiplayers|Session")
	static UC_FindMultiplayerSessionsCallbackProxy* FindMultiplayerSessions(APlayerController* PlayerController, int32 MaxResult);

	virtual void Activate() override;

private:
	FOnFindMultiplayerSessionCompleteDelegate FindCompleteDelegate;
	FDelegateHandle FindCompleteDelegateHandle;
	void OnFindMultiplayerSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccess) const;

	TWeakObjectPtr<APlayerController> PlayerController;
	int MaxResults;

	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
