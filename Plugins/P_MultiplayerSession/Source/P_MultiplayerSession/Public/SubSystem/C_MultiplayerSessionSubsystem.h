// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/C_MultiplayerSesssionDelegates.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "C_MultiplayerSessionSubsystem.generated.h"


class FOnlineSessionSearch;
class IOnlineSession;
/**
 * 在线多人会话子系统
 */
UCLASS()
class P_MULTIPLAYERSESSION_API UC_MultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UC_MultiplayerSessionSubsystem();
	static UC_MultiplayerSessionSubsystem* Get();
	virtual void Deinitialize() override;

	/**
	 * 生成在线会话
	 * @param UserID 玩家的唯一ID（通过PlayerState获取）
	 * @return 成功建立生成会话请求返回true，否则返回false
	 */
	bool CreateSession(FUniqueNetIdPtr UserID);

	/**
	 * 查找在线会话
	 * @param MaxSearchResult 最大查找结果
	 * @param UserID 玩家的唯一ID（通过PlayerState获取）
	 * @return 成功建立查找会话请求返回true,否则返回false
	 */
	bool FindSessions(int MaxSearchResult, FUniqueNetIdPtr UserID);

	/**
	 * 加入在线会话
	 * @param SessionSearchResult 会话查找的返回值，表示当前需要加入的会话
	 * @param UserID 
	 * @return 
	 */
	bool JoinSession(const FOnlineSessionSearchResult& SessionSearchResult, FUniqueNetIdPtr UserID);

	/**
	 * 启动在线会话
	 * @return 
	 */
	bool StartSession(FName SessionName = NAME_None);
	
	/**
	 * 会话创建完成委托
	 */
	FOnCreateMultiplayerSessionComplete OnMultiplayerSessionCreated;

	/**
	 * 会话查找完成委托
	 */
	FOnFindMultiplayerSessionComplete OnMultiplayerSessionFound;

	/**
	 * 会话加入完成委托
	 */
	FOnJoinMultiplayerSessionComplete OnMultiplayerSessionJoined;

	/**
	 * 会话启动完成委托
	 */
	FOnStartMultiplayerSessionComplete OnMultiplayerSessionStarted;

protected:
	/**
	 * 初始化会话名称
	 */
	// void InitSessionName();
	void FilterSearchResult() const;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	/**
	 * 在线会话名称
	 */
	FName PrivateSessionName = NAME_None;
	
	/**
	 * 在线会话接口
	 */
	TWeakPtr<IOnlineSession> SessionInterface;

	/**
	 * 处理创建会话的委托
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	
	/**
	 * 绑定创建会话委托返回的Handle
	 */
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	/**
	 * 上一次的会话搜索
	 */
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	/**
	 * 处理查找会话的委托
	 */
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	/**
	 * 绑定查找会话委托返回的Handle
	 */
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	
	/**
	 * 处理加入会话的委托
	 */
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	/**
	 * 绑定加入会话委托返回的Handle
	 */
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	/**
	 * 处理启动会话的委托
	 */
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	/**
	 * 绑定启动会话委托返回的Handle
	 */
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
