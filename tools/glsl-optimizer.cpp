#include <iostream>
#include <fstream>

#include "../../src/glsl/glsl_optimizer.h"

bool load(const char* file, char** shader_source, int* length)
{
    std::ifstream s;
    s.open(file);

    if (!s.is_open())
    {
        std::cout << "Error: Can't open file for read " << file << std::endl;
        return false;
    }

    s.seekg(0, std::ios::end);
    int len = s.tellg();
    s.seekg(0, std::ios::beg);

    char* buf = new char[len + 1];

    s.read(buf, len); buf[len] = 0;
    s.close();

    *shader_source = buf;
    *length = len;

    return true;
}

bool dump(const char* file, const char* shader_source, int length)
{
    std::ofstream s;
    s.open(file);

    if (!s.is_open())
    {
        std::cout << "Error: Can't open file for write " << file << std::endl;
        return false;
    }

    s.write(shader_source, length);
    s.close();

    return true;
}

void optimize_shader(glslopt_shader_type shader_type, const char* in_file, const char* out_file)
{
    char* shader_source = 0;
    int len = 0;

    if (!load(in_file, &shader_source, &len))
        return;

    auto ctx    = glslopt_initialize(kGlslTargetOpenGLES20);
    auto shader = glslopt_optimize(ctx, shader_type, shader_source, 0);

    if (glslopt_get_status(shader)) {
        auto new_shader_source = glslopt_get_output(shader);
        dump( out_file, new_shader_source, strlen(new_shader_source) );
    }
    else {
        auto errorLog = glslopt_get_log(shader);
        std::cout << "Error: " << errorLog << std::endl;
    }
    glslopt_shader_delete(shader);
    glslopt_cleanup(ctx);
}



struct Options
{
    bool            is_vertex_shader;

    const char*     in_file;
    const char*     out_file;
} options;

int main(int argc, char** argv)
{
    options.is_vertex_shader    = true;
    options.in_file             = 0;
    options.out_file            = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-p", argv[i]) == 0)
        {
            if (i + 1 == argc) break;

            options.is_vertex_shader = false;
        }
        else if (strcmp("-v", argv[i]) == 0)
        {
            if (i + 1 == argc) break;

            options.is_vertex_shader = true;
        }
        else if (argv[i][0] != '-')
        {
            options.in_file = argv[i];

            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options.out_file = argv[i + 1];
            }

            break;
        }
    }

    if (options.in_file == 0 || options.out_file == 0)
    {
        printf("_____________________________________________________________________\n");
        printf("|                  Copyright (c) Adrian SIMINCIUC 2019               |\n");
        printf("|                       Authors: Adrian SIMINCIUC                    |\n");
        printf("|____________________________________________________________________|\n\n");
        printf("USAGE: glsl-optimizer -[p, v] in_file_path out_file_path\n\n");

        return 0;
    }

    optimize_shader(options.is_vertex_shader ? kGlslOptShaderVertex : kGlslOptShaderFragment, options.in_file, options.out_file);

    return 0;
}
