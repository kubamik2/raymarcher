#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

uniform vec3 camera_position;
uniform vec3 camera_direction;

const vec3 UP = vec3(0.0, 1.0, 0.0);
const float FOV = radians(60.0);

const vec3 SPHERE_POS = vec3(0.0, 0.0, -2.0);
const float SPHERE_RADIUS = 1.0;

const vec3 LIGHT = vec3(0.0, 10.0, -2.0);

const float MIN_LIMIT = 0.001;
const float EDGE_LIMIT = 10.0 * MIN_LIMIT;
const float MAX_LIMIT = 1000.0;
const int ITERS = 80;

float sdf(vec3 position) {
    return length(SPHERE_POS - position) - SPHERE_RADIUS;
}

float sdf_round_box(vec3 position, vec3 b, float r) {
    vec3 q = abs(position) - b + r;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - r;
}

vec3 normal(vec3 position, vec3 b, float r) {
    vec3 small_step = vec3(0.001, 0.0, 0.0);
    float gradient_x = sdf_round_box(position + small_step.xyy, b, r) - sdf_round_box(position - small_step.xyy, b, r);
    float gradient_y = sdf_round_box(position + small_step.yxy, b, r) - sdf_round_box(position - small_step.yxy, b, r);
    float gradient_z = sdf_round_box(position + small_step.yyx, b, r) - sdf_round_box(position - small_step.yyx, b, r);

    return normalize(vec3(gradient_x, gradient_y, gradient_z));
}

void main() {
    ivec2 screen_size = imageSize(screen);
    float aspect_ratio = float(screen_size.x) / float(screen_size.y);
    vec2 viewport_size = vec2(2.0*tan(0.5*FOV), 2.0*tan(0.5*FOV) / aspect_ratio);

    vec3 crossed = normalize(cross(camera_direction, UP));
    vec3 up = normalize(cross(camera_direction, crossed));

    vec4 pixel = vec4(0.075, 0.133, 0.173, 1.0);
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    vec2 screen_pixel_coords = vec2(
        float(pixel_coords.x - screen_size.x / 2) / float(screen_size.x / 2) * viewport_size.x,
        -float(pixel_coords.y - screen_size.y / 2) / float(screen_size.y / 2) * viewport_size.y
    );

    vec3 d = normalize(screen_pixel_coords.x * crossed + screen_pixel_coords.y * up + camera_direction);

    float traveled = 0.0;
    float min_dist = MAX_LIMIT;
    vec3 p;
    for (int i = 0; i < ITERS; i++) {
        vec3 pos = camera_position + traveled * d;
        float dist = sdf_round_box(pos, vec3(1.0, 2.0, 1.0), 0.1);
        traveled += dist;

        if (dist < MIN_LIMIT) {
            vec3 normal = normal(pos, vec3(1, 2, 1), 0.2);
            vec3 light_dir = normalize(LIGHT - pos);
            float light = max(0.0, dot(normal, light_dir));
            pixel = vec4(vec3(light), 1.0);
            break;
        }
        if (dist >= MAX_LIMIT) {
            break;
        }
    }

    // if (min_dist > MIN_LIMIT && min_dist <= EDGE_LIMIT) {
    //
    // }

    imageStore(screen, pixel_coords, pixel);
}
