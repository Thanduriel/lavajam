# lavajam
InfinityJam 10/2017 project using vulkan

# System Requirements
- A Graphics Card with Vulkan support
- A keyboard with Latin keys

# Controls

Control | Player Blue | Player Lime
--- | --- | ---
Forward | `W` | `Arrow Up`
Backward | `S` | `Arrow Down`
Turn Left | `A` | `Arrow Left`
Turn Right | `D` | `Arrow Right`
Shoot | `Space` | `Enter`

# Win Condition
The player who first reaches 300 minions loses. The winning player will be displayed at the end.

# Game mechanics
- Both players spawn with some minions. Every minion will try to reach its master by steering towards him.
- When a minion touches *his* master, another minion of the same color will be spawned anywhere random.
- If a minion gets shot then it dies and disappears. A minion of every other color will be spawned.
- If a master gets shot then a minion of his color will be spawned.
- Bullets bounce one time regardless the type of the collision, even walls.
There are three types of bullets:

Type | Description
--- | ---
Normal | A single bullet spawns in front of the player and moves with a constant speed in the direction the player is heading towards.
Double | Two bullets spawn in front of the player. They spawn next to each other and move side by side.
Spread | Three bullets spawn in front of the player. They spawn next to each other and spread in a cone.

Have fun!
