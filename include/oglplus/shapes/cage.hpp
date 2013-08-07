/**
 *  @file oglplus/shapes/cage.hpp
 *  @brief Cage builder
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SHAPES_CUBE_1107121519_HPP
#define OGLPLUS_SHAPES_CUBE_1107121519_HPP

#include <oglplus/face_mode.hpp>
#include <oglplus/shapes/draw.hpp>

#include <oglplus/shapes/vert_attr_info.hpp>
#include <oglplus/matrix.hpp>

namespace oglplus {
namespace shapes {

/// Class providing vertex attributes and instructions for rendering of a cage
class Cage
 : public DrawingInstructionWriter
{
private:
	Vector<GLdouble, 3> _size;
	Vector<GLdouble, 3> _barw;
	Vector<GLdouble, 3> _divs;

	static const Matrix<GLdouble, 3, 3>& _face_mat(GLuint face)
	{
		assert(face < 6);
		typedef Matrix<GLdouble, 3, 3> M;
		static M m[6] = {
			M( 0, 0, 1,  0,-1, 0,  1, 0, 0),//[0]+x
			M( 0, 0,-1,  0,-1, 0, -1, 0, 0),//[1]-x
			M(-1, 0, 0,  0, 0, 1,  0, 1, 0),//[2]+y
			M(-1, 0, 0,  0, 0,-1,  0,-1, 0),//[3]-y
			M( 1, 0, 0,  0, 1, 0,  0, 0, 1),//[4]+z
			M(-1, 0, 0,  0, 1, 0,  0, 0,-1) //[5]-z
		};
		return m[face];
	}

	GLdouble _face_size(GLuint face, GLuint axis) const
	{
		return std::fabs(Dot(_face_mat(face).Row(axis), _size));
	}

	GLdouble _face_barw(GLuint face, GLuint axis) const
	{
		return std::fabs(Dot(_face_mat(face).Row(axis), _barw));
	}

	GLuint _face_divs(GLuint face, GLuint axis) const
	{
		return GLuint(std::fabs(Dot(_face_mat(face).Row(axis), _divs)));
	}

	static const Vector<GLdouble, 3> _face_vec(
		GLuint face,
		const Vector<GLdouble, 3> vec
	)
	{
		return _face_mat(face)*vec;
	}

	template <typename Iter>
	static Iter _write(Iter iter, const Vector<GLdouble, 3>& vec)
	{
		*iter++ = vec.x();
		*iter++ = vec.y();
		*iter++ = vec.z();
		return iter;
	}

	GLuint _vert_count(void) const
	{
		return 6*8+
			(_divs.z()-1)*2*4*2+
			(_divs.x()-1)*4*4*2+

			(_divs.y()-1)*_divs.z()*2*4*2+
			(_divs.z()-1)*_divs.x()*2*4*2+
			(_divs.y()-1)*_divs.x()*2*4*2+

			(_divs.x()*_divs.y())*32+
			(_divs.x()*_divs.z())*32+
			(_divs.y()*_divs.z())*32;
	}

	// primitive-restart-index
	GLuint _pri(void) const { return _vert_count(); }

	GLuint _index_count(void) const
	{
		return 6*11+
			(_divs.z()-1)*2*5*2+
			(_divs.x()-1)*4*5*2+

			(_divs.y()-1)*_divs.z()*2*5*2+
			(_divs.z()-1)*_divs.x()*2*5*2+
			(_divs.y()-1)*_divs.x()*2*5*2+

			(_divs.x()*_divs.y())*40+
			(_divs.x()*_divs.z())*40+
			(_divs.y()*_divs.z())*40;
	}
public:
	/// Constructs a unit cage centered at the origin
	Cage(void)
	 : _size(1, 1, 1)
	 , _barw(0.15, 0.15, 0.15)
	 , _divs(4, 4, 4)
	{ }

	/// Constructs a cage with width, height, depth
	Cage(
		GLdouble xs, GLdouble ys, GLdouble zs,
		GLdouble xb, GLdouble yb, GLdouble zb,
		GLuint xd, GLuint yd, GLuint zd
	): _size(xs, ys, zs)
	 , _barw(xb, yb, zb)
	 , _divs(xd, yd, zd)
	{
		assert(xs > 0.0);
		assert(ys > 0.0);
		assert(zs > 0.0);

		assert(xd > 0);
		assert(yd > 0);
		assert(zd > 0);

		assert(xs > xb*(xd-1));
		assert(ys > yb*(yd-1));
		assert(zs > zb*(zd-1));
	}

	/// Returns the winding direction of faces
	FaceOrientation FaceWinding(void) const
	{
		return FaceOrientation::CW;
	}

	typedef GLuint (Cage::*VertexAttribFunc)(std::vector<GLfloat>&) const;

	/// Makes the vertices and returns the number of values per vertex
	template <typename T>
	GLuint Positions(std::vector<T>& dest) const
	{
		dest.resize(_vert_count()*3);
		auto p=dest.begin();

		typedef Vector<GLdouble, 3> V;

		// for each face
		for(GLuint f=0; f!=6; ++f)
		{
			GLdouble sx = _face_size(f, 0);
			GLdouble sy = _face_size(f, 1);
			GLdouble sz = _face_size(f, 2);

			GLdouble bx = _face_barw(f, 0);
			GLdouble by = _face_barw(f, 1);
			GLdouble bz = _face_barw(f, 1);

			p = _write(p, _face_vec(f, V(+bx-sx,+by-sy, sz)));
			p = _write(p, _face_vec(f, V(   -sx,   -sy, sz)));
			p = _write(p, _face_vec(f, V(+bx-sx,-by+sy, sz)));
			p = _write(p, _face_vec(f, V(   -sx,   +sy, sz)));
			p = _write(p, _face_vec(f, V(-bx+sx,-by+sy, sz)));
			p = _write(p, _face_vec(f, V(   +sx,   +sy, sz)));
			p = _write(p, _face_vec(f, V(-bx+sx,+by-sy, sz)));
			p = _write(p, _face_vec(f, V(   +sx,   -sy, sz)));

			GLuint dx = _face_divs(f, 0);
			GLuint dy = _face_divs(f, 1);

			GLdouble hx = (2*sx - bx*(dx+1))/dx;
			GLdouble hy = (2*sy - by*(dy+1))/dy;

			GLdouble xo = -sx+bx;
			for(GLuint x=1; x!=dx; ++x)
			{
				xo += hx;
				p = _write(p, _face_vec(f, V(+bx+xo,+by-sy, sz)));
				p = _write(p, _face_vec(f, V(   +xo,+by-sy, sz)));
				p = _write(p, _face_vec(f, V(+bx+xo,-by+sy, sz)));
				p = _write(p, _face_vec(f, V(   +xo,-by+sy, sz)));
				xo += bx;
			}

			xo = -sx+bx;
			for(GLuint x=1; x!=dx; ++x)
			{
				xo += hx;
				p = _write(p, _face_vec(f, V(   +xo,+by-sy, sz-bz)));
				p = _write(p, _face_vec(f, V(+bx+xo,+by-sy, sz-bz)));
				p = _write(p, _face_vec(f, V(   +xo,-by+sy, sz-bz)));
				p = _write(p, _face_vec(f, V(+bx+xo,-by+sy, sz-bz)));
				xo += bx;
			}

			GLdouble yo = -sy+by;
			for(GLuint y=1; y!=dy; ++y)
			{
				yo += hy;
				xo = -sx;
				for(GLuint x=0; x!=dx; ++x)
				{
					xo += bx;
					p = _write(p, _face_vec(f, V(   +xo,   +yo, sz)));
					p = _write(p, _face_vec(f, V(   +xo,+by+yo, sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,+by+yo, sz)));
					xo += hx;
				}
				yo += by;
			}

			yo = -sy+by;
			for(GLuint y=1; y!=dy; ++y)
			{
				yo += hy;
				xo = -sx;
				for(GLuint x=0; x!=dx; ++x)
				{
					xo += bx;
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, sz-bz)));
					p = _write(p, _face_vec(f, V(+hx+xo,+by+yo, sz-bz)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo, sz-bz)));
					p = _write(p, _face_vec(f, V(   +xo,+by+yo, sz-bz)));
					xo += hx;
				}
				yo += by;
			}

			yo = -sy+by;
			for(GLuint y=0; y!=dy; ++y)
			{
				xo = -sx+bx;
				for(GLuint x=0; x!=dx; ++x)
				{
					p = _write(p, _face_vec(f, V(   +xo,   +yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo,   +sz)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo,   +sz)));

					p = _write(p, _face_vec(f, V(   +xo,+hy+yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo,   +sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo,   +sz)));

					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo,   +sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo,   +sz)));

					p = _write(p, _face_vec(f, V(+hx+xo,   +yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo,   +sz)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo,-bz+sz)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo,   +sz)));
					xo += hx+bx;
				}
				yo += hy+by;
			}
		}
		assert(p == dest.end());
		return 3;
	}

	/// Makes the normals and returns the number of values per vertex
	template <typename T>
	GLuint Normals(std::vector<T>& dest) const
	{
		dest.resize(_vert_count()*3);
		auto p=dest.begin();

		typedef Vector<GLdouble, 3> V;

		// for each face
		for(GLuint f=0; f!=6; ++f)
		{
			V t = _face_mat(f).Row(0);
			V b = _face_mat(f).Row(1);
			V n = _face_mat(f).Row(2);

			for(GLuint v=0; v!=8; ++v)
				p = _write(p, n);

			GLuint dx = _face_divs(f, 0);
			for(GLuint s=0; s!=2; ++s)
			{
				GLdouble sig = s?-1:1;
				for(GLuint x=1; x!=dx; ++x)
				{
					for(GLuint v=0; v!=4; ++v)
						p = _write(p, n*sig);
				}
			}

			GLuint dy = _face_divs(f, 1);
			for(GLuint s=0; s!=2; ++s)
			{
				GLdouble sig = s?-1:1;
				for(GLuint y=1; y!=dy; ++y)
				{
					for(GLuint x=0; x!=dx; ++x)
					{
						for(GLuint v=0; v!=4; ++v)
							p = _write(p, n*sig);
					}
				}
			}

			for(GLuint y=0; y!=dy; ++y)
			{
				for(GLuint x=0; x!=dx; ++x)
				{
					for(GLuint s=0; s!=4; ++s)
						p = _write(p,  t);
					for(GLuint s=0; s!=4; ++s)
						p = _write(p, -b);
					for(GLuint s=0; s!=4; ++s)
						p = _write(p, -t);
					for(GLuint s=0; s!=4; ++s)
						p = _write(p,  b);
				}
			}
		}
		assert(p == dest.end());
		return 3;
	}

	/// Makes the tangents and returns the number of values per vertex
	template <typename T>
	GLuint Tangents(std::vector<T>& dest) const
	{
		dest.resize(_vert_count()*3);
		auto p=dest.begin();

		typedef Vector<GLdouble, 3> V;

		// for each face
		for(GLuint f=0; f!=6; ++f)
		{
			V t = -_face_mat(f).Row(0);
			V n = -_face_mat(f).Row(2);

			for(GLuint v=0; v!=8; ++v)
				p = _write(p, t);

			GLuint dx = _face_divs(f, 0);
			for(GLuint s=0; s!=2; ++s)
			{
				GLdouble sig = s?-1:1;
				for(GLuint x=1; x!=dx; ++x)
				{
					for(GLuint v=0; v!=4; ++v)
						p = _write(p, t*sig);
				}
			}

			GLuint dy = _face_divs(f, 1);
			for(GLuint s=0; s!=2; ++s)
			{
				GLdouble sig = s?-1:1;
				for(GLuint y=1; y!=dy; ++y)
				{
					for(GLuint x=0; x!=dx; ++x)
					{
						for(GLuint v=0; v!=4; ++v)
							p = _write(p, t*sig);
					}
				}
			}

			for(GLuint y=0; y!=dy; ++y)
			{
				for(GLuint x=0; x!=dx; ++x)
				{
					for(GLuint s=0; s!=16; ++s)
						p = _write(p, -n);
				}
			}
		}
		assert(p == dest.end());
		return 3;
	}

	/// Makes the texture coordinates and returns the number of values per vertex
	template <typename T>
	GLuint TexCoordinates(std::vector<T>& dest) const
	{
		dest.resize(_vert_count()*3);
		auto p=dest.begin();

		typedef Vector<GLdouble, 3> V;

		// for each face
		for(GLuint f=0; f!=6; ++f)
		{
			GLdouble z = 0.0;
			GLdouble o = 1.0;

			GLdouble bx = 0.5*_face_barw(f, 0)/_face_size(f, 0);
			GLdouble by = 0.5*_face_barw(f, 1)/_face_size(f, 1);

			p = _write(p, _face_vec(f, V(+bx+z,+by+z, f)));
			p = _write(p, _face_vec(f, V(   +z,   +z, f)));
			p = _write(p, _face_vec(f, V(+bx+z,-by+o, f)));
			p = _write(p, _face_vec(f, V(   +z,   +o, f)));
			p = _write(p, _face_vec(f, V(-bx+o,-by+o, f)));
			p = _write(p, _face_vec(f, V(   +o,   +o, f)));
			p = _write(p, _face_vec(f, V(-bx+o,+by+z, f)));
			p = _write(p, _face_vec(f, V(   +o,   +z, f)));

			GLuint dx = _face_divs(f, 0);
			GLuint dy = _face_divs(f, 1);

			GLdouble hx = (1.0 - bx*(dx+1))/dx;
			GLdouble hy = (1.0 - by*(dy+1))/dy;

			GLdouble xo = bx;
			for(GLuint x=1; x!=dx; ++x)
			{
				xo += hx;
				p = _write(p, _face_vec(f, V(+bx+xo,+by+z, f)));
				p = _write(p, _face_vec(f, V(   +xo,+by+z, f)));
				p = _write(p, _face_vec(f, V(+bx+xo,-by+o, f)));
				p = _write(p, _face_vec(f, V(   +xo,-by+o, f)));
				xo += bx;
			}

			xo = bx;
			for(GLuint x=1; x!=dx; ++x)
			{
				xo += hx;
				p = _write(p, _face_vec(f, V(   +xo,+by+z, f)));
				p = _write(p, _face_vec(f, V(+bx+xo,+by+z, f)));
				p = _write(p, _face_vec(f, V(   +xo,-by+o, f)));
				p = _write(p, _face_vec(f, V(+bx+xo,-by+o, f)));
				xo += bx;
			}

			GLdouble yo = by;
			for(GLuint y=1; y!=dy; ++y)
			{
				yo += hy;
				xo = z;
				for(GLuint x=0; x!=dx; ++x)
				{
					xo += bx;
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,+by+yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,+by+yo, f)));
					xo += hx;
				}
				yo += by;
			}

			yo = by;
			for(GLuint y=1; y!=dy; ++y)
			{
				yo += hy;
				xo = z;
				for(GLuint x=0; x!=dx; ++x)
				{
					xo += bx;
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,+by+yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,+by+yo, f)));
					xo += hx;
				}
				yo += by;
			}

			yo = by;
			for(GLuint y=0; y!=dy; ++y)
			{
				xo = bx;
				for(GLuint x=0; x!=dx; ++x)
				{
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo, f)));

					p = _write(p, _face_vec(f, V(   +xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo, f)));

					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,+hy+yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));

					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(+hx+xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					p = _write(p, _face_vec(f, V(   +xo,   +yo, f)));
					xo += hx+bx;
				}
				yo += hy+by;
			}
		}
		assert(p == dest.end());
		return 3;
	}

#if OGLPLUS_DOCUMENTATION_ONLY
	/// Vertex attribute information for this shape builder
	/** Cage provides build functions for the following named
	 *  vertex attributes:
	 *  - "Position" the vertex positions (Positions)
	 *  - "Normal" the vertex normal vectors (Normals)
	 *  - "Tangent" the vertex tangent vector (Tangents)
	 *  - "TexCoord" the ST texture coordinates (TexCoordinates)
	 */
	typedef VertexAttribsInfo<Cage> VertexAttribs;
