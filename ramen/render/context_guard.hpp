// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_RENDER_CONTEXT_GUARD_HPP
#define	RAMEN_RENDER_CONTEXT_GUARD_HPP

#include<ramen/config.hpp>

#include<map>

#include<boost/noncopyable.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/render/context.hpp>

namespace ramen
{
namespace render
{

class RAMEN_API context_guard_t : boost::noncopyable
{
public:

    explicit context_guard_t( const context_t& context, node_t *n = 0);
    ~context_guard_t();

private:

    void save( const node_t *n);
    void restore( node_t *n);

    context_t context_;
    node_t *n_;

    struct saved_info_t
    {
        math::box2i_t roi;
    };

    std::map<const node_t*, saved_info_t> saved_;
};

} // render
} // ramen

#endif
