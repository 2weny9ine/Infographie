#version 330

in vec4 position;
in vec4 normal;

out vec3 surface_color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

uniform int num_active_lights;
uniform vec3 light_positions[4];

void main() {
  mat4 normalMatrix = transpose(inverse(modelViewMatrix));
  vec3 surface_normal = vec3(normalMatrix * normal);
  vec3 surface_position = vec3(modelViewMatrix * position);
  vec3 n = normalize(surface_normal);

  vec3 total_diffuse = vec3(0.0);
  vec3 total_specular = vec3(0.0);

  for(int i = 0; i < num_active_lights; ++i) {
    vec3 light_pos = light_positions[i];
    vec3 l = normalize(light_pos - surface_position);
    float reflection_diffuse = max(dot(n, l), 0.0);
    total_diffuse += reflection_diffuse;

    if(reflection_diffuse > 0.0) {
      vec3 v = normalize(-surface_position);
      vec3 r = reflect(-l, n);
      float reflection_specular = pow(max(dot(v, r), 0.0), brightness);
      total_specular += reflection_specular;
    }
  }

  surface_color = color_ambient + color_diffuse * total_diffuse + color_specular * total_specular;

  gl_Position = projectionMatrix * modelViewMatrix * position;
}