// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "C_MultiplayerSessionSetting.generated.h"

class AC_LobbyGameMode;
class UC_SessionMenu;
/**
 * 在线多人会话配置
 */
UCLASS(Config=_MultiplayerSession, ProjectUserConfig)
class P_MULTIPLAYERSESSION_API UC_MultiplayerSessionSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UC_MultiplayerSessionSetting(const FObjectInitializer& ObjectInitializer);
	static UC_MultiplayerSessionSetting* Get();

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
	
#endif
	
	/**
	 * 主要区域名称（各个项目需要保持唯一）
	 */
	UPROPERTY(Config, EditAnywhere, Category=OnlineSession, DisplayName="主区域名")
	FString MainAreaName = TEXT("MainArea");
	
	/**
	 * 是否共享主区域（共享主区域表示项目通用一个会话，都能加入，否则各个子区域建立独立会话）
	 */
	UPROPERTY(Config, EditAnywhere, Category=OnlineSession, DisplayName="跨子区域")
	bool bShareMainArea = false;
	
	/**
	 * 子区域名称，用于对相同项目划定小区域组建会话时使用
	 */
	UPROPERTY(Config, EditAnywhere, Category=OnlineSession, DisplayName="子区域名", meta=(EditCondition="!bCrossMinorArea"))
	FString MinorAreaName = TEXT("MinorArea");

	/**
	 * 席位名称
	 */
	UPROPERTY(Config, EditAnywhere, Category=OnlineSession, DisplayName="席位名称")
	FString SeatName = TEXT("SeatName");
	
	/**
	 * 最大玩家数
	 */
	UPROPERTY(Config, EditAnywhere, Category=OnlineSession, DisplayName="最大玩家数")
	uint8 MaxPlayers = 6;
	
	/**
	 * 大厅地图
	 */
	UPROPERTY(Config, EditAnywhere, Category="Map & Mode", DisplayName="大厅地图")
	TSoftObjectPtr<UWorld> LobbyMap;
	
	/**
	 * 创建会话或者加入会话后进入中转地图
	 */
	UPROPERTY(Config, EditAnywhere, Category="Map & Mode", DisplayName="中转地图")
	TSoftObjectPtr<UWorld> TransitionMap;

	UPROPERTY(Config, EditAnywhere, Category="Map & Mode", DisplayName="中转GameMode")
	TSubclassOf<AC_LobbyGameMode> TransitionGameMode;
};
