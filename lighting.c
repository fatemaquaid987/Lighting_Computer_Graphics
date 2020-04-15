
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "matrix.h"
#include "initShader.h"
#include <math.h>
#include <time.h>
#include<stdio.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
int num_vertices;
GLuint ctm_location, isShadow_location,light_location, lightchange_location,proj_location, modelview_location;
GLuint Ambientproduct_location, Diffuseproduct_location, Specularproduct_location;
GLuint attenuationc_location, attenuationl_location, attenuationq_location, shininess_location, islight_location;
mat4 twincube_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 modelview = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball1_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball2_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball3_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball4_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball5_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 ball6_ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
mat4 proj = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
GLfloat eye_x, eye_y, eye_z, at_x, at_y, at_z, light_x, light_y, light_z, lx, ly, lz, a_c, a_l, a_q, shininess;
vec4 light_source, light_change, productAmbient, productDiffuse, productSpecular;
vec4 light_ambient = { 0.2, 0.2, 0.2, 1.0 };
vec4 light_diffuse = { 1.0, 1.0, 1.0, 1.0 };
vec4 light_specular = { 1.0, 1.0, 1.0, 1.0 };
GLfloat degree1,degree2,degree3,degree4,radius, theta, phi = 0;
material materials[7] = {
	{ { 0.0, 0.7, 0.0, 1.0 },{ 0.0, 0.7, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 5 },
	{ { 0.4, 0.0, 0.0, 1.0 },{ 0.8, 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 50 },
	{ { 1.0, 0.4, 0.0, 1.0 },{ 1.0, 0.4, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 50 },
	{ { 0.0, 1.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 50 },
	{ { 0.0, 0.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 50 },
	{ { 1.0, 1.0, 0.0, 1.0 },{ 1.0, 1.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 50 },
	{ { 5, 5, 5.0, 1.0 },{ 1.0, 0.5, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 }, 0 } };
vec4 v4(float x, float y, float z, float w)
{
vec4 result = { x, y, z, w };
return result;
}

mat4 matTrans(mat4 m)                        //Performs transformation of a matrix, rows become columns, columns become rows
{
mat4 result = {
{ m.x.x, m.y.x, m.z.x, m.w.x },
{ m.x.y, m.y.y, m.z.y, m.w.y },
{ m.x.z, m.y.z, m.z.z, m.w.z },
{ m.x.w, m.y.w, m.z.w, m.w.w }
};

return result;
}

vec4 unit(vec4 v)
{
GLfloat mag = magnitude(v);
//printf("%f", mag);
if (mag <= 0.0001)
{
mag = 0.0001;
}
vec4 result = scalarMult(1 / mag, v);
return result;

}
float magnitude(vec4 u)
{
GLfloat u_mag = sqrt(u.x*u.x + u.y*u.y + u.z*u.z + u.w*u.w);
return u_mag;
}

vec4 scalarMult(float s, vec4 v)             //Multiplies scalar and 4x1 vector
{
vec4 result;
result.x = s*v.x;
result.y = s*v.y;
result.z = s*v.z;
result.w = s*v.w;
return result;

}
vec4 vecAdd(vec4 u, vec4 v)                 //Adds 2 4x1 vectors
{
vec4 result;
result.x = u.x + v.x;
result.y = u.y + v.y;
result.z = u.z + v.z;
result.w = u.w + v.w;
return result;
}
vec4 vecSub(vec4 u, vec4 v)                // Subtracts 2 4x1 vectors
{
	vec4 result;
	result.x = u.x - v.x;
	result.y = u.y - v.y;
	result.z = u.z - v.z;
	result.w = u.w - v.w;
	return result;
}
vec4 matVecMult(mat4 m, vec4 v)            //multiplies 4x4 matrix with a 4x1 vector resulting in a 4x1 vector.
{
vec4 result;
result = vecAdd(vecAdd(scalarMult(v.x, m.x), scalarMult(v.y, m.y)), vecAdd(scalarMult(v.z, m.z), scalarMult(v.w, m.w)));
return result;
}

mat4 matMult(mat4 m1, mat4 m2)              //Multiplies 2 4x4 matrices resulting in a 4x4 matrix
{
mat4 result = { matVecMult(m1, m2.x), matVecMult(m1, m2.y), matVecMult(m1, m2.z), matVecMult(m1, m2.w) };
return result;

}

GLfloat vecDot(vec4 u, vec4 v)               //Performs 4x1 vector dot multiplication , returns a  floating point number
{
GLfloat d = (u.x*v.x) + (u.y*v.y) + (u.z*v.z) + (u.w*v.w);
return d;
}

vec4 vecCross(vec4 u, vec4 v)             //Performs 4x1 vector cross multiplication, returns a 4x1 vector
{
vec4 result;
result.x = u.y*v.z - u.z*v.y;
result.y = u.z*v.x - u.x*v.z;
result.z = u.x*v.y - u.y*v.x;
result.w = 0;
return result;
}

mat4 translate(GLfloat x, GLfloat y, GLfloat z) //translation matrix
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
result.w.x = x;
result.w.y = y;
result.w.z = z;
return result;
}


mat4 Scale(GLfloat x, GLfloat y, GLfloat z) //scale matrix
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
result.x.x = x;
result.y.y = y;
result.z.z = z;
return result;
}

mat4 rotate_z(GLfloat theta)  //rotation matrix about z-axis
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
result.x.x = cos(theta);
result.y.x = -sin(theta);
result.x.y = sin(theta);
result.y.y = cos(theta);
return result;
}

mat4 rotate_x(GLfloat theta) //rotation matrix about x-axis
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };

result.y.y = cos(theta);
result.z.y = -sin(theta);
result.y.z = sin(theta);
result.z.z = cos(theta);

return result;
}

mat4 rotate_y(GLfloat theta) //rotation matrix about y axis
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
result.x.x = cos(theta);
result.z.x = sin(theta);
result.x.z = -sin(theta);
result.z.z = cos(theta);

return result;
}

