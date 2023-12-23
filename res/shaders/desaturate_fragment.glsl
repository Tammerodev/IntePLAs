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
	float darknessFactor = 0.5;

    vec2 texCoords = gl_TexCoord[0].xy;

    vec4 color = texture2D(texture, texCoords);

	if (distortionAmount != 0.0) {
		distortionAmount /= 1000.0;

    	vec2 uv = gl_TexCoord[0].xy;

		float pulse = 0.5 * cos(time * 0.5);

		uv += pulse * distortionAmount * vec2(sin(uv.y * 10.0), cos(uv.x * 10.0));

		color = texture2D(texture, uv);

		color.r += distortionAmount;
		color.g += distortionAmount;


		color.a -= (hash(uv.x) * distortionAmount);

	}
    float brightness = (color.r + color.b + color.g + color.a) / 4.0;

	if(brightness < 0.8) {

		float darkness = 1.0 - desaturationAmount * darknessFactor;

		// Darken the color based on the time of day
		color.rgb *= darkness;
	}

	if(isDead == 1) {
		color.r += abs(sin(time * 0.1) * 10.0);
	}

	gl_FragColor = color;
}