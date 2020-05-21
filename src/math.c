/////////////////////////        MATH         //////////////////////////
// ALGEBRA
#define D2R 0.01745329251994    // degrees to radians
#define R2D 57.295779513082321  // radians to degrees
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
	_a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
	_a > _b ? _b : _a; })
float modulusContext (float complete, int modulus) {
	double wholePart;
	double fracPart = modf(complete, &wholePart);
	return ( ((int)wholePart) % modulus ) + fracPart;
}
// MATRICES
unsigned char mat4Inverse(const float m[16], float inverse[16]){
	float inv[16], det;
	int i;
	inv[0] = m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] + m[9]*m[7]*m[14] +m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] = -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] = m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] = -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] = m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] = -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] = m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] = m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15] + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] = -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15] - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] = m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15] + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0] *m[6]*m[13] + m[4]*m[1]*m[14] - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] = -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11] - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] = m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11] + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11] - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
	inv[15] = m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10] + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
	det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if (det == 0)
		return 0;
	det = 1.0 / det;
	for (i = 0; i < 16; i++)
		inverse[i] = inv[i]*det;
	return 1;
}
void mat4x4MultUnique(const float *a, const float *b, float *result){
	// this is counting on a or b != result   eg: cannot do mat4x4MultUnique(a, b, a);
	result[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	result[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	result[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	result[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];
	result[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	result[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	result[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	result[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];
	result[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	result[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	result[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	result[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];
	result[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	result[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	result[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	result[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}
void mat4x4Mult(const float *a, const float *b, float *result) {
	float c[16];
	mat4x4MultUnique(a, b, c);
	memcpy(result, c, sizeof(float)*16);
}
void mat3x3MultUnique(const float *a, const float *b, float *result){
	// this is counting on a or b != result   eg: cannot do mat3x3MultUnique(a, b, a);
	result[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
	result[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
	result[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];
	result[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	result[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	result[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
	result[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	result[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	result[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
}
void mat3x3Mult(const float *a, const float *b, float *result) {
	float c[9];
	mat3x3MultUnique(a, b, c);
	memcpy(result, c, sizeof(float)*9);
}
void mat3ToMat4(float *i, float *result){
	result[0] = i[0];   result[1] = i[1];   result[2] = i[2];   result[3] = 0.0f;
	result[4] = i[3];   result[5] = i[4];   result[6] = i[5];   result[7] = 0.0f;
	result[8] = i[6];   result[9] = i[7];   result[10] = i[8];  result[11] = 0.0f;
	result[12] = 0.0f;  result[13] = 0.0f;  result[14] = 0.0f;  result[15] = 1.0f;
}
void mat4Transpose(float *m) {
	float t[16];
	t[0] = m[0];   t[1] = m[4];   t[2] = m[8];   t[3] = m[12];
	t[4] = m[1];   t[5] = m[5];   t[6] = m[9];   t[7] = m[13];
	t[8] = m[2];   t[9] = m[6];   t[10] = m[10]; t[11] = m[14];
	t[12] = m[3];  t[13] = m[7];  t[14] = m[11]; t[15] = m[15];
	memcpy(m, t, sizeof(float)*16);
}
void mat3Transpose(float *m) {
	float t[9];
	t[0] = m[0];  t[1] = m[3];  t[2] = m[6];
	t[3] = m[1];  t[4] = m[4];  t[5] = m[7];
	t[6] = m[2];  t[7] = m[5];  t[8] = m[8];
	memcpy(m, t, sizeof(float)*9);
}
void makeMat3XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;
	m[3] = 0;	m[4] = cos(angle);	m[5] = -sin(angle);
	m[6] = 0;	m[7] = sin(angle);	m[8] = cos(angle);
}
void makeMat3YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);
	m[3] = 0;			m[4] = 1;	m[5] = 0;
	m[6] = sin(angle);	m[7] = 0;	m[8] = cos(angle);
}
void makeMat3ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;
	m[3] = sin(angle);	m[4] = cos(angle);	m[5] = 0;
	m[6] = 0;			m[7] = 0;			m[8] = 1;
}
void makeMat4XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;			m[3] = 0;
	m[4] = 0;	m[5] = cos(angle);	m[6] = -sin(angle);	m[7] = 0;
	m[8] = 0;	m[9] = sin(angle);	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;	m[13] = 0;			m[14] = 0;			m[15] = 1;
}
void makeMat4YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);	m[3] = 0;
	m[4] = 0;			m[5] = 1;	m[6] = 0;			m[7] = 0;
	m[8] = sin(angle);	m[9] = 0;	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;			m[13] = 0;	m[14] = 0;			m[15] = 1;
}
void makeMat4ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;	m[3] = 0;
	m[4] = sin(angle);	m[5] = cos(angle);	m[6] = 0;	m[7] = 0;
	m[8] = 0;			m[9] = 0;			m[10] = 1;	m[11] = 0;
	m[12] = 0;			m[13] = 0;			m[14] = 0;	m[15] = 1;
}
void setMat3FromMat4(float *m3, float *m4){
	m3[0] = m4[0]; m3[1] = m4[1]; m3[2] = m4[2];
	m3[3] = m4[4]; m3[4] = m4[5]; m3[5] = m4[6];
	m3[6] = m4[8]; m3[7] = m4[9]; m3[8] = m4[10];
}
void setMat4FromMat3(float *m4, float *m3){
	m4[0] = m3[0]; m4[1] = m3[1]; m4[2] = m3[2]; m4[3] = 0;
	m4[4] = m3[3]; m4[5] = m3[4]; m4[6] = m3[5]; m4[7] = 0;
	m4[8] = m3[6]; m4[9] = m3[7]; m4[10] = m3[8]; m4[11] = 0;
	m4[12] = 0; m4[13] = 0; m4[14] = 0; m4[15] = 1;
}
void setMat4Identity(float *m){
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}
// MATRICES & VECTORS
void mat4Vec4Mult(const float m[16], const float v[4], float result[4]){
	result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3];
	result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7] * v[3];
	result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11] * v[3];
	result[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];
}
void mat3Vec3Mult(const float m[9], const float v[3], float result[3]){
	result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
	result[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
	result[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}
// VECTORS
void vec3Cross(const float u[3], const float v[3], float result[3]){
	result[0] = u[1]*v[2] - u[2]*v[1];
	result[1] = u[2]*v[0] - u[0]*v[2];
	result[2] = u[0]*v[1] - u[1]*v[0];
}
void vec3Normalize(float vec[3]){
	float m = sqrt( powf(vec[0],2) + powf(vec[1],2) + powf(vec[2],2) );
	vec[0] /= m;
	vec[1] /= m;
	vec[2] /= m;
}
