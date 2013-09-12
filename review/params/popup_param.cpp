// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/popup_param.hpp>

#include<QComboBox>
#include<QLabel>

#include<ramen/app/application.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/ui/user_interface.hpp>

#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

popup_param_t::popup_param_t() : static_param_t()
{
    set_default_value( 0);
}

popup_param_t::popup_param_t( const popup_param_t& other) : static_param_t( other), menu_items_( other.menu_items_), menu_( 0) {}

void popup_param_t::set_default_value( int x)
{
    value().assign( x);
}

void popup_param_t::set_value( int x, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

void popup_param_t::add_menu_item( const core::string8_t& item) { menu_items_.push_back( item);}

void popup_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << menu_items()[ get_value<int>( *this)];
}

/*
void popup_param_t::do_read( const serialization::yaml_node_t& node)
{
    serialization::yaml_node_t n = node.get_node( "value");
    core::string8_t val;
    n >> val;

    int index = find_index_for_string( val);

    if( index != -1)
        value().assign( index);
    else
    {
        node.error_stream() << "error: item " << val << " not found on param\n";
        value().assign( 0);
    }
}

void popup_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    int v = get_value<int>( *this);
    out << YAML::Key << "value" << YAML::Value;
    out << YAML::DoubleQuoted << menu_items()[v];
}
*/

int popup_param_t::find_index_for_string( const core::string8_t& s) const
{
    for( int i = 0; i < menu_items().size(); ++i)
    {
        if( menu_items()[i] == s)
            return i;
    }

    return -1;
}

QWidget *popup_param_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    menu_ = new QComboBox( top);
    menu_->setFocusPolicy( Qt::NoFocus);

    for( int i = 0; i < menu_items_.size(); ++i)
    {
        if( menu_items_[i] == "--")
            menu_->insertSeparator( menu_->count());
        else
            menu_->addItem( menu_items_[i].c_str());
    }

    QSize s = menu_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( ui_label().c_str());
    label->setToolTip( id().c_str());

    menu_->move( app().ui()->inspector().left_margin(), 0);
    menu_->resize( s.width(), s.height());
    menu_->setCurrentIndex( get_value<int>( *this));
    menu_->setEnabled( enabled());
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void popup_param_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);
        menu_->setCurrentIndex( get_value<int>( *this));
        menu_->blockSignals( false);
    }
}

void popup_param_t::do_enable_widgets( bool e)
{
    if( menu_)
        menu_->setEnabled( e);
}

void popup_param_t::item_picked( int index)
{
    param_set()->begin_edit();
    set_value( index);
    param_set()->end_edit();
}

} // namespace