// requires:
// defining IMGUI_INCLUDE_IMGUI_USER_H and IMGUI_INCLUDE_IMGUI_USER_INL
// at the project level

#pragma once
#ifndef IMGUI_USER_ADDONS_H_
#define IMGUI_USER_ADDONS_H_

#ifndef IMGUI_INCLUDE_IMGUI_USER_INL
//#warning IMGUI_INCLUDE_IMGUI_USER_INL should be defined
#define IMGUI_INCLUDE_IMGUI_USER_INL
#endif //IMGUI_INCLUDE_IMGUI_USER_INL

#ifndef IMGUI_FORCE_INLINE
#	ifdef _MSC_VER
#		define IMGUI_FORCE_INLINE __forceinline
#	elif (defined(__clang__) || defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__))
#		define IMGUI_FORCE_INLINE inline __attribute__((__always_inline__))
#	else
#		define IMGUI_FORCE_INLINE inline
#	endif
#endif//IMGUI_FORCE_INLINE

#ifndef IMGUI_NO_INLINE
#	ifdef _MSC_VER
#		define IMGUI_NO_INLINE __declspec((noinline))
#	elif (defined(__clang__) || defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__))
#		define IMGUI_NO_INLINE __attribute__((__noinline__))
#	else
#		define IMGUI_NO_INLINE
#	endif
#endif//IMGUI_NO_INLINE

#ifdef NO_IMGUIADDONS	// Mispell fix
#	undef NO_IMGUI_ADDONS
#	define NO_IMGUI_ADDONS
#endif //NO_IMGUIADDONS

#ifdef NO_IMGUI_ADDONS  // This definition turns all "normal" addons into "yes_addons"
#   if (!defined(YES_IMGUISTYLESERIALIZER) && !defined(NO_IMGUISTYLESERIALIZER))
#       define NO_IMGUISTYLESERIALIZER
#   endif //YES_IMGUISTYLESERIALIZER
#   if (!defined(YES_IMGUIFILESYSTEM) && !defined(NO_IMGUIFILESYSTEM))
#       define NO_IMGUIFILESYSTEM
#   endif //YES_IMGUIFILESYSTEM
#   if (!defined(YES_IMGUIDATECHOOSER) && !defined(NO_IMGUIDATECHOOSER))
#       define NO_IMGUIDATECHOOSER
#   endif //YES_IMGUIDATECHOOSER
#   if (!defined(YES_IMGUILISTVIEW) && !defined(NO_IMGUILISTVIEW))
#       define NO_IMGUILISTVIEW
#   endif //YES_IMGUILISTVIEW
#   if (!defined(YES_IMGUITOOLBAR) && !defined(NO_IMGUITOOLBAR))
#       define NO_IMGUITOOLBAR
#   endif //YES_IMGUITOOLBAR
#   if (!defined(YES_IMGUIPANELMANAGER) && !defined(NO_IMGUIPANELMANAGER))
#       define NO_IMGUIPANELMANAGER
#   endif //YES_IMGUIPANELMANAGER
#   if (!defined(YES_IMGUITABWINDOW) && !defined(NO_IMGUITABWINDOW))
#       define NO_IMGUITABWINDOW
#   endif //YES_IMGUITABWINDOW
#   if (!defined(YES_IMGUIDOCK) && !defined(NO_IMGUIDOCK))
#       define NO_IMGUIDOCK
#   endif //YES_IMGUIDOCK
#   if (!defined(YES_IMGUINODEGRAPHEDITOR) && !defined(NO_IMGUINODEGRAPHEDITOR))
#       define NO_IMGUINODEGRAPHEDITOR
#   endif //YES_IMGUINODEGRAPHEDITOR
#   if (!defined(YES_IMGUICODEEDITOR) && !defined(NO_IMGUICODEEDITOR))
#       define NO_IMGUICODEEDITOR
#   endif //YES_IMGUICODEEDITOR
#   if (!defined(YES_IMGUISTRING) && !defined(NO_IMGUISTRING))
#       define NO_IMGUISTRING
#   endif //YES_IMGUISTRING
#   if (!defined(YES_IMGUIHELPER) && !defined(NO_IMGUIHELPER))
#       define NO_IMGUIHELPER
#   endif //YES_IMGUIHELPER
#   if (!defined(YES_IMGUIVARIOUSCONTROLS) && !defined(NO_IMGUIVARIOUSCONTROLS))
#       define NO_IMGUIVARIOUSCONTROLS
#   endif //YES_IMGUIVARIOUSCONTROLS
#endif // NO_IMGUI_ADDONS

// Defining a custom placement new() with a dummy parameter allows us to bypass including <new> which on some platforms complains when user has disabled exceptions.
// (This is necessary because the original IM_PLACEMENT_NEW macro is not exposed by imgui.h, but only by imgui_internal.h)
#ifndef IMIMPL_HAS_PLACEMENT_NEW
#define IMIMPL_HAS_PLACEMENT_NEW
struct ImImplPlacementNewDummy {};
inline void* operator new(size_t, ImImplPlacementNewDummy, void* ptr) { return ptr; }
inline void operator delete(void*, ImImplPlacementNewDummy, void*) {}
#define IMIMPL_PLACEMENT_NEW(_PTR)  new(ImImplPlacementNewDummy() ,_PTR)
#endif //IMIMPL_HAS_PLACEMENT_NEW

#ifndef NO_IMGUISTRING
#include "./imguistring/imguistring.h"
#endif //NO_IMGUISTRING
#ifndef NO_IMGUIHELPER
#include "./imguihelper/imguihelper.h"
#endif //NO_IMGUIHELPER
#ifndef NO_IMGUITABWINDOW
#include "./imguitabwindow/imguitabwindow.h"
#endif //NO_IMGUITABWINDOW
#ifndef NO_IMGUILISTVIEW
#include "./imguilistview/imguilistview.h"
#endif //NO_IMGUILISTVIEW
#ifndef NO_IMGUIFILESYSTEM
#include "./imguifilesystem/imguifilesystem.h"
#endif //NO_IMGUIFILESYSTEM
#ifndef NO_IMGUITOOLBAR
#include "./imguitoolbar/imguitoolbar.h"
#endif //NO_IMGUITOOLBAR
#ifndef NO_IMGUIPANELMANAGER
#include "./imguipanelmanager/imguipanelmanager.h"
#endif //NO_IMGUIPANELMANAGER
#ifndef NO_IMGUIVARIOUSCONTROLS
#include "./imguivariouscontrols/imguivariouscontrols.h"
#endif //NO_IMGUIVARIOUSCONTROLS
#ifndef NO_IMGUISTYLESERIALIZER
#include "./imguistyleserializer/imguistyleserializer.h"
#endif //NO_IMGUISTYLESERIALIZER
#ifndef NO_IMGUIDATECHOOSER
#include "./imguidatechooser/imguidatechooser.h"
#endif //NO_IMGUIDATECHOOSER
#ifndef NO_IMGUICODEEDITOR
#include "./imguicodeeditor/imguicodeeditor.h"
#endif //NO_IMGUICODEEDITOR
#ifndef NO_IMGUINODEGRAPHEDITOR
#include "./imguinodegrapheditor/imguinodegrapheditor.h"
#endif //NO_IMGUINODEGRAPHEDITOR
#ifndef NO_IMGUIDOCK
#include "./imguidock/imguidock.h"
#endif //NO_IMGUIDOCK

#endif //IMGUI_USER_ADDONS_H_

