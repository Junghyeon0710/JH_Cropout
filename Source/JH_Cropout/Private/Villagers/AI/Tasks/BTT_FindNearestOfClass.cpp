// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_FindNearestOfClass.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_FindNearestOfClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

    // 이미 타겟이 설정되어 있고 유효하면 성공 반환
    if (IsValid(BBC->GetValueAsObject(Target.SelectedKeyName)))
    {
        return EBTNodeResult::Succeeded;
    }

    // 태그 필터 설정
    FName ResultTag = bUseBlackboardTag ? BBC->GetValueAsName(BlackboardTag.SelectedKeyName) : TagFilter;

    // 찾을 클래스 설정
    
    UClass* ResultObject = bUseBlackboardClass 
        ? BBC->GetValueAsClass(TargetClass.SelectedKeyName)
        : ManualClass.Get();
    
    if (!ResultObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("ResultObject가 올바르지 않습니다!"));
        return EBTNodeResult::Failed;
    }

    // 해당 클래스의 액터들을 찾음
    TSubclassOf<AActor> ResultActor = ResultObject;
 
    if (ResultTag.IsNone())
    {
        UGameplayStatics::GetAllActorsOfClass(this, ResultActor, PossibleActors);
    }
    else
    {
        UGameplayStatics::GetAllActorsOfClassWithTag(this, ResultActor, ResultTag, PossibleActors);
    }

    // 사용 가능한 액터가 없으면 실패 반환
    if (PossibleActors.IsEmpty())
    {
        return EBTNodeResult::Failed;
    }

    // 가장 가까운 액터 찾기
    AActor* OwnerActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this, NearestTo);
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("NearestTo가 설정되지 않았습니다!"));
        return EBTNodeResult::Failed;
    }

    FVector OwnerLocation = OwnerActor->GetActorLocation();
    for(auto PossibleActor : PossibleActors)
    {
        if(bPathFound)
        {
            continue;
        }
        float Distance;
        AActor* NearestActor = UGameplayStatics::FindNearestActor(OwnerLocation, PossibleActors, Distance);
        UNavigationPath* NaviPath = UNavigationSystemV1::FindPathToActorSynchronously(this, OwnerLocation, NearestActor, 100.f);

        //If Path is only partial, remove actor from array
        if (NaviPath && NaviPath->IsPartial()) 
        {
            PossibleActors.Remove(NearestActor);
            continue;
        }
        //If Path is available, set target and stop looking
        bPathFound = true;
        NewTarget = NearestActor;
    }

    if (bPathFound && NewTarget)
    {
        BBC->SetValueAsObject(Target.SelectedKeyName, NewTarget);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
