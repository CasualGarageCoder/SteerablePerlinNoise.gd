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

#include <glm/glm.hpp>

class SteerablePerlinNoise : public Noise {
	GDCLASS(SteerablePerlinNoise, Noise);
	OBJ_SAVE_TYPE(SteerablePerlinNoise);

public:
	SteerablePerlinNoise();

	virtual ~SteerablePerlinNoise() {}

	_FORCE_INLINE_ int get_seed() const;
	_FORCE_INLINE_ void set_seed(int s);

	_FORCE_INLINE_ Vector3 get_frequency() const;
	_FORCE_INLINE_ void set_frequency(Vector3 f);

	_FORCE_INLINE_ Vector3 get_offset() const;
	_FORCE_INLINE_ void set_offset(Vector3 f);

	_FORCE_INLINE_ Vector3 get_scale() const;
	_FORCE_INLINE_ void set_scale(Vector3 s);

	_FORCE_INLINE_ real_t get_anisotropy_strength() const;
	_FORCE_INLINE_ void set_anisotropy_strength(real_t a);

	_FORCE_INLINE_ Vector2 get_anisotropy_vector_scale() const;
	_FORCE_INLINE_ void set_anisotropy_vector_scale(Vector2 v);

	_FORCE_INLINE_ Ref<Noise> get_anisotropy_map() const;
	_FORCE_INLINE_ void set_anisotropy_map(Ref<Noise> t);

	_FORCE_INLINE_ real_t get_octave_bias() const;
	_FORCE_INLINE_ void set_octave_bias(real_t b);

	_FORCE_INLINE_ int get_octaves() const;
	_FORCE_INLINE_ void set_octaves(int c);

	_FORCE_INLINE_ int get_noise_order() const;
	_FORCE_INLINE_ void set_noise_order(int o);

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
	_FORCE_INLINE_ void _changed() { emit_changed(); }

	_FORCE_INLINE_ static real_t random3(glm::vec3);

	static glm::vec3 random33(glm::vec3);

	static glm::vec3 rsphere(glm::vec3);

	_FORCE_INLINE_ static real_t random2(glm::vec2);

	_FORCE_INLINE_ static glm::vec2 rand_dir(glm::vec2);

	_FORCE_INLINE_ static real_t smootherstep(real_t);

	_FORCE_INLINE_ static real_t interp(real_t);

	_FORCE_INLINE_ static glm::vec3 interp(glm::vec3);

	_FORCE_INLINE_ static real_t fitrange(real_t, real_t, real_t, real_t, real_t);

	_FORCE_INLINE_ static glm::vec2 fitrange(glm::vec2, real_t, real_t, real_t, real_t);

	_FORCE_INLINE_ static glm::vec3 fitrange(glm::vec3, real_t, real_t, real_t, real_t);

	_FORCE_INLINE_ static glm::mat2 outerprod(glm::vec2, glm::vec2);

	_FORCE_INLINE_ static glm::mat3 outerprod(glm::vec3, glm::vec3);

	_FORCE_INLINE_ static glm::mat3 vec_projector(glm::vec3);

	static glm::mat3 dihedral(glm::vec3, glm::vec3);

	static glm::mat3 make_projection(glm::vec3);

	glm::vec2 image_grad(glm::vec2, real_t) const;

	glm::mat3 generate_metric(glm::vec3) const;

	glm::mat2 generate_metric(glm::vec2) const;

	real_t steerable_perlin(glm::vec3, glm::mat3) const;

	real_t steerable_perlin_projected(glm::vec3, glm::mat2, glm::mat3) const;

	real_t fbm(glm::vec3, glm::mat3) const;

	real_t fbm_artifact_free(glm::vec3, glm::mat3) const;

	real_t fbm_projected(glm::vec3, glm::mat2, glm::mat3) const;

	real_t aniso_perlin(glm::vec2, glm::mat2) const;

private:
	int seed;

	glm::vec3 frequency;

	glm::vec3 offset;

	glm::vec3 scale;

	real_t anisotropy_strength;

	glm::vec2 anisotropy_vector_scale;

	Ref<Noise> anisotropy_map;

	real_t octave_bias;

	int octaves;

	int noise_order;

	real_t eigen_value_sum;
};