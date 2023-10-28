
precision lowp float;


uniform sampler2D texture;
uniform float time;
uniform vec2 explosion;
uniform vec2 worldpos;
uniform float str_;

float cubicEaseIn(float t, float d) {

    if(t > 1.0) {
        t = 0.0;
    }
    float retVal = 0.0;

    if(t <= 0.01) {
        retVal = 1.0 - pow(1 - t, 3.0);
    } else {
        retVal = cos(((t - 0.01) * 3.14) / 2.0);
    }

    if(retVal <= 0.0) retVal = 0.0;
    return retVal;
}

void main( void ) {
    vec2 position = gl_TexCoord[0].xy + worldpos / 2000.0;

    float pixelate = 160.0;
    
    vec2 exp_pos = explosion / 2000.0;

    position.x = floor(position.x * pixelate);
    position.x /= pixelate;

    position.y = floor(position.y * pixelate);
    position.y /= pixelate;
    
    float dist = floor(
        distance(position,exp_pos) * pixelate 
        );
    dist /= pixelate;

    float intens = cubicEaseIn(time * 4.0, 15.0) * str_ / 1500.0;

    vec4 color = vec4(1.0);
    
    color.r = 1.0 - dist / (intens);
    color.b = color.r - (time * 8.0);
    color.g = color.r - (time * 10.0);

    color *= 2.0;

    color.a = (color.r);
    if(color.a < 0.3) {
        color.a = 0.0;
    }

    color.r = floor(color.r*pixelate);
    color.r /= pixelate;
    color.g = floor(color.g*pixelate);
    color.g /= pixelate;
    color.b = floor(color.b*pixelate);
    color.b /= pixelate;

    gl_FragColor = color;
}