#pragma once
constexpr double pi = 3.14159265358979323846;

namespace math {
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	void angle_vectors_alternative(const vec3_t& angles, vec3_t forward);
	void vector_angles_alternative_2(const vec3_t& forward, vec3_t& angles);
	vec3_t calculate_angle(vec3_t& a, vec3_t& b);
	vec3_t calculate_angle_2(vec3_t a, vec3_t b);
	void sin_cos(float r, float* s, float* c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector( vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors_qangle(QAngle& angles, vec3_t* forward, vec3_t* right, vec3_t* up);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	float random_float(float min, float max);
	int random_int(int min, int max);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	void vector_angles_3(const vec3_t& forward, vec3_t& up, vec3_t& angles);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	float angle_diff(float destAngle, float srcAngle);
	void movement_fix(vec3_t ang, c_usercmd* cmd);
	bool world_to_screen(const vec3_t& origin, vec3_t& screen);
	
};