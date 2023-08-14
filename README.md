# Microlution

Predator & Prey (kind of) simulation made with raylib.

## Controls

|              Key | Action                                  |
|-----------------:|-----------------------------------------|
|            SPACE | Pause / Unpause                         |
|                0 | Set default speed (1)                   |
|                = | Increase simulation speed               |
|                - | Decrease simulation speed               |
|                ~ | Show / Hide simulation stats            |
|          ALT + G | Show / Hide collision grid              |
|          ALT + P | Show / Hide microbes perception radius  |
|          ALT + D | Show / Hide microbes direction          |
|         CTRL + H | Spawn herbivorous (prey) microbe        |
|         CTRL + P | Spawn predator microbe                  |
| CTRL + SHIFT + H | Spawn 10 herbivorous (prey) microbes    |
| CTRL + SHIFT + P | Spawn 10 predator microbes              |

## Description

There are two types of Microbes: Predators and Herbivorous (Prey).

### Food

Herbivorous feed on vegetables that are randomly spawns in the world.
Predators kill herbivorous to eat their meat.
Without food microbes will starve to death.

### Reproduction

To reproduce microbes must eat food.
Microbes have different stats that are randomized at the start.
Newborns inherit some stats from their parents and mutate, sometimes becoming weaker or stronger.

### Behaviour

Microbes are looking for food. When microbe is sated enough to reproduce it starts looking for a partner.
When herbivorous see a predator, it flees. When predator see a herbivorous, it chases it.

## Conclusion

There are two outcomes of this simulation.

1 - Predators start to reproduce rapidly, then kill all the herbivorous, and then starve to death. The predator kills the herbivorous, then reproduces, each of the newborn predators starts killing herbivorous, then eats their meat and reproduces. The number of predators grows exponentially. Predators evolve and become faster, so herbivorous cannot escape from them. The herbivorous try to eat to reproduce, but then they die because the food in their stomachs slows them.

2 - Predators exist in very small numbers, reproduce enough to sustain the population, and feed on the remains of herbivorous that have been driven to death. Sometimes they kill herbivorous not by starving them to death, but by killing them when they are slowed by food in their stomachs, but this is very rare. After a while, all the Predators will eventually die of starvation, and only the Herbivorous will remain.

## Building

Use CMake.

## Dependencies

 [raylib](https://github.com/raysan5/raylib/releases) - A simple and easy-to-use library to enjoy videogames programming 