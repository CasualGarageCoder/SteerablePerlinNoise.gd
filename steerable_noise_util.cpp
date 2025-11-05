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

#include "steerable_noise_util.h"
#include "core/math/math_funcs.h"

#ifdef REAL_T_IS_DOUBLE
const Vector3 RANDOM3_DOT = Vector3(64.25375463, 23.27536534, 86.29678483);
#define D 59482.7542
#define THREES .33333
#else
const Vector3 RANDOM3_DOT = Vector3(64.25375463f, 23.27536534f, 86.29678483f);
#define D 59482.7542f
#define THREES .33333f
#endif

real_t SteerablePerlin::random3(Vector3 pos) {
	return Math::fract(Math::sin(pos.dot(RANDOM3_DOT)) * D);
}

Vector3 SteerablePerlin::random33(Vector3 pos) {
	Vector3 p = pos;
	Vector3 offset = Vector3(.01, .01, .01);
	p += offset;
	real_t x = random3(p);
	p += offset;
	real_t y = random3(p);
	p += offset;
	real_t z = random3(p);

	return Vector3(x, y, z);
}

Vector3 SteerablePerlin::rsphere(Vector3 p) {
	Vector3 vals = random33(p);

	real_t u = vals.x, v = vals.y;
	real_t theta = u * 2. * Math::PI;
	real_t phi = Math::acos(2. * v - 1.);
	real_t r = Math::pow(vals.z, THREES);
	real_t sin_theta = Math::sin(theta);
	real_t cos_theta = Math::cos(theta);
	real_t sin_phi = Math::sin(phi);
	real_t cos_phi = Math::cos(phi);
	real_t x = r * sin_phi * cos_theta;
	real_t y = r * sin_phi * sin_theta;
	real_t z = r * cos_phi;
	return Vector3(x, y, z);
}
