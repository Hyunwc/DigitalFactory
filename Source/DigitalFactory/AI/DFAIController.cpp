

#include "AI/DFAIController.h"
#include "AI/DFAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "EngineUtils.h"
#include "Cell/DFCellBase.h"
#include "Engine/TargetPoint.h"
#include "GameplayTagContainer.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFAGV.h"

ADFAIController::ADFAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_DFAGV.BB_DFAGV'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_DFAGV.BT_DFAGV'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}

	//bEnableCrowdAvoidance = true;

	bEnableCrowdAvoidance = false;
	AvoidanceRangeMultiplier = 1.0f;
	CollisionQueryRange = 500.0f;
}

void ADFAIController::RunAI()
{
	// Blackboard : AIController에서 기본으로 제공하는 멤버변수
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 복귀장소 저장
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 복귀 경로 저장
		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			ATargetPoint* Target = *It;
			if (Target)
			{
				Blackboard->SetValueAsVector(BBKEY_RETURNPOS, Target->GetActorLocation());
				break;
			}
		}
		
		// 구동되었으니 목적지를 Supply로
		ADFAGV* CurrentAGV = Cast<ADFAGV>(GetPawn());
		if (CurrentAGV)
		{
			CurrentAGV->SetAGVPhaseTag(FGameplayTag::RequestGameplayTag(TEXT("AGV.Phase.Supply")));
		}

		// 다음 작업할 단계 미리 설정
		//Blackboard->SetValueAsName(BBKEY_CURRENTPHASE, FGameplayTag::RequestGameplayTag("AGV.Phase.Supply").GetTagName());

		// 월드에 모든 Cell Free 상태로
		//for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
		//{
		//	ADFCellBase* CurrentCell = *It;
		//	if (IsValid(CurrentCell->GetDFAbilitySystemComponent()))
		//	{
		//		CurrentCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
		//	}
		//}

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ADFAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ADFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("DFAIController : 저 빙의 했어요"));

	// TODO : 분석 해볼 것
	// Crowd 그룹 설정(선택 사항)
	if (bEnableCrowdAvoidance)
	{
		if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
		{
			// 모든 AGV를 같은 그룹(1)으로 설정하고 서로 회피
			CrowdComp->SetAvoidanceGroup(1);
			CrowdComp->SetGroupsToAvoid(1);
			CrowdComp->SetGroupsToIgnore(0);
		}
	}
	//RunAI();
	//RunAI();
}

void ADFAIController::BeginPlay()
{
	Super::BeginPlay();

	//// Crowd Avoidance 설정
	if (bEnableCrowdAvoidance)
	{
		if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
		{
			// Crowd Simulation 활성화
			CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::Enabled);
			// 회피 품질 설정 (Good : 성능/품질 균형)
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
	
			// 회피 범위 설정
			CrowdComp->SetCrowdAvoidanceRangeMultiplier(AvoidanceRangeMultiplier);
			CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	
			// 경로 최적화 범위
			CrowdComp->SetCrowdPathOptimizationRange(1000.0f);
	
			// 분리 가중치(AGV끼리 떨어지려는 힘)
			CrowdComp->SetCrowdSeparationWeight(2.0f);
	
			UE_LOG(LogTemp, Log, TEXT("DFAIController : Crowd Avoidance Enabled"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("DFAIController : Failed to get CrowdFollowingComponent"));
		}
	}
}
