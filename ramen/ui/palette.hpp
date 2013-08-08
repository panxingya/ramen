// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PALETTE_HPP
#define	RAMEN_UI_PALETTE_HPP

#include<map>

#include<boost/noncopyable.hpp>

#include<QColor>

#include<ramen/core/string8.hpp>
#include<ramen/core/memory.hpp>

#include<ramen/color/color3.hpp>

namespace ramen
{
namespace ui
{

class palette_t
{
public:

    static palette_t& instance();

    void rebuild();

    const color::color3c_t& color( const core::string8_t& k) const;
    QColor qcolor( const core::string8_t& k) const;

    bool find_color( const core::string8_t& k) const;

    void set_color( const core::string8_t& k, const color::color3c_t& c);
    void set_color( const core::string8_t& k, const QColor& c);

private:

    palette_t();
    ~palette_t();

    struct impl;
    core::auto_ptr_t<impl> pimpl_;
};

} // ui
} // ramen

#endif
