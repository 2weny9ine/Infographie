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
  vec3 v = normalize(-surface_position);

  vec3 ambient = color_ambient;
  vec3 diffuse = vec3(0.0);
  vec3 specular = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 light_pos = light_positions[i];
    vec3 l = normalize(light_pos - surface_position);
    float reflection_diffuse = max(dot(n, l), 0.0);
    diffuse += reflection_diffuse * color_diffuse;

    if(reflection_diffuse > 0.0) {
      vec3 h = normalize(v + l);
      float reflection_specular = pow(max(dot(n, h), 0.0), brightness);
      specular += reflection_specular * color_specular;
    }
  }

  fragment_color = vec4(ambient + diffuse + specular, 1.0);
}