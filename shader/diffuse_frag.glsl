#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 LightDirection2;
uniform vec3 LightColor2;
uniform vec3 AmbientLightColor;

//Material Properties
uniform vec3 Ambient;
uniform vec4 Diffuse;
uniform vec3 Specular;
uniform float Shininess;

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

vec4 processFragmentColor(vec3 lightDirection, vec3 lightColor) {
  // Lookup the color in Texture on coordinates given by texCoord. NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec4 ObjectTextureColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  //Calculate the lambert lighting factor based on the object normal
  float LightDiffuseFactor = max(dot(normal, vec4(normalize(lightDirection), 1.0f)), 0.0f);

  //Calculate the specular color
  vec3 ViewDirection = normalize(CameraPosition);
  vec3 ReflectionDirection = reflect(-lightDirection, normal.xyz);
  float SpecularFactor = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
  vec3 SpecularColor = vec3(SpecularFactor) * lightColor * Specular;

  //Finalize the light's color contribution
  vec4 LightFragmentColor = ObjectTextureColor * vec4(LightDiffuseFactor) * vec4(lightColor.rgb,1.0) * Diffuse;

    //combine all the light colors with the scene's ambient color
  return vec4(SpecularColor,1.0) + (LightFragmentColor);
}

void main() {
  // Create first values for fragment color with first light
  FragmentColor = vec4(AmbientLightColor * Ambient, 1.0) + processFragmentColor(LightDirection, LightColor);

  // Add values computed for the second light to fragment
  FragmentColor +=  processFragmentColor(LightDirection2, LightColor2);
  FragmentColor.a = Transparency;
}