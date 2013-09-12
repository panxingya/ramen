// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DOCUMENT_HPP
#define RAMEN_DOCUMENT_HPP

#include<ramen/app/document_fwd.hpp>

#include<memory>

#include<ramen/core/memory.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/undo/stack_fwd.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Document class.
*/
class document_t
{
public:

    document_t();

    bool dirty() const	    { return dirty_;}
    void set_dirty( bool d) { dirty_ = d;}

    const undo::stack_t& undo_stack() const { return *undo_;}
    undo::stack_t& undo_stack()             { return *undo_;}

    bool has_file() const { return !file_.empty();}

    boost::filesystem::path file() const { return file_;}
    void set_file( const boost::filesystem::path& p);

    const nodes::world_node_t& world_node() const;
    nodes::world_node_t& world_node();

private:

    // non-copyable
    document_t( const document_t&);
    document_t& operator=( const document_t&);

    mutable bool dirty_;
    core::auto_ptr_t<undo::stack_t> undo_;
    boost::filesystem::path file_;

    nodes::world_node_t world_node_;
};

} // ramen

#endif
