#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//Misc. core
float DegToRad(float deg)
{
	return deg*float(M_PI)/180;
}
unsigned WIDTH = 500;
unsigned HEIGHT = 500;

//Viewport
struct Viewport
{
	unsigned x, y;
	unsigned w, h;
	float minz, maxz;

	Viewport()
	{};

	void Update(unsigned i_x, unsigned i_y, unsigned i_w, unsigned i_h, float i_minz, float i_maxz)
	{
		x = i_x; y = i_y;
		w = i_w; h = i_h;
		minz = i_minz; maxz = i_maxz;
	}
};

//Vector
struct Vec3
{
	float x, y, z;

	Vec3()
	{x = 0; y = 0; z = 0;};
	Vec3(float xx)
	{x = xx; y = xx; z = xx;};
	Vec3(float xx, float yy, float zz)
	{x = xx; y = yy; z = zz;};

	float lengthSq()
	{
		return x*x + y*y + z*z;
	}
	float length()
	{
		return sqrt(lengthSq());
	}

	Vec3 operator * (float f)
	{return Vec3(x*f, y*f, z*f);}
	Vec3 operator / (float f)
	{return Vec3(x/f, y/f, z/f);}
	Vec3 operator + (Vec3 v)
	{return Vec3(x + v.x, y + v.y, z + v.z);}
	Vec3 operator - (Vec3 v)
	{return Vec3(x - v.x, y - v.y, z - v.z);}
	Vec3& operator += (Vec3 v)
	{x += v.x, y += v.y, z += v.z; return *this;}
	Vec3& operator -= (Vec3 v)
	{x -= v.x, y -= v.y, z -= v.z; return *this;}
	Vec3 operator - ()
	{return Vec3(-x, -y, -z);}
};
float Vec3Dot(Vec3 *v1, Vec3 *v2)
{
	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}
Vec3 Vec3Cross(Vec3 *v1, Vec3 *v2)
{
	return Vec3(v1->y*v2->z - v1->z*v2->y, v1->z*v2->x - v1->x*v2->z, v1->x*v2->y - v1->y*v2->x);
}
Vec3 Vec3Normalized(Vec3 *v)
{
	float magSq = v->lengthSq();

	return magSq > 0 ? *v/sqrt(magSq) : Vec3();
}
Vec3 StringToVec3(string s)
{
	Vec3 result;
	size_t startpos = 0, endpos;

	endpos = s.find(",", startpos);
	result.x = stof(s.substr(startpos, endpos - startpos));

	startpos = endpos + 1; endpos = s.find(",", startpos);
	result.y = stof(s.substr(startpos, endpos - startpos));

	startpos = endpos + 1; endpos = s.length();
	result.z = stof(s.substr(startpos, endpos - startpos));

	return result;
}

