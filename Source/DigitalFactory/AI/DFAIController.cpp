

#include "AI/DFAIController.h"
#include "AI/DFAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Cell/DFCellBase.h"
#include "Engine/TargetPoint.h"
#include "GameplayTagContainer.h"

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

		// 다음 작업할 단계 미리 설정
		Blackboard->SetValueAsName(BBKEY_CURRENTPHASE, FGameplayTag::RequestGameplayTag("AGV.Phase.Supply").GetTagName());

		// 월드에서 보급셀 찾아서 블랙보드에 할당.
		ADFCellBase* SupplyCell = nullptr;
		FGameplayTag SupplyCellTag = FGameplayTag::RequestGameplayTag("Cell.Type.Supply");

		for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
		{
			ADFCellBase* CurrentCell = *It;
			if (CurrentCell->GetCellTypeTag().HasTag(SupplyCellTag))
			{
				SupplyCell = CurrentCell;
				break;
			}
		}
		
		if (SupplyCell)
		{
			Blackboard->SetValueAsObject(BBKEY_TARGETCELL, SupplyCell);
			UE_LOG(LogTemp, Log, TEXT("DFAICon : 보급셀 찾아서 넣기 성공!"));
		}

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

void ADFAIController::BeginPlay()
{
	Super::BeginPlay();

}
