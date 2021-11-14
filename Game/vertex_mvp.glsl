# version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = vec4(position, 1.0); 
}