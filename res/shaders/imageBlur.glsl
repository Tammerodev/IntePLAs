precision lowp float;

uniform sampler2D textureSampler;

void main()
{   
    int kernelSize = 0;
    vec2 TexCoord = gl_TexCoord[0].xy;

    vec2 texelSize = 1.0 / textureSize(textureSampler, 0);
    vec4 result = vec4(0.0);

    // Horizontal pass
    float horizontalWeightSum = 0.0;
    for (int i = -kernelSize; i <= kernelSize; ++i)
    {
        vec2 offset = vec2(float(i), 0.0) * texelSize;
        float weight = 1.0 / float(2 * kernelSize + 1);
        result += texture2D(textureSampler, TexCoord + offset) * weight;
        horizontalWeightSum += weight;
    }

    // Vertical pass
    float verticalWeightSum = 0.0;
    for (int i = -kernelSize; i <= kernelSize; ++i)
    {
        vec2 offset = vec2(0.0, float(i)) * texelSize;
        float weight = 1.0 / float(2 * kernelSize + 1);
        result += texture2D(textureSampler, TexCoord + offset) * weight;
        verticalWeightSum += weight;
    }
    result /= verticalWeightSum * 2 - 0.14; // Normalize the result for vertical pass

    gl_FragColor = result;
}
