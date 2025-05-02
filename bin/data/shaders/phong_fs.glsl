#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3  color_ambient;
uniform vec3  color_diffuse;
uniform vec3  color_specular;
uniform float brightness;

uniform int   num_active_lights;
uniform vec3  light_positions[4];
uniform vec3  light_colors[4];
uniform float global_intensity;

void main() {
  vec3 N = normalize(surface_normal);
  vec3 V = normalize(-surface_position);
  vec3 total_diffuse = vec3(0.0);
  vec3 total_specular = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 L = normalize(light_positions[i] - surface_position);
    float diff = max(dot(N, L), 0.0);
    total_diffuse += diff * color_diffuse * light_colors[i];
    if(diff > 0.0) {
      vec3 R = reflect(-L, N);
      float spec = pow(max(dot(V, R), 0.0), brightness);
      total_specular += spec * color_specular * light_colors[i];
    }
  }

  vec3 result = color_ambient + global_intensity * (total_diffuse + total_specular);
  fragment_color = vec4(result, 1.0);
}