mat4 rotate_arb(GLfloat theta, vec4 v) //rotation about arbitrary axis
{
mat4 result = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
GLfloat d = sqrt((v.y*v.y) + (v.z*v.z));
if (d == 0)                      // if y and z components are zero, rotate about x-axis
{

return rotate_x(theta);

}
if (d <= 0.0001)          //to avoid division by zero
{
d = 0.0001;
}
mat4 rot_x = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } }; //rotation matrix to bring vector on x-z plane

rot_x.y.y = v.z / d;
rot_x.z.y = -v.y / d;
rot_x.y.z = v.y / d;
rot_x.z.z = v.z / d;


mat4 rot_y = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } }; //rotation matrix to align vector with z-axis
rot_y.x.x = d;
rot_y.z.x = -v.x;
rot_y.x.z = v.x;
rot_y.z.z = d;


result = matMult(matTrans(rot_x), matMult(matTrans(rot_y), matMult(rotate_z(theta), matMult(rot_y, rot_x)))); //final rotation matrix

return result;
}

mat4 sMult4(float s, mat4 m)              //Multiplies a scalar and 4x4 matrix
{
mat4 result;

result.x.x = s*m.x.x;
result.x.y = s*m.x.y;
result.x.z = s*m.x.z;
result.x.w = s*m.x.w;

result.y.x = s*m.y.x;
result.y.y = s*m.y.y;
result.y.z = s*m.y.z;
result.y.w = s*m.y.w;

result.z.x = s*m.z.x;
result.z.y = s*m.z.y;
result.z.z = s*m.z.z;
result.z.w = s*m.z.w;

result.w.x = s*m.w.x;
result.w.y = s*m.w.y;
result.w.z = s*m.w.z;
result.w.w = s*m.w.w;

return result;

}

float matdet(float m[9])                   // Calculates determinant of a 3x3 matrix, returns a float
{
return m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7] - m[6] * m[4] * m[2] - m[7] * m[5] * m[0] - m[8] * m[3] * m[1];
}

mat4 matMinor(mat4 m)                    // returns matrix minor of a 4x4 matrix.
{

mat3 a = { m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w };
mat3 b = { m.y.x, m.z.x, m.w.x, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w };
mat3 c = { m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.w, m.z.w, m.w.w };
mat3 d = { m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z };

mat3 e = { m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w };
mat3 f = { m.x.x, m.z.x, m.w.x, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w };
mat3 g = { m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.w, m.z.w, m.w.w };
mat3 h = { m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z };

mat3 i = { m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w };
mat3 j = { m.x.x, m.y.x, m.w.x, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w };
mat3 k = { m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.w, m.y.w, m.w.w };
mat3 l = { m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z };

mat3 n = { m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w };
mat3 o = { m.x.x, m.y.x, m.z.x, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w };
mat3 p = { m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.w, m.y.w, m.z.w };
mat3 q = { m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z };

mat4 r = { { matdet(a), matdet(b), matdet(c), matdet(d) }, //calculates 3x3 determinant for each entry in a 4x4 matrix.
{ matdet(e), matdet(f), matdet(g), matdet(h) },
{ matdet(i), matdet(j), matdet(k), matdet(l) },
{ matdet(n), matdet(o), matdet(p), matdet(q) } };

return r;

}
mat4 matCof(mat4 m)                     // turns a matrix minor into cofactor matrix
{
mat4 r = { { m.x.x, -m.x.y, m.x.z, -m.x.w },
{ -m.y.x, m.y.y, -m.y.z, m.y.w },
{ m.z.x, -m.z.y, m.z.z, -m.z.w },
{ -m.w.x, m.w.y, -m.w.z, m.w.w } };

return r;
}

