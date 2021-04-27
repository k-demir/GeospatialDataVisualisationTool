#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in float area_id;

out vec4 area_color;

uniform mat4 u_MVP;
uniform int u_idMap[1000];

void main() {
   gl_Position = u_MVP * vec4(position, 0.0, 1.0);
   area_color = vec4(float(u_idMap[int(area_id)] % 4) / 4.0, float(int(float(u_idMap[int(area_id)])/4.0) % 4) / 4.0, float(int(float(u_idMap[int(area_id)])/16.0) % 4)/4.0, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 area_color;

void main() {
   color = area_color;
}
