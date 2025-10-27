// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot/DFRobotArm.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ControlRigComponent.h"
#include "AI/RobotArm/DFRobotArmAIController.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ControlRig.h"
#include "RigVMHost.h"
#include "GAS/GA/DFGA_RobotArmMasterAbility.h"
#include "GAS/GA/DFGA_TargetAttach.h"

ADFRobotArm::ADFRobotArm()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADFRobotArmAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	EffectorScene = CreateDefaultSubobject<USceneComponent>(TEXT("EffectorScene"));
	EffectorScene->SetupAttachment(Root);

	PreviewTire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire"));
	PreviewTire->SetupAttachment(EffectorScene);
	PreviewTire->SetVisibility(false);

	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	Skeletal->SetupAttachment(Root);

	ControlRigComponent = CreateDefaultSubobject<UControlRigComponent>(TEXT("ControlRig"));
	ControlRigComponent->SetupAttachment(Root);

	SearchComp = CreateDefaultSubobject<USphereComponent>(TEXT("SearchComp"));
	SearchComp->SetupAttachment(Root);

	EndEffectorName = "Robot1_End_ctrl";
	RotatorName = "Robot1_A_ctrl";

	OwnerDuration = 0.0f;

	DFASC = CreateDefaultSubobject<UDFAbilitySystemComponent>(TEXT("DFASC"));
}

void ADFRobotArm::BeginPlay()
{
	Super::BeginPlay();
	
	// 컨트롤릭 스켈레탈에 결합
	ControlRigComponent->AddMappedCompleteSkeletalMesh(Skeletal);

	FTransform EffectorWorldTransform = ControlRigComponent->GetControlTransform(EndEffectorName, EControlRigComponentSpace::WorldSpace);

	// 이펙터 씬의 트랜스폼으로 설정
	EffectorScene->SetWorldTransform(EffectorWorldTransform);

	if (DFASC)
	{
		MasterSpecHandle = DFASC->GiveAbility(FGameplayAbilitySpec(RobotArmMasterAbility, 1, 0, this));
		FindSpecHandle = DFASC->GiveAbility(FGameplayAbilitySpec(FindAbility, 1, 1, this));
		TargetAttachSpecHandle = DFASC->GiveAbility(FGameplayAbilitySpec(TargetAttachAbility, 1, 2, this));
		ReturnToHomeSpecHandle = DFASC->GiveAbility(FGameplayAbilitySpec(ReturnToHomeAbility, 1, 3, this));
		CombineSpecHandle = DFASC->GiveAbility(FGameplayAbilitySpec(CombineTireAbility, 1, 4, this));
	}

	if (ControlRigComponent)
	{
		EndHomeTransform = ControlRigComponent->GetControlTransform(EndEffectorName, EControlRigComponentSpace::WorldSpace);
		//UE_LOG(LogTemp, Warning, TEXT("RobotArm : End의 복귀위치 X : %f, Y : %f, Z : %f")
		//	, EndHomeTransform.GetLocation().X, EndHomeTransform.GetLocation().Y, EndHomeTransform.GetLocation().Z);
		AHomeRotate = ControlRigComponent->GetControlRotator(RotatorName, EControlRigComponentSpace::WorldSpace);
		//UE_LOG(LogTemp, Warning, TEXT("RobotArm : A의 복귀위치 X : %f, Y : %f, Z : %f")
		//	, AHomeRotate.Pitch, AHomeRotate.Yaw, AHomeRotate.Roll);
	}
}

void ADFRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ControlRigComponent)
	{
		FTransform EffectorWorldTransform = ControlRigComponent->GetControlTransform(EndEffectorName, EControlRigComponentSpace::WorldSpace);

		// 이펙터 씬의 트랜스폼으로 설정
		EffectorScene->SetWorldTransform(EffectorWorldTransform);
	}
}

void ADFRobotArm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UDFAbilitySystemComponent* ADFRobotArm::GetDFAbilitySystemComponent()
{
	return DFASC;
}

void ADFRobotArm::StartRobotArmAbility()
{
	if (DFASC && TargetAttachSpecHandle.IsValid())
	{
		// 스펙 찾고
		FGameplayAbilitySpec* Spec = DFASC->FindAbilitySpecFromHandle(TargetAttachSpecHandle);
		if (Spec && Spec->GetPrimaryInstance())
		{
			if (UDFGA_TargetAttach* AttachGA = Cast<UDFGA_TargetAttach>(Spec->GetPrimaryInstance()))
			{
				AttachGA->OnTargetAttachSucceed.AddDynamic(this, &ADFRobotArm::HandleAttachFinished);
				if (AttachGA->OnTargetAttachSucceed.IsAlreadyBound(this, &ADFRobotArm::HandleAttachFinished))
				{
					UE_LOG(LogTemp, Warning, TEXT("로봇암 : 타겟 델리게이트 바인딩 성공"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("로봇암 : 타겟 델리게이트 바인딩 실패"));
				}
			}
		}
	}
	if (DFASC && MasterSpecHandle.IsValid())
	{
		bool bActivated = DFASC->TryActivateAbility(MasterSpecHandle);

		// 스펙을 찾고
		FGameplayAbilitySpec* Spec = DFASC->FindAbilitySpecFromHandle(MasterSpecHandle);
		// 어빌리티스펙이 유효하고 활성화된 인스턴스를 가지고 있는지?
		if (Spec && Spec->GetPrimaryInstance())
		{
			if (UDFGA_RobotArmMasterAbility* Master = Cast<UDFGA_RobotArmMasterAbility>(Spec->GetPrimaryInstance()))
			{
				bool bIsBound = Master->OnFinishTireAssembly.IsAlreadyBound(this, &ADFRobotArm::HandleAbilityFinished);

				if (!bIsBound)
				{
					Master->OnFinishTireAssembly.AddDynamic(this, &ADFRobotArm::HandleAbilityFinished);
					UE_LOG(LogTemp, Warning, TEXT("로봇암 : 델리게이트 바인딩 시도"));

					if (Master->OnFinishTireAssembly.IsAlreadyBound(this, &ADFRobotArm::HandleAbilityFinished))
					{
						UE_LOG(LogTemp, Warning, TEXT("로봇암 : 델리게이트 바인딩 성공"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("로봇암 : 델리게이트 바인딩 실패"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("로봇암 : 델리게이트가 이미 바인딩 되어있습니다"));
				}
			}
		}

		if (!bActivated)
		{
			UE_LOG(LogTemp, Warning, TEXT("로봇암 : 마스터 어빌리티 활성화 실패"));
		}
	}
}

void ADFRobotArm::NotifySubAbilityFinished()
{
	if (ActiveMaster)
	{
		ActiveMaster->OnAbilityEnded();
	}
}

void ADFRobotArm::HandleAbilityFinished(FGameplayTag OwnerTag, bool bFinished)
{
	UE_LOG(LogTemp, Warning, TEXT("로봇암 : 마스터 어빌리티에게 끝났단 신호받음. 다음 구독자에게 보냄"));
	OnRobotArmFinished.Broadcast(OwnerTag, bFinished);
}

void ADFRobotArm::HandleAttachFinished()
{
	// 타이어 준비해놓으라고 신호를 보냄
	OnReadyTireSpawn.Broadcast(TypeTag);
}

