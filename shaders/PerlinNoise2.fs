#version 110

varying vec2 position;
varying float time;

uniform vec2 RESOLUTION;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(14.11, 78.1122))) * 43758.1234);
}

float QunticCurve(float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float Noise(in vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = smoothstep(0.0, 1.0, f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    vec2 st = gl_FragCoord.xy / RESOLUTION;
    st *= 15.0;
    float value = Noise(st);
    gl_FragColor = vec4(vec3(value), 1.0);
}