#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <imgui.h>
#include <fstream>

#include "Program.hpp"
#include "FileUtils.hpp"
#include "linmath.h"

class ShaderWatcher : public efsw::FileWatchListener
{
public:
    ShaderWatcher(std::function<void(void)> callback)
    {
        _callback = callback;
    }

    void handleFileAction(efsw::WatchID watchid,
                          const std::string &dir,
                          const std::string &filename,
                          efsw::Action action,
                          std::string oldFilename) override
    {
        switch (action)
        {
        case efsw::Actions::Modified:
            _callback();
            break;
        default:
            break;
        }
    }

private:
    std::function<void(void)> _callback;
};

static std::filesystem::path ShadersDictory = "../shaders/";

static const char* SelectedShadeNameFile = "NoiseShaderSandbox.settings";

static const struct
{
    float x, y;
} Vertices[6] =
    {
        {-0.5f, -0.5f},
        {-0.5f, 0.5f},
        {0.5f, 0.5f},
        {-0.5f, -0.5f},
        {0.5f, -0.5f},
        {0.5f, 0.5f}};

void PrintGLErrors()
{
    gl::GLenum err;
    while ((err = gl::glGetError()) != gl::GL_NO_ERROR)
    {
        std::cerr << "Error: " << std::hex << (int)err << std::endl;
    }
}

Program::Program()
{
    _fileWatcher = std::unique_ptr<efsw::FileWatcher>(
        new efsw::FileWatcher());
    _watcher = new ShaderWatcher([=]()
                                 { _commands.push([=]()
                                                  { CreateShaders(this->_selectedShader); }); });

    _fileWatcher->addWatch(ShadersDictory.c_str(), _watcher, true);
    _fileWatcher->watch();
}

Program::~Program()
{
    delete _watcher;
}

void Program::Init()
{
    FindShaders();
    CreateShaders(_selectedShader);
}

void Program::OnGUI()
{
    PopulateMainMenuBar();
}

void Program::FindShaders()
{
    for (const auto& entry: std::filesystem::directory_iterator(ShadersDictory)) {
        const auto filename = entry.path().filename().string();
        const auto trimmedName = filename.substr(0, filename.find("."));

        if (std::find(_shadersFound.begin(), _shadersFound.end(), trimmedName) == _shadersFound.end()) {
            _shadersFound.push_back(trimmedName);
        }
    }

    _selectedShader = LoadSelectedShaderName();
    if (_selectedShader.length() <= 0) {
        _selectedShader = "SolidColor";
    }
}

std::string Program::LoadSelectedShaderName() {
    return ReadFileAsString(SelectedShadeNameFile);
}

void Program::SaveSelectedShaderName(const std::string& selectedShaderName) {
    std::ofstream out(SelectedShadeNameFile);
    out << selectedShaderName;
    out.close();
}

void Program::PopulateMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Shaders"))
        {
            for (const auto& shaderName: _shadersFound)
            {
                if (ImGui::MenuItem(shaderName.c_str()))
                {
                    SaveSelectedShaderName(shaderName);
                    _selectedShader = shaderName;
                    CreateShaders(_selectedShader);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Program::CreateShaders(std::string &shaderName)
{
    if (_program)
    {
        gl::glDeleteProgram(_program);
        _program = 0;
    }

    // Vertex shader.
    std::filesystem::path vertexShaderPath = ShadersDictory.c_str() + (shaderName + ".vs");
    _vertexShader = CreateShader(gl::GL_VERTEX_SHADER, vertexShaderPath);

    // Fragment shader.
    std::filesystem::path fragmentShaderPath = ShadersDictory.c_str() + (shaderName + ".fs");
    _fragmentShader = CreateShader(gl::GL_FRAGMENT_SHADER, fragmentShaderPath);

    if (_fragmentShader && _vertexShader)
    {
        _program = gl::glCreateProgram();
        gl::glAttachShader(_program, _vertexShader);
        gl::glAttachShader(_program, _fragmentShader);
        gl::glLinkProgram(_program);

        gl::glDeleteShader(_vertexShader);
        gl::glDeleteShader(_fragmentShader);

        _mvpLocation = gl::glGetUniformLocation(_program, "MVP");
        _vposLocation = gl::glGetAttribLocation(_program, "vPos");
        _timeLocation = gl::glGetUniformLocation(_program, "TIME");

        gl::glEnableVertexAttribArray(_vposLocation);
        gl::glVertexAttribPointer(_vposLocation, 2, gl::GL_FLOAT, gl::GL_FALSE,
                                  sizeof(Vertices[0]), (void *)Vertices);
    }
    else
    {
        _program = 0;
    }
}

gl::GLuint Program::CreateShader(gl::GLenum type, std::filesystem::path shaderPath)
{
    auto shaderText = ReadFileAsString(shaderPath);
    if (shaderText.length() <= 0)
    {
        std::cerr << "Can't find the shader's code for " << shaderPath.c_str() << "." << std::endl;
        return 0;
    }
    else
    {
        auto shader = gl::glCreateShader(type);
        const char *text = shaderText.c_str();
        const int length = shaderText.length();
        gl::glShaderSource(shader, 1, &text, &length);
        gl::glCompileShader(shader);

        gl::GLint isCompiled = 0;
        gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &isCompiled);

        if (!isCompiled)
        {
            gl::GLint maxLength = 0;
            gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<gl::GLchar> errorLog(maxLength);
            gl::glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            std::cerr << "Shader compilation error in the file " << shaderPath.filename() << std::endl;
            std::cerr << "  " << &errorLog[0] << std::endl;

            gl::glDeleteShader(shader);

            return 0;
        }

        return shader;
    }
}

void Program::Draw(float time)
{
    while (!_commands.empty())
    {
        _commands.front()();
        _commands.pop();
    }

    mat4x4 m, p, mvp;

    mat4x4_identity(m);
    mat4x4_ortho(p, -0.5f, 0.5f, -0.5f, 0.5f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    if (_program)
    {
        gl::glUseProgram(_program);
        gl::glUniform1f(_timeLocation, time);
        gl::glUniformMatrix4fv(_mvpLocation, 1, gl::GL_FALSE, (const gl::GLfloat *)mvp);
        gl::glDrawArrays(gl::GL_TRIANGLES, 0, 6);
    }

    PrintGLErrors();
}