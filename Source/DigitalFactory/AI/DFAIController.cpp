

#include "AI/DFAIController.h"
#include "AI/DFAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"

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
}

void ADFAIController::RunAI()
{
	// Blackboard : AIController에서 기본으로 제공하는 멤버변수
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 복귀장소 저장
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// CellPos 설정 (테스트용)
		FVector TargetLocation;

		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			ATargetPoint* Target = *It;
			if (Target->Tags.Contains("CellPos"))
			{
				TargetLocation = Target->GetActorLocation();
				break;
			}
		}

		// 타겟장소 저장
		Blackboard->SetValueAsVector(BBKEY_CELLPOS, TargetLocation);

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
	RunAI();
}
