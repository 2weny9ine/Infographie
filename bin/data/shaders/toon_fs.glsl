#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3  color_ambient;
uniform vec3  color_diffuse;

uniform int   num_active_lights;
uniform vec3  light_positions[4];
uniform vec3  light_colors[4];
uniform float global_intensity;

float toonLevel(float d) {
  if(d > 0.95) return 1.0;
  if(d > 0.5)  return 0.7;
  if(d > 0.25) return 0.4;
  return 0.1;
}

void main() {
  vec3 N = normalize(surface_normal);
  vec3 color = color_ambient;
  for(int i = 0; i < num_active_lights; ++i) {
    vec3 L = normalize(light_positions[i] - surface_position);
    float diff = max(dot(N, L), 0.0);
    float lvl = toonLevel(diff);
    color += global_intensity * lvl * color_diffuse * light_colors[i];
  }
  fragment_color = vec4(clamp(color, 0.0, 1.0), 1.0);
}
