// #pragma once
//
// #include <Engine/DataTable.h>
// #include "Hario_Enum.h"
// #include "Hario_Struct.generated.h"
//
// USTRUCT(BlueprintType)
// struct FAreaObjectData:public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	int AreaObjectID = 0;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	FName Name = "";
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	EAreaObjectType AreaObjectType = EAreaObjectType::None;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	EEnemyType EnemyType = EEnemyType::None;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	float HPMax = 1.0f;
// };