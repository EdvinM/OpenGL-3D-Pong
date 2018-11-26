#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 AmbientLightColor;

//Material Properties
uniform vec3 MaterialAmbient;
uniform vec4 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShininess;

//camera pos
uniform vec3 CameraPosition;
// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;
in vec3 worldPosition;
// Wordspace normal passed from vertex shader
in vec4 normal;

// The final color
out vec4 FragmentColor;

void main() {
  // Lookup the color in Texture on coordinates given by texCoord. NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec4 ObjectTextureColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  //Calculate the lambert lighting factor based on the object normal
  float LightDiffuseFactor = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);

  //Calculate the specular color
  vec3 ViewDirection = normalize(CameraPosition - worldPosition);
  vec3 ReflectionDirection = reflect(-LightDirection, normal.xyz);
  float SpecularFactor = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), MaterialShininess);
  float specularStrength = 0.1;
  vec3 SpecularColor = vec3(specularStrength) * vec3(SpecularFactor) * LightColor * MaterialSpecular;

  //Finalize the light's color contribution
  vec4 LightFragmentColor = ObjectTextureColor * vec4(LightDiffuseFactor) * vec4(LightColor.rgb,1.0) * MaterialDiffuse;

  //combine all the light colors with the scene's ambient color
  FragmentColor = vec4(AmbientLightColor * MaterialAmbient,1.0) + vec4(SpecularColor,1.0) + (LightFragmentColor);
  //FragmentColor = vec4(AmbientLightColor,1.0) + (LightFragmentColor);
  FragmentColor.a = Transparency;
}