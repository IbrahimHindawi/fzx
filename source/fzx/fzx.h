#include "vector.h"
#include "core.h"

typedef struct {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    f32 mass;
} fzxParticle;

fzxParticle fzxParticleCreate(f32 x, f32 y, f32 mass) {
    fzxParticle result = {0};
    result.position.x = x;
    result.position.y = y;
    result.velocity.x = 0.f;
    result.velocity.y = 0.f;
    result.acceleration.x = 0.f;
    result.acceleration.y = 0.f;
    result.mass = mass;
    return result;
}

