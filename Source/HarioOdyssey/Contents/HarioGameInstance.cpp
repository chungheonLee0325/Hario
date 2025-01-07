// Fill out your copyright notice in the Description page of Project Settings.


#include "HarioGameInstance.h"

#include "GameplayTagsManager.h"
#include "K2Node_GetDataTableRow.h"

void UHarioGameInstance::Init()
{
	Super::Init();

	UDataTable* AreaObjectTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/_BluePrints/Common/Data/db_AreaObject.db_AreaObject'"));
	if (nullptr != AreaObjectTable)
	{
		TArray<FName> RowNames = AreaObjectTable->GetRowNames();

		for (const FName& RowName : RowNames)
		{
			FAreaObjectData* Row = AreaObjectTable->FindRow<FAreaObjectData>(RowName, TEXT("") );
			if (nullptr != Row)
			{
				dt_AreaObject.Add(Row->AreaObjectID, *Row);
			}
		}
	}
}

FAreaObjectData* UHarioGameInstance::GetDataAreaObject(const int AreaObjectID)
{
	if (FAreaObjectData* data = dt_AreaObject.Find(AreaObjectID))
	{
			return data;
	}

	return nullptr;
}
