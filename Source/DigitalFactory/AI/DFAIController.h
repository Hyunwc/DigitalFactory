
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DFAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;
class UCrowdFollowingComponent;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADFAIController();

protected:
	// 컨트롤러가 폰에 빙의해서 조종을 할 때 발생되는 이벤트 함수
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

public:
	void RunAI();

	void StopAI();

private:
	UPROPERTY()
	UBlackboardData* BBAsset;

	UPROPERTY()
	UBehaviorTree* BTAsset;

public:
	UPROPERTY(EditAnywhere, Category = "Crowd Avoidance")
	bool bUseCrowdFollowing = false;

	UPROPERTY(EditAnywhere, Category = "Crowd Avoidance")
	bool bEnableCrowdAvoidance = true;

	UPROPERTY(EditAnywhere, Category = "Crowd Avoidance")
	float AvoidanceRangeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Crowd Avoidance")
	float CollisionQueryRange = 500.0f;
};
