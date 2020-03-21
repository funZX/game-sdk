// requires:
// defining IMGUI_INCLUDE_IMGUI_USER_H and IMGUI_INCLUDE_IMGUI_USER_INL
// at the project level

#pragma once
#ifndef IMGUI_USER_ADDONS_INL_
#define IMGUI_USER_ADDONS_INL_

#ifndef NO_IMGUI_WIDGETS_CPP_AUTO_COMPILATION
#   ifdef STB_TEXTEDIT_IMPLEMENTATION  // because imstb_textedit.h is included by imgui_widgets.cpp
#       error. Please do not compile imgui_widgets.cpp yourself when using this file.
#   endif //STB_TEXTEDIT_IMPLEMENTATION
#   include <imgui_widgets.cpp>
#else // NO_IMGUI_WIDGETS_CPP_AUTO_COMPILATION
#   ifndef NO_IMGUICODEEDITOR
#       error. Please define NO_IMGUICODEEDITOR or remove NO_IMGUI_WIDGETS_CPP_AUTO_COMPILATION.
#   endif //NO_IMGUICODEEDITOR
#endif //NO_IMGUI_WIDGETS_CPP_AUTO_COMPILATION


#ifndef NO_IMGUISTRING
#include "./imguistring/imguistring.cpp"
#endif //NO_IMGUISTRING
#ifndef NO_IMGUIHELPER
#include "./imguihelper/imguihelper.cpp"
#endif //NO_IMGUIHELPER
#ifndef NO_IMGUITABWINDOW
#include "./imguitabwindow/imguitabwindow.cpp"
#endif //NO_IMGUITABWINDOW
#ifndef NO_IMGUILISTVIEW
#include "./imguilistview/imguilistview.cpp"
#endif //NO_IMGUILISTVIEW
#ifndef NO_IMGUIFILESYSTEM
#include "./imguifilesystem/imguifilesystem.cpp"
#endif //NO_IMGUIFILESYSTEM
#ifndef NO_IMGUITOOLBAR
#include "./imguitoolbar/imguitoolbar.cpp"
#endif //NO_IMGUITOOLBAR
#ifndef NO_IMGUIPANELMANAGER
#include "./imguipanelmanager/imguipanelmanager.cpp"
#endif //NO_IMGUIPANELMANAGER
#ifndef NO_IMGUIVARIOUSCONTROLS
#include "./imguivariouscontrols/imguivariouscontrols.cpp"
#endif //NO_IMGUIVARIOUSCONTROLS
#ifndef NO_IMGUISTYLESERIALIZER
#include "./imguistyleserializer/imguistyleserializer.cpp"
#endif //NO_IMGUISTYLESERIALIZER
#ifndef NO_IMGUIDATECHOOSER
#include "./imguidatechooser/imguidatechooser.cpp"
#endif //NO_IMGUIDATECHOOSER
#ifndef NO_IMGUICODEEDITOR
#include "./imguicodeeditor/imguicodeeditor.cpp"
#endif //NO_IMGUICODEEDITOR
#ifdef IMGUISCINTILLA_ACTIVATED
#include "./imguiscintilla/imguiscintilla.cpp"
#endif //IMGUISCINTILLA_ACTIVATED
#ifndef NO_IMGUINODEGRAPHEDITOR
#include "./imguinodegrapheditor/imguinodegrapheditor.cpp"
#endif //NO_IMGUINODEGRAPHEDITOR
#ifndef NO_IMGUIDOCK
#include "./imguidock/imguidock.cpp"
#endif //NO_IMGUIDOCK

#endif //IMGUI_USER_ADDONS_INL_

