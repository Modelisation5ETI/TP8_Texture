#version 330

// Variables d'entrée
in vec3 position;
in vec3 normal;

uniform mat4 projection;

// Variables de sortie
out vec3  vf_position;
out vec3  vf_normal;
out float vf_light;

const vec3 light_pos=vec3(-50,100, -30);

void main(void)
{
  gl_Position = projection * vec4(position, 1.0);
  vf_position = position;
  
  // Weighted normal for wetness
  vec3 weighted_normal = normal * normal;
  vf_normal = weighted_normal;
  
  vec3 p = normalize(light_pos - vf_position);
  vf_light = max(0.0, dot(normal, p));
}
