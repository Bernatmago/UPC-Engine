# version 330
out vec4 color;

in vec3 normal;
in vec3 frag_world_position;
in vec2 tex_coord;

uniform sampler2D texture;

uniform vec3 light_position;
uniform vec3 ligh_direction;
uniform vec3 light_color;
uniform float ambient_strength;
uniform bool is_directional;

void main()
{
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_world_position);
    float diffuse_strength = max(dot(norm, light_direction), 0.0);

    vec3 texture_color = texture2D(texture, tex_coord).xyz;
    texture_color = (ambient_strength + diffuse_strength) * light_color * texture_color;
    color = vec4(texture_color.xyz, texture2D(texture, tex_coord).w);
}
