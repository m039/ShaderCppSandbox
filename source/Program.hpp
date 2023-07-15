#ifndef _PROGRAM_
#define _PROGRAM_

#include <glbinding/gl/gl.h>
#include <filesystem>
#include <efsw/efsw.hpp>
#include <queue>
#include <string>
#include <vector>

class Program {
public:
    Program();

    ~Program();

    void Init();

    void Draw(float time);

    void OnGUI();

private:
    gl::GLuint _vertexShader, _fragmentShader, _program = 0;

    gl::GLint _mvpLocation, _vposLocation, _timeLocation;

    gl::GLuint CreateShader(gl::GLenum type, std::filesystem::path path);

    void CreateShaders(std::string& shaderName);

    void FindShaders();

    void PopulateMainMenuBar();

    std::vector<std::string> _shadersFound;

    std::string _selectedShader;

    efsw::FileWatchListener* _watcher;

    std::unique_ptr<efsw::FileWatcher> _fileWatcher;

    std::queue<std::function<void(void)>> _commands;
};

#endif