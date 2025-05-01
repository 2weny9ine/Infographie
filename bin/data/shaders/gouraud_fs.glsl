
#version 330

in vec3 surface_color;

out vec4 fragment_color;

void main() {
  fragment_color = vec4(surface_color, 1.0);
}
