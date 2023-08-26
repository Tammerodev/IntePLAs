
uniform sampler2D textureSampler;
uniform int kernelSize;  // Adjust this for different levels of blur

void main()
{
    vec2 TexCoord = gl_TexCoord[0].xy;

    vec2 texelSize = 1.0 / textureSize(textureSampler, 0);
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0);

    // Horizontal pass
    for (int i = -kernelSize; i <= kernelSize; ++i)
    {
        vec2 offset = vec2(float(i), 0.0) * texelSize;
        result += texture2D(textureSampler, TexCoord + offset);
    }

    // Vertical pass
    for (int i = -kernelSize; i <= kernelSize; ++i)
    {
        vec2 offset = vec2(0.0, float(i)) * texelSize;
        result += texture2D(textureSampler, TexCoord + offset);
    }

    result /= float(2 * kernelSize + 1);
    gl_FragColor = result;
}
