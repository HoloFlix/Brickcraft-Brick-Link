# Brickcraft Lua Modding Guide (Definitive Edition)

This guide is for developers who wish to create Lua mods using the un-patched, original game server.

## I. The Unbreakable Rules of Lua Modding:

Based on our research, the un-patched game engine has four hardcoded limitations:

* **The API is Fixed:** The functions listed below are the *only* ones available.
* **Items Cannot Be Unequipped:** `player:SetItem(nil)` will crash the server. Flight via the `/fly` command is a permanent, one-way action for that session.
* **Many Native Behaviors are Unstoppable:** The default "hover" effect of the Jetpack is coded in C++ and cannot be turned off from Lua while the item is equipped.
* **The Physics Engine is Fragile:** Applying large forces via `PushPlayer` will cause players to clip through the terrain. All forces must be small and gentle.

## II. The Definitive Lua API:

* **Player:** `:PushPlayer(vector)`, `:SetItem(item)`, `:GetGuidAsInteger()`, `:GetName()`
* **ServerCommander:** All functions for creating commands are stable.
* **EntityManager:** The `.Players` iterator is stable.
* **JetPack:** The `JetPack()` constructor returns a working item.

## III. Feasible "Vanilla" Lua Mods:

* **Physics-Based Tools:** A `/hop` command with a very small upward force, or a `/nudge` command that applies a tiny force to another player.
* **Server-Side Economy:** A virtual economy can be tracked entirely in Lua tables, with commands like `/givecredits <player> <amount>`. This will have no in-game visual representation.
* **The One-Way Jetpack:** A stable command that permanently equips the Jetpack.
