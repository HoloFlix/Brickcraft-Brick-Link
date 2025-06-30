package.path = "?;?.lua;../../assets/lua/?;../../assets/lua/?.lua"
vec1 = Vector3(4,1,69);
vec2 = Vector3(516,5148,23);
vec3 = vec1 + vec2;
function AddCommand(name, description, numParams, securityLevel)
    commandToAdd = ServerCommand(name, description, numParams, securityLevel);
    ServerCommander:AddServerCommand(commandToAdd);
end
ServerCommander:ClearLuaCommands();
ServerCommander:ClearAllAliases();
-- commandToAdd = ServerCommand("listcommands", "I list commands :)", 0, ServerCommand.SecurityLevel0);
AddCommand("listcommands", "I list commands :)", 0, ServerCommand.SecurityLevel0);

function listcommands(connector)
    ServerCommander:ListServerCommands(connector);
end

AddCommand("msg", "I relay what you said to everyone :)", 1, ServerCommand.SecurityLevel0);
function msg(connector, message)
    connector:Print(message);
end

AddCommand("reloadscripts", "I reload all lua scripts :)", 0, ServerCommand.SecurityLevel0);
function reloadscripts(connector)
   ServerCommander:ReloadLuaScripts(connector);
end

AddCommand("spawnentity", "I spawn a simple box in the game :)", 0, ServerCommand.SecurityLevel0);
function spawnentity(connector)
    ServerInstance:SpawnPhysicsEntity();
    connector:Print("Spawned Box!");
end

AddCommand("testlxfml", "I import a test LXFML file onto the server", 0, ServerCommand.SecurityLevel0);
function testlxfml(connector)
    ServerInstance:ImportTestLXFML();
    connector:Print("LXFML import finished");
end

AddCommand("importlxfml", "I import a base64-encoded LXFML file onto the server", 1, ServerCommand.SecurityLevel0);
function importlxfml(connector, base64lxfml)
    ServerInstance:ImportBase64LXFML(connector, base64lxfml);
    connector:Print("LXFML import finished");
end

AddCommand("undoimport", "I undo the last import made by the player who calls the command", 0, ServerCommand.SecurityLevel0);
function undoimport(connector)
    ServerInstance:UndoImportLXFML(connector);
    connector:Print("Undid LXFML import");
end

AddCommand("alias", "I create an alias for a command :)", 2, ServerCommand.SecurityLevel0);
function alias(connector, command, alias)
    ServerCommander:AddAlias(command, alias);
    connector:Print("Create alias!");
end

AddCommand("unalias", "I remove an alias for a command :)", 1, ServerCommand.SecurityLevel0);
function unalias(connector, alias)
    ServerCommander:RemoveAlias(alias);
    connector:Print("Remove alias!");
end

function TestPush(p)
    print("Testing PushPlayer for player type: " .. type(p));
    if p and p.PushPlayer and Vector3 then
        p:PushPlayer(Vector3(0, 1, 0));
        print("PushPlayer called manually.");
    else
        print("Manual PushPlayer failed. Player or PushPlayer or Vector3 missing.");
    end
end

ServerCommander:AddAlias("spawnentity", "se");
package.loaded["Item"] = nil;
require "Item"
require "MyFirstMod"

ServerCommander:AddAlias("testlxfml", "tl");
