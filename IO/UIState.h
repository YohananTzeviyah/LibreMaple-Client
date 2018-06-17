//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Template/EnumMap.h"
#include "Components/Icon.h"
#include "Components/Tooltip.h"
#include "Keyboard.h"
#include "UIElement.h"

#include <memory>

namespace jrc
{
class UIState
{
public:
    using Iterator = EnumMap<UIElement::Type,
                             std::unique_ptr<UIElement>,
                             UIElement::NUM_TYPES>::iterator;

    virtual ~UIState() = default;

    virtual void draw(float inter, Point<std::int16_t> cursor) const = 0;
    virtual void update() = 0;

    virtual void double_click(Point<std::int16_t> pos) = 0;
    virtual void send_key(KeyType::Id type, std::int32_t action, bool pressed)
        = 0;
    virtual Cursor::State send_cursor(Cursor::State mst,
                                      Point<std::int16_t> pos)
        = 0;

    virtual void drag_icon(Icon* icon) = 0;
    virtual void clear_tooltip(Tooltip::Parent parent) = 0;
    virtual void show_equip(Tooltip::Parent parent, std::int16_t slot) = 0;
    virtual void show_item(Tooltip::Parent parent, std::int32_t itemid) = 0;
    virtual void show_skill(Tooltip::Parent parent,
                            std::int32_t skill_id,
                            std::int32_t level,
                            std::int32_t masterlevel,
                            std::int64_t expiration)
        = 0;

    virtual Iterator pre_add(UIElement::Type type, bool toggled, bool focused)
        = 0;
    virtual void remove(UIElement::Type type) = 0;
    virtual UIElement* get(UIElement::Type type) = 0;
    virtual UIElement* get_front(Point<std::int16_t> pos) = 0;
};

class UIStateNull : public UIState
{
    void draw(float, Point<std::int16_t>) const override
    {
    }
    void update() override
    {
    }
    void double_click(Point<std::int16_t>) override
    {
    }
    void send_key(KeyType::Id, std::int32_t, bool) override
    {
    }
    Cursor::State send_cursor(Cursor::State, Point<std::int16_t>) override
    {
        return Cursor::IDLE;
    }
    void drag_icon(Icon*) override
    {
    }
    void clear_tooltip(Tooltip::Parent) override
    {
    }
    void show_equip(Tooltip::Parent, std::int16_t) override
    {
    }
    void show_item(Tooltip::Parent, std::int32_t) override
    {
    }
    void show_skill(Tooltip::Parent,
                    std::int32_t,
                    std::int32_t,
                    std::int32_t,
                    std::int64_t) override
    {
    }
    Iterator pre_add(UIElement::Type, bool, bool) override
    {
        return {nullptr, UIElement::NUM_TYPES};
    }
    void remove(UIElement::Type) override
    {
    }
    UIElement* get(UIElement::Type) override
    {
        return nullptr;
    }
    UIElement* get_front(Point<std::int16_t>) override
    {
        return nullptr;
    }
};
} // namespace jrc
