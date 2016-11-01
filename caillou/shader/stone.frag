#version 330

uniform sampler2D tex;

in vec3  vf_position;
in vec3  vf_normal;
in float vf_light;

out vec4 color;

void main(void)
{  
  //
  vec4 base_color_x = vec4(texture(tex, (vf_position / 10.0).yz));
  vec4 base_color_y = vec4(texture(tex, (vf_position / 20.0).zx));
  vec4 base_color_z = vec4(texture(tex, (vf_position / 10.0).xy));

  vec4 textureColor = vf_normal.x * base_color_x + vf_normal.y * base_color_y + vf_normal.z * base_color_z;
  
  //
  vec3 lightPos = vec3( 10, 10, -10.0 );
  vec3 cameraPos = vec3( 0.0, 0.0, 0.0 );

  vec3 t = normalize( cameraPos - vf_position );
  vec3 l = normalize( lightPos - vf_position );
  vec3 s = normalize(reflect( l, vf_normal ));
  
  
  float Ka = 0.2;
  float Kd = 0.8;
  float Ks = 0.6;
  int e_s = 128;

  float ambient = Ka;
  float diffuse = Kd * max( 0.0, dot( vf_normal, l ) );
  float specular = Ks * pow( max( 0.0, dot( s, t ) ), e_s );

  color = ( ambient + diffuse ) * textureColor + specular * vec4( 1.0, 1.0, 1.0, 1.0 );

}
