#include <stdlib.h>

#include "Shader.h"

HaruShader HaruShaderCreate(const char *VertexSource, const char *FragmentSource) {
    sg_shader_desc Description = {0};

    Description.vertex_func.source = VertexSource;
    Description.fragment_func.source = FragmentSource;

    sg_shader Shader = sg_make_shader(&Description);

    return (HaruShader){ .ShaderID = Shader, .IsValid = 1 };
}

void HaruShaderDestroy(HaruShader *Shader) {
    if (Shader && Shader -> IsValid) {
        sg_destroy_shader(Shader -> ShaderID);
        
        Shader -> IsValid = 0;
    }
}