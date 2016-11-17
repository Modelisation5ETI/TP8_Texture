#version 330

uniform sampler2D tex;
uniform sampler2D caustic_tex;


in vec3  vf_position;
in vec3  vf_normal;
in float vf_light;

out vec4 color;

float ClampRange( float input0, float min, float max )
{
  return clamp((input0-min)/(max-min),0,1);
}

void main(void)
{  
  //Get texture colors. Texture coordinates are computed according to the vertex position
  vec4 base_color_x = vec4(texture(tex, (vf_position / 10.0).yz));
  vec4 base_color_y = vec4(texture(tex, (vf_position / 20.0).zx));
  vec4 base_color_z = vec4(texture(tex, (vf_position / 10.0).xy));

  //Add caustic noise to the side parts of the stone
  vec4 caustic_color_x = vec4(texture(caustic_tex, (vf_position / 10.0).yz));
  base_color_x = base_color_x + 5*caustic_color_x/exp(vf_position.y/1.25);
  vec4 caustic_color_z = vec4(texture(caustic_tex, (vf_position / 10.0).xy));
  base_color_z = base_color_z + 5 * caustic_color_z/exp(vf_position.y/1.25);

  // Wetness (TODO : Use a wetness map texture instead)
  float wetness = abs(cos(0.17*vf_position.y));

  // Mix parameters according to the wetness.
  //   This is a quick adaptation of what can be found p.59 of :
  //   http://dindinx.net/cpe/eti5/NaughtyDog_TechArt_Final.pdf
  vec3 normal = normalize(vf_position - vec3(0,0,0));
  normal = normalize(mix(normal, vec3(0,1,0), ClampRange(wetness,0.98,1.0)));

  vec4 textureColor = vf_normal.x * base_color_x + vf_normal.y * base_color_y + vf_normal.z * base_color_z;
  textureColor.w = 1.0;
  vec4 baseColor_y_SQR = textureColor * textureColor * textureColor;
  textureColor = mix( textureColor, baseColor_y_SQR, ClampRange(wetness, 0.0, 0.35) );

  // Phong lightning
  vec3 lightPos = vec3(25,17, 0);
  vec3 cameraPos = vec3( 0.0, 0.0, 0.0 );

  vec3 t = normalize( cameraPos - vf_position );
  vec3 l = normalize( lightPos - vf_position );
  vec3 s = normalize(reflect( l, normal ));
  
  float Ka = 0.2;
  float Kd = 0.5;
  float Ks = 0.1;
  int e_s = 128;

  float ambient = Ka;
  float diffuse = Kd * max( 0.0, dot( normal, l ) );
  float specular = Ks * pow( max( 0.0, dot( s, t ) ), e_s );

  specular = mix(specular, 0.1, ClampRange(wetness,0.6,0.8));
  // Final color
  color = ( ambient + diffuse ) * textureColor + specular * vec4( 1.0, 1.0, 1.0, 0.0 );
}
