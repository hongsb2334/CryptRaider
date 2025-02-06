// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}





// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TArray<AActor*> Actors;                                             //AActor 타입의 TArray(배열 컨테이너) 생성 후 GetOverlappingActors 호출해서 오버래핑된 컴포넌트를 리턴
    GetOverlappingActors(Actors);

    if(Actors.Num() > 0)                                                //Actors 원소가 0보다 클때(오버래핑된 경우) 오버래핑된 액터 이름 출력
    {
        FString ActorName = Actors[0]->GetActorNameOrLabel();
        UE_LOG(LogTemp, Display, TEXT("Overlapping: %s"), *ActorName);
    }
}