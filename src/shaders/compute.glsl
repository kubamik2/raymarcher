#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;

uniform vec3 camera_position;
uniform vec3 camera_direction;

struct Shape {
    int type;
    vec4 a;
    vec4 b;
};

struct Sphere {
    vec3 position;
    float radius;
};

struct Box {
    vec3 position;
    vec3 b;
};

layout(std430, binding = 3) buffer shape_data {
    Shape shapes[];
};

const vec3 UP = vec3(0.0, 1.0, 0.0);
const float FOV = radians(60.0);

const vec3 SPHERE_POS = vec3(0.0, 0.0, 2.0);
const float SPHERE_RADIUS = 1.0;

const vec3 LIGHT = vec3(0.0, 10.0, 0.0);

const float MIN_LIMIT = 0.001;
const float MAX_LIMIT = 1000.0;
const int ITERS = 80;

const int SHAPE_SPHERE = 0;
const int SHAPE_BOX = 1;

// sphere signed distance function
float sdf_sphere(vec3 sphere_pos, float radius, vec3 pos) {
    return length(sphere_pos - pos) - radius;
}

float sdf_box(vec3 box_pos, vec3 b, vec3 pos) {
    vec3 q = abs(box_pos - pos) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// rounded box signed distance function
float sdf_round_box(vec3 position, vec3 b, float r) {
    vec3 q = abs(position) - b + r;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - r;
}

float smooth_min(float a, float b, float k) {
    float h = max(k - abs(a - b), 0) / k;
    return min(a, b) - h * h * h * k * 1 / 6.0;
}

Sphere sphere(Shape shape) {
    Sphere sphere;
    sphere.position = shape.a.xyz;
    sphere.radius = shape.a.w;
    return sphere;
}

Box box(Shape shape) {
    Box box;
    box.position = shape.a.xyz;
    box.b = vec3(shape.a.w, shape.b.xy);
    return box;
}

// get distance to shape at position
float get_shape_dist(int index, vec3 position) {
    Shape shape = shapes[index];
    switch (shape.type) {
    case SHAPE_SPHERE:
        Sphere sphere = sphere(shape);
        return sdf_sphere(sphere.position, sphere.radius, position);
    case SHAPE_BOX:
        Box box = box(shape);
        return sdf_box(box.position, box.b, position);
    }
}

// get distance to any shape at position
float get_dist(vec3 position) {
    if (shapes.length() == 0) {
        return MAX_LIMIT;
    }

    float dist = get_shape_dist(0, position);
    for (int i = 1; i < shapes.length(); i++) {
        dist = min(get_shape_dist(i, position), dist);
    }

    return dist;
}

float raymarch(vec3 start_pos, vec3 dir) {
    float traveled = 0.0;
    for (int i = 0; i < ITERS; i++) {
        vec3 pos = start_pos + traveled * dir;
        float dist = get_dist(pos);
        traveled += dist;
        if (dist >= MAX_LIMIT) {
            return -1.0;
        }
        if (dist < MIN_LIMIT) {
            return traveled;
        }
    }
    return traveled;
}

// calculate shape surface normal vector using finite differences method
vec3 normal(vec3 pos) {
    float epsilon = 0.0001;
    return normalize(vec3(
        get_dist(pos + vec3(epsilon, 0, 0)) - get_dist(pos - vec3(epsilon, 0, 0)),
        get_dist(pos + vec3(0, epsilon, 0)) - get_dist(pos - vec3(0, epsilon, 0)),
        get_dist(pos + vec3(0, 0, epsilon)) - get_dist(pos - vec3(0, 0, epsilon))
    ));
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

    vec3 dir = normalize(screen_pixel_coords.x * crossed + screen_pixel_coords.y * up + camera_direction);

    float col_dist = raymarch(camera_position, dir);
    vec3 col_pos = camera_position + dir * col_dist;
    if (col_dist >= 0.0) {
        vec3 n = normal(col_pos);
        vec3 light_dir = normalize(LIGHT - col_pos);
        float light = max(0.0, dot(n, light_dir));
        pixel = vec4(vec3(light), 1.0);
    }

    imageStore(screen, pixel_coords, pixel);
}
