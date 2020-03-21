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

    ImGui::CreateContext( m_fontAtlas->m_imAtlas );
    ImGui::StyleColorsDark();

    ImGui::GetIO().BackendPlatformName = m_name.c_str();
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

void CCanvas::Resize( f32 w, f32 h )
{
	Bound( 0, 0, w, h );
    ImGui::GetIO().DisplaySize = { Width(), Height() };
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
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

