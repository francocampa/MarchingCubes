#version 330 core

in vec3 vNormal;
out vec4 FragColor;

void main()
{
    vec3 N = normalize(vNormal);

    float slope = max(0.0, dot(N, vec3(0.0, 1.0, 0.0)));

    vec3 flatColor   = vec3(0.1, 0.8, 0.1); // green for flat
    vec3 steepColor  = vec3(0.5, 0.25, 0.1); // brown for cliffs

    vec3 color = mix(steepColor, flatColor, slope);

    FragColor = vec4(color, 1.0);
}