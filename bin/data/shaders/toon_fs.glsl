
#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

uniform float brightness;

uniform vec3 light_position;

void main()
{
  vec3 n = normalize(surface_normal);

  vec3 l = normalize(light_position - surface_position);

  float reflection_diffuse = max(dot(n, l), 0.0);
  
  float toonShade;
  
  if (reflection_diffuse > 0.73)       // Valeur non standard
      toonShade = 1.0;
  else if (reflection_diffuse > 0.42)  // Valeur non standard
      toonShade = 0.67;                // Valeur non standard
  else if (reflection_diffuse > 0.18)  // Valeur non standard
      toonShade = 0.35;                // Valeur non standard
  else
      toonShade = 0.15;
  
  vec3 finalColor = color_ambient + (color_diffuse * toonShade);
  
  fragment_color = vec4(finalColor, 1.0);
}