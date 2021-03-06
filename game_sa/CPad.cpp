/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "CPad.h"

// mouse states
CMouseControllerState & CPad::PCTempMouseControllerState = *(CMouseControllerState *)0xB73404;
CMouseControllerState & CPad::NewMouseControllerState = *(CMouseControllerState *)0xB73418;
CMouseControllerState & CPad::OldMouseControllerState = *(CMouseControllerState *)0xB7342C;

CPad::CPad()
{
	((void(__thiscall*)(CPad*))(0x541D80))(this);
}

// Converted from thiscall void CPad::UpdateMouse(void) 0x53F3C0
void CPad::UpdateMouse()
{
	((void(__thiscall*)(CPad*))(0x53F3C0))(this);
}

// Converted from thiscall void CPad::ReconcileTwoControllersInput(CControllerState const&controllerA,CControllerState const&controllerB) 0x53F530
void CPad::ReconcileTwoControllersInput(CControllerState const& controllerA, CControllerState const& controllerB)
{
	((void(__thiscall*)(CPad*, CControllerState const&, CControllerState const&))(0x53F530))(this, controllerA, controllerB);
}

// Converted from thiscall void CPad::SetDrunkInputDelay(int delay) 0x53F910
void CPad::SetDrunkInputDelay(int delay)
{
	((void(__thiscall*)(CPad*, int))(0x53F910))(this, delay);
}

// Converted from thiscall void CPad::StartShake(short time,uchar frequency,uint) 0x53F920
void CPad::StartShake(short time, unsigned char frequency, unsigned int arg2)
{
	((void(__thiscall*)(CPad*, short, unsigned char, unsigned int))(0x53F920))(this, time, frequency, arg2);
}

// Converted from thiscall void CPad::StartShake_Distance(short time,uchar frequency,float x,float y,float z) 0x53F9A0
void CPad::StartShake_Distance(short time, unsigned char frequency, float x, float y, float z)
{
	((void(__thiscall*)(CPad*, short, unsigned char, float, float, float))(0x53F9A0))(this, time, frequency, x, y, z);
}

// Converted from thiscall void CPad::StartShake_Train(float x,float y) 0x53FA70
void CPad::StartShake_Train(float x, float y)
{
	((void(__thiscall*)(CPad*, float, float))(0x53FA70))(this, x, y);
}

// Converted from thiscall void CPad::ProcessPCSpecificStuff(void) 0x53FB40
void CPad::ProcessPCSpecificStuff()
{
	((void(__thiscall*)(CPad*))(0x53FB40))(this);
}

// Converted from thiscall void CPad::StopShaking(short) 0x53FB50
void CPad::StopShaking(short arg0)
{
	((void(__thiscall*)(CPad*, short))(0x53FB50))(this, arg0);
}

// Converted from cdecl CPad* CPad::GetPad(int padNumber) 0x53FB70
CPad* CPad::GetPad(int padNumber)
{
	return ((CPad*(__cdecl*)(int))(0x53FB70))(padNumber);
}

// Converted from thiscall short CPad::GetSteeringLeftRight(void) 0x53FB80
short CPad::GetSteeringLeftRight()
{
	return ((short(__thiscall*)(CPad*))(0x53FB80))(this);
}

// Converted from thiscall short CPad::GetSteeringUpDown(void) 0x53FBD0
short CPad::GetSteeringUpDown()
{
	return ((short(__thiscall*)(CPad*))(0x53FBD0))(this);
}

// Converted from thiscall short CPad::GetPedWalkLeftRight(void) 0x53FC90
short CPad::GetPedWalkLeftRight()
{
	return ((short(__thiscall*)(CPad*))(0x53FC90))(this);
}

// Converted from thiscall short CPad::GetPedWalkUpDown(void) 0x53FD30
short CPad::GetPedWalkUpDown()
{
	return ((short(__thiscall*)(CPad*))(0x53FD30))(this);
}

// Converted from thiscall bool CPad::GetLookLeft(void) 0x53FDD0
bool CPad::GetLookLeft()
{
	return ((bool(__thiscall*)(CPad*))(0x53FDD0))(this);
}

