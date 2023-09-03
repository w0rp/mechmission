#ifndef __MECHMISSION_COMPONENTS_MECH_H_
#define __MECHMISSION_COMPONENTS_MECH_H_

#include <array>

const int max_weapon_slots = 6;
const int max_utility_slots = 4;

// The base type of any mech item.
struct MechItem {
    // The name of the item.
    const char * name;
    // A rank for how good the item is.
    int rank;
    // The health of the item.
    int health;
    // The maximum health of the item.
    int max_health;
    // The weight of item.
    int weight;
};

enum class WeaponType {
    // Indicates there's no weapon in a slot.
    none,
    // An energy weapon type.
    energy,
    // A physical weapon type.
    physical,
};

// A Weapon that may be placed in a Mech.
struct Weapon: MechItem {
    // The weapon type.
    WeaponType type;
    // The current amount of ammo.
    // Ammo can be -1 for infinite ammo.
    int ammo;
    // The maximum ammo for a weapon.
    int max_ammo;
    // The energy cost of firing the weapon.
    int energy_cost;
    // The maximum range of the weapon.
    int maximum_range;
    // The base accuracy of the weapon.
    double base_accuracy;
    // The minimum physical damage a weapon does on hit.
    // This is before armor subtracts the damage.
    int min_physical_damage;
    // The maximum physical damage a weapon does on hit.
    // This is before armor subtracts the damage.
    int max_physical_damage;
    // The minimum amount of damage a weapon does to shields on hit.
    int min_shields_damage;
    // The maximum amount of damage a weapon does to shields on hit.
    int max_shields_damage;
    // The splash damage range for a weapon.
    // 0 indicates no splash damage.
    int splash_range;
    // The modifier for how much damage splash damage does.
    // This modifies both physical and shield damage.
    // Splash damage should have no additional effect on other parts
    // of a mech after deciding which parts of a mech are hit.
    double splash_damage;
};

struct Armor: MechItem {
    // How much protection the armor adds.
    int protection;
};

struct Legs: MechItem {
    // How much protection the Legs add.
    int protection;
    // A speed multiplier for the legs.
    double speed_multiplier;
};

struct Engine: MechItem {
    // A speed multiplier for the engine.
    double speed_multiplier;
};

struct Battery: MechItem {
    // The energy a battery provides per turn.
    int energy;
    // The maximum energy a battery can store.
    int max_energy;
};

// IDEA: Implement shield direction in data.
// For now, we'll just have shields uniformly protect all directions.
struct ShieldGenerator: MechItem {
    // The energy cost of running the shield generator.
    int energy_cost;
    // The maximum strength of the shield generator.
    int max_strength;
};

// IDEA: Implement sensor ability to see over terrain better.
struct Sensor: MechItem {
    // The energy cost of running sensors;
    int energy_cost;
    // How many spaces the sensor can sense units around.
    int sensor_range;
};

// A utility type.
enum class UtilityType {
    none,
    ore_extractor,
};

struct Utility: MechItem {
    UtilityType type;
    // The energy cost of using the utility.
    int energy_cost;
    // true if a utility is enabled or not.
    bool enabled;
};

// A weapon slot type.
enum class WeaponSlotType {
    // Indicate no weapon, or the absence of a slot.
    none,
    // A slot can hold energy weapons.
    energy,
    // A slot can hold physical weapons.
    physical,
    // A slot can hold either energy or physical weapons.
    either,
};

// A specification for describing what a Mech can be composed of.
// This structure can be used in an array of different mech types in the game
// for describing what can be used in a Mech, and for validating what options
// are possible for a Mech.
struct MechSpec {
    // The name of the mech spec.
    const char * name;
    // How good the weapons can be in the mech.
    int max_weapon_rank;
    // How good the armor can be in the mech.
    int max_armor_rank;
    // How good the legs can be in the mech.
    int max_legs_rank;
    // How good of an engine the mech can equip.
    int max_engine_rank;
    // How good of a battery the mech can equip.
    int max_battery_rank;
    // How good of a shield generator the mech can equip.
    int max_shield_generator_rank;
    // How good the sensor can be for the mech.
    int max_sensor_rank;
    // How good the utilities can be for the mech.
    int max_utility_rank;
    // The available weapon slots for the mech.
    std::array<WeaponSlotType, max_weapon_slots> weapon_types;
    // How many utilities the Mech can hold.
    int utility_count;
};

struct Mech {
    // An ID for a MechSpec, which could be an index into an array.
    int mech_spec_id;
    // The player number controlling the mech.
    int player_number;
    // The number of this mech, running from 1 to n.
    int number;
    // The name of the mech
    const char* name;
    // The current energy a Mech has.
    int energy;
    // The current shield strength, in all directions.
    int shield_strength;
    // Additional weight for the frame of the mech.
    int frame_weight;
    Armor armor;
    Legs legs;
    Engine engine;
    Battery battery;
    ShieldGenerator shield_generator;
    Sensor sensor;
    std::array<Weapon, max_weapon_slots> weapons;
    std::array<Utility, max_utility_slots> utils;
};

#endif
