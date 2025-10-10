
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DFAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADFAIController();

	void RunAI();
	void StopAI();

protected:
	// 컨트롤러가 폰에 빙의해서 조종을 할 때 발생되는 이벤트 함수
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	UBlackboardData* BBAsset;

	UPROPERTY()
	UBehaviorTree* BTAsset;
};
