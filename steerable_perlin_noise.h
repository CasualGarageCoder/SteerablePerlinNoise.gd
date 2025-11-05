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
#pragma once

#include "core/object/object.h"
#include "core/typedefs.h"
#include "modules/noise/noise.h"

class SteerablePerlinNoise : public Noise {
	GDCLASS(SteerablePerlinNoise, Noise);
	OBJ_SAVE_TYPE(SteerablePerlinNoise);

public:
	SteerablePerlinNoise();

	virtual ~SteerablePerlinNoise();

	_FORCE_INLINE_ Vector3 get_frequency() const;
	_FORCE_INLINE_ void set_frequency(Vector3 &f);

	_FORCE_INLINE_ Vector3 get_offset() const;
	_FORCE_INLINE_ void set_offset(Vector3 &f);

	_FORCE_INLINE_ real_t get_octave_bias() const;
	_FORCE_INLINE_ void set_octave_bias(real_t b);

	_FORCE_INLINE_ int get_octaves() const;
	_FORCE_INLINE_ void set_octaves(int c);

	_FORCE_INLINE_ real_t get_eigen_value_sum() const;
	_FORCE_INLINE_ void set_eigen_value_sum(real_t s);

	real_t get_noise_1d(real_t p_x) const override;

	real_t get_noise_2dv(Vector2 p_v) const override;
	real_t get_noise_2d(real_t p_x, real_t p_y) const override;

	real_t get_noise_3dv(Vector3 p_v) const override;
	real_t get_noise_3d(real_t p_x, real_t p_y, real_t p_z) const override;

protected:
	static void _bind_methods();

private:
	Vector3 frequency;

	Vector3 offset;

	real_t octave_bias;

	int octaves;

	real_t eigen_value_sum;
};