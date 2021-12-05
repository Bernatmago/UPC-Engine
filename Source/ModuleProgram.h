#pragma once
#include "Module.h"

#include "MathGeoLib.h"

class ModuleProgram :
    public Module
{
public:
    ModuleProgram();
    ~ModuleProgram();

    bool Init() override;
    bool CleanUp() override;

    void Activate();
    void BindUniformFloat4x4(const char* name, const float* data, bool transpose = true);

    inline const int GetProgramId() const { return program_id; }
    
private:
    char* LoadShaderSource(const char* shader_file_name);
    unsigned int CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

    unsigned int vertex_shader_id;
    unsigned int fragment_shader_id;
    unsigned int program_id;
};

