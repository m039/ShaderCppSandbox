#version 110

varying vec2 position;

void main()
{
    gl_FragColor = vec4(position.x * 1.0, position.y, 0.0, 0.0);
}