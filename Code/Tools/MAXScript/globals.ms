-- Variables globales
global g_MeshesPath = "data\\Meshes\\"
global g_XMLPath = "data\\XML\\"
global g_TexturesPath = "data\\Textures\\"

global g_ddlLevel = #("Level01","Level02")
global g_ddlExportType = #("mesh_instance","animated_model_instance")
global g_ddlLayers = #("solid","solid_MR","solid_ME","alpha_blend_objects_MR","alpha_blend_objects_ME","particles")
global g_ddlGeneratePhysics = #("triangle_mesh","convex_hull","bounding_box","bounding_sphere","plane","capsule")
global g_ddlLayerPhysics = #("logic","pushable","pullable","breakable","collision")
global g_ddlGenerateLogic = #("Player", "Soldier", "Monster","Mirror","Spawn Point","Tutorial", "Door", "Elevator", "Fan", "Lasers", "Life", "Energy", "Mirror piece", "Broken mirror")

global g_ddlSMDimension = #("128","256","512","1024","2048","4096")
global g_ddlSMTextureType = #("R32F","A8R8G8B8","R8G8B8","X8R8G8B8")


global g_ProcessName = "_GAMEPROCESS"
global g_MirrorTriggerName = "mirror_trigger"
global g_TutorialTriggerName = "tutorial_trigger"
global g_DoorTriggerName = "door_trigger"
global g_ElevatorTriggerName = "elevator_trigger"
global g_LasersTriggerName = "lasers_trigger"
global g_LifeTriggerName = "life_trigger"
global g_EnergyTriggerName = "energy_trigger"
global g_MirrorPieceTriggerName = "mirror_piece_trigger"
global g_BrokenMirrorTriggerName = "broken_mirror_trigger"

global g_OnEnter = "OnEnter"
global g_OnLeave = "OnLeave"
global g_OnTrigger = "OnTrigger"