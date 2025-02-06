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
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
	
	
}

void UGrabber::Release()											
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();		//GetPhysicsHandle 호출하여 UPhysicsHandleComponent 가져오기
	
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())					//GetGrabbedComponent가 nullptr이 아닐시(잡은 물체가 있을 때)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();		//WakeAllRigidBodies()로 물체 깨우
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();								//ReleaseComponent()로 물체를 놓음
										
	}
										
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();		//GetPhysicsHandle()로 UPhysicsHandleComponent 가져오기
	if(PhysicsHandle == nullptr)
	{
		return;															//해당 컴포넌트가 없을 시 그냥 반환
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if(HasHit)															//충돌한 경우(물체가 감지)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();	//충돌한 컴포넌트 가져오기
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();								//충돌한 컴포넌트의 물체 깨우기
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);			
		PhysicsHandle->GrabComponentAtLocationWithRotation(				//물체 잡는 함수 호출
			HitComponent,												//잡을 물체
			NAME_None,													//이름 지정 안함
			HitResult.ImpactPoint,										//물체 잡을 위치
			GetComponentRotation()										//현재 UGrabber의 회전값
		);
	}
	
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();	//UGrabber가 속한 액터에서 UPhysicsHandleComponent를 찾아 반환환
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	
	return Result;
}


bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const		//
{
	FVector Start = GetComponentLocation();								//시작 위치: 현재 컴포넌트의 위치
	FVector End = Start + GetForwardVector() * MaxGrabDistance; 		// Start + (GetForwardVector() * MaxGrabDistance): Start에서 GetForwardVector()방향으로 MaxGrabDistance만큼 떨어진 위치
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);					//빨간색으로 트레이스 선을 보여줌
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);	//파란색으로 트레이스 구체를 보여줌

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);	//Grabradius 크기의 구체 생성
	

	return GetWorld()->SweepSingleByChannel(						//아웃 매개변수인 OutHitResult에 Start부터 End까지 충돌 정보를 저장하고 SweepSingleByChannel에서 호출된 bool값 리턴
	OutHitResult, 
	Start, End, 
	FQuat::Identity, 
	ECC_GameTraceChannel2,
	Sphere
	);
}