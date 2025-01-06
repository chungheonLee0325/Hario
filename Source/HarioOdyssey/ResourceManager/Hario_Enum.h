#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

// 언리얼 리플렉션 시스템과 통합하기 위해 UENUM() 매크로를 사용
UENUM(BlueprintType)
enum class EAreaObjectType : uint8 {
	None        UMETA(DisplayName = "None"),       // 블루프린트에서 표시될 이름
	Player     UMETA(DisplayName = "Player"),
	Enemy     UMETA(DisplayName = "Enemy"),
};

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	None        UMETA(DisplayName = "None"),       // 블루프린트에서 표시될 이름
	Monster     UMETA(DisplayName = "Monster"),
	NamedMonster     UMETA(DisplayName = "NamedMonster"),
	Boss     UMETA(DisplayName = "Boss"),
};