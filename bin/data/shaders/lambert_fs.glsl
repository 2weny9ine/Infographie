#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;

uniform int   num_active_lights;
uniform vec3 light_positions[4];
uniform vec3 light_colors[4];
uniform float global_intensity;

void main() {
  vec3 n = normalize(surface_normal);
  vec3 total_diffuse = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 l = normalize(light_positions[i] - surface_position);
    float diff = max(dot(n, l), 0.0);
    total_diffuse += color_diffuse * light_colors[i] * diff;
  }

  vec3 result = color_ambient + global_intensity * total_diffuse;
  fragment_color = vec4(result, 1.0);
}
