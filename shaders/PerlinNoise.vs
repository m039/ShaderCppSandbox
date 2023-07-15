#version 110

uniform mat4 MVP;
uniform float TIME;
attribute vec2 vPos;

varying vec2 position;
varying float time;

void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    position = vPos + vec2(0.5, 0.5);
    time = TIME;
}