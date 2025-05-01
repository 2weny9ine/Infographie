#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

uniform int num_active_lights;
uniform vec3 light_positions[4];

void main() {
  vec3 n = normalize(surface_normal);
  vec3 total_diffuse = vec3(0.0);
  vec3 total_specular = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 light_pos = light_positions[i];
    vec3 l = normalize(light_pos - surface_position);
    float reflection_diffuse = max(dot(n, l), 0.0);
    total_diffuse += reflection_diffuse * color_diffuse;

    if(reflection_diffuse > 0.0) {
      vec3 v = normalize(-surface_position);
      vec3 r = reflect(-l, n);
      float spec = pow(max(dot(v, r), 0.0), brightness);
      total_specular += spec * color_specular;
    }
  }

  vec3 result = color_ambient + total_diffuse + total_specular;
  fragment_color = vec4(result, 1.0);
}