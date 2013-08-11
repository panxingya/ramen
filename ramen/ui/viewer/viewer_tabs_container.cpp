// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer_tabs_container.hpp>

#include<QApplication>
#include<QMouseEvent>
#include<QDrag>
#include<QMimeData>

#include<iostream>

namespace ramen
{
namespace ui
{

viewer_tabs_bar_t::viewer_tabs_bar_t( QWidget *parent) : QTabBar( parent)
{
    selected_tab_ = -1;
}

void viewer_tabs_bar_t::mousePressEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
        selected_tab_ = tabAt( e->pos());

    QTabBar::mousePressEvent( e);
}

void viewer_tabs_bar_t::mouseMoveEvent( QMouseEvent *e)
{
    QTabBar::mouseMoveEvent( e);
}

void viewer_tabs_bar_t::mouseReleaseEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
    {
        if( selected_tab_ != -1 && !rect().contains( e->pos()))
        {
            viewer_tabs_container_t *src_tabs = dynamic_cast<viewer_tabs_container_t*>( parent());

            if( QWidget *target = qApp->topLevelAt( e->globalPos()))
            {
                QPoint p = target->mapFromGlobal( e->globalPos());
                QWidget *w = target->childAt( p);
                while( w)
                {
                    if( w == src_tabs)
                        break;

                    if( viewer_tabs_container_t *dst_tabs = dynamic_cast<viewer_tabs_container_t*>( w))
                    {
                        src_tabs->transfer_tab( selected_tab_, dst_tabs);
                        selected_tab_ = -1;
                        return;
                    }

                    w = w->parentWidget();
                }
            }

            src_tabs->detach_tab( selected_tab_, e->globalPos());
            selected_tab_ = -1;
            return;
        }
    }

    QTabBar::mouseReleaseEvent( e);
}

/*************************************************************/

viewer_tabs_container_t::viewer_tabs_container_t( QWidget *parent) : QTabWidget( parent)
{
    setTabBar( new viewer_tabs_bar_t());

    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));
}

void viewer_tabs_container_t::add_tab( const QString& name)
{
    addTab( new QWidget(), name);
}

void viewer_tabs_container_t::add_tab( const QString& name, QWidget *tab)
{
    addTab( tab, name);
}

void viewer_tabs_container_t::detach_tab( int index, const QPoint& pos)
{
    QWidget *tab = widget( index);
    int w = tab->width();
    int h = tab->height();

    QString name = tabText( index);
    removeTab( index);

    viewer_tabs_container_t *window = new viewer_tabs_container_t();
    window->setWindowFlags( Qt::Window);
    window->setWindowTitle( name);
    window->addTab( tab, name);
    window->move( pos);
    window->resize( w, h);
    window->show();

    if( !parent() && !count())
        close();
}

void viewer_tabs_container_t::transfer_tab( int index, viewer_tabs_container_t *other_tabs)
{
    QWidget *tab = widget( index);
    QString name = tabText( index);
    removeTab( index);

    other_tabs->add_tab( name, tab);

    if( !parent() && !count())
        close();
}

void viewer_tabs_container_t::delete_tab( int index)
{
    QWidget *tab = widget( index);
    removeTab( index);
    delete tab;

    if( !parent())
        close();
}

} // ui
} // ramen