float matDet4(mat4 m)                  //Calculates determinant of a 4x4 matrix. Returns a float
{
mat4 minor = matMinor(m);
float d = m.x.x* minor.x.x - m.y.x* minor.y.x + m.z.x*minor.z.x - m.w.x*minor.w.x;
return d;
}

mat4 matInv(mat4 m)                   //Returns inverse of a matrix if determinant is not zero.
{
if (matDet4(m) != 0)
{
mat4 r = sMult4((1 / matDet4(m)), matTrans(matCof(matMinor(m))));
return r;
}
else printf("Matrix is singular, Inverse not possible!");
return;
}

vec4 product(vec4 u, vec4 v)
{
	vec4 result;
	result.x = u.x * v.x;
	result.y = u.y * v.y;
	result.z = u.z * v.z;
	result.w = u.w * v.w;
	return result;
}
mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat atx, GLfloat aty, GLfloat atz, GLfloat upx, GLfloat upy, GLfloat upz)
{
vec4 vpn = v4(eyex - atx, eyey - aty, eyez - atz, 0.0);
vec4 n = unit(vpn);

vec4 vup = v4(upx, upy, upz, 0.0);
vec4 u = vecCross(vup, n);
u = unit(u);

vec4 v = vecCross(n, u);
v = unit(v);

vec4 e = v4(eyex, eyey, eyez, 1);

mat4 modelView = { u,v,n,e };
//modelView = matTrans(modelView);
modelView.x.w = 0;
modelView.y.w = 0;
modelView.z.w = 0;

modelView = matInv(modelView);


return modelView;

}
mat4 projection(GLfloat Near, GLfloat Far, GLfloat top, GLfloat bottom, GLfloat left, GLfloat right)
{
	mat4 result = { { (-2 * Near) / (right - left), 0,0,0 },{ 0, (-2 * Near) / (top - bottom),0,0 },
	{ (right + left) / (right - left), (top + bottom) / (top - bottom), (Near + Far) / (Far - Near), -1 },{ 0, 0, (-2 * Far*Near) / (Far - Near),0 } };
	return result;

}

//single cube vertices
vec4 cubeVertices[36] = {
//front
{-0.5,-0.5,0.5,1.0},
{0.5,-0.5, 0.5,1.0 },
{0.5, 0.5, 0.5,1.0},

{0.5, 0.5, 0.5,1.0},
{-0.5, 0.5, 0.5,1.0},
{-0.5, -0.5, 0.5,1.0},

//right
{0.5, -0.5, 0.5,1},
{0.5, -0.5, -0.5,1},
{0.5, 0.5, -0.5,1},

{0.5, 0.5, -0.5,1},
{0.5, 0.5, 0.5,1},
{0.5, -0.5, 0.5,1},

//back
{-0.5, -0.5, -0.5,1},
{0.5, -0.5, -0.5,1},
{0.5, 0.5, -0.5,1},

{0.5, 0.5, -0.5,1},
{-0.5, 0.5, -0.5,1},
{-0.5, -0.5, -0.5,1},

//left
{-0.5, -0.5, 0.5,1},
{-0.5, -0.5, -0.5,1},
{-0.5, 0.5, -0.5,1},

{-0.5, 0.5, -0.5,1},
{-0.5, 0.5, 0.5,1},
{-0.5, -0.5, 0.5,1},

//top
{-0.5, 0.5, 0.5,1},
{0.5, 0.5, 0.5,1},
{0.5, 0.5, -0.5,1},

{0.5, 0.5, -0.5,1},
{-0.5, 0.5, -0.5,1},
{-0.5, 0.5, 0.5,1},

//bottom
{-0.5, -0.5, 0.5,1},
{0.5, -0.5, 0.5,1},
{0.5, -0.5, -0.5,1},

{0.5, -0.5, -0.5,1},
{-0.5, -0.5, -0.5,1},
{-0.5, -0.5, 0.5,1},
};

