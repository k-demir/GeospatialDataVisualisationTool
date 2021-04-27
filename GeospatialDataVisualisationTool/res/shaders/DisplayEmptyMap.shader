#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

out vec4 area_color;

uniform mat4 u_MVP;
uniform vec4 u_fgColor;

void main() {
   gl_Position = u_MVP * vec4(position, 0.0, 1.0);
   area_color = u_fgColor;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 area_color;

void main() {
   color = area_color;
}
