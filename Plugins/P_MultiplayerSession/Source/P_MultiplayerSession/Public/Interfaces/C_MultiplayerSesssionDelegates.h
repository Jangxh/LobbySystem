// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"

/**
 * 当多人会话创建请求完成时触发委托
 *
 * @param bSuccess 当异步完成时没有错误返回true，否则返回false
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateMultiplayerSessionComplete, bool bSuccess);
typedef FOnCreateMultiplayerSessionComplete::FDelegate FOnCreateMultiplayerSessionCompleteDelegate;

/**
 * 当多人会话查找请求完成时触发委托
 *
 * @param  SessionResults 查找到的会话结果
 * @param bSuccess 当异步完成时没有错误返回true，否则返回false
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindMultiplayerSessionComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccess);
typedef FOnFindMultiplayerSessionComplete::FDelegate FOnFindMultiplayerSessionCompleteDelegate;

/**
 * 当多人会话加入请求完成时触发委托
 *
 * @param bSuccess 当异步完成时没有错误返回true，否则返回false
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinMultiplayerSessionComplete, EOnJoinSessionCompleteResult::Type Result);
typedef FOnJoinMultiplayerSessionComplete::FDelegate FOnJoinMultiplayerSessionCompleteDelegate;

/**
 * 当多人会话启动请求完成时触发委托
 *
 * @param bSuccess 当异步完成时没有错误返回true，否则返回false
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartMultiplayerSessionComplete, bool bSuccess);
typedef FOnStartMultiplayerSessionComplete::FDelegate FOnStartMultiplayerSessionCompleteDelegate;



