uniform sampler2D texture;
uniform float time;
uniform vec2 explosion;
uniform vec2 worldpos;
uniform float str_;

float custom_explosion_easing(float t, float start_value, float end_value, float duration) {
    // Ensure t is within the valid range [0, duration]
    t = max(0.0, min(t, duration));
    
    // Split the easing into two phases
    float half_duration = duration / 2.0;
    float eased_t;
    
    if (t < half_duration) {
        // Apply exponential easing (quadratic easing in) for the first half
        float normalized_t = t / half_duration;
        eased_t = normalized_t * normalized_t;
    } else {
        // Apply exponential easing (cubic easing out) for the second half
        float normalized_t = (t - half_duration) / half_duration;
        eased_t = 1.0 - pow(1.0 - normalized_t, 3.0);
    }
    
    // Calculate current value using the eased time
    float current_value = start_value + eased_t * (end_value - start_value);
    
    return current_value;
}


float random(float seed) {
    return fract(sin(seed) * 43758.5453);
}

void main( void ) {
    vec2 position = gl_TexCoord[0].xy + worldpos / 2000;

    float pixelate = 100.0;
    
    vec2 exp_pos = explosion / 2000.0;

    position.x = floor(position.x * pixelate);
    position.x /= pixelate;

    position.y = floor(position.y * pixelate);
    position.y /= pixelate;
    
    float dist = floor(
        distance(position,exp_pos) * pixelate 
        );
    dist /= pixelate;

    float intens = custom_explosion_easing(time * 5.0, 5.0, 0.0, 10.0) * str_ / 500.0;

    vec4 color;
    
    color.r = 1.0 - dist / intens;
    color.b = color.r - time * 15.0;
    color.g = color.b;


    color.r = floor(color.r*pixelate);
    color.r /= pixelate;
    color.g = floor(color.g*pixelate);
    color.g /= pixelate;
    color.b = floor(color.b*pixelate);
    color.b /= pixelate;

    color.a = color.r;

    gl_FragColor = color;
}