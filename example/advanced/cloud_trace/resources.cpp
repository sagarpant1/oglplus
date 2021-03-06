/**
 *  @file advanced/cloud_trace/resources.cpp
 *  @brief Implements resource-allocation-related classes and functions.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "resources.hpp"

namespace oglplus {
namespace cloud_trace {

ResourceAllocator::ResourceAllocator()
  : tex_unit(0)
  , ub_index(0) {}

unsigned ResourceAllocator::GetNextTexUnit() {
    return tex_unit++;
}

unsigned ResourceAllocator::GetNextUniformIndex() {
    return ub_index++;
}

} // namespace cloud_trace
} // namespace oglplus
