#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 camPos;

void main()
{
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    vec3 N = normalize(mat3(model) * normal);

    vec3 lightPos = vec3(-5.0, 5.0, 5.0);

    float Ka = 0.2f;
    float Kd = 0.7f;
    float Ks = 0.2f;
    float shininess = 8.0f;
    float Ia = 0.3f;
    float Il = 5.0f;
    float k = 1.0f;

    vec3 L = normalize(lightPos - worldPos);
    vec3 V = normalize(camPos - worldPos);

    float NL = max(dot(N, L), 0.0);

    vec3 R = reflect(-L, N);
    float RS = max(dot(R, V), 0.0);

    float d = distance(worldPos, lightPos);

    vec3 intensity = vec3(Ia * Ka + (Il / (k + d)) * (Kd * NL + Ks * pow(RS, shininess)));

    color = intensity + vec3(0.2f, 0.2f, 0.2f);

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
