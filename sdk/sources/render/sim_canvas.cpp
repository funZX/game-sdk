/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*/

#include <render/sim_font_atlas.h>

#include <render/sim_driver.h>
#include <render/sim_canvas.h>

#include <imgui_internal.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCanvas::CCanvas( CFontAtlas* fontAtlas )
	: CRect2D()
{
    m_fontAtlas     = fontAtlas;
    m_vertexSource  = SIM_NEW CVertexSource;

    m_vertexSource->m_type = CVertexSource::Type::Triangle;
    m_vertexSource->m_vertexFormat = CVertexSource::AttributeFormat::ScreenPos | CVertexSource::AttributeFormat::TexCoord_0 | CVertexSource::AttributeFormat::Color;
    m_vertexSource->m_vertexStride = CVertexSource::AttributeStride::ScreenPos + CVertexSource::AttributeStride::TexCoord_0 + CVertexSource::AttributeStride::Color;

    m_vertexGroup = SIM_NEW CVertexGroup;
    m_vertexGroup->SetVertexSource( m_vertexSource );

    ImGui::SetAllocatorFunctions(&CCanvas::ImGuiAlloc, &CCanvas::ImGuiDelloc, this);
    ImGui::CreateContext( m_fontAtlas->m_imAtlas );

    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style.FrameRounding     = 4.0f;
    style.GrabRounding      = 4.0f;
    style.AntiAliasedLines  = true;
    style.AntiAliasedFill   = true;
}

// ----------------------------------------------------------------------//

CCanvas::CCanvas( const std::string& name, CFontAtlas* fontAtlas)
	: CCanvas( fontAtlas )
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CCanvas::~CCanvas()
{
    SIM_SAFE_DELETE(m_vertexGroup);

    m_vertexSource->m_vboData   = nullptr;
    SIM_SAFE_DELETE( m_vertexSource );

    ImGui::DestroyContext();
}

// ----------------------------------------------------------------------//

void* CCanvas::ImGuiAlloc(size_t sz, void* userData)
{    
    void* ptr = (void*) SIM_NEW u8[sz];
    return ptr;
}

// ----------------------------------------------------------------------//

void CCanvas::ImGuiDelloc(void* ptr, void* userData)
{
    SIM_SAFE_DELETE_ARRAY(ptr)
}

// ----------------------------------------------------------------------//

void CCanvas::Resize( f32 w, f32 h )
{
	Bound( 0, 0, w, h );
    ImGui::GetIO().DisplaySize = { Width(), Height() };
}

// ----------------------------------------------------------------------//

void CCanvas::ClearEvents() 
{

}

// ----------------------------------------------------------------------//

void CCanvas::Update(f32 dt, void* userData)
{
    ImGui::GetIO().DeltaTime = dt;

    ImGui::NewFrame();
    OnGui.Emit( this );
    ImGui::Render();
}

// ----------------------------------------------------------------------//

void CCanvas::Render( CDriver* driver )
{
    u32 x = (u32)zpl_floor(m_position.x);
    u32 y = (u32)zpl_floor(m_position.y);
    u32 w = (u32)zpl_floor(m_size.x);
    u32 h = (u32)zpl_floor(m_size.y);
    driver->SetViewport(x, y, w, h);

    ImDrawData* imData = ImGui::GetDrawData();

    ImVec2 clipOff   = imData->DisplayPos;
    ImVec2 clipScale = imData->FramebufferScale;

    s32 fbWidth = (s32)(imData->DisplaySize.x * imData->FramebufferScale.x);
    s32 fbHeight = (s32)(imData->DisplaySize.y * imData->FramebufferScale.y);

    if (fbWidth == 0 || fbHeight == 0)
        return;

    driver->EnableScissor(true);
    for ( s32 n = 0; n < imData->CmdListsCount; n++ )
    {
        const ImDrawList* cmdList = imData->CmdLists[n];

        m_vertexSource->m_vboData = (f32*)cmdList->VtxBuffer.Data;
        m_vertexSource->m_vboSize = cmdList->VtxBuffer.Size * m_vertexSource->GetVertexStride();
        m_vertexSource->BufferData( GL_STREAM_DRAW );
        SIM_CHECK_OPENGL();

        m_vertexGroup->m_vboSize = cmdList->IdxBuffer.Size * sizeof(u16);
        m_vertexGroup->m_vboData = (u16*)cmdList->IdxBuffer.Data;
        m_vertexGroup->BufferData( GL_STREAM_DRAW );
        SIM_CHECK_OPENGL();

        for (s32 i = 0; i < cmdList->CmdBuffer.Size; i++)
        {
            const ImDrawCmd* pcmd = &cmdList->CmdBuffer[i];
            SIM_ASSERT( pcmd->ElemCount % 3 == 0 );

            ImVec4 clipRect;
            clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
            clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
            clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
            clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;

            if (clipRect.x < fbWidth && clipRect.y < fbHeight && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
            {
                u32 x = (u32)zpl_floor(clipRect.x);
                u32 y = (u32)zpl_floor(fbHeight - clipRect.w);
                u32 w = (u32)zpl_floor(clipRect.z - clipRect.x);
                u32 h = (u32)zpl_floor(clipRect.w - clipRect.y);

                m_vertexGroup->m_vboSize    = pcmd->ElemCount * sizeof( u16 );
                m_vertexGroup->m_vboOffset  = pcmd->IdxOffset * sizeof( u16 );
                m_vertexGroup->SetMaterial((CMaterial*)pcmd->TextureId);

                driver->SetScissor(x, y, w, h);
                driver->Render( m_vertexGroup );
            }
        }
    }
    driver->EnableScissor(false);
}

// ----------------------------------------------------------------------//
void CCanvas::MouseDown(int button)
{
    OnMouseDown.Emit( this, button );
}
// ----------------------------------------------------------------------//
void CCanvas::MouseUp(int button)
{
    OnMouseUp.Emit( this, button );
}
// ----------------------------------------------------------------------//
void CCanvas::MouseMove(f32 x, f32 y)
{
    OnMouseMove.Emit( this, x, y );
}
// ----------------------------------------------------------------------//
void CCanvas::KeyDown(int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt)
{
    OnKeyDown.Emit( this, Key, KeyShift, KeyCtrl, KeyAlt );
}
// ----------------------------------------------------------------------//
void CCanvas::KeyUp(int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt)
{
    OnKeyUp.Emit( this, Key, KeyShift, KeyCtrl, KeyAlt );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

