uniform sampler2D texture;

const float treshold = 5.75;

void main() {
    vec2 texCoords = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, texCoords);

    color -= vec4(treshold, treshold, treshold, treshold);
    color *= 4.0;

    gl_FragColor = gl_Color * color;
}