vec4* sphere()                             //creates a spiral
{
	float theta, theta_r, theta10_r, angle, y_s;
	int index = 0;
	y_s = 0;
	//*num_vertices = 108 * 72;                                 //number of vertices in spiral
	vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * (108) * 72); //array of vertices

																//makes one ring of spiral
	for (theta = 0; theta <= 170; theta = theta + 10)     //loops over and makes triangles
	{
		theta_r = theta * 3.14 / 180.0;                  //angle
		theta10_r = (theta + 10) * 3.14 / 180.0;          //angle + 10

		vertices[index] = v4(sin(theta_r), 0.1 + y_s, cos(theta_r), 1.0);   //vertex 0
		vertices[index + 1] = v4(sin(theta_r), 0 - y_s, cos(theta_r), 1.0); //vertex 1

		vertices[index + 5] = v4(sin(theta_r), 0.1 + y_s, cos(theta_r), 1.0);//vertex 4
		if (theta <= 80)
		{
			y_s = y_s + 0.008;
		}
		else
		{
			y_s = y_s - 0.008;
		}
		vertices[index + 4] = v4(sin(theta10_r), 0.1 + y_s, cos(theta10_r), 1.0);//vertex 5
		vertices[index + 2] = v4(sin(theta10_r), 0 - y_s, cos(theta10_r), 1.0);//vertex 2
		vertices[index + 3] = v4(sin(theta10_r), 0 - y_s, cos(theta10_r), 1.0);   //vertex 3

		index += 6;
	}


	//makes other rings by performing suitable rotation , one on top of the other
	int angle_5 = 0;

	for (int j = 1; j < (72); j++)
	{

		angle_5 += 5;
		for (int i = 0; i < 108; i++)
		{
			float ang = angle_5 * 3.14 / 180.0;
			vertices[i + (108 * j)] = matVecMult(rotate_z(-ang), vertices[i]);

		}

	}

	
	return vertices;
}



void init(void)
{
GLuint program = initShader("vshader2.glsl", "fshader2.glsl");
glUseProgram(program);

num_vertices =2*( 36+ (108*72)); //number of vertices for cube and sphere and shadows
vec4*sphere_vertices = sphere();
vec4 vertices[2 * (36 + (108 * 72))];
vec4 normals[2 * (36 + (108 * 72))];
//cube 
for (int i = 0; i < 36; i++)
{
vertices[i] = matVecMult(matMult(translate(0.0, -0.1, 0.0), Scale(20, 0.2,20)), cubeVertices[i]);
}
//sphere
for (int i = 36; i < (36+ (108 * 72)); i++)
{
	vertices[i] = matVecMult(matMult(translate(0.0, 0.0, 0.0), Scale(0.5, 0.5, 0.5)),sphere_vertices[i]);
	
}
int index = 0;
for (int i = 0; i < 36/3; i++)
{
	vec4 p1 = vecSub(vertices[index + 1], vertices[index]);
	vec4 p2 = vecSub(vertices[index + 2], vertices[index + 1]);

	vec4 cross_product = vecCross(p1, p2);
	cross_product = unit(cross_product);
	cross_product.w = 0;
	normals[index] = cross_product;
	normals[index + 1] = cross_product;
	normals[index + 2] = cross_product;

	index += 3;
}
for (int i = 0; i < (108 * 72); i++)
{
	vec4 origin = { 0, 0, 0, 1 };
	normals[index] = vertices[index];
	index++;
}


for (int i = 0; i < (36 + (108 * 72)); i++)
{
	//float xs = light_x - (light_y*((light_x - vertices[i].x) / (light_y - vertices[i].y)));
	//float zs = light_z - (light_y*((light_z - vertices[i].z) / (light_y - vertices[i].y)));
	//vec4 v = { xs, 0.0, zs, 1.0 };
	vertices[i+ (36 + (108 * 72))] = vertices[i];
	normals[i + (36 + (108 * 72))] = v4(0.0, 0.0, 0.0, 1.0);

}


GLuint vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);

GLuint buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, vertices);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, normals);

GLuint vPosition = glGetAttribLocation(program, "vPosition");
glEnableVertexAttribArray(vPosition);
glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

GLuint vNormal = glGetAttribLocation(program, "vNormal");
glEnableVertexAttribArray(vNormal);
glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * num_vertices));

