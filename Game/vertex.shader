# version 330
layout(location=0) in vec3 my_vertex_position;
void main()
{
 gl_Position = vec4(my_vertex_position, 1.0); 
}