#include <iostream>

#include <render/scene/sim_light.h>

#include <dae.h>

#include "dae2sim.h"

int export_light(daeScene& scene, daeElement* elem, const std::string& path)
{
    int status = 0;

    Vec4 diffuse = col::White;
    f32 attenuation = 0.05f;
    f32 fallOffAngle = 0.0f;
    f32 fallOffExp = 0.0f;

    auto technique = ((domLight*)elem)->getTechnique_common();
    CLight::Type type = CLight::Type::Directional;
    CLight::Kind kind = CLight::Kind::Linear;
    
    auto directional = technique->getDirectional();
    if (directional)
    {
        type = CLight::Type::Directional;

        auto color = directional->getColor()->getValue();
        diffuse.x = (f32)color.get(0);
        diffuse.y = (f32)color.get(1);
        diffuse.z = (f32)color.get(2);
    }

    auto point = technique->getPoint();
    if (point)
    {
        type = CLight::Type::Point;

        auto color = point->getColor()->getValue();
        diffuse.x = (f32)color.get(0);
        diffuse.y = (f32)color.get(1);
        diffuse.z = (f32)color.get(2);

        if (point->getLinear_attenuation())
        {
            kind = CLight::Kind::Linear;
            attenuation = (f32)point->getLinear_attenuation()->getValue();
        }
        if (point->getConstant_attenuation())
        {
            kind = CLight::Kind::Constant;
            attenuation = (f32)point->getConstant_attenuation()->getValue();
        }
        if (point->getQuadratic_attenuation())
        {
            kind = CLight::Kind::Quadratic;
            attenuation = (f32)point->getQuadratic_attenuation()->getValue();
        }
    }

    auto spot = technique->getSpot();
    if (spot)
    {
        type = CLight::Type::Spot;

        auto color = spot->getColor()->getValue();
        diffuse.x = (f32)color.get(0);
        diffuse.y = (f32)color.get(1);
        diffuse.z = (f32)color.get(2);

        if (spot->getLinear_attenuation())
        {
            kind = CLight::Kind::Linear;
            attenuation = (f32)spot->getLinear_attenuation()->getValue();
        }
        if (point->getConstant_attenuation())
        {
            kind = CLight::Kind::Constant;
            attenuation = (f32)spot->getConstant_attenuation()->getValue();
        }
        if (point->getQuadratic_attenuation())
        {
            kind = CLight::Kind::Quadratic;
            attenuation = (f32)spot->getQuadratic_attenuation()->getValue();
        }

        fallOffAngle = (f32)spot->getFalloff_angle()->getValue();
        if (spot->getFalloff_exponent())
            fallOffExp = (f32)spot->getFalloff_exponent()->getValue();
    }


    //f32 attenuation = (f32)perspective->getYfov()->getValue()

    sim::io::CMemStream ms(1024);
    sim::rnr::CLight light;

    light.SetType(type);
    light.SetKind(kind);
    light.SetDiffuse(diffuse);
    light.SetAttenuation(attenuation);
    light.SetFallOffAngle(fallOffAngle);
    light.SetFallOffExponent(fallOffExp);

    status |= !light.Save(&ms);

    daeString file = daeGetID(elem);
    std::string dir = path + "/light/";
    filesystem::create_directories(dir);

    status |= dump(ms, dir + file);

    sim::rnr::CLight l2;
    l2.Load(&ms);

    if (OK == status)
        scene.content.lights[daeGetName(elem)] = file;

    return status;
    return 0;
}