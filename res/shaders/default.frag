#version 330 core
in vec3 gFragPos;
in vec3 gNormal;
in vec3 gVertexPos[3];
in vec3 gVertexNormal[3];

out vec4 FragColor;
uniform vec3 camPos;    

vec3 normalInterpolation(vec3 P)
{
    vec3 A = gVertexPos[0];
    vec3 B = gVertexPos[1];
    vec3 C = gVertexPos[2];

    vec3 Na = normalize(gVertexNormal[0]);
    vec3 Nb = normalize(gVertexNormal[1]);
    vec3 Nc = normalize(gVertexNormal[2]);

    vec3 v0 = B - A;
    vec3 v1 = C - A;
    vec3 v2 = P - A;

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    if (abs(denom) < 1e-6)
        return normalize(gNormal);

    float beta = (d11 * d20 - d01 * d21) / denom;
    float gamma = (d00 * d21 - d01 * d20) / denom;
    float alpha = 1.0 - beta - gamma;

    float u = beta;
    float w = 1.0 - beta;

    float denom_t = alpha + gamma;
    float t = max((gamma / denom_t), 0.0);

    vec3 Nq = (1.0 - u) * Na + u * Nb;
    vec3 Nr = (1.0 - w) * Nb + w * Nc; 
    vec3 Np = (1.0 - t) * Nq + t * Nr;

    return normalize(Np);
}


void main()
{
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    float Ka = 1.7;
    float Kd = 0.7;
    float Ks = 0.2;
    float n = 32.0; 
    float Ia = 0.3;   
    float Il = 5.0;
    float k = 1.0;
    
    vec3 N = normalInterpolation(gFragPos);
    
    vec3 L = normalize(lightPos - gFragPos);
    vec3 V = normalize(camPos - gFragPos);
    float NL = max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N); 
    float RS = max(dot(R, V), 0.0); 
    float d = distance(gFragPos, lightPos);
    vec3 intensity = vec3(Ia * Ka + (Il / (k + d)) * (Kd * NL + Ks * pow(RS, n)));
    vec3 color = intensity;
    FragColor = vec4(color, 1.0);
}