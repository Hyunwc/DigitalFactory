
#include "DFGameplayTags.h"

namespace DFGameplayTags
{
	/* AGV Phase*/
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_None, "AGV.Phase.None");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Home, "AGV.Phase.Home");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Supply, "AGV.Phase.Supply");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Flexible, "AGV.Phase.Flexible");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Trim, "AGV.Phase.Trim");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Inspection, "AGV.Phase.Inspection");
	UE_DEFINE_GAMEPLAY_TAG(AGV_Phase_Load, "AGV.Phase.Load");

	/* AGV State */
	UE_DEFINE_GAMEPLAY_TAG(AGV_State_Idle, "AGV.State.Idle");
	UE_DEFINE_GAMEPLAY_TAG(AGV_State_Move, "AGV.State.Move");
	UE_DEFINE_GAMEPLAY_TAG(AGV_State_Return, "AGV.State.Return");
	UE_DEFINE_GAMEPLAY_TAG(AGV_State_Work, "AGV.State.Work");

	/* Cell State */
	UE_DEFINE_GAMEPLAY_TAG(Cell_State_Free, "Cell.State.Free");
	UE_DEFINE_GAMEPLAY_TAG(Cell_State_Pending, "Cell.State.Pending");
	UE_DEFINE_GAMEPLAY_TAG(Cell_State_Working, "Cell.State.Working");

	/* Cell Type */
	UE_DEFINE_GAMEPLAY_TAG(Cell_Type_Supply, "Cell.Type.Supply");
	UE_DEFINE_GAMEPLAY_TAG(Cell_Type_Flexible, "Cell.Type.Flexible");
	UE_DEFINE_GAMEPLAY_TAG(Cell_Type_Trim, "Cell.Type.Trim");
	UE_DEFINE_GAMEPLAY_TAG(Cell_Type_Inspection, "Cell.Type.Inspection");
	UE_DEFINE_GAMEPLAY_TAG(Cell_Type_Load, "Cell.Type.Load");

	/* RobotArm CombinePhase */
	UE_DEFINE_GAMEPLAY_TAG(RobotArm_CombinePhase_Front, "RobotArm.CombinePhase.Front");
	UE_DEFINE_GAMEPLAY_TAG(RobotArm_CombinePhase_Back, "RobotArm.CombinePhase.Back");
	UE_DEFINE_GAMEPLAY_TAG(RobotArm_CombinePhase_Finish, "RobotArm.CombinePhase.Finish");

	/* RobotArm Type */
	UE_DEFINE_GAMEPLAY_TAG(RobotArm_Type_Left, "RobotArm.Type.Left");
	UE_DEFINE_GAMEPLAY_TAG(RobotArm_Type_Right, "RobotArm.Type.Right");

	/* Tire State */
	UE_DEFINE_GAMEPLAY_TAG(Tire_State_Combine, "Tire.State.Combine");
	UE_DEFINE_GAMEPLAY_TAG(Tire_State_None, "Tire.State.None");

	/* Vehicle */
	UE_DEFINE_GAMEPLAY_TAG(Vehicle_Tire_Back_Left, "Vehicle.Tire.Back.Left");
	UE_DEFINE_GAMEPLAY_TAG(Vehicle_Tire_Back_Right, "Vehicle.Tire.Back.Right");
	UE_DEFINE_GAMEPLAY_TAG(Vehicle_Tire_Front_Left, "Vehicle.Tire.Front.Left");
	UE_DEFINE_GAMEPLAY_TAG(Vehicle_Tire_Front_Right, "Vehicle.Tire.Front.Right");
}