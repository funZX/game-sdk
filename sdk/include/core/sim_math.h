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

#ifndef __SIM_MATH_LIB_H
#define __SIM_MATH_LIB_H

// ----------------------------------------------------------------------//



namespace sim
{
    typedef zpl_vec2  Vec2;
    typedef zpl_vec3  Vec3;
    typedef zpl_vec4  Vec4;
                      
    typedef zpl_mat2  Mat2;
    typedef zpl_mat3  Mat3;
    typedef zpl_mat4  Mat4;

    typedef zpl_quat  Quat;
    typedef zpl_plane Plane;

    typedef union
    {
        struct 
        {
            zpl_vec3		translation;
            zpl_vec3		scale;
            zpl_quat		quaternion;
        };
    } Transform;

// ----------------------------------------------------------------------//
    static const u32 Matrix4StackMaxSize = 64;
// ----------------------------------------------------------------------//

    struct Mat4Stack
    {
        Mat4 	 matrices[Matrix4StackMaxSize];
        Mat4*    topmatrix;
        u32	 	 top;
    };

    static inline void Mat4StackIdentity( Mat4Stack* m4s )
    {
        zpl_mat4_identity( m4s->topmatrix );
    }

    static inline void Mat4StackLoad( Mat4Stack* m4s, Mat4* m )
    {
        zpl_mat4_copy( m4s->topmatrix, m );
    }

    static inline void Mat4StackMultiply( Mat4Stack* m4s, Mat4* m )
    {
        zpl_mat4_mul( m4s->topmatrix, m, m4s->topmatrix);
    }

    static inline void Mat4StackTranslate( Mat4Stack* m4s, Vec3 translation )
    {
        m4s->topmatrix->w.xyz = translation;
    }

    static inline void Mat4StackRotate( Mat4Stack* m4s, Vec3 axis, f32 angle )
    {
        Mat4 m;
        zpl_mat4_rotate( &m, axis, angle );

        Mat4StackMultiply( m4s , &m );
    }

    static inline void Mat4StackScale( Mat4Stack* m4s, Vec3 scale)
    {
        m4s->topmatrix->x.x *= scale.x;
        m4s->topmatrix->y.y *= scale.y;
        m4s->topmatrix->z.z *= scale.z;
    }

    static inline void Mat4StackClear( Mat4Stack* m4s )
    {
        m4s->topmatrix = &m4s->matrices[0];
        m4s->top = 0;

        Mat4StackIdentity(m4s);

#if SIM_DEBUG
        for ( u32 k = 0; k < Matrix4StackMaxSize; k++ )
            SIM_MEMSET( &m4s->topmatrix[k], 0, sizeof( zpl_mat4 ) );
#endif
    }

    static inline void Mat4StackPush( Mat4Stack* m4s, bool load )
    {
        SIM_ASSERT(m4s->top < Matrix4StackMaxSize);

        ++m4s->top;
        m4s->topmatrix = &m4s->matrices[m4s->top];

        if (load)
            Mat4StackLoad(m4s, &m4s->matrices[m4s->top - 1]);
    }

    static inline void Mat4StackPop( Mat4Stack* m4s )
    {
        SIM_ASSERT(m4s->top > 0);

#if SIM_DEBUG
        SIM_MEMSET( m4s->topmatrix, 0, sizeof( zpl_mat4 ) );
#endif

        --m4s->top;
        m4s->topmatrix = &m4s->matrices[m4s->top];
    }

    typedef union Rgb {
        struct {
            u8 r, g, b;
        };
        u8 e[3];
    } Rgb;

    typedef union Rgba {
        struct {
            u8 r, g, b, a;
        };
        u32 rgba;
        Rgb rgb;
        u8  e[4];
    } Rgba;

    // ----------------------------------------------------------------------//
    static inline void RgbaToVec4( Vec4* vcol, const Rgba* col )
    {
        vcol->x = zpl_clamp(col->r / 255.0f, 0.0f, 1.0f);
        vcol->y = zpl_clamp(col->g / 255.0f, 0.0f, 1.0f);
        vcol->z = zpl_clamp(col->b / 255.0f, 0.0f, 1.0f);
        vcol->w = zpl_clamp(col->a / 255.0f, 0.0f, 1.0f);
    }

    static inline void Vec4ToRgba( Rgba* col, const Vec4* vcol )
    {
        col->r = (u8)zpl_clamp(vcol->x * 255.0f, 0, 255);
        col->g = (u8)zpl_clamp(vcol->y * 255.0f, 0, 255);
        col->b = (u8)zpl_clamp(vcol->z * 255.0f, 0, 255);
        col->a = (u8)zpl_clamp(vcol->w * 255.0f, 0, 255);
    }
// ----------------------------------------------------------------------//
}; // namespace sim
#endif // __SIM_MATH_LIB_H
