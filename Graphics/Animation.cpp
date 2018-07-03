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
#include "Animation.h"

#include "../Constants.h"
#include "../Util/Misc.h"

namespace jrc
{
Frame::Frame(nl::node src) : texture{src}, bounds{src}
{
    head = src["head"];
    delay = src["delay"];
    bool no_delay = false;
    if (delay == 0) {
        delay = 100;
        no_delay = true;
    }

    bool has_a0 = src["a0"].data_type() == nl::node::type::integer;
    bool has_a1 = src["a1"].data_type() == nl::node::type::integer;
    if (has_a0 && has_a1) {
        opacities = {src["a0"], src["a1"]};
    } else if (has_a0) {
        std::uint8_t a0 = src["a0"];
        opacities = {a0, no_delay ? a0 : 255 - a0};
    } else if (has_a1) {
        std::uint8_t a1 = src["a1"];
        opacities = {no_delay ? a1 : 255 - a1, a1};
    } else {
        opacities = {255, 255};
    }

    bool has_z0 = src["z0"].data_type() == nl::node::type::integer;
    bool has_z1 = src["z1"].data_type() == nl::node::type::integer;
    if (has_z0 && has_z1) {
        scales = {src["z0"], src["z1"]};
    } else if (has_z0) {
        scales = {src["z0"], no_delay ? src["z0"] : 0};
    } else if (has_z1) {
        scales = {no_delay ? src["z1"] : 100, src["z1"]};
    } else {
        scales = {100, 100};
    }
}

Frame::Frame() noexcept : delay{0}, opacities{0, 0}, scales{0, 0}
{
}

void Frame::draw(const DrawArgument& args) const
{
    texture.draw(args);
}

std::uint8_t Frame::start_opacity() const
{
    return opacities.first;
}

std::uint8_t Frame::end_opacity() const
{
    return opacities.second;
}

std::uint16_t Frame::start_scale() const
{
    return static_cast<std::uint16_t>(scales.first);
}

std::uint16_t Frame::end_scale() const
{
    return static_cast<std::uint16_t>(scales.second);
}

std::uint16_t Frame::get_delay() const
{
    return delay;
}

Point<std::int16_t> Frame::get_origin() const
{
    return texture.get_origin();
}

Point<std::int16_t> Frame::get_dimensions() const
{
    return texture.get_dimensions();
}

Point<std::int16_t> Frame::get_head() const
{
    return head;
}

Rectangle<std::int16_t> Frame::get_bounds() const
{
    return bounds;
}

float Frame::opc_step(std::uint16_t timestep) const
{
    if (delay == 0) {
        return 0.0f;
    }

    return timestep * static_cast<float>(opacities.second - opacities.first)
           / delay;
}

float Frame::scale_step(std::uint16_t timestep) const
{
    if (delay == 0) {
        return 0.0f;
    }

    return timestep * static_cast<float>(scales.second - scales.first) / delay;
}

Animation::Animation(nl::node src) : finished(false)
{
    bool is_texture = src.data_type() == nl::node::type::bitmap;
    if (is_texture) {
        frames.emplace_back(src);
    } else {
        std::vector<std::int16_t> frame_ids;
        frame_ids.reserve(src.size());
        for (auto sub : src) {
            if (sub.data_type() == nl::node::type::bitmap) {
                auto fid = string_conversion::or_default<std::int16_t>(
                    sub.name(), -1);
                if (fid >= 0) {
                    frame_ids.push_back(fid);
                }
            }
        }
        std::sort(frame_ids.begin(), frame_ids.end());

        for (auto fid : frame_ids) {
            frames.emplace_back(src[fid]);
        }

        if (frames.empty()) {
            frames.emplace_back();
        }
    }

    animated = frames.size() > 1;
    zigzag = src["zigzag"].get_bool();
    repeat = src["repeat"];

    reset();
}

Animation::Animation() noexcept
    : animated(false), zigzag(false), finished(true)
{
    frames.emplace_back();

    reset();
}

void Animation::reset()
{
    frame.set(0);
    opacity.set(frames[0].start_opacity());
    xy_scale.set(frames[0].start_scale());
    delay = frames[0].get_delay();
    frame_step = 1;
}

void Animation::draw(const DrawArgument& args, float alpha) const
{
    std::int16_t interframe = frame.get(alpha);
    float inter_opc = opacity.get(alpha) / 255.0f;
    float inter_scale = xy_scale.get(alpha) / 100.0f;

    bool modify_opc = inter_opc != 1.0f;
    bool modify_scale = inter_scale != 1.0f;
    if (modify_opc || modify_scale) {
        frames[interframe].draw(
            args + DrawArgument{inter_scale, inter_scale, inter_opc});
    } else {
        frames[interframe].draw(args);
    }
}

bool Animation::update()
{
    return update(Constants::TIMESTEP);
}

bool Animation::update(std::uint16_t timestep)
{
    if (finished) {
        return true;
    }

    const Frame& frame_data = get_frame();

    opacity += frame_data.opc_step(timestep);
    if (opacity.last() < 0.0f) {
        opacity.set(0.0f);
    } else if (opacity.last() > 255.0f) {
        opacity.set(255.0f);
    }

    xy_scale += frame_data.scale_step(timestep);
    if (xy_scale.last() < 0.0f) {
        opacity.set(0.0f);
    }

    if (timestep >= delay) {
        auto last_frame = static_cast<std::int16_t>(frames.size() - 1);
        std::int16_t next_frame;
        bool ended;
        if (zigzag && last_frame > 0) {
            if (frame_step == 1 && frame == last_frame) {
                frame_step = -frame_step;
                ended = false;
            } else if (frame_step == -1 && frame == 0) {
                frame_step = -frame_step;
                ended = true;
            } else {
                ended = false;
            }

            next_frame = frame + frame_step;
        } else {
            if (frame == last_frame) {
                next_frame = 0;
                ended = true;
            } else {
                next_frame = frame + 1;
                ended = false;
            }
        }

        if (ended && repeat == -1) {
            finished = true;

            opacity.set(frames[last_frame].end_opacity());
            xy_scale.set(frames[last_frame].end_scale());
        } else {
            std::uint16_t delta = timestep - delay;
            float threshold = static_cast<float>(delta) / timestep;
            frame.next(next_frame, threshold);

            delay = frames[next_frame].get_delay();
            if (delay >= delta) {
                delay -= delta;
            }

            opacity.set(frames[next_frame].start_opacity());
            xy_scale.set(frames[next_frame].start_scale());
        }

        return ended;
    }

    frame.normalize();
    delay -= timestep;

    return false;
}

std::uint16_t Animation::get_delay(std::int16_t frame_id) const
{
    return frame_id < static_cast<std::int16_t>(frames.size())
               ? frames[frame_id].get_delay()
               : 0u;
}

std::uint16_t Animation::get_delay_until(std::int16_t frame_id) const
{
    std::uint16_t total = 0;
    for (std::int16_t i = 0; i < frame_id; ++i) {
        if (i >= static_cast<std::int16_t>(frames.size())) {
            break;
        }

        total += frames[frame_id].get_delay();
    }

    return total;
}

Point<std::int16_t> Animation::get_origin() const
{
    return get_frame().get_origin();
}

Point<std::int16_t> Animation::get_dimensions() const
{
    return get_frame().get_dimensions();
}

Point<std::int16_t> Animation::get_head() const
{
    return get_frame().get_head();
}

Rectangle<std::int16_t> Animation::get_bounds() const
{
    return get_frame().get_bounds();
}

const Frame& Animation::get_frame() const
{
    return frames[frame.get()];
}
} // namespace jrc
