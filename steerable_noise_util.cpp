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

#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"
#include "core/typedefs.h"
#include "steerable_perlin_noise.h"

#ifdef REAL_T_IS_DOUBLE
const glm::vec3 RANDOM3_DOT(64.25375463, 23.27536534, 86.29678483);
const glm::vec2 RANDOM2_DOT(12.9898, 78.233);
#define D 59482.7542
#define D_P 43758.5453123
#define THREES .33333
#else
const glm::vec3 RANDOM3_DOT(64.25375463f, 23.27536534f, 86.29678483f);
const glm::vec2 RANDOM2_DOT(12.9898f, 78.233f);
#define D 59482.7542f
#define D_P 43758.5453123f
#define THREES .33333f
#endif

real_t SteerablePerlinNoise::random3(glm::vec3 pos) {
	return glm::fract(Math::sin(dot(pos, RANDOM3_DOT)) * D);
}

glm::vec3 SteerablePerlinNoise::random33(glm::vec3 pos) {
	return glm::vec3(random3(pos + .01f), random3(pos + .02f), random3(pos + .03f));
}

glm::vec3 SteerablePerlinNoise::rsphere(glm::vec3 p) {
	glm::vec3 vals = random33(p);

	real_t u = vals.x;
	real_t v = vals.y;
	real_t theta = u * 2.0f * Math::PI;
	real_t phi = Math::acos(2.0f * v - 1.0f);
	real_t r = std::pow(vals.z, .33333);
	real_t sinTheta = Math::sin(theta);
	real_t cosTheta = Math::cos(theta);
	real_t sinPhi = Math::sin(phi);
	real_t cosPhi = Math::cos(phi);
	real_t x = r * sinPhi * cosTheta;
	real_t y = r * sinPhi * sinTheta;
	real_t z = r * cosPhi;
	return glm::vec3(x, y, z);
}

real_t SteerablePerlinNoise::random2(glm::vec2 st) {
	return glm::fract(glm::sin(glm::dot(st, RANDOM2_DOT))) * D_P;
}

glm::vec2 SteerablePerlinNoise::rand_dir(glm::vec2 p) {
	real_t r = random2(p) * Math::PI * 2.;
	return glm::vec2(glm::cos(r), glm::sin(r));
}

real_t SteerablePerlinNoise::smootherstep(real_t x) {
#ifdef REAL_T_IS_DOUBLE
	return CLAMP(6. * (x * x * x * x * x) - 15. * (x * x * x * x) + 10. * (x * x * x), 0., 1.);
#else
	return CLAMP(6.0f * (x * x * x * x * x) - 15.0f * (x * x * x * x) + 10.0f * (x * x * x), 0.0f, 1.0f);
#endif
}

real_t SteerablePerlinNoise::interp(real_t u) {
	return 1. - smootherstep(Math::abs(u));
}

glm::vec3 SteerablePerlinNoise::interp(glm::vec3 u) {
	return glm::vec3(interp(u.x), interp(u.y), interp(u.z));
}

real_t SteerablePerlinNoise::fitrange(real_t x, real_t in_low, real_t in_high, real_t out_low, real_t out_high) {
	real_t u = CLAMP(x, in_low, in_high);
	return ((out_high - out_low) * (u - in_low)) / (in_high - in_low) + out_low;
}

glm::vec2 SteerablePerlinNoise::fitrange(glm::vec2 x, real_t in_low, real_t in_high, real_t out_low, real_t out_high) {
	return glm::vec2(fitrange(x.x, in_low, in_high, out_low, out_high),
			fitrange(x.y, in_low, in_high, out_low, out_high));
}

glm::vec3 SteerablePerlinNoise::fitrange(glm::vec3 x, real_t in_low, real_t in_high, real_t out_low, real_t out_high) {
	return glm::vec3(fitrange(x.x, in_low, in_high, out_low, out_high),
			fitrange(x.y, in_low, in_high, out_low, out_high),
			fitrange(x.z, in_low, in_high, out_low, out_high));
}

glm::mat2 SteerablePerlinNoise::outerprod(glm::vec2 x, glm::vec2 y) {
	return glm::mat2(glm::vec2(x.x * y.x, x.y * y.x),
			glm::vec2(x.x * y.y, x.y * y.y));
}

glm::mat3 SteerablePerlinNoise::outerprod(glm::vec3 x, glm::vec3 y) {
	return glm::mat3(glm::vec3(x.x * y.x, x.y * y.x, x.z * y.x),
			glm::vec3(x.x * y.y, x.y * y.y, x.z * y.y),
			glm::vec3(x.x * y.z, x.y * y.z, x.z * y.z));
}

glm::mat3 SteerablePerlinNoise::vec_projector(glm::vec3 n) {
	return glm::mat3(1.0) - outerprod(n, n);
}

