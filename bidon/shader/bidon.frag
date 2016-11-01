#version 330

// Variables d'entrée
in vec3 vf_position;
in vec3 vf_normal;
in vec2 vf_tex_coords;

// Textures
uniform sampler2D decal_tex;
uniform sampler2D rust_tex;
uniform sampler2D brushed_tex;
uniform sampler2D shader_map_tex;

// Variable de sortie
out vec4 color;

void main(void)
{
  //
  vec4 col1 = texture(decal_tex, vec2(0.65 - vf_tex_coords.x, vf_tex_coords.y));
  vec4 col2 = texture(rust_tex, vf_tex_coords);
  vec4 col3 = texture(brushed_tex, vf_tex_coords);
  
  
  //
  // TODO: Illumination propre au matériau 
  //  + position light fixe en appliquant la rot au mesh et pas a la view.
  vec3 lightPos = vec3( 0,0, -30.0f );
  vec3 cameraPos = vec3( 0.0f, 0.0f, 0.0f );

  vec3 t = normalize( cameraPos - vf_position );
  vec3 l = normalize( lightPos - vf_position );
  vec3 s = normalize(reflect( l, vf_normal ));
  
  
  float Ka = 0.2f;
  float Kd = 0.8f;
  float Ks = 0.6f;
  int e_s = 128;

  float ambient = Ka;
  float diffuse = Kd * max( 0.0, dot( vf_normal, l ) );
  float specular = Ks * pow( max( 0.0, dot( s, t ) ), e_s );

  col1 = ( ambient + diffuse ) * col1 + specular * vec4( 1.0f, 1.0f, 1.0f, 1.0f );
  
  
  // Mix texture
  vec4 map = texture(shader_map_tex, vf_tex_coords);
  vec4 textureColor = map.x * col1 + map.y * col3 + map.z * col2;
  
  color = textureColor;
//   if (gl_FrontFacing)
//     color = col1;
//   else
//     color = col1 / 2;
}
