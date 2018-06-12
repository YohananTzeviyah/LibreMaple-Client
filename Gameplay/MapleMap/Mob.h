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
#include "../../Audio/Audio.h"
#include "../../Constants.h"
#include "../../Graphics/EffectLayer.h"
#include "../../Graphics/Geometry.h"
#include "../../Graphics/Text.h"
#include "../../Template/Interpolated.h"
#include "../../Template/Rectangle.h"
#include "../../Util/Randomizer.h"
#include "../../Util/TimedBool.h"
#include "../Combat/Attack.h"
#include "../Combat/Bullet.h"
#include "../Combat/DamageNumber.h"
#include "../Movement.h"
#include "MapObject.h"

#include <unordered_map>

namespace jrc
{
class Mob : public MapObject
{
public:
    static constexpr const std::size_t NUM_STANCES = 6;
    enum Stance : std::uint8_t {
        MOVE = 2,
        STAND = 4,
        JUMP = 6,
        HIT = 8,
        DIE = 10
    };

    static std::string_view name_of(Stance stance) noexcept
    {
        static constexpr const std::string_view stance_names[NUM_STANCES] = {
            "move", "stand", "jump", "hit1", "die1", "fly"};

        return stance_names[(stance - 1) / 2];
    }

    static std::uint8_t value_of(Stance stance, bool flip) noexcept
    {
        return flip ? stance : stance + 1;
    }

    //! Construct a mob by combining data from game files with
    //! data sent by the server.
    Mob(std::int32_t oid,
        std::int32_t mob_id,
        std::int8_t mode,
        std::uint8_t stance,
        std::uint16_t fhid,
        bool new_spawn,
        std::int8_t team,
        Point<std::int16_t> position);

    //! Draw the mob.
    void draw(double viewx, double viewy, float alpha) const override;
    //! Update movement and animations.
    std::int8_t update(const Physics& physics) override;

    //! Change this mob's control mode:
    //!
    //! * 0 : No control
    //! * 1 : Control
    //! * 2 : Aggro
    void set_control(std::int8_t mode);
    //! Send movement to the mob.
    void send_movement(Point<std::int16_t> start,
                       std::vector<Movement>&& movements);
    //! Kill the mob with the appropriate type:
    //!
    //! * 0 : Make inactive
    //! * 1 : Death animation
    //! * 2 : Fade out
    void kill(std::int8_t kill_type);
    //! Display the HP percentage above the mob.
    //! Use the player's level to determine color of nametag.
    void show_hp(std::int8_t percentage, std::uint16_t player_level);
    //! Show an effect at the mob's position.
    void show_effect(const Animation& animation,
                     std::int8_t pos,
                     std::int8_t z,
                     bool flip);

    //! Calculate the damage to this mob with the spcecified attack.
    std::vector<std::pair<std::int32_t, bool>>
    calculate_damage(const Attack& attack);
    //! Apply damage to the mob.
    void apply_damage(std::int32_t damage, bool to_left);

    //! Create a touch damage attack to the player.
    MobAttack create_touch_attack() const;

    //! Check if this mob collides with the specified rectangle.
    bool is_in_range(const Rectangle<std::int16_t>& range) const;
    //! Check if this mob is still alive.
    bool is_alive() const;
    //! Return the head position.
    Point<std::int16_t> get_head_position() const;

private:
    enum FlyDirection { STRAIGHT, UPWARDS, DOWNWARDS, NUM_DIRECTIONS };

    //! Set the stance by byte value.
    void set_stance(std::uint8_t stancebyte);
    //! Set the stance by enum value.
    void set_stance(Stance newstance);
    //! Start the death animation.
    void apply_death();
    //! Decide on the next state.
    void next_move();
    //! Send the current position and state to the server.
    void update_movement();

    //! Calculate the hit chance.
    float calculate_hit_chance(std::int16_t level_delta,
                               std::int32_t accuracy) const;
    //! Calculate the minimum damage.
    double calculate_min_damage(std::int16_t level_delta,
                                double min_damage,
                                bool magic) const;
    //! Calculate the maximum damage.
    double calculate_max_damage(std::int16_t level_delta,
                                double max_damage,
                                bool magic) const;
    //! Calculate a random damage line based on the specified values.
    std::pair<std::int32_t, bool> next_damage(double min_damage,
                                              double max_damage,
                                              float hit_chance,
                                              float critical) const;

    //! Return the current 'head' position.
    Point<std::int16_t> get_head_position(Point<std::int16_t> position) const;

    std::unordered_map<Stance, Animation> animations;
    std::string name;
    Sound hit_sound;
    Sound die_sound;
    std::uint16_t level;
    float speed;
    float fly_speed;
    std::uint16_t watk;
    std::uint16_t matk;
    std::uint16_t wdef;
    std::uint16_t mdef;
    std::uint16_t accuracy;
    std::uint16_t avoid;
    std::uint16_t knockback;
    bool undead;
    bool touch_damage;
    bool no_flip;
    bool not_attack;
    bool can_move;
    bool can_jump;
    bool can_fly;

    EffectLayer effects;
    Text name_label;
    MobHpBar hp_bar;
    Randomizer randomizer;

    TimedBool do_show_hp;

    std::vector<Movement> movements;
    std::uint16_t counter;

    std::int32_t id;
    std::int8_t effect;
    std::int8_t team;
    bool dying;
    bool dead;
    bool await_death;
    bool control;
    bool aggro;
    Stance stance;
    bool flip;
    FlyDirection fly_direction;
    float walk_force;
    std::int8_t hp_percent;
    bool fading;
    bool fade_in;
    Linear<float> opacity;
};
} // namespace jrc
