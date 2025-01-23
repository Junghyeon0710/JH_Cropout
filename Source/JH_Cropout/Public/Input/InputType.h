#pragma once

#include "InputType.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	
	Unknown UMETA(DisplayName = "Unknown"),
	KeyMouse UMETA(DisplayName = "KeyMouse"),
	Gamepad UMETA(DisplayName = "Gamepad"),
	Touch UMETA(DisplayName = "Touch"),
	
};