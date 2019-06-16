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
    m_imContext = ImGui::CreateContext( fontAtlas->m_imAtlas );

    m_imStyle = SIM_NEW ImGuiStyle();
    ImGui::StyleColorsDark(m_imStyle);
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
    SIM_SAFE_DELETE(m_imStyle);
    ImGui::DestroyContext(m_imContext);
}

// ----------------------------------------------------------------------//

void CCanvas::Resize( f32 w, f32 h )
{
	Bound( 0, 0, w, h );
}

// ----------------------------------------------------------------------//

void CCanvas::PointerDown( u32 x, u32 y )
{
	std::cout << "PointerDown: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::PointerDrag( u32 x, u32 y )
{
	std::cout << "PointerDrag: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::PointerUp( u32 x, u32 y )
{
	std::cout << "PointerUp: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::ClearEvents() 
{

}

// ----------------------------------------------------------------------//

void CCanvas::Update(f32 dt, void* userData)
{
    m_imContext->IO.DisplaySize = ImVec2(m_size.x, m_size.y);
    m_imContext->IO.DeltaTime = dt;

    ImGui::SetCurrentContext(m_imContext);
    ImGui::NewFrame();
    ImGui::Begin(m_name.c_str());

    CRect2D::Update( dt, userData );
}

// ----------------------------------------------------------------------//

void CCanvas::Render( CDriver* driver )
{
    u32 x = (u32)zpl_floor( m_position.x );
    u32 y = (u32)zpl_floor( m_position.y );
    u32 w = (u32)zpl_floor( m_size.x );
    u32 h = (u32)zpl_floor( m_size.y );

	driver->SetViewport( x, y, w, h );

    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();

    ImDrawData* imData = ImGui::GetDrawData();

    ImVec2 clipOff = imData->DisplayPos;
    ImVec2 clipScale = imData->FramebufferScale;

    s32 fbWidth = (s32)(imData->DisplaySize.x * imData->FramebufferScale.x);
    s32 fbHeight = (s32)(imData->DisplaySize.y * imData->FramebufferScale.y);

    if (fbWidth == 0 || fbHeight == 0)
        return;

    for (s32 n = 0; n < imData->CmdListsCount; n++)
    {
        const ImDrawList* cmdList = imData->CmdLists[n];
        const ImDrawVert* vtxBuffer = cmdList->VtxBuffer.Data;
        const ImDrawIdx* idxBuffer = cmdList->IdxBuffer.Data;

        CVertexSource vs;
        vs.m_type = CVertexSource::Type::Triangle;
        vs.m_vertexFormat = CVertexSource::AttributeFormat::ScreenPos | CVertexSource::AttributeFormat::TexCoord_0 | CVertexSource::AttributeFormat::Color;
        vs.m_vertexStride = CVertexSource::AttributeStride::ScreenPos + CVertexSource::AttributeStride::TexCoord_0 + CVertexSource::AttributeStride::Color;

        vs.m_vboSize = imData->TotalVtxCount * Value(vs.m_vertexStride);

        CVertexGroup vg;
        vg.SetVertexSource(&vs);

        for (s32 i = 0; i < cmdList->CmdBuffer.Size; i++)
        {
            const ImDrawCmd* pcmd = &cmdList->CmdBuffer[i];

            vs.m_vboData = (f32*)(&vtxBuffer[pcmd->VtxOffset]);
            vg.m_vboData = (u16*)(&idxBuffer[pcmd->IdxOffset]);

            vg.m_vboSize = pcmd->ElemCount;

            vg.SetMaterial((CMaterial*)pcmd->TextureId);

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

                driver->SetScissor(x, y, w, h);
                driver->Render(&vg);
            }
        }

        vs.m_vboData = nullptr;
        vg.m_vboData = nullptr;
    }
}

// ----------------------------------------------------------------------//

void CCanvas::DrawString(CDriver* driver, s32 x, s32 y, const std::string& text, Vec4 color)
{
    //ImGui::Text(text.c_str());
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
