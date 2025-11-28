#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;
uniform vec3 camPos;    

void main()
{
   vec3 lightPos = vec3(5.0, 5.0, 5.0);
   float Ka = 0.2;
   float Kd = 0.7;
   float Ks = 0.2;
   float shininess = 32.0; 
   float Ia = 0.3;   
   float Il = 5.0;
   float k = 1.0;

   vec3 N = normalize(Normal);
   vec3 L = normalize(lightPos - FragPos);
   vec3 V = normalize(camPos - FragPos);

   float NL = max(dot(N, L), 0.0);

   vec3 R = reflect(-L, N); 
   float RS = max(dot(R, V), 0.0); 
   float d = distance(FragPos, lightPos);

   vec3 intensity = vec3(Ia * Ka + (Il / (k + d)) * (Kd * NL + Ks * pow(RS, shininess)));

   vec3 color = intensity + vec3(0.4f, 0.4f, 0.4f);

   FragColor = vec4(color, 1.0);
}