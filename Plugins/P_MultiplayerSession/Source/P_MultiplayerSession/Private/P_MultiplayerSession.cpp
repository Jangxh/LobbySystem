// Copyright Epic Games, Inc. All Rights Reserved.

#include "P_MultiplayerSession.h"
#include "C_MultiplayerSessionLogCategory.h"

#define LOCTEXT_NAMESPACE "FP_MultiplayerSessionModule"

DEFINE_LOG_CATEGORY(LogMultiplayerSession)
void FP_MultiplayerSessionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FP_MultiplayerSessionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FP_MultiplayerSessionModule, P_MultiplayersSession)