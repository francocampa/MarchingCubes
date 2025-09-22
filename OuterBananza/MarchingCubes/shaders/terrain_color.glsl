#version 330 core

layout (location = 0) in vec3 aPos;    // vertex position
layout (location = 1) in vec3 aNormal; // vertex normal

out vec3 vNormal; // pass to fragment shader
out vec3 vPos;    // optional: world position if you want height-based coloring

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass normal to fragment shader (convert to world space if needed)
    vNormal = mat3(transpose(inverse(model))) * aNormal;

    // Pass world position
    vPos = vec3(model * vec4(aPos, 1.0));
}
