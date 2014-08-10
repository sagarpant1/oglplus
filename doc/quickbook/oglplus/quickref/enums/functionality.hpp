//  File doc/quickbook/oglplus/quickref/enums/functionality.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/functionality.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_functionality
namespace oglplus {

enum class Functionality : GLenum
{
	ClipDistance = GL_CLIP_DISTANCE0
};

#if !__OGLPLUS_NO_ENUM_VALUE_RANGES
template <>
__Range<Functionality> __EnumValueRange<Functionality>(void);
#endif

#if !__OGLPLUS_NO_ENUM_VALUE_NAMES
__StrCRef __EnumValueName(Functionality);
#endif

} // namespace oglplus
//]