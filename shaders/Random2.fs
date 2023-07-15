#version 110

varying vec2 position;
varying float time;

uniform vec2 RESOLUTION;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(14.11, 78.1122))) * 43758.1234);
}

const float Scale = 10.0;

void main()
{
    vec2 st = gl_FragCoord.xy / RESOLUTION;
    st *= Scale;
    st.x = floor(st.x);
    st.y = floor(st.y);
    float value = random(st);
    gl_FragColor = vec4(vec3(value), 1.0);
}