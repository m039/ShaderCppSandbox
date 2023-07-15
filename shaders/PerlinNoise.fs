#version 110

varying vec2 position;
varying float time;

uniform vec2 RESOLUTION;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(14.11, 78.1122))) * 43758.1234);
}

vec2 GetPseudoRandomGradientVector(float x, float y) {
    float r = random(vec2(x, y));
    if (r > 0.75) {
        return vec2(1.0, 0.0);
    } else if (r > 0.5) {
        return vec2(-1.0, 0.0);
    } else if (r > 0.25) {
        return vec2(0.0, 1.0);
    } else {
        return vec2(0.0, -1.0);
    }
}

float QunticCurve(float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float Noise(vec2 p) {
    float left = floor(p.x);
    float top = floor(p.y);

    float pointInQuadX = p.x - left;
    float pointInQuadY = p.y - top;

    vec2 topLeftGradient = GetPseudoRandomGradientVector(left, top);
    vec2 topRightGradient = GetPseudoRandomGradientVector(left + 1.0, top);
    vec2 bottomLeftGradient = GetPseudoRandomGradientVector(left, top + 1.0);
    vec2 bottomRightGradient = GetPseudoRandomGradientVector(left + 1.0, top + 1.0);

    vec2 distanceToTopLeft = vec2(pointInQuadX, pointInQuadY);
    vec2 distanceToTopRight = vec2(pointInQuadX-1.0, pointInQuadY);
    vec2 distanceToBottomLeft = vec2(pointInQuadX, pointInQuadY - 1.0);
    vec2 distanceToBottomRight = vec2(pointInQuadX-1.0, pointInQuadY - 1.0);

    float tx1 = dot(distanceToTopLeft, topLeftGradient);
    float tx2 = dot(distanceToTopRight, topRightGradient);
    float bx1 = dot(distanceToBottomLeft, bottomLeftGradient);
    float bx2 = dot(distanceToBottomRight, bottomRightGradient);

    pointInQuadX = QunticCurve(pointInQuadX);
    pointInQuadY = QunticCurve(pointInQuadY);

    float tx = Lerp(tx1, tx2, pointInQuadX);
    float bx = Lerp(bx1, bx2, pointInQuadX);
    float tb = Lerp(tx, bx, pointInQuadY);

    return tb;
}

void main()
{
    vec2 st = gl_FragCoord.xy / RESOLUTION;
    st *= 30.0;
    float value = Noise(st);
    gl_FragColor = vec4(vec3(value), 1.0);
}