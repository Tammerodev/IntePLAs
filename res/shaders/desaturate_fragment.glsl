// Fragment shader
uniform sampler2D texture;
uniform float time; // Time variable for animation
uniform float distortionAmount; // Adjust distortion effect

uniform float desaturationAmount; // Adjustable amount
uniform float temp;

uniform int isDead;

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

void main()
{
	//float darknessFactor = clamp(desaturationAmount, 0.0, 1.0);
    float darknessFactor = 1.0; 

    vec2 texCoords = gl_TexCoord[0].xy;

    vec4 color = texture2D(texture, texCoords);
   
	color -= vec4(darknessFactor, darknessFactor, darknessFactor, 1.0) / 2.0;

	if(temp > 100.0) {
		color.r += abs(temp - 100.0) / 10.0;
	}

	if(temp < -20.0) {
		color.b += abs(temp + 20.0) / 10.0;
	}

	if (distortionAmount != 0.0) {
		distortionAmount /= 500.0;

    	vec2 uv = gl_TexCoord[0].xy;

		float pulse = 0.5 * cos(time * 0.5);

		uv += pulse * distortionAmount * vec2(sin(uv.y * 10.0), cos(uv.x * 10.0));

		color = texture2D(texture, uv);

		color.r += distortionAmount;
		color.g += distortionAmount;


		color.a -= (hash(uv.x) * distortionAmount);

	}

	if(isDead == 1) {
		int kernelSize = 100;  // Adjust this for different levels of blur

		// BLUR screen
		vec2 texelSize = 1.0 / textureSize(texture, 0);
		vec4 color = vec4(0.0);

		// Horizontal pass
		for (int i = -kernelSize; i <= kernelSize; ++i)
		{
			vec2 offset = vec2(float(i), 0.0) * texelSize;
			float weight = 1.0 / float(2 * kernelSize + 1);
			color += texture2D(texture, texCoords + offset) * weight;
		}

		// Vertical pass
		for (int i = -kernelSize; i <= kernelSize; ++i)
		{
			vec2 offset = vec2(0.0, float(i)) * texelSize;
			float weight = 1.0 / float(2 * kernelSize + 1);
			color += texture2D(texture, texCoords + offset) * weight;
		}
	}

	gl_FragColor = color;
}
