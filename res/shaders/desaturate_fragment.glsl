// Fragment shader
uniform sampler2D texture;
uniform float time; // Time variable for animation
uniform float distortionAmount; // Adjust distortion effect

uniform float desaturationAmount; // Adjustable amount

uniform int isDead;

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

void main()
{
	float darknessFactor = 0.2;

    vec2 texCoords = gl_TexCoord[0].xy;

    vec4 color = texture2D(texture, texCoords);

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
