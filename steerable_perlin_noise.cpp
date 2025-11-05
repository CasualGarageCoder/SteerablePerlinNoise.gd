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

Vector3 SteerablePerlinNoise::get_frequency() const {
	return frequency;
}
void SteerablePerlinNoise::set_frequency(Vector3 &f) {
	frequency = f;
}

Vector3 SteerablePerlinNoise::get_offset() const {
	return offset;
}
void SteerablePerlinNoise::set_offset(Vector3 &f) {
	offset = f;
}

real_t SteerablePerlinNoise::get_octave_bias() const {
	return octave_bias;
}
void SteerablePerlinNoise::set_octave_bias(real_t b) {
	octave_bias = b;
}

int SteerablePerlinNoise::get_octaves() const {
	return octaves;
}
void SteerablePerlinNoise::set_octaves(int c) {
	if (c < 0) {
		WARN_PRINT("Invalid octave number. Set to 0.");
	}
	octaves = MAX(c, 0);
}

real_t SteerablePerlinNoise::get_eigen_value_sum() const {
	return eigen_value_sum;
}
void SteerablePerlinNoise::set_eigen_value_sum(real_t s) {
	eigen_value_sum = s;
}

real_t SteerablePerlinNoise::get_noise_1d(real_t p_x) const {
	return 0.;
}

real_t SteerablePerlinNoise::get_noise_2dv(Vector2 p_v) const {
	return get_noise_2d(p_v.x, p_v.y);
}

real_t SteerablePerlinNoise::get_noise_2d(real_t p_x, real_t p_y) const {
	return 0.;
}

real_t SteerablePerlinNoise::get_noise_3dv(Vector3 p_v) const {
	return get_noise_3d(p_v.x, p_v.y, p_v.z);
}

real_t SteerablePerlinNoise::get_noise_3d(real_t p_x, real_t p_y, real_t p_z) const {
	return 0.;
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

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "frequency"), "set_frequency", "get_frequency");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "offset"), "set_offset", "get_offset");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eigen_value_sum"), "set_eigen_value_sum", "get_eigen_value_sum");

	ADD_GROUP("Fractal", "fractal_");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "octave_bias"), "set_octave_bias", "get_octave_bias");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "octaves"), "set_octaves", "get_octaves");
}