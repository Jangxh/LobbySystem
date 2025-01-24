// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/C_SessionMenu.h"

#include "Components/Button.h"
#include "Settings/C_MultiplayerSessionSetting.h"
#include "SubSystem/C_MultiplayerSessionSubsystem.h"

UC_SessionMenu::UC_SessionMenu(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SetIsFocusable(true);

	const UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance == nullptr)
	{
		return;
	}

	SessionSubsystem = GameInstance->GetSubsystem<UC_MultiplayerSessionSubsystem>();
}

bool UC_SessionMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (HostBtn)
	{
		HostBtn->OnClicked.AddDynamic(this, &ThisClass::OnHostBtnClicked);
	}

	if (JoinBtn)
	{
		JoinBtn->OnClicked.AddDynamic(this, &ThisClass::OnJoinBtnClicked);
	}

	return true;
}

void UC_SessionMenu::NativeDestruct()
{
	TearDown();
	Super::NativeDestruct();
}

void UC_SessionMenu::OnHostBtnClicked()
{
	if (SessionSubsystem == nullptr)
		return;

	SessionSubsystem->OnMultiplayerSessionCreated.AddUObject(this, &ThisClass::OnSessionCreateComplete);
	// SessionSubsystem->CreateSession(TODO);
}

void UC_SessionMenu::OnJoinBtnClicked()
{
}

void UC_SessionMenu::TearDown()
{
}

void UC_SessionMenu::OnSessionCreateComplete(bool bSuccess)
{
	const UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
	if (SessionSubsystem == nullptr || Setting == nullptr)
		return;

	SessionSubsystem->OnMultiplayerSessionCreated.RemoveAll(this);
	if (!bSuccess)
		return;

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		return;
	}

	
	// World->ServerTravel()
}