// Converted from thiscall bool CPad::GetLookRight(void) 0x53FE10
bool CPad::GetLookRight()
{
	return ((bool(__thiscall*)(CPad*))(0x53FE10))(this);
}

// Converted from thiscall bool CPad::GetLookBehindForCar(void) 0x53FE70
bool CPad::GetLookBehindForCar()
{
	return ((bool(__thiscall*)(CPad*))(0x53FE70))(this);
}

// Converted from thiscall bool CPad::GetLookBehindForPed(void) 0x53FEC0
bool CPad::GetLookBehindForPed()
{
	return ((bool(__thiscall*)(CPad*))(0x53FEC0))(this);
}

// Converted from thiscall bool CPad::GetHorn(void) 0x53FEE0
bool CPad::GetHorn()
{
	return ((bool(__thiscall*)(CPad*))(0x53FEE0))(this);
}

// Converted from thiscall bool CPad::HornJustDown(void) 0x53FF30
bool CPad::HornJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x53FF30))(this);
}

// Converted from thiscall bool CPad::GetHydraulicJump(void) 0x53FF70
bool CPad::GetHydraulicJump()
{
	return ((bool(__thiscall*)(CPad*))(0x53FF70))(this);
}

// Converted from thiscall short CPad::GetCarGunFired(void) 0x53FF90
short CPad::GetCarGunFired()
{
	return ((short(__thiscall*)(CPad*))(0x53FF90))(this);
}

// Converted from thiscall short CPad::CarGunJustDown(void) 0x53FFE0
short CPad::CarGunJustDown()
{
	return ((short(__thiscall*)(CPad*))(0x53FFE0))(this);
}

// Converted from thiscall short CPad::GetHandBrake(void) 0x540040
short CPad::GetHandBrake()
{
	return ((short(__thiscall*)(CPad*))(0x540040))(this);
}

// Converted from thiscall short CPad::GetBrake(void) 0x540080
short CPad::GetBrake()
{
	return ((short(__thiscall*)(CPad*))(0x540080))(this);
}

// Converted from thiscall bool CPad::GetExitVehicle(void) 0x5400D0
bool CPad::GetExitVehicle()
{
	return ((bool(__thiscall*)(CPad*))(0x5400D0))(this);
}

// Converted from thiscall bool CPad::ExitVehicleJustDown(void) 0x540120
bool CPad::ExitVehicleJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540120))(this);
}

// Converted from thiscall uchar CPad::GetMeleeAttack(void) 0x540340
unsigned char CPad::GetMeleeAttack()
{
	return ((unsigned char(__thiscall*)(CPad*))(0x540340))(this);
}

// Converted from thiscall uchar CPad::MeleeAttackJustDown(void) 0x540390
unsigned char CPad::MeleeAttackJustDown()
{
	return ((unsigned char(__thiscall*)(CPad*))(0x540390))(this);
}

// Converted from thiscall short CPad::GetAccelerate(void) 0x5403F0
short CPad::GetAccelerate()
{
	return ((short(__thiscall*)(CPad*))(0x5403F0))(this);
}

// Converted from thiscall bool CPad::GetAccelerateJustDown(void) 0x540440
bool CPad::GetAccelerateJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540440))(this);
}

// Converted from thiscall bool CPad::NextStationJustUp(void) 0x5405B0
bool CPad::NextStationJustUp()
{
	return ((bool(__thiscall*)(CPad*))(0x5405B0))(this);
}

// Converted from thiscall bool CPad::LastStationJustUp(void) 0x5405E0
bool CPad::LastStationJustUp()
{
	return ((bool(__thiscall*)(CPad*))(0x5405E0))(this);
}

// Converted from thiscall bool CPad::CycleWeaponLeftJustDown(void) 0x540610
bool CPad::CycleWeaponLeftJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540610))(this);
}

// Converted from thiscall bool CPad::CycleWeaponRightJustDown(void) 0x540640
bool CPad::CycleWeaponRightJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540640))(this);
}

// Converted from thiscall bool CPad::GetTarget(void) 0x540670
bool CPad::GetTarget()
{
	return ((bool(__thiscall*)(CPad*))(0x540670))(this);
}

