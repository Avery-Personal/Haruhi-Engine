#ifndef SHADER_H
#define SHADER_H

    #include "../../Core/System.h"
    #include "../../Third-Party/sokol_gfx.h"

    typedef struct {
        sg_shader ShaderID;

        HaruBoolean IsValid;
    } HaruShader;

    HaruShader HaruShaderCreate(const char *VertexSource, const char *FragmentSource);
    void HaruShaderDestroy(HaruShader *Shader);

#endif