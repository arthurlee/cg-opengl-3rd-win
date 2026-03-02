#version 430

layout(location=0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 p_matrix;
// uniform mat4 mv_matrix;
uniform float tf;	// time factor for animation and placement of cubes

out vec4 varyingColor;

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    );
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(rad), -sin(rad), 0.0,
        1.0, sin(rad), cos(rad), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    return mat4(        
        cos(rad), 0.0, sin(rad), 0.0, 
        0.0, 1.0, 0.0, 0.0,
        -sin(rad), 0.0, cos(rad), 0.0, 
        0.0, 0.0, 0.0, 1.0
    );
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    return mat4(        
        cos(rad), sin(rad), 0.0, 0.0, 
        -sin(rad), cos(rad), 0.0, 0.0, 
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// builds and returns a scale matrix
mat4 buildScale(float x, float y, float z) {
    return mat4(        
        x, 0.0, 0.0, 0.0, 
        0.0, y, 0.0, 0.0, 
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}


void main(void) {

	float x = gl_InstanceID + tf;	// value based on time factor, but different for each cube instance
	
	// these are the x, y, and z components for the translation, below
	float a = sin(203.0 * x / 8000.0) * 403.0;
	float b = sin(301.0 * x / 8000.0) * 401.0;
	float c = sin(400.0 * x / 8000.0) * 405.0;
	mat4 localTrans = buildTranslate(a, b, c);

	// build the rotation and translation to be applied to this cube's model matrix
	mat4 localRotX = buildRotateX(1.75 * x);
	mat4 localRotY = buildRotateY(1.75 * x);
	mat4 localRotZ = buildRotateZ(1.75 * x);

	// build the model matrix and then the model-view matrix
	mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * newM_matrix;
	
	gl_Position = p_matrix * mv_matrix * vec4(position, 1.0);
	varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}