#ifndef _PROGRAM_
#define _PROGRAM_

#include <glbinding/gl/gl.h>
#include <filesystem>
#include <efsw/efsw.hpp>
#include <queue>
#include <string>
#include <vector>
#include "Context.hpp"

class Program {
public:
    Program(std::shared_ptr<Context> &context);

    ~Program();

    void Init();

    void Draw();

    void OnGUI();

private:
    std::shared_ptr<Context> _context;

    gl::GLuint _vertexBuffer, _vertexShader, _fragmentShader, _program = 0;

    gl::GLint _mvpLocation, _vposLocation, _timeLocation, _resolutionLocation;

    gl::GLuint CreateShader(const gl::GLenum type, const std::filesystem::path &path);

    void CreateShaders(const std::string& shaderName);

    void FindShaders();

    void PopulateMainMenuBar();

    void SaveSelectedShaderName(const std::string &selectedShader);

    std::string LoadSelectedShaderName();

    std::vector<std::string> _shadersFound;

    std::string _selectedShader;

    efsw::FileWatchListener* _watcher;

    std::unique_ptr<efsw::FileWatcher> _fileWatcher;

    std::queue<std::function<void(void)>> _commands;
};

#endif