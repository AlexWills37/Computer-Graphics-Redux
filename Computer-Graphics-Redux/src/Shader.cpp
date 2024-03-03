#include "Shader.h"

#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <iostream>



Shader::Shader(const std::string& shaderLocation)
    : m_RendererID(0)
{
    std::string vertexShaderFilepath = shaderLocation + ".vert";
    std::string fragmentShaderFilepath = shaderLocation + ".frag";
    ShaderProgramSource source = ParseShader(vertexShaderFilepath, fragmentShaderFilepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

/*
 * Reads in the shader files and return the strings with their source code.
 *
 * @input vertexFilepath - filepath to the vertex shader
 * @input fragFilepath - filepath to the fragment shader
 */
ShaderProgramSource Shader::ParseShader(const std::string& vertexFilepath, const std::string& fragFilepath)
{
    std::stringstream strings[2];   // stringstream creates a mutable string to my understanding (it acts as a buffer, like cout)

    // Copy files into the strings
    std::ifstream stream(vertexFilepath);
    strings[0] << stream.rdbuf();
    stream.close();

    stream.open(fragFilepath);
    strings[1] << stream.rdbuf();
    stream.close();

    // Return a struct with the two strings
    return { strings[0].str(), strings[1].str() };
}

/*
 * Creates an individual shader for a program
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{

    unsigned int id = glCreateShader(type); // Create space for the shader
    const char* src = source.c_str();   // Get char* version of string
    glShaderSource(id, 1, &src, nullptr);   // Create the shader
    glCompileShader(id);    // Compile shader

    // Handle errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);  // Place compile status in 'result'
    if (result == GL_FALSE) {
        // Get the length of the status
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // Allocate a space for the message and get the actual message
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);

        // Print error message
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" :  // specify which shader is having the issue
                (type == GL_FRAGMENT_SHADER ? "fragment" : "other type of"))
            << " shader." << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Create space for a shader program
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Create the shader program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Clean up shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}


void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}



int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformsLocationCache.find(name) != m_UniformsLocationCache.end())
        return m_UniformsLocationCache[name];


    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) // If uniform is not in shader OR not used in shader
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformsLocationCache[name] = location;
    return location;
}