glm::mat3 SteerablePerlinNoise::dihedral(glm::vec3 a, glm::vec3 b) {
	glm::vec3 v = glm::cross(a, b);
	real_t c = glm::dot(a, b);
	glm::mat3 v_prime;
	glm::mat3 I = glm::mat3(1.0);
	v_prime[0] = glm::vec3(0.f, -v.z, v.y);
	v_prime[1] = glm::vec3(v.z, 0, -v.x);
	v_prime[2] = glm::vec3(-v.y, v.x, 0);

	real_t scale = 1.f / (1.f + c);
	glm::mat3 v_prime_2 = v_prime * v_prime;

	return I + v_prime + v_prime_2 * scale;
}

glm::mat3 SteerablePerlinNoise::make_projection(glm::vec3 n) {
	return vec_projector(n) * dihedral(n, glm::vec3(0., 0., 1.));
}

glm::mat3 SteerablePerlinNoise::generate_metric(glm::vec3 p) const {
	glm::vec3 x = normalize(p);

	//hack to avoid division by zero when extracting the eigenvectors :)
	//without this i think there'd need to be a bunch of if statements for all the different cases
	//like if x = (.5,.5,0) or x = (0,.5,.5) or x = (0,0,1), etc.
	x = glm::max(glm::abs(x), glm::vec3(1e-5)) * (glm::mix(glm::vec3(1.0f), glm::vec3(-1.0f), glm::greaterThan(x, glm::vec3(0.0))));

	glm::vec3 evals = glm::vec3(0, 0, 1.0);
	glm::vec3 evec0, evec1, evec2;
	//eigenvectors of outerproduct(x, x) have a closed form solution:

	evec0 = normalize(glm::vec3(-x.z / x.x, 0, 1));
	evec1 = normalize(glm::vec3(-x.y / x.x, 1.0, 0.0));
	evec2 = normalize(glm::vec3(x.x / x.z, x.y / x.z, 1.0));

	float k = .51;
	int dimensions = 3;
	float denom = 1. / float(dimensions - 1);
	glm::vec3 mapped_evals = fitrange(evals, 0.0, 1.0, (eigen_value_sum - k) * denom - 1e-4, k);
	//vec3 mapped_evals = vec3(1.5, 1.5, 1.);
	return mapped_evals.x * outerprod(evec0, evec0) +
			mapped_evals.y * outerprod(evec1, evec1) +
			mapped_evals.z * outerprod(evec2, evec2);
}

glm::mat2 SteerablePerlinNoise::generate_metric(glm::vec2 p) const {
	glm::vec2 x = glm::normalize(p);

	//hack to avoid division by zero when extracting the eigenvectors :)
	//without this i think there'd need to be a bunch of if statements for all the different cases
	//like if x = (.5,.5,0) or x = (0,.5,.5) or x = (0,0,1), etc.
	x = glm::max(abs(x), glm::vec2(1e-5)) * (glm::mix(glm::vec2(1.0f), glm::vec2(-1.0f), greaterThan(x, glm::vec2(0.0))));

	glm::vec2 evals(0, 1.0);
	glm::vec2 evec0, evec1;
	//eigenvectors of outerproduct(x, x) have a closed form solution:

	evec0 = glm::normalize(glm::vec2(-x.y / x.x, 1));
	evec1 = glm::normalize(glm::vec2(x.x / x.y, 1));

	float k = .51;
	glm::vec2 mapped_evals = fitrange(evals, 0.0, 1.0, (eigen_value_sum - k) - 1e-4, k);
	//vec3 mapped_evals = vec3(1.5, 1.5, 1.);
	return mapped_evals.x * outerprod(evec0, evec0) +
			mapped_evals.y * outerprod(evec1, evec1);
}

real_t SteerablePerlinNoise::steerable_perlin(glm::vec3 pos, glm::mat3 metric) const {
	glm::vec3 noise_p = glm::floor(pos);
	glm::vec3 noise_f = pos - noise_p; //frac(pos);

	real_t out_val = 0.0;

	//perlin weights
	glm::vec3 blend = interp(noise_f);

	//we opt to use a weighted average instead of lerps.
	//if we remove the anisotropy from the dot product and the interpolation
	//the weighted average results in the same output as standard perlin noise.
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= 1; j++) {
			for (int k = 0; k <= 1; k++) {
				glm::vec3 o = glm::vec3(float(i), float(j), float(k));

				glm::vec3 g = noise_p + o;
				glm::vec3 r = rsphere(g);
				glm::vec3 v = (o - noise_f);

				glm::vec3 metric_v = metric * v;

				//regular perlin is dot(r, v)
				//applying the metric to v, adds one level of anisotropy
				real_t d = glm::dot(r, metric_v);

				glm::vec3 wv = glm::abs(o - blend);

				//we get another level of anisotropy by introducing anisotropic weights to the algorithm
				real_t w = wv.x * wv.y * wv.z * interp(glm::dot(v, metric_v));
				out_val += d * w;
			}
		}
	}

	return out_val;
}

