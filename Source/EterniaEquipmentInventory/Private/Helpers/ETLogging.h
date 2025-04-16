// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEqInv, Log, All);

#define EEIS_ULOG_ERROR(fmt, ...) \
UE_LOG(LogEqInv, Error, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOwner()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGO_ERROR(fmt, ...) \
UE_LOG(LogEqInv, Error, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOuter()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGS_ERROR(fmt, ...) \
UE_LOG(LogEqInv, Error, \
TEXT("[%s] %s:%i [%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*StaticClass()->GetName(), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOG_WARNING(fmt, ...) \
UE_LOG(LogEqInv, Warning, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOwner()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGO_WARNING(fmt, ...) \
UE_LOG(LogEqInv, Warning, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOuter()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGS_WARNING(fmt, ...) \
UE_LOG(LogEqInv, Warning, \
TEXT("[%s] %s:%i [%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*StaticClass()->GetName(), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOG_DISPlAY(fmt, ...) \
UE_LOG(LogEqInv, Display, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOwner()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGO_DISPlAY(fmt, ...) \
UE_LOG(LogEqInv, Display, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOuter()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGS_DISPlAY(fmt, ...) \
UE_LOG(LogEqInv, Display, \
TEXT("[%s] %s:%i [%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*StaticClass()->GetName(), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOG(fmt, ...) \
UE_LOG(LogEqInv, Log, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOwner()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGO(fmt, ...) \
UE_LOG(LogEqInv, Log, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOuter()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGS(fmt, ...) \
UE_LOG(LogEqInv, Log, \
TEXT("[%s] %s:%i [%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*StaticClass()->GetName(), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOG_VERBOSE(fmt, ...) \
UE_LOG(LogEqInv, Verbose, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOwner()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGO_VERBOSE(fmt, ...) \
UE_LOG(LogEqInv, Verbose, \
TEXT("[%s] %s:%i [%s]->[%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*GetNameSafe(GetOuter()), *GetNameSafe(this), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)

#define EEIS_ULOGS_VERBOSE(fmt, ...) \
UE_LOG(LogEqInv, Verbose, \
TEXT("[%s] %s:%i [%s] %s"), \
TEXT("Context"), \
*FString(__FUNCTION__), __LINE__, \
*StaticClass()->GetName(), \
*FString::Printf(fmt, ##__VA_ARGS__) \
)