uniform sampler2D texture;

const float treshold = 0.3;

void main() {
    vec2 texCoords = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, texCoords);

    float brightness = (color.r + color.b + color.g) / 3.0;

    if(brightness >= treshold) {
        color = color;
    } else {
        color = vec4(0,0,0,0);
    }
   
    gl_FragColor = gl_Color * color;
}