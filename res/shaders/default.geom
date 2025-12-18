#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 FragPos[];
in vec3 Normal[];

out vec3 gFragPos;
out vec3 gNormal;
out vec3 gVertexPos[3];
out vec3 gVertexNormal[3];

void main()
{
    for(int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        gFragPos = FragPos[i];
        gNormal = Normal[i];

        gVertexPos[0] = FragPos[0];
        gVertexPos[1] = FragPos[1];
        gVertexPos[2] = FragPos[2];
        
        gVertexNormal[0] = Normal[0];
        gVertexNormal[1] = Normal[1];
        gVertexNormal[2] = Normal[2];
        
        EmitVertex();
    }
    EndPrimitive();
}