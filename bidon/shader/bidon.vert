#version 330

// Variables d'entrée
in vec3 position;
in vec3 normal;
in vec2 tex_coords;

uniform mat4 projection;

// Variables de sortie
out vec3 vf_position;
out vec3 vf_normal;
out vec2 vf_tex_coords;


void main(void)
{
  gl_Position = projection * vec4(position, 1.0);
  vf_position = position;
  vf_normal = normal;
  vf_tex_coords = tex_coords;
}