#else
	typedef VertexAttribsInfo<
		Cage,
		std::tuple<
			VertexPositionsTag,
			VertexNormalsTag,
			VertexTangentsTag,
			VertexTexCoordinatesTag
		>
	> VertexAttribs;
#endif

	/// Queries the bounding sphere coordinates and dimensions
	template <typename T>
	void BoundingSphere(Vector<T, 4>& center_and_radius) const
	{
		center_and_radius = Vector<T, 4>(
			T(0),
			T(0),
			T(0),
			Length(_size)
		);
	}

	/// The type of the index container returned by Indices()
	typedef std::vector<GLuint> IndexArray;

	/// Returns element indices that are used with the drawing instructions
	IndexArray Indices(void) const
	{
		IndexArray indices(_index_count(), _pri());
		auto i=indices.begin();

		GLuint offs = 0;

		for(GLuint f=0; f!=6; ++f)
		{
			for(GLuint v=0; v!=10; ++v)
			{
				*i++ = offs + v%8;
			}
			++i; // end of strip

			offs += 8;

			GLuint dx = _face_divs(f, 0);
			for(GLuint s=0; s!=2; ++s)
			{
				for(GLuint x=1; x!=dx; ++x)
				{
					for(GLuint v=0; v!=4; ++v)
						*i++ = offs + v;
					++i; // end of strip
					offs += 4;
				}
			}

			GLuint dy = _face_divs(f, 1);
			for(GLuint s=0; s!=2; ++s)
			{
				for(GLuint y=1; y!=dy; ++y)
				{
					for(GLuint x=0; x!=dx; ++x)
					{
						for(GLuint v=0; v!=4; ++v)
							*i++ = offs + v;
						++i; // end of strip
						offs += 4;
					}
				}
			}

			for(GLuint y=0; y!=dy; ++y)
			{
				for(GLuint x=0; x!=dx; ++x)
				{
					for(GLuint s=0; s!=4; ++s)
					{
						for(GLuint w=0; w!=4; ++w)
							*i++ = offs + w;
						++i; // end of strip
						offs += 4;
					}
				}
			}
		}
		assert(i == indices.end());
		return indices;
	}

	/// Returns the instructions for rendering of faces
	DrawingInstructions Instructions(void) const
	{
		DrawOperation operation;
		operation.method = DrawOperation::Method::DrawElements;
		operation.mode = PrimitiveType::TriangleStrip;
		operation.first = 0;
		operation.count = _index_count();
		operation.restart_index = _pri();
		operation.phase = 0;

		return this->MakeInstructions(operation);
	}
};

} // shapes
} // oglplus

#endif // include guard