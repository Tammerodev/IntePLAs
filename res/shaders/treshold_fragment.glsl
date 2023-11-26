uniform sampler2D texture;

const float treshold = 0.8;

void main() {
    vec2 texCoords = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, texCoords);

    float brightness = (color.r + color.b + color.g + color.a) / 4.0;

    color -= vec4(treshold, treshold, treshold, treshold);
    color *= 4.0;

    gl_FragColor = gl_Color * color;
}