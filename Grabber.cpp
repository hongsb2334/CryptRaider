// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance; // Start + (GetForwardVector() * MaxGrabDistance)
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	float Damage;					//1. 값을 받지 못하고 바로 함수로 전달된 변수가 보이면 아웃 매개변수가 아닐지 의심
	if (HasDamage(Damage))
	{
		PrintDamage(Damage);
	}
	
}

void UGrabber::PrintDamage(const float& Damage)
{
	//Damage = 2;
	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
}

bool UGrabber:: HasDamage(float& OutDamage) 		//2. 참조(&)가 매개변수인데 const가 앞에 없다면 아웃매개변수일 수 있다
													//3. 변수나 매개변수 이름이 Out으로 시작하면 아웃매개변수 의심
{
	OutDamage = 5;
	return true;
}
