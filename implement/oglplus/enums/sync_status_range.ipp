//  File implement/oglplus/enums/sync_status_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/sync_status.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2019 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
OGLPLUS_LIB_FUNC aux::CastIterRange<
	const GLenum*,
	SyncStatus
> ValueRange_(SyncStatus*)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVR_SYNCSTATUS)
#define OGLPLUS_IMPL_EVR_SYNCSTATUS
{
static const GLenum _values[] = {
#if defined GL_SIGNALED
GL_SIGNALED,
#endif
#if defined GL_UNSIGNALED
GL_UNSIGNALED,
#endif
0
};
return aux::CastIterRange<
	const GLenum*,
	SyncStatus
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