proj_location = glGetUniformLocation(program, "projection");
printf("%d\n", proj_location);
modelview_location = glGetUniformLocation(program, "modelview");
printf("%d\n", modelview_location);
ctm_location = glGetUniformLocation(program, "ctm");
printf("%d\n", ctm_location);
isShadow_location = glGetUniformLocation(program, "isShadow");
printf("%d\n", isShadow_location);
light_location = glGetUniformLocation(program, "light_s");
printf("%d\n", light_location);
Ambientproduct_location = glGetUniformLocation(program, "AmbientProduct");
printf("%d\n", Ambientproduct_location);
Diffuseproduct_location = glGetUniformLocation(program, "DiffuseProduct");
printf("%d\n", Diffuseproduct_location);
Specularproduct_location = glGetUniformLocation(program, "SpecularProduct");
printf("%d\n", Specularproduct_location);
attenuationc_location = glGetUniformLocation(program, "AttenuationConstant");
printf("%d\n", attenuationc_location);
attenuationl_location = glGetUniformLocation(program, "AttenuationLinear");
printf("%d\n", attenuationl_location);
attenuationq_location = glGetUniformLocation(program, "AttenuationQuadratic");
printf("%d\n", attenuationq_location);
shininess_location = glGetUniformLocation(program, "shininess");
printf("%d\n", shininess_location);
islight_location = glGetUniformLocation(program, "islight");
printf("%d\n", islight_location);
lightchange_location = glGetUniformLocation(program, "light_change");
printf("%d\n", lightchange_location);
glEnable(GL_DEPTH_TEST);
glClearColor(0.0, 0.0, 0.0, 1.0);
glDepthRange(1, 0);
}

void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glPolygonMode(GL_FRONT, GL_FILL);
glPolygonMode(GL_BACK, GL_FILL);
glUniform1i(isShadow_location, 0);
glUniform1i(islight_location, 0);
glUniformMatrix4fv(proj_location, 1, GL_FALSE, (GLfloat *)&proj);
glUniformMatrix4fv(modelview_location, 1, GL_FALSE, (GLfloat *)&modelview);
glUniform4fv(light_location, 1, (GLfloat *)&light_source);
glUniform4fv(lightchange_location, 1, (GLfloat *)&light_change);
a_c = 10;
a_l = 5;
a_q = 1;

glUniform1fv(attenuationc_location, 1, (GLfloat *)&a_c);
glUniform1fv(attenuationc_location, 1, (GLfloat *)&a_l);
glUniform1fv(attenuationc_location, 1, (GLfloat *)&a_q);


productAmbient = product(light_ambient, materials[0].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
productDiffuse = product(light_diffuse, materials[0].reflect_diffuse);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
productSpecular = product(light_specular, materials[0].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[0].shininess);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&twincube_ctm);
glDrawArrays(GL_TRIANGLES, 0, 36);

productAmbient = product(light_ambient, materials[1].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
productDiffuse = product(light_diffuse, materials[1].reflect_diffuse);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
productSpecular = product(light_specular, materials[1].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[1].shininess);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball1_ctm);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(isShadow_location, 1);
glDrawArrays(GL_TRIANGLES, 72 + (108 * 72), (108 * 72));

glUniform1i(isShadow_location, 0);
productAmbient = product(light_ambient, materials[2].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
productDiffuse = product(light_diffuse, materials[2].reflect_diffuse);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
productSpecular = product(light_specular, materials[2].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[2].shininess);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball2_ctm);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(isShadow_location, 1);
glDrawArrays(GL_TRIANGLES, 72 + (108 * 72), (108 * 72));

glUniform1i(isShadow_location, 0);
productAmbient = product(light_ambient, materials[3].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
productDiffuse = product(light_diffuse, materials[3].reflect_diffuse);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
productSpecular = product(light_specular, materials[3].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[3].shininess);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball3_ctm);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(isShadow_location, 1);
glDrawArrays(GL_TRIANGLES, 72 + (108 * 72), (108 * 72));

glUniform1i(isShadow_location, 0);
productAmbient = product(light_ambient, materials[4].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
productDiffuse = product(light_diffuse, materials[4].reflect_diffuse);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
productSpecular = product(light_specular, materials[4].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[4].shininess);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball4_ctm);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(isShadow_location, 1);
glDrawArrays(GL_TRIANGLES, 72 + (108 * 72), (108 * 72));

