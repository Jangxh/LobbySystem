// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "C_LobbyGameMode.generated.h"

class IOnlineSession;
/**
 * 
 */
UCLASS()
class P_MULTIPLAYERSESSION_API AC_LobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AC_LobbyGameMode(const FObjectInitializer& ObjectInitializer);


	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	/**
	 * 在线会话接口
	 */
	TWeakPtr<IOnlineSession> SessionInterface;
};
