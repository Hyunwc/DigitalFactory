// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot/DFRobotArm.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ControlRigComponent.h"
#include "AI/RobotArm/DFRobotArmAIController.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ControlRig.h"
#include "RigVMHost.h"
#include "GAS/GA/DFGA_RobotArmMasterAbility.h"

ADFRobotArm::ADFRobotArm()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ADFRobotArmAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	Skeletal->SetupAttachment(Root);

	ControlRigComponent = CreateDefaultSubobject<UControlRigComponent>(TEXT("ControlRig"));
	ControlRigComponent->SetupAttachment(Root);

	SearchComp = CreateDefaultSubobject<USphereComponent>(TEXT("SearchComp"));
	SearchComp->SetupAttachment(Root);

	EndEffectorName = "Robot1_End_ctrl";
	RotatorName = "Robot1_A_ctrl";

	DFASC = CreateDefaultSubobject<UDFAbilitySystemComponent>(TEXT("DFASC"));
}

void ADFRobotArm::BeginPlay()
{
	Super::BeginPlay();
	
	// 컨트롤릭 스켈레탈에 결합
	ControlRigComponent->AddMappedCompleteSkeletalMesh(Skeletal);

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
		UE_LOG(LogTemp, Warning, TEXT("RobotArm : End의 복귀위치 X : %f, Y : %f, Z : %f")
			, EndHomeTransform.GetLocation().X, EndHomeTransform.GetLocation().Y, EndHomeTransform.GetLocation().Z);
		AHomeRotate = ControlRigComponent->GetControlRotator(RotatorName, EControlRigComponentSpace::WorldSpace);
		UE_LOG(LogTemp, Warning, TEXT("RobotArm : A의 복귀위치 X : %f, Y : %f, Z : %f")
			, AHomeRotate.Pitch, AHomeRotate.Yaw, AHomeRotate.Roll);
	}
}

void ADFRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if (DFASC && MasterSpecHandle.IsValid())
	{
		bool bActivated = DFASC->TryActivateAbility(MasterSpecHandle);

		FGameplayAbilitySpec* Spec = DFASC->FindAbilitySpecFromClass(UDFGA_RobotArmMasterAbility::StaticClass());
		if (Spec && Spec->GetPrimaryInstance())
		{
			if (UDFGA_RobotArmMasterAbility* Inst = Cast<UDFGA_RobotArmMasterAbility>(Spec->GetPrimaryInstance()))
			{
				UE_LOG(LogTemp, Warning, TEXT("로봇암 : 델리게이트 등록 성공"));
				Inst->OnFinishTireAssembly.AddDynamic(this, &ADFRobotArm::HandleAbilityFinished);
			}
		}

		//if (UDFGA_RobotArmMasterAbility* GA_Master = Cast<UDFGA_RobotArmMasterAbility>(MasterSpecHandle))
		//{
		//	GA_Master->OnFinishTireAssembly.AddDynamic(this, &ADFRobotArm::HandleAbilityFinished);
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("로봇암 : 델리게이트 등록 실패"));
		//}

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
	OnRobotArmFinished.Broadcast(OwnerTag, bFinished);
}

