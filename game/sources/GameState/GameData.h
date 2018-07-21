#ifndef __GAMEDATA_H
#define __GAMEDATA_H
namespace GameData
{
// ----------------------------------------------------------------------//
typedef enum
{
	eState_None = -1,

	eState_AppInit,
	eState_AppLoad,

	eState_MenuMain,
	eState_Game,

	eState_Count

} EStateID;
// ----------------------------------------------------------------------
}; // namespace GameData

#endif // __GAMEDATA_H

