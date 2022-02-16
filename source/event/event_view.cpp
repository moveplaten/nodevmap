/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "nvp_event.h"
#include "draw/nvp_draw.h"

NvpEventView* NvpEventView::event_view = nullptr;

NvpPoint NvpEventView::convertPt(const NvpPoint& pt)
{
    if (!event_view)
    {
        event_view = new NvpEventView;
    }

    NvpPoint PT;
    PT.x = ((pt.x / event_view->dpi_scale_x - event_view->current_mtx._31) / event_view->current_mtx._11);
    PT.y = ((pt.y / event_view->dpi_scale_y - event_view->current_mtx._32) / event_view->current_mtx._22);

    return PT;
}

void NvpEventView::setDpiScale(float x, float y)
{
    if (!event_view)
    {
        event_view = new NvpEventView;
    }

    event_view->dpi_scale_x = x;
    event_view->dpi_scale_y = y;
}

void NvpEventView::mouseEvent(NvpEventRef& event)
{
    const float zoom_speed = 0.05f;

    if (event.getMouseType() == NvpEventMouse::WheelUp)
    {
        event_view->scale_x = 1 - zoom_speed;
        event_view->scale_y = 1 - zoom_speed;
    }
    else if (event.getMouseType() == NvpEventMouse::WheelDown)
    {
        event_view->scale_x = 1 + zoom_speed;
        event_view->scale_y = 1 + zoom_speed;
    }

    event_view->center_x = event.getWindowPt().x / event_view->dpi_scale_x;
    event_view->center_y = event.getWindowPt().y / event_view->dpi_scale_y;

    NvpPoint center;
    center.x = event_view->center_x;
    center.y = event_view->center_y;
    auto scale = NvpMatrix32::calcScale(event_view->scale_x, event_view->scale_y, center);
    auto product = NvpMatrix32::calcProduct(event_view->current_mtx, scale);
    event_view->current_mtx = product;

    //NvpDrawPort::setDrawMatrix(event_view->current_mtx);
}
