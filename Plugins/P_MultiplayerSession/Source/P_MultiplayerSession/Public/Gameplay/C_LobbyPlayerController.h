// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class P_MULTIPLAYERSESSION_API AC_LobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 离开房间
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Multiplayer|Session")
	void LeaveRoom();

	/**
	 * 切换队伍
	 *
	 * 根据当前所在队伍的顺序依次切换
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Multiplayer|Session")
	void SwitchTeam();

	/**
	 * 获取当前所在队伍名称
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "Multiplayer|Session")
	FString GetTeamName() const;

	/**
	 * 获取当前队伍的成员
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "Multiplayer|Session")
	TArray<FString> GetTeamMembers() const;

	/**
	 * 获取席位名称
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "Multiplayer|Session")
	FString GetSeatName() const;
	
	UFUNCTION(Server, Reliable, Category = "Multiplayer|Session")
	void SetSeatName(const FString& InSeatName);
	
	/**
	 * 销毁本地会话
	 * @param SessionName 
	 */
	UFUNCTION(Client, Reliable, Category = "Multiplayer|Session")
	void DestroyLocalOnlineSession(FName SessionName=NAME_None);
	
	
	bool bIsKicked = false;
private:
	/**
	 * 服务端离开房间
	 */
	void Server_LeaveRoom();
	
	/**
	 * 客户端离开房间
	 */
	void Client_LeaveRoom();

	/**
	 * 转移到大厅地图
	 */
	void TravelToLobbyMap();

	UPROPERTY(Replicated)
	int TeamIndex = INDEX_NONE;

	UPROPERTY(Replicated)
	FString SeatName;
};
