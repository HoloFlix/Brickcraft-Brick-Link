-- MyFirstMod.lua (v7.0 - The Stable Release)
-- The definitive, stable version containing only 100% functional and reliable commands.

print("MyFirstMod.lua v7.0 loaded successfully!")

-- --- Global Objects ---
local GlobalJetpack = JetPack() 
print("Global Jetpack instance created.")

-- --- Helper Functions ---
function AddMyModCommand(name, description, numParams, securityLevel)
    local commandToAdd = ServerCommand(name, description, numParams, securityLevel)
    ServerCommander:AddServerCommand(commandToAdd)
    print("Added mod command: ".. name)
end

function GetPlayerFromConnector(connector)
    if connector.Player then return connector.Player end
    if type(connector) == "table" and type(connector.GetGuidAsInteger) == "function" then return connector end
    if EntityManager and EntityManager.Players then
        for p in EntityManager.Players do return p end
    end
    return nil
end

-- --- Final, Stable Commands ---

-- The fly command is a one-way action. Once equipped, flight cannot be disabled.
AddMyModCommand("fly", "Permanently equips a jetpack, enabling flight.", 0, ServerCommand.SecurityLevel0)
function fly(connector)
    local player = GetPlayerFromConnector(connector)
    if not player then return end

    -- We use a simple flag on the player object as a one-time check.
    if player.hasBeenGivenJetpack then
        connector:Print("A jetpack is already equipped.")
    else
        player:SetItem(GlobalJetpack)
        player.hasBeenGivenJetpack = true
        connector:Print("Flight PERMANENTLY ENABLED for this session.")
    end
end

-- --- Final, Stable Commands ---

-- The shockwave command is a one-way action. Once entered every player is pushed up and will continuously bounce.
--AddMyModCommand("shockwave", "Pushes ALL players gently into the air.", 0, ServerCommand.SecurityLevel0)
--function shockwave(connector)
    --local pushForce = Vector3(0, 3.0, 0) 
    --for p in EntityManager.Players do
        --p:PushPlayer(pushForce)
    --end
    --connector:Print("A gentle puff of air ripples through the server!")
--end
