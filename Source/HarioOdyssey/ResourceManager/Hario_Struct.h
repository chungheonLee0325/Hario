#pragma once

#include <Engine/DataTable.h>
#include "Hario_Enum.h"
#include "Hario_Struct.generated.h"

USTRUCT(BlueprintType)
struct FAreaObjectData:public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int AreaObjectID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	EAreaObjectType AreaObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float HPMax;
};