//Matrix
struct Mat4x4
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	Mat4x4()
	{_11 = 0; _12 = 0; _13 = 0; _14 = 0;
	 _21 = 0; _22 = 0; _23 = 0; _24 = 0;
	 _31 = 0; _32 = 0; _33 = 0; _34 = 0;
	 _41 = 0; _42 = 0; _43 = 0; _44 = 0;};
	Mat4x4(float i_all)
	{_11 = i_all; _12 = i_all; _13 = i_all; _14 = i_all;
	 _21 = i_all; _22 = i_all; _23 = i_all; _24 = i_all;
	 _31 = i_all; _32 = i_all; _33 = i_all; _34 = i_all;
	 _41 = i_all; _42 = i_all; _43 = i_all; _44 = i_all;};
	Mat4x4(float i_11, float i_12, float i_13, float i_14,
		   float i_21, float i_22, float i_23, float i_24,
		   float i_31, float i_32, float i_33, float i_34,
		   float i_41, float i_42, float i_43, float i_44)
	{_11 = i_11; _12 = i_12; _13 = i_13; _14 = i_14;
	 _21 = i_21; _22 = i_22; _23 = i_23; _24 = i_24;
	 _31 = i_31; _32 = i_32; _33 = i_33; _34 = i_34;
	 _41 = i_41; _42 = i_42; _43 = i_43; _44 = i_44;};

	Mat4x4 operator * (float f)
	{return Mat4x4(_11*f, _12*f, _13*f, _14*f,
				   _21*f, _22*f, _23*f, _24*f,
				   _31*f, _32*f, _33*f, _34*f,
				   _41*f, _42*f, _43*f, _44*f);}
	Mat4x4 operator / (float f)
	{return Mat4x4(_11/f, _12/f, _13/f, _14/f,
				   _21/f, _22/f, _23/f, _24/f,
				   _31/f, _32/f, _33/f, _34/f,
				   _41/f, _42/f, _43/f, _44/f);}
	Mat4x4 operator + (Mat4x4 m)
	{return Mat4x4(_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
				   _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
				   _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
				   _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44);}
	Mat4x4 operator - (Mat4x4 m)
	{return Mat4x4(_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
				   _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
				   _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
				   _41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44);}
	Mat4x4 operator * (Mat4x4 m)
	{return Mat4x4(_11*m._11 + _12*m._21 + _13*m._31 + _14*m._41, _11*m._12 + _12*m._22 + _13*m._32 + _14*m._42, _11*m._13 + _12*m._23 + _13*m._33 + _14*m._43, _11*m._14 + _12*m._24 + _13*m._34 + _14*m._44,
				   _21*m._11 + _22*m._21 + _23*m._31 + _24*m._41, _21*m._12 + _22*m._22 + _23*m._32 + _24*m._42, _21*m._13 + _22*m._23 + _23*m._33 + _24*m._43, _21*m._14 + _22*m._24 + _23*m._34 + _24*m._44,
				   _31*m._11 + _32*m._21 + _33*m._31 + _34*m._41, _31*m._12 + _32*m._22 + _33*m._32 + _34*m._42, _31*m._13 + _32*m._23 + _33*m._33 + _34*m._43, _31*m._14 + _32*m._24 + _33*m._34 + _34*m._44,
				   _41*m._11 + _42*m._21 + _43*m._31 + _44*m._41, _41*m._12 + _42*m._22 + _43*m._32 + _44*m._42, _41*m._13 + _42*m._23 + _43*m._33 + _44*m._43, _41*m._14 + _42*m._24 + _43*m._34 + _44*m._44);}
	Mat4x4& operator += (Mat4x4 m)
	{_11 += m._11; _12 += m._12; _13 += m._13; _14 += m._14;
	 _21 += m._21; _22 += m._22; _23 += m._23; _24 += m._24;
	 _31 += m._31; _32 += m._32; _33 += m._33; _34 += m._34;
	 _41 += m._41; _42 += m._42; _43 += m._43; _44 += m._44;
	 return *this;}
	Mat4x4& operator -= (Mat4x4 m)
	{_11 -= m._11; _12 -= m._12; _13 -= m._13; _14 -= m._14;
	 _21 -= m._21; _22 -= m._22; _23 -= m._23; _24 -= m._24;
	 _31 -= m._31; _32 -= m._32; _33 -= m._33; _34 -= m._34;
	 _41 -= m._41; _42 -= m._42; _43 -= m._43; _44 -= m._44;
	 return *this;}
	Mat4x4 operator - ()
	{return Mat4x4(-_11, -_12, -_13, -_14,
				   -_21, -_22, -_23, -_24,
				   -_31, -_32, -_33, -_34,
				   -_41, -_42, -_43, -_44);}
};
Mat4x4 Mat4x4Identity()
{
	return Mat4x4(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1);
}
Mat4x4 Mat4x4Translation(Vec3 *v)
{
	return Mat4x4(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  v->x, v->y, v->z, 1);
}
Mat4x4 Mat4x4RotationX(float angle)
{
	return Mat4x4(1, 0,	0, 0,
				  0, cos(angle), sin(angle), 0,
				  0, -sin(angle), cos(angle), 0,
				  0, 0, 0, 1);
}
Mat4x4 Mat4x4RotationY(float angle)
{
	return Mat4x4(cos(angle), 0, -sin(angle), 0,
				  0, 1, 0, 0,
				  sin(angle), 0, cos(angle), 0,
				  0, 0, 0, 1);
}
Mat4x4 Mat4x4RotationZ(float angle)
{
	return Mat4x4(cos(angle), sin(angle), 0, 0,
				  -sin(angle), cos(angle), 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1);
}
Mat4x4 Mat4x4Scale(float sx, float sy, float sz)
{
	return Mat4x4(sx, 0, 0, 0,
				  0, sy, 0, 0,
				  0, 0, sz, 0,
				  0, 0, 0, 1);
}
Mat4x4 Mat4x4LookAtLH(Vec3 *eye, Vec3 *at, Vec3 *up)
{
	Vec3 zaxis = Vec3Normalized(&(*at - *eye));
	Vec3 xaxis = Vec3Normalized(&Vec3Cross(up, &zaxis));
	Vec3 yaxis = Vec3Cross(&zaxis, &xaxis);

	return Mat4x4(xaxis.x, yaxis.x, zaxis.x, 0,
				  xaxis.y, yaxis.y, zaxis.y, 0,
				  xaxis.z, yaxis.z, zaxis.z, 0,
				  -Vec3Dot(&xaxis, eye), -Vec3Dot(&yaxis, eye), -Vec3Dot(&zaxis, eye), 1);
}
Mat4x4 Mat4x4PerspectiveFovLH(float fovy, float aspect, float zn, float zf)
{
	float yScale = 1/tan(fovy/2);
	float xScale = yScale/aspect;

	return Mat4x4(xScale, 0, 0, 0,
				  0, yScale, 0, 0,
				  0, 0, zf/(zf - zn), 1,
				  0, 0, -zn*zf/(zf - zn), 0);
}
Vec3 Vec3Mat4x4Multiply(Vec3 *v, Mat4x4 *m)
{
	float x = v->x*m->_11 + v->y*m->_21 + v->z*m->_31 + m->_41;
	float y = v->x*m->_12 + v->y*m->_22 + v->z*m->_32 + m->_42;
	float z = v->x*m->_13 + v->y*m->_23 + v->z*m->_33 + m->_43;
	float w = v->x*m->_14 + v->y*m->_24 + v->z*m->_34 + m->_44;

	return Vec3(x, y, z)/w;
}
Vec3 Vec3Project(Vec3 *v, Viewport *vp, Mat4x4 *world, Mat4x4 *view, Mat4x4 *proj)
{
	Vec3 projectedpos = Vec3Mat4x4Multiply(v, &(*world**view**proj));

	return Vec3(vp->x + (1 + projectedpos.x)*vp->w/2, vp->y + (1 - projectedpos.y)*vp->h/2, vp->minz + projectedpos.z*(vp->maxz - vp->minz));
}

