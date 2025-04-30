
#version 330

in vec4 position;
in vec4 normal;

out vec3 surface_position;
out vec3 surface_normal;

uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main()
{
  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

  surface_normal = vec3(normalMatrix * normal);

  surface_position = vec3(modelViewMatrix * position);

  gl_Position = projectionMatrix * modelViewMatrix * position;
}