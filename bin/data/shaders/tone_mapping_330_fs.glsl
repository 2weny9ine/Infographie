#version 330

in vec2 surface_texcoord;
out vec4 fragment_color;

uniform sampler2D image;
uniform float tone_mapping_exposure;
uniform bool tone_mapping_toggle;
uniform float tone_mapping_gamma;

vec3 tone_mapping_reinhard(vec3 x)
{
  return x / (x + vec3(1.0));
}

vec3 tone_mapping_aces_filmic(vec3 x)
{
  float a = 2.51f;
  float b = 0.03f;
  float c = 2.43f;
  float d = 0.59f;
  float e = 0.14f;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
  vec3 color = texture(image, surface_texcoord).rgb;

  color = pow(color, vec3(tone_mapping_gamma));
  color = vec3(1.0) - exp(-color * tone_mapping_exposure);
  color = tone_mapping_toggle ? tone_mapping_aces_filmic(color)
                              : tone_mapping_reinhard(color);
  color = pow(color, vec3(1.0 / tone_mapping_gamma));

  fragment_color = vec4(color, 1.0);
}
