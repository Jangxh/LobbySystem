// Copyright Epic Games, Inc. All Rights Reserved.

#include "P_MultiplayerSession.h"
#include "C_MultiplayerSessionLogCategory.h"

#define LOCTEXT_NAMESPACE "FP_MultiplayerSessionModule"

DEFINE_LOG_CATEGORY(LogMultiplayerSession)
void FP_MultiplayerSessionModule::StartupModule()
{
}

void FP_MultiplayerSessionModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FP_MultiplayerSessionModule, P_MultiplayersSession)