glUniform1i(isShadow_location, 0);
productAmbient = product(light_ambient, materials[5].reflect_ambient);
glUniform4fv(Ambientproduct_location, 1, (GLfloat *)&productAmbient);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball5_ctm);
productDiffuse = product(light_diffuse, materials[5].reflect_diffuse);
productSpecular = product(light_specular, materials[5].reflect_specular);
glUniform4fv(Specularproduct_location, 1, (GLfloat *)&productSpecular);
glUniform4fv(Diffuseproduct_location, 1, (GLfloat *)&productDiffuse);
glUniform1fv(shininess_location, 1, (GLfloat *)&materials[5].shininess);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(isShadow_location, 1);
glDrawArrays(GL_TRIANGLES, 72 + (108 * 72), (108 * 72));

glUniform1i(isShadow_location, 0);
glUniform1i(islight_location, 1);
glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball6_ctm);
glDrawArrays(GL_TRIANGLES, 36, (108 * 72));
glUniform1i(islight_location, 0);
glUniform1i(isShadow_location, 0);
glutSwapBuffers();
}



void idle()
{
	degree1 += 0.005*3.14 / 180;
	degree2 += 0.007*3.14 / 180;
	degree3 += 0.01*3.14 / 180;
	degree4 += 0.003*3.14 / 180;
	light_source= v4(light_x, light_y, light_z, 1.0);
	light_change = v4(lx, ly, lz, 1.0);
	eye_x = radius*cos(phi);
	eye_y = radius*sin(theta);
	eye_z = radius*cos(theta) + radius*sin(phi);
	at_x = 0;
	at_y = 0;
	at_z = 0;
	modelview = look_at(eye_x, eye_y, eye_z, at_x, at_y, at_z, 0, 1, 0);
	twincube_ctm =  translate(0.0, -0.1, 0.0);
	proj = projection(-2, -20, 3, -3, -3, 3);
	ball1_ctm = translate(0.0, 0.5, 0.0);
	ball2_ctm = matMult(rotate_y(degree1),matMult(translate(1.0, 0.5, 0.0), Scale(1, 1, 1)));
	ball3_ctm = matMult(rotate_y(degree2), matMult(translate(2.0, 0.5, 0.0), Scale(1, 1, 1)));
	ball4_ctm = matMult(rotate_y(degree3), matMult(translate(3.0, 0.5, 0.0), Scale(1, 1, 1)));
	ball5_ctm = matMult(rotate_y(degree4), matMult(translate(4.0, 0.5, 0.0), Scale(1, 1, 1)));
	ball6_ctm = matMult(translate(light_x+1,light_y,light_z), Scale(0.2, 0.2, 0.2));
	
	glutPostRedisplay();
}

void keys(int key, int mousex, int mousey)
{
	
	if (key == GLUT_KEY_RIGHT)       //if key is 3, scale down
	{
		lx += 0.1;
	
	}
	else if (key == GLUT_KEY_LEFT) //if key is 4, scale up
	{
		lx -= 0.1;
	}
	else if (key == GLUT_KEY_UP)
	{
		ly += 0.1;
	}
	else if(key == GLUT_KEY_DOWN)
	{
		ly -= 0.1;
	}
	
	glutPostRedisplay();
}
void keyboard(unsigned char key, int mousex, int mousey)
{
	if (key == 'q')
		exit(0);
	else if (key == '4')
	{
		lz += 0.1;
	}
	else if(key == '3')
	{
		lz -= 0.1;
	}
	else if(key == 'k')
	{
		phi -= 0.1;
	}
	else if(key == 'h')
	{
		phi += 0.1;
	}
	else if(key == 'u')
	{
		theta += 0.1;
	}
	else if(key == 'n')
	{
		theta -= 0.1;
	}
	else if(key == '8')
	{
		radius += 0.1;
	}
	else if(key == '7')
	{
		radius -= 0.1;
	}
	light_source = v4(light_x, light_y, light_z, 1.0);
	glutPostRedisplay();
}
int main(int argc, char **argv)
{

	radius = 10;
	theta = 90*3.14/180;
	phi = 90 * 3.14 / 180;
	light_x = 0.0;
	light_y = 3.0;
	light_z = 0.0;
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
glEnable(GL_DEPTH_TEST);
glutInitWindowSize(512, 512);
glutInitWindowPosition(100, 100);
glutCreateWindow("cubes");
glewInit();
init();
glutDisplayFunc(display);
glutSpecialFunc(keys);
glutKeyboardFunc(keyboard);
glutIdleFunc(idle);
glutMainLoop();
return 0;
}