//Mesh
struct Mesh
{
	vector <Vec3> vertexbuffer;
	vector <Vec3> indexbuffer;
	vector <Vec3> normalbuffer;
	string filename;

	Mesh(string name)
	{filename = name;};

	bool Load()
	{
		 vertexbuffer.clear();
		 indexbuffer.clear();
		 normalbuffer.clear();

		 ifstream meshdata(filename);
		 if (!meshdata.is_open()){return false;}

		 string line = "";

		 while (getline(meshdata, line))
		 {
			 //Vertex buffer
			if (line == "[VERTEX]")
			{
				while (getline(meshdata, line))
				{
					if (line != "[/VERTEX]")
					{
						vertexbuffer.push_back(StringToVec3(line));
					}
					else {break;}
				}
			}

			//Index buffer
			if (line == "[INDEX]")
			{
				while (getline(meshdata, line))
				{
					if (line != "[/INDEX]")
					{
						indexbuffer.push_back(StringToVec3(line));
					}
					else {break;}
				}
			}

			//Normal buffer
			if (line == "[NORMAL]")
			{
				while (getline(meshdata, line))
				{
					if (line != "[/NORMAL]")
					{
						normalbuffer.push_back(StringToVec3(line));
					}
					else {break;}
				}
			}
		 }

		 meshdata.close();

		 return true;
	}
};

//Misc. program
Mesh meshTestObject("testobject.txt");
Viewport mainViewport;
Mat4x4 worldMatrix, viewMatrix, projMatrix;
Vec3 camPos(-8, 0, 0);
float objRotationY = 0, objRotationX = 0;;
unsigned drawMode = 0;

#endif
