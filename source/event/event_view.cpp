/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"
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

float NvpEventView::calcView(bool coord_x, float value)
{
    if (coord_x)
    {
        return (value - event_view->current_mtx._31) / event_view->current_mtx._11;
    }
    else
    {
        return (value - event_view->current_mtx._32) / event_view->current_mtx._22;
    }
}

void NvpEventView::handleView(float width, float height, bool set)
{
    if (!event_view)
    {
        event_view = new NvpEventView;
    }

    if (set)
    {
        event_view->view_width = width;
        event_view->view_height = height;
    }

    const float stat_height = 20;
    NvpRect rect;
    rect.left = calcView(true, 0);
    rect.top = calcView(false, 0);
    rect.right = calcView(true, event_view->view_width);
    rect.bottom = calcView(false, event_view->view_height - stat_height);
    NvpLayout::setLayoutRect(NvpLayout::Build()->getNodeView(), rect);

    rect.left = calcView(true, 0);
    rect.top = calcView(false, event_view->view_height - stat_height);
    rect.right = calcView(true, event_view->view_width);
    rect.bottom = calcView(false, event_view->view_height);
    NvpLayout::setLayoutRect(NvpLayout::Build()->getMenuStat(), rect);
}

void NvpEventView::mouseEvent(NvpEventRef& event)
{
    const float zoom_speed = 0.05f;

    if (event.getMouseType() == NvpEventMouse::WheelPush)
    {
        event_view->last_pt = event.getWindowPt();
        return;
    }
    else if (event.getMouseType() == NvpEventMouse::WheelUp)
    {
        event_view->scale_x = 1 - zoom_speed;
        event_view->scale_y = 1 - zoom_speed;
    }
    else if (event.getMouseType() == NvpEventMouse::WheelDown)
    {
        event_view->scale_x = 1 + zoom_speed;
        event_view->scale_y = 1 + zoom_speed;
    }
    else if (event.getMouseType() == NvpEventMouse::WheelDrag)
    {
        auto trans_x = event.getWindowPt().x - event_view->last_pt.x;
        auto trans_y = event.getWindowPt().y - event_view->last_pt.y;
        trans_x /= event_view->dpi_scale_x;
        trans_y /= event_view->dpi_scale_y;
        event_view->last_pt = event.getWindowPt();
        event_view->trans_x = trans_x;
        event_view->trans_y = trans_y;

        auto translate = NvpMatrix32::calcTranslation(trans_x, trans_y);
        auto product = NvpMatrix32::calcProduct(event_view->current_mtx, translate);
        event_view->current_mtx = product;

        handleView(0, 0, false);
        return;
    }

    event_view->center_x = event.getWindowPt().x / event_view->dpi_scale_x;
    event_view->center_y = event.getWindowPt().y / event_view->dpi_scale_y;

    NvpPoint center;
    center.x = event_view->center_x;
    center.y = event_view->center_y;
    auto scale = NvpMatrix32::calcScale(event_view->scale_x, event_view->scale_y, center);
    auto product = NvpMatrix32::calcProduct(event_view->current_mtx, scale);
    event_view->current_mtx = product;

    handleView(0, 0, false);
}