// Converted from thiscall bool CPad::GetDuck(void) 0x540700
bool CPad::GetDuck()
{
	return ((bool(__thiscall*)(CPad*))(0x540700))(this);
}

// Converted from thiscall bool CPad::DuckJustDown(void) 0x540720
bool CPad::DuckJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540720))(this);
}

// Converted from thiscall bool CPad::GetJump(void) 0x540750
bool CPad::GetJump()
{
	return ((bool(__thiscall*)(CPad*))(0x540750))(this);
}

// Converted from thiscall bool CPad::JumpJustDown(void) 0x540770
bool CPad::JumpJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540770))(this);
}

// Converted from thiscall bool CPad::GetSprint(void) 0x5407A0
bool CPad::GetSprint()
{
	return ((bool(__thiscall*)(CPad*))(0x5407A0))(this);
}

// Converted from thiscall bool CPad::SprintJustDown(void) 0x5407F0
bool CPad::SprintJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x5407F0))(this);
}

// Converted from thiscall bool CPad::ShiftTargetLeftJustDown(void) 0x540850
bool CPad::ShiftTargetLeftJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540850))(this);
}

// Converted from thiscall bool CPad::ShiftTargetRightJustDown(void) 0x540880
bool CPad::ShiftTargetRightJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540880))(this);
}

// Converted from thiscall short CPad::GetDisplayVitalStats(CPed *ped) 0x5408B0
short CPad::GetDisplayVitalStats(CPed* ped)
{
	return ((short(__thiscall *)(CPad*, CPed*))(0x5408B0))(this, ped);
}

// Converted from thiscall bool CPad::CollectPickupJustDown(void) 0x540A70
bool CPad::CollectPickupJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x540A70))(this);
}

// Converted from thiscall bool CPad::GetForceCameraBehindPlayer(void) 0x540AE0
bool CPad::GetForceCameraBehindPlayer()
{
	return ((bool(__thiscall*)(CPad*))(0x540AE0))(this);
}

// Converted from thiscall bool CPad::SniperZoomIn(void) 0x540B30
bool CPad::SniperZoomIn()
{
	return ((bool(__thiscall*)(CPad*))(0x540B30))(this);
}

// Converted from thiscall bool CPad::SniperZoomOut(void) 0x540B80
bool CPad::SniperZoomOut()
{
	return ((bool(__thiscall*)(CPad*))(0x540B80))(this);
}

// Converted from thiscall bool CPad::GetGroupControlForward(void) 0x541190
bool CPad::GetGroupControlForward()
{
	return ((bool(__thiscall*)(CPad*))(0x541190))(this);
}

// Converted from thiscall bool CPad::GetGroupControlBack(void) 0x5411B0
bool CPad::GetGroupControlBack()
{
	return ((bool(__thiscall*)(CPad*))(0x5411B0))(this);
}

// Converted from thiscall bool CPad::ConversationYesJustDown(void) 0x5411D0
bool CPad::ConversationYesJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x5411D0))(this);
}

// Converted from thiscall bool CPad::ConversationNoJustDown(void) 0x541200
bool CPad::ConversationNoJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x541200))(this);
}

// Converted from thiscall bool CPad::GroupControlForwardJustDown(void) 0x541230
bool CPad::GroupControlForwardJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x541230))(this);
}

// Converted from thiscall bool CPad::GroupControlBackJustDown(void) 0x541260
bool CPad::GroupControlBackJustDown()
{
	return ((bool(__thiscall*)(CPad*))(0x541260))(this);
}

// Converted from thiscall void CPad::Clear(bool enablePlayerControls, bool resetPhase) 0x541A70
void CPad::Clear(bool enablePlayerControls, bool resetPhase)
{
	((void(__thiscall *)(CPad*, bool, bool))(0x541A70))(this, enablePlayerControls, resetPhase);
}

// Converted from thiscall void CPad::UpdatePads(void) 0x541DD0
void CPad::UpdatePads()
{
	((void(__cdecl *)(void))(0x541DD0))();
}

void CPad::ClearMouseHistory()
{
	((void(__cdecl *)(void))(0x541BD0))();
}