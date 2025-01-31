// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "C_LobbyGameState.generated.h"

USTRUCT(BlueprintType)
struct FTeamStateInfo
{
	GENERATED_BODY()

	FTeamStateInfo()
		: MaxPlayers(0)
	{
	}
	UPROPERTY(BlueprintReadOnly, Category="TeamState")
	FString TeamName;

	UPROPERTY(BlueprintReadOnly, Category="TeamState")
	uint8 MaxPlayers;

	UPROPERTY(BlueprintReadOnly, Category="TeamState")
	TArray<FString> TeamMembers;
};
/**
 * 
 */
UCLASS()
class P_MULTIPLAYERSESSION_API AC_LobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AC_LobbyGameState(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 获取队伍成员
	 * @param TeamIndex 
	 * @return 
	 */
	TArray<FString> GetTeamMembers(const int& TeamIndex) const;

	/**
	 * 分配到默认队伍 
	 * @param MemberName 
	 * @return 
	 */
	bool AssignToDefaultTeam(const FString& MemberName);

	/**
	 * 从队伍中移除
	 * @param MemberName 
	 */
	void RemoveFromTeam(const FString& MemberName);
	
	/**
	 * 切换队伍
	 * @param CurTeamIndex 
	 * @param NewTeamIndex 
	 * @param MemberName 
	 * @return 
	 */
	bool SwitchTeam(const int& CurTeamIndex, const int& NewTeamIndex, const FString& MemberName);

	
private:
	UPROPERTY(Replicated)
	TArray<FTeamStateInfo> TeamStateInfos;
};
