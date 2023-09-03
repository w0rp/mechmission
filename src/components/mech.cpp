#include "mech.hpp"

#include <algorithm>

bool mech_follows_spec(const Mech& mech, const MechSpec& spec) {
    auto within_rank = [&](auto&& max_rank){
        return [&](auto&& item) { return item.rank <= max_rank; };
    };

    return
        mech.frame_weight == spec.frame_weight
        && mech.armor.rank <= spec.max_armor_rank
        && mech.legs.rank <= spec.max_legs_rank
        && mech.engine.rank <= spec.max_engine_rank
        && mech.battery.rank <= spec.max_battery_rank
        && mech.shield_generator.rank <= spec.max_shield_generator_rank
        && mech.sensor.rank <= spec.max_sensor_rank
        && std::ranges::all_of(mech.weapons, within_rank(spec.max_weapon_rank))
        && std::ranges::all_of(mech.utils, within_rank(spec.max_utility_rank))
    ;
}
