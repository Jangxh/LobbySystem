// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_SessionMenu.generated.h"

class UC_MultiplayerSessionSubsystem;
class UButton;
/**
 * 会话主菜单页面
 */
UCLASS(Abstract)
class P_MULTIPLAYERSESSION_API UC_SessionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UC_SessionMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	/**
	 * 创建服务器按钮点击
	 */
	UFUNCTION()
	void OnHostBtnClicked();

	/**
	 * 加入服务器按钮点击
	 */
	UFUNCTION()
	void OnJoinBtnClicked();

	void TearDown();

	UFUNCTION()
	void OnSessionCreateComplete(bool bSuccess);
	
private:
	/**
	 * 创建服务器按钮
	 */
	UPROPERTY(meta=(BindWidget))
	UButton* HostBtn;

	/**
	 * 加入会话按钮
	 */
	UPROPERTY(meta=(BindWidget))
	UButton* JoinBtn;

	/**
	 * 会话子系统
	 */
	UPROPERTY()
	UC_MultiplayerSessionSubsystem* SessionSubsystem;
};
