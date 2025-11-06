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

SteerablePerlinNoise::SteerablePerlinNoise() :
		frequency(1., 1., 1.),
		offset(0., 0., 0.),
		octave_bias(.67),
		octaves(6),
		eigen_value_sum(4.) {}

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

real_t SteerablePerlinNoise::get_octave_bias() const {
	return octave_bias;
}
void SteerablePerlinNoise::set_octave_bias(real_t b) {
	octave_bias = b;
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
	/*Vector2 p(p_v.x, p_v.y);
	glm::vec3 anisotropy_dir = glm::vec3(p_v.y, 0., -p_v.x);
	glm::mat3 projection = make_projection(glm::normalize(glm::vec3(1., 1., 0.)));
	glm::vec3 m = anisotropy_dir * projection;
	glm::mat2 x = generate_metric(glm::vec2(m.x, m.y));
	real_t c = fbm_projected(glm::vec3(p_v.x, 0., p_v.y), x, projection);
	return c;*/
	return get_noise_3d(p_v.x, 0., p_v.y);
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
	ClassDB::bind_method(D_METHOD("get_frequency"), &SteerablePerlinNoise::get_frequency);
	ClassDB::bind_method(D_METHOD("set_frequency", "f"), &SteerablePerlinNoise::set_frequency);

	ClassDB::bind_method(D_METHOD("get_offset"), &SteerablePerlinNoise::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "o"), &SteerablePerlinNoise::set_offset);

	ClassDB::bind_method(D_METHOD("get_octave_bias"), &SteerablePerlinNoise::get_octave_bias);
	ClassDB::bind_method(D_METHOD("set_octave_bias", "b"), &SteerablePerlinNoise::set_octave_bias);

	ClassDB::bind_method(D_METHOD("get_octaves"), &SteerablePerlinNoise::get_octaves);
	ClassDB::bind_method(D_METHOD("set_octaves", "c"), &SteerablePerlinNoise::set_octaves);

	ClassDB::bind_method(D_METHOD("get_eigen_value_sum"), &SteerablePerlinNoise::get_eigen_value_sum);
	ClassDB::bind_method(D_METHOD("set_eigen_value_sum"), &SteerablePerlinNoise::set_eigen_value_sum);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "frequency", PROPERTY_HINT_RANGE, "0.,16,0.001,or_less,or_greater"), "set_frequency", "get_frequency");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "offset", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_offset", "get_offset");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eigen_value_sum"), "set_eigen_value_sum", "get_eigen_value_sum");

	ADD_GROUP("Fractal", "fractal_");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "octave_bias"), "set_octave_bias", "get_octave_bias");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "octaves"), "set_octaves", "get_octaves");
}