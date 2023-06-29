#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 a_TexCoord;
out vec3 a_NormalVec;
out vec3 a_FragPos;

uniform float u_OffsetX;

// Coordinate system transformation matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    a_FragPos = vec3(u_Model * vec4(aPos.x + u_OffsetX, aPos.yz, 1.0));
    // TODO: inverse-transpose the upper 3x3 matrix and send as a uniform instead of doing it in shaders
    a_NormalVec = mat3(transpose(inverse(u_Model))) * aNormal;
    a_TexCoord = aTexCoord;
    gl_Position = (u_Projection * u_View) * vec4(a_FragPos, 1.0);
}