/**
 *  @file oglplus/dsa/ext/vertex_array.hpp
 *  @brief VertexArray wrappers with direct state access
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_DSA_EXT_VERTEX_ARRAY_1107121519_HPP
#define OGLPLUS_DSA_EXT_VERTEX_ARRAY_1107121519_HPP

#include <oglplus/data_type.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/vertex_attrib_slot.hpp>

namespace oglplus {

#if OGLPLUS_DOCUMENTATION_ONLY || GL_EXT_direct_state_access

/// Class wrapping vertex array-related functionality with direct state access
/** @note Do not use this class directly, use DSAVertexArrayEXT instead.
 *
 */
template <>
class ObjectOps<tag::DirectStateEXT, tag::VertexArray>
  : public ObjZeroOps<tag::DirectStateEXT, tag::VertexArray> {
protected:
    ObjectOps(VertexArrayName name) noexcept
      : ObjZeroOps<tag::DirectStateEXT, tag::VertexArray>(name) {}

public:
    ObjectOps(ObjectOps&&) = default;
    ObjectOps(const ObjectOps&) = default;
    ObjectOps& operator=(ObjectOps&&) = default;
    ObjectOps& operator=(const ObjectOps&) = default;

    /// Setup the properties of the specified vertex attribute array
    /**
     *  @glsymbols
     *  @glfunref{VertexArrayVertexAttribOffsetEXT}
     */
    const ObjectOps& VertexAttribOffset(
      BufferName buffer,
      VertexAttribSlot location,
      GLint values_per_vertex,
      DataType data_type,
      Boolean normalized,
      SizeType stride,
      GLintptr offset) const {
        OGLPLUS_GLFUNC(VertexArrayVertexAttribOffsetEXT)
        (_obj_name(),
         GetGLName(buffer),
         GLuint(location),
         values_per_vertex,
         GLenum(data_type),
         normalized._get(),
         stride,
         offset);
        OGLPLUS_CHECK(
          VertexArrayVertexAttribOffsetEXT,
          ObjectError,
          Object(*this).Index(GLuint(location)));
        return *this;
    }

    /// Setup the properties of the specified vertex attribute array
    /**
     *  @glsymbols
     *  @glfunref{VertexArrayVertexAttribIOffsetEXT}
     */
    const ObjectOps& VertexAttribIOffset(
      BufferName buffer,
      VertexAttribSlot location,
      GLint values_per_vertex,
      DataType data_type,
      SizeType stride,
      GLintptr offset) const {
        OGLPLUS_GLFUNC(VertexArrayVertexAttribIOffsetEXT)
        (_obj_name(),
         GetGLName(buffer),
         GLuint(location),
         values_per_vertex,
         GLenum(data_type),
         stride,
         offset);
        OGLPLUS_CHECK(
          VertexArrayVertexAttribIOffsetEXT,
          ObjectError,
          Object(*this).Index(GLuint(location)));
        return *this;
    }

    /// Enable the specified vertex attribute array
    /**
     *  @glsymbols
     *  @glfunref{EnableVertexArrayAttribEXT}
     */
    const ObjectOps& EnableVertexAttrib(VertexAttribSlot location) {
        OGLPLUS_GLFUNC(EnableVertexArrayAttribEXT)
        (_obj_name(), GLuint(location));
        OGLPLUS_CHECK(
          EnableVertexArrayAttribEXT,
          ObjectError,
          Object(*this).Index(GLuint(location)));
        return *this;
    }

    /// Enable the specified vertex attribute array
    /**
     *  @glsymbols
     *  @glfunref{DisableVertexArrayAttribEXT}
     */
    const ObjectOps& DisableVertexAttrib(VertexAttribSlot location) {
        OGLPLUS_GLFUNC(DisableVertexArrayAttribEXT)
        (_obj_name(), GLuint(location));
        OGLPLUS_CHECK(
          DisableVertexArrayAttribEXT,
          ObjectError,
          Object(*this).Index(GLuint(location)));
        return *this;
    }
};

/// VertexArray operations with direct state access
using DSAVertexArrayOpsEXT = ObjectOps<tag::DirectStateEXT, tag::VertexArray>;

/// An @ref oglplus_object encapsulating the OpenGL vertex array functionality
/**
 *  @ingroup oglplus_objects
 */
using DSAVertexArrayEXT = Object<DSAVertexArrayOpsEXT>;

#endif // GL_EXT_direct_state_access

} // namespace oglplus

#endif // include guard
