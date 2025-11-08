/*
MIT License

Copyright (c) 2025 Casual Garage Coder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "steerable_perlin_noise.h"
#include "core/error/error_macros.h"

SteerablePerlinNoise::SteerablePerlinNoise() :
		seed(0),
		frequency(1., 1., 1.),
		offset(0., 0., 0.),
		scale(1., 1., 1.),
		anisotropy_strength(1.),
		anisotropy_vector_scale(1., 1.),
		octave_bias(.67),
		octaves(6),
		noise_order(0),
		eigen_value_sum(4.) {}

int SteerablePerlinNoise::get_seed() const {
	return seed;
}

void SteerablePerlinNoise::set_seed(int s) {
	seed = s % 16777216;
	emit_changed();
}

Vector3 SteerablePerlinNoise::get_frequency() const {
	return Vector3(frequency.x, frequency.y, frequency.z);
}
void SteerablePerlinNoise::set_frequency(Vector3 f) {
	frequency = glm::vec3(f.x, f.y, f.z);
	emit_changed();
}

Vector3 SteerablePerlinNoise::get_offset() const {
	return Vector3(offset.x, offset.y, offset.z);
}
void SteerablePerlinNoise::set_offset(Vector3 f) {
	offset = glm::vec3(f.x, f.y, f.z);
	emit_changed();
}

Vector3 SteerablePerlinNoise::get_scale() const {
	return Vector3(scale.x, scale.y, scale.z);
}
void SteerablePerlinNoise::set_scale(Vector3 f) {
	scale = glm::vec3(f.x, f.y, f.z);
	emit_changed();
}

real_t SteerablePerlinNoise::get_octave_bias() const {
	return octave_bias;
}
void SteerablePerlinNoise::set_octave_bias(real_t b) {
	octave_bias = b;
	emit_changed();
}

real_t SteerablePerlinNoise::get_anisotropy_strength() const {
	return anisotropy_strength;
}
void SteerablePerlinNoise::set_anisotropy_strength(real_t a) {
	anisotropy_strength = a;
	emit_changed();
}

Vector2 SteerablePerlinNoise::get_anisotropy_vector_scale() const {
	return Vector2(anisotropy_vector_scale.x, anisotropy_vector_scale.y);
}
void SteerablePerlinNoise::set_anisotropy_vector_scale(Vector2 v) {
	anisotropy_vector_scale = glm::vec2(v.x, v.y);
	emit_changed();
}

Ref<Noise> SteerablePerlinNoise::get_anisotropy_map() const {
	return anisotropy_map;
}
void SteerablePerlinNoise::set_anisotropy_map(Ref<Noise> t) {
	if (anisotropy_map.is_valid()) {
		anisotropy_map->disconnect_changed(callable_mp(this, &SteerablePerlinNoise::_changed));
	}
	anisotropy_map = t;
	if (anisotropy_map.is_valid()) {
		anisotropy_map->connect_changed(callable_mp(this, &SteerablePerlinNoise::_changed));
	}
	emit_changed();
}

int SteerablePerlinNoise::get_octaves() const {
	return octaves;
}
void SteerablePerlinNoise::set_octaves(int c) {
	if (c < 0) {
		WARN_PRINT("Invalid octave number. Set to 0.");
	}
	octaves = MAX(c, 0);
	emit_changed();
}

int SteerablePerlinNoise::get_noise_order() const {
	return noise_order;
}

void SteerablePerlinNoise::set_noise_order(int o) {
	if (o < 0) {
		WARN_PRINT("Noise order must be positive. Set to 0.");
	}
	noise_order = MAX(o, 0);
	emit_changed();
}

real_t SteerablePerlinNoise::get_eigen_value_sum() const {
	return eigen_value_sum;
}
void SteerablePerlinNoise::set_eigen_value_sum(real_t s) {
	eigen_value_sum = s;
	emit_changed();
}

real_t SteerablePerlinNoise::get_noise_1d(real_t p_x) const {
	return get_noise_2d(p_x, 0.);
}

real_t SteerablePerlinNoise::get_noise_2dv(Vector2 p_v) const {
	glm::vec2 pv(p_v.x, p_v.y);
	glm::vec2 p = pv;
	glm::vec2 s2(scale.x, scale.y);
	p /= s2;
	p -= .5;
	p *= 2.;
	glm::vec2 aniso_dir;
	if (anisotropy_map.is_valid()) {
		real_t h = .05;
		aniso_dir = image_grad(pv * anisotropy_vector_scale, h);
	} else {
		aniso_dir = glm::vec2(p.y, -p.x);
	}

	glm::mat2 metric = generate_metric(aniso_dir * anisotropy_vector_scale);
	metric = anisotropy_strength * metric + glm::mat2(1.) * (1.f - anisotropy_strength);
	real_t out_val = 0.;
	glm::vec2 f2(frequency.x, frequency.y);
	for (int i = 0; i < octaves; ++i) {
		out_val += pow(octave_bias, static_cast<real_t>(i)) * aniso_perlin(glm::pow(2.0f, static_cast<real_t>(i)) * p * f2, metric);
	}
	return out_val;
}

real_t SteerablePerlinNoise::get_noise_2d(real_t p_x, real_t p_y) const {
	return get_noise_2dv(Vector2(p_x, p_y));
}

real_t SteerablePerlinNoise::get_noise_3dv(Vector3 p_v) const {
	glm::vec3 anisotropy_dir(p_v.z, 0., -p_v.x);
	glm::mat3 x = generate_metric(anisotropy_dir);
	return fbm_artifact_free(glm::vec3(p_v.x, p_v.y, p_v.z), x);
}

real_t SteerablePerlinNoise::get_noise_3d(real_t p_x, real_t p_y, real_t p_z) const {
	return get_noise_3dv(Vector3(p_x, p_y, p_z));
}

void SteerablePerlinNoise::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_seed"), &SteerablePerlinNoise::get_seed);
	ClassDB::bind_method(D_METHOD("set_seed", "s"), &SteerablePerlinNoise::set_seed);

	ClassDB::bind_method(D_METHOD("get_frequency"), &SteerablePerlinNoise::get_frequency);
	ClassDB::bind_method(D_METHOD("set_frequency", "f"), &SteerablePerlinNoise::set_frequency);

	ClassDB::bind_method(D_METHOD("get_offset"), &SteerablePerlinNoise::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "o"), &SteerablePerlinNoise::set_offset);

	ClassDB::bind_method(D_METHOD("get_scale"), &SteerablePerlinNoise::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "s"), &SteerablePerlinNoise::set_scale);

	ClassDB::bind_method(D_METHOD("get_octave_bias"), &SteerablePerlinNoise::get_octave_bias);
	ClassDB::bind_method(D_METHOD("set_octave_bias", "b"), &SteerablePerlinNoise::set_octave_bias);

	ClassDB::bind_method(D_METHOD("get_anisotropy_strength"), &SteerablePerlinNoise::get_anisotropy_strength);
	ClassDB::bind_method(D_METHOD("set_anisotropy_strength", "a"), &SteerablePerlinNoise::set_anisotropy_strength);

	ClassDB::bind_method(D_METHOD("get_anisotropy_vector_scale"), &SteerablePerlinNoise::get_anisotropy_vector_scale);
	ClassDB::bind_method(D_METHOD("set_anisotropy_vector_scale", "s"), &SteerablePerlinNoise::set_anisotropy_vector_scale);

	ClassDB::bind_method(D_METHOD("get_anisotropy_map"), &SteerablePerlinNoise::get_anisotropy_map);
	ClassDB::bind_method(D_METHOD("set_anisotropy_map", "s"), &SteerablePerlinNoise::set_anisotropy_map);

	ClassDB::bind_method(D_METHOD("get_octaves"), &SteerablePerlinNoise::get_octaves);
	ClassDB::bind_method(D_METHOD("set_octaves", "c"), &SteerablePerlinNoise::set_octaves);

	ClassDB::bind_method(D_METHOD("get_noise_order"), &SteerablePerlinNoise::get_noise_order);
	ClassDB::bind_method(D_METHOD("set_noise_order", "o"), &SteerablePerlinNoise::set_noise_order);

	ClassDB::bind_method(D_METHOD("get_eigen_value_sum"), &SteerablePerlinNoise::get_eigen_value_sum);
	ClassDB::bind_method(D_METHOD("set_eigen_value_sum"), &SteerablePerlinNoise::set_eigen_value_sum);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "frequency", PROPERTY_HINT_RANGE, "0.,16,0.001,or_less,or_greater"), "set_frequency", "get_frequency");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "offset", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_offset", "get_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_scale", "get_scale");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "noise_order"), "set_noise_order", "get_noise_order");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eigen_value_sum"), "set_eigen_value_sum", "get_eigen_value_sum");

	ADD_GROUP("Anisotropy", "anisotropy_");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "anisotropy_strength"), "set_anisotropy_strength", "get_anisotropy_strength");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "anisotropy_vector_scale", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_anisotropy_vector_scale", "get_anisotropy_vector_scale");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "anisotropy_map",
						 PROPERTY_HINT_RESOURCE_TYPE, "Noise"),
			"set_anisotropy_map", "get_anisotropy_map");

	ADD_GROUP("Fractal", "fractal_");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fractal_octave_bias"), "set_octave_bias", "get_octave_bias");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fractal_octaves"), "set_octaves", "get_octaves");
}