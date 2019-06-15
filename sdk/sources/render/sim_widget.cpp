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

#include <render/sim_widget.h>
#include <render/sim_driver.h>

#include <render/sim_font_atlas.h>

#include <imgui_internal.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidget::CWidget( CFontAtlas* atlas )
	: CRect2D()
{
    m_imContext = ImGui::CreateContext( atlas->m_imAtlas );

    m_imStyle = SIM_NEW ImGuiStyle();
    ImGui::StyleColorsDark(m_imStyle);
}

// ----------------------------------------------------------------------//

CWidget::CWidget( const std::string& name, CFontAtlas* atlas )
	: CWidget(atlas)
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CWidget::~CWidget()
{
    SIM_SAFE_DELETE( m_imStyle );
    ImGui::DestroyContext( m_imContext );
}

// ----------------------------------------------------------------------//

void CWidget::Update( f32 dt, void* userData )
{
    m_imContext->IO.DisplaySize = ImVec2( m_size.x, m_size.y );
    m_imContext->IO.DeltaTime = dt;
    
    ImGui::SetCurrentContext(m_imContext);
    ImGui::NewFrame();
    ImGui::Begin( m_name.c_str() );
}
// ----------------------------------------------------------------------//

void CWidget::Render( CDriver *driver )
{
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();

    ImDrawData* imData = ImGui::GetDrawData();

    for ( s32 n = 0; n < imData->CmdListsCount; n++ )
    {
        const ImDrawList* cmdList   = imData->CmdLists[n];
        const ImDrawVert* vtxBuffer = cmdList->VtxBuffer.Data;
        const ImDrawIdx* idxBuffer  = cmdList->IdxBuffer.Data;

        for ( s32 i = 0; i < cmdList->CmdBuffer.Size; i++ )
        {
            const ImDrawCmd* pcmd = &cmdList->CmdBuffer[ i ];

            /*
            CRect2D r;
            r.Bound((f32)x, (f32)y, (f32)m_width, (f32)m_height);
            r.SetMaterial( m_material );

            r.Render(driver, &m_texRect);
            */
        }
    }
}

// ----------------------------------------------------------------------//

void CWidget::DrawString( CDriver* driver, s32 x, s32 y, const std::string& text, Vec4 color )
{
    ImGui::Text( text.c_str() );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

