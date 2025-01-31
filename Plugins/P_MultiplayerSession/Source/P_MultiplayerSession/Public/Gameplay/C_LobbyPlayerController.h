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

	/**
	 * 离开房间
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Multiplayer|Session")
	void LeaveRoom();

	/**
	 * 销毁本地会话
	 * @param SessionName 
	 */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Multiplayer|Session")
	void DestroyLocalOnlineSession(FName SessionName=NAME_None);

	// UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnReplicated_Kicked)
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

	// void OnReplicated_Kicked();
};
