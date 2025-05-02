#version 330

in vec4 position;
in vec4 normal;

out vec3 surface_color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3  color_ambient;
uniform vec3  color_diffuse;
uniform vec3  color_specular;
uniform float brightness;

uniform int   num_active_lights;
uniform vec3  light_positions[4];
uniform vec3  light_colors[4];
uniform float global_intensity;

void main() {
  vec3 surface_position = (modelViewMatrix * position).xyz;
  mat4 normalMatrix = transpose(inverse(modelViewMatrix));
  vec3 n = normalize((normalMatrix * normal).xyz);
  vec3 v = normalize(-surface_position);

  vec3 total_diffuse = vec3(0.0);
  vec3 total_specular = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 l = normalize(light_positions[i] - surface_position);
    float diff = max(dot(n, l), 0.0);
    total_diffuse += diff * color_diffuse * light_colors[i];
    if(diff > 0.0) {
      vec3 r = reflect(-l, n);
      float spec = pow(max(dot(v, r), 0.0), brightness);
      total_specular += spec * color_specular * light_colors[i];
    }
  }

  surface_color = color_ambient + global_intensity * (total_diffuse + total_specular);
  gl_Position = projectionMatrix * modelViewMatrix * position;
}