real_t SteerablePerlinNoise::steerable_perlin_projected(glm::vec3 pos, glm::mat2 metric, glm::mat3 projection) const {
	glm::vec3 noise_p = glm::floor(pos);
	glm::vec3 noise_f = pos - noise_p; //frac(pos);

	real_t out_val = 0.0;

	//perlin weights
	glm::vec3 blend = interp(noise_f);

	//we opt to use a weighted average instead of lerps.
	//if we remove the anisotropy from the dot product and the interpolation
	//the weighted average results in the same output as standard perlin noise.
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= 1; j++) {
			for (int k = 0; k <= 1; k++) {
				glm::vec3 o = glm::vec3(float(i), float(j), float(k));

				glm::vec3 g = noise_p + o;
				glm::vec3 r3 = rsphere(g) * projection;
				glm::vec3 v3 = (o - noise_f) * projection;

				glm::vec2 r(r3.x, r3.y);
				glm::vec2 v(v3.x, v3.y);

				glm::vec2 metric_v = metric * v;

				//regular perlin is dot(r, v)
				//applying the metric to v, adds one level of anisotropy
				real_t d = glm::dot(r, metric_v);

				glm::vec3 wv = glm::abs(o - blend);

				//we get another level of anisotropy by introducing anisotropic weights to the algorithm
				real_t w = wv.x * wv.y * wv.z * interp(glm::dot(v, metric_v));

				out_val += d * w;
			}
		}
	}

	return out_val;
}

glm::vec2 SteerablePerlinNoise::image_grad(glm::vec2 p, float h) const {
	if (anisotropy_map.is_valid()) {
		real_t c_l = anisotropy_map->get_noise_2d(p.x - h, p.y);
		real_t c_r = anisotropy_map->get_noise_2d(p.x + h, p.y);
		real_t c_u = anisotropy_map->get_noise_2d(p.x, p.y - h);
		real_t c_d = anisotropy_map->get_noise_2d(p.x, p.y + h);

		float grad_x = c_l - c_r;
		float grad_y = c_u - c_d;
		return glm::vec2(grad_x, grad_y) / (2.f * h);
	} else {
		WARN_PRINT_ONCE("Invalid anisotropy map.");
		return glm::vec2(1., 0.);
	}
}

real_t SteerablePerlinNoise::fbm(glm::vec3 p, glm::mat3 metric) const {
	real_t out_val = 0.0;
	glm::vec3 shift_offset = offset + glm::vec3(seed);

	for (int i = 0; i < octaves; ++i) {
		out_val += glm::pow(octave_bias, static_cast<real_t>(i)) * steerable_perlin(glm::pow(2.0f, static_cast<real_t>(i)) * (p + shift_offset) * frequency, metric);
	}

	return out_val;
}

real_t SteerablePerlinNoise::fbm_artifact_free(glm::vec3 p, glm::mat3 metric) const {
	real_t out_val = 0.0;
	glm::vec3 shift_offset = offset + glm::vec3(seed);

	for (int i = 0; i < octaves; i++) {
		//since the weights are always heighest at the .5 position, combine two noises at the same octave to remove artifacts.
		out_val += glm::pow(octave_bias, static_cast<real_t>(i)) * (steerable_perlin(glm::pow(2.0f, static_cast<real_t>(i)) * (p + shift_offset) * frequency, metric) + steerable_perlin(glm::pow(2.0f, static_cast<real_t>(i)) * ((p + shift_offset + glm::vec3(.5)) * frequency), metric)) * .5;
	}

	return out_val;
}

real_t SteerablePerlinNoise::fbm_projected(glm::vec3 p, glm::mat2 metric, glm::mat3 projection) const {
	real_t out_val = 0.0;
	glm::vec3 shift_offset = offset + glm::vec3(seed);

	for (int i = 0; i < octaves; i++) {
		out_val += glm::pow(octave_bias, static_cast<real_t>(i)) * steerable_perlin_projected(glm::pow(2.0f, static_cast<real_t>(i)) * (p + shift_offset) * frequency, metric, projection);
	}

	return out_val;
}

real_t SteerablePerlinNoise::aniso_perlin(glm::vec2 p, glm::mat2 metric) const {
	glm::vec2 noise_p = floor(p);
	glm::vec2 noise_f = fract(p);
	real_t out_val = 0.0;
	int start = -(noise_order);
	int end = noise_order + 1;
	float scale = 2. / float(abs(start) + end + 1);

	for (int i = start; i <= end; i++) {
		for (int j = start; j <= end; j++) {
			glm::vec2 o = glm::vec2(i, j);
			glm::vec2 g = noise_p + o;
			glm::vec2 r = rand_dir(g); // random vector
			glm::vec2 v = o - noise_f; //dir to corner
			glm::vec2 metric_v = v * metric;
			float d = dot(r, metric_v); //inner product
			float w = interp(v.x * scale) * interp(v.y * scale);
			w *= interp(dot(v, metric_v)); //aniso weights
			out_val += d * w;
		}
	}
	return out_val;
}
