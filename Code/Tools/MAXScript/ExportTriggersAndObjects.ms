fileIn "functions.ms" 

fn CreateTriggersXml LogicObjects DataPath Level =
(
	-- Creamos fichero triggers.xml
	local file1=createfile (DataPath+"\\"+g_XMLPath+Level+"\\"+"triggers.xml")
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<TriggerManager>\n" to:file1
	for i=1 to LogicObjects.count do
	(
		local l_GenerateLogic = getUserProp LogicObjects[i] "generate_logic"
		
		if (isLogic l_GenerateLogic) then
		(
			-- Extraemos nombre, posición y bounding box del trigger
			local l_Name = LogicObjects[i].name
			local l_Position = LogicObjects[i].position
			local l_Size = GetBoundingBoxSize LogicObjects[i]
			local l_Mask=""
			
			-- Lua code
			if (l_GenerateLogic=="Mirror") then		-- Triggers tipo Mirror
			(
				local l_TargetMirror = getUserProp LogicObjects[i] "target_mirror"
				local l_RotatedTargetMirror = getUserProp LogicObjects[i] "rotated_target_mirror"
				
				local l_OnEnter=g_ProcessName + ":" + g_MirrorTriggerName + "('" + g_OnEnter + "', '" + l_Name + "', '" + l_TargetMirror + "', '" + l_RotatedTargetMirror + "')"
				local l_OnLeave=g_ProcessName + ":" + g_MirrorTriggerName + "('" + g_OnLeave + "', '" + l_Name + "', '" + l_TargetMirror + "', '" + l_RotatedTargetMirror + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_MirrorTriggerName + "('" + g_OnTrigger + "', '" + l_Name + "', '" + l_TargetMirror + "', '" + l_RotatedTargetMirror + "')"
			)
			else if (l_GenerateLogic=="Tutorial") then	-- Triggers tipo Tutorial
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_TutorialTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_TutorialTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_TutorialTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)
			else if (l_GenerateLogic=="Door") then	-- Triggers tipo Door
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_DoorTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_DoorTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_DoorTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)	
			else if (l_GenerateLogic=="Elevator") then	-- Triggers tipo Elevator
			(
				local l_TriggerName = getUserProp LogicObjects[i] "elevator_trigger"
				
				if (l_TriggerName!=undefined and l_TriggerName!="undefined") then
				(
					local l_ObjName = "$" + l_TriggerName
					--format "trigger name = %\n" l_ObjName
					local l_Obj = execute (l_ObjName)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					
					if (l_Obj==undefined or l_Obj=="undefined") then
					(
						local l_MsgError = "AVISO: No se encuentra el trigger " + l_TriggerName + " asociado al ascensor !\n"
						Messagebox l_MsgError							
					)
					else
					(
						l_Position = l_Obj.position
						l_Size = GetBoundingBoxSize l_Obj
						
						--format "trigger position = %\n" l_Position
						--format "trigger size = %\n" l_Size
					)
				)
				
				local l_OnEnter  =  g_ProcessName + ":" + g_ElevatorTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_ElevatorTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_ElevatorTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)				
			else if (l_GenerateLogic=="Lasers") then	-- Triggers tipo Laser
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_LasersTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_LasersTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_LasersTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)				
			else if (l_GenerateLogic=="Life") then	-- Triggers tipo Life (Maquinas vida)
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_LifeTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_LifeTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_LifeTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)	
			else if (l_GenerateLogic=="Energy") then	-- Triggers tipo Energy (Maquinas energia)
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_EnergyTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_EnergyTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_EnergyTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)	
			else if (l_GenerateLogic=="Mirror piece") then	-- Triggers tipo trozo espejo final
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_MirrorPieceTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_MirrorPieceTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_MirrorPieceTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)				
			else if (l_GenerateLogic=="Broken mirror") then	-- Trigger tipo espejo roto final
			(
				local l_OnEnter  =  g_ProcessName + ":" + g_BrokenMirrorTriggerName + "('" + g_OnEnter + "','" + l_Name + "')"
				local l_OnLeave =  g_ProcessName + ":" + g_BrokenMirrorTriggerName + "('" + g_OnLeave + "','" + l_Name + "')"
				local l_OnTrigger= g_ProcessName + ":" + g_BrokenMirrorTriggerName + "('" + g_OnTrigger + "','" + l_Name + "')"
			)				
			else
			(
				local l_MsgError = "ERROR: Tipo de trigger desconocido en " + l_Name
				Messagebox l_MsgError
			)
			

			
			-- Create trigger entry in xml
			format "\t<trigger name=\"%\" pos=\"% % %\" size=\"% % %\" mask=\"%\" >\n" l_Name l_Position.x l_Position.z l_Position.y (l_Size.x/2) (l_Size.y/2) (l_Size.z/2) l_Mask to:file1
			format "\t\t<onEnter lua=\"%\" />\n" l_OnEnter to:file1
			format "\t\t<onLeave lua=\"%\" />\n" l_OnLeave to:file1
			format "\t\t<onTrigger lua=\"%\" />\n" l_OnTrigger to:file1
			format "\t</trigger>\n" to:file1			
		)
		else
		(
			if (isTrigger LogicObjects[i]) then
			(
				--format "Scripting trigger !! \n"
				-- Extraemos nombre, posición y bounding box del trigger
				local l_Name = LogicObjects[i].name
				local l_Position = LogicObjects[i].position
				local l_Size = GetBoundingBoxSize LogicObjects[i]
				local l_Mask=""
				
				local l_OnEnter  =  getUserProp LogicObjects[i] "onEnter_trigger"
				local l_OnLeave =  getUserProp LogicObjects[i] "onLeave_trigger"
				local l_OnTrigger= getUserProp LogicObjects[i] "onTrigger_trigger"

				-- Create trigger entry in xml
				format "\t<trigger name=\"%\" pos=\"% % %\" size=\"% % %\" mask=\"%\" >\n" l_Name l_Position.x l_Position.z l_Position.y (l_Size.x/2) (l_Size.y/2) (l_Size.z/2) l_Mask to:file1
				format "\t\t<onEnter lua=\"%\" />\n" l_OnEnter to:file1
				format "\t\t<onLeave lua=\"%\" />\n" l_OnLeave to:file1
				format "\t\t<onTrigger lua=\"%\" />\n" l_OnTrigger to:file1
				format "\t</trigger>\n" to:file1						
			)				
		)
	)
	format "</TriggerManager>\n" to:file1

	close file1	
)

fn CreateObjectFileXml LogicObjects DataPath Level =
(
	-- Creamos fichero logic_objects.xml
	local file1=createfile (DataPath+"\\"+g_XMLPath+Level+"\\"+"logic_objects.xml")
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<Logic_Objects>\n" to:file1
	for i=1 to LogicObjects.count do
	(
		local l_GenerateLogic = getUserProp LogicObjects[i] "generate_logic"
		local l_ActiveTrigger = getUserProp LogicObjects[i] "active_trigger"

		if (l_GenerateLogic!=false and l_GenerateLogic!=undefined and (l_ActiveTrigger==false or l_ActiveTrigger==undefined)) then
		(
			local l_Name = LogicObjects[i].name
			
			if (l_GenerateLogic=="Mirror") then
			(
					local l_RenderableObject = getUserProp LogicObjects[i] "renderable_object"
					if (l_RenderableObject == undefined or l_RenderableObject == "undefined" or l_RenderableObject=="") then
					(
						local l_MsgError = "AVISO: Es necesario asignar un renderable_object a " + l_Name + " !\n"
						Messagebox l_MsgError					
					)
					else
					(
						local l_ObjName = "$" + l_RenderableObject
						local l_Obj = execute (l_ObjName)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
						
						if l_Obj==undefined then
						(
							local l_MsgError = "AVISO: Objeto " + l_ObjName + " no encontrado para el tipo de logica Mirror!\n"
							Messagebox l_MsgError								
						)
						
						local l_Layer = getUserProp l_Obj "layer"
						if l_Layer == undefined or l_Layer==false then
						(
							l_Layer = g_ddlLayers[1]
						)	
						
						local l_SpawnPoint = getUserProp LogicObjects[i] "spawn_point"
						local l_AngleRot = getUserProp LogicObjects[i] "angle_rot"
						if (l_AngleRot=="" or l_AngleRot==undefined) then
						(
							l_AngleRot=0.0
						)
						
						local l_LinkedMirrors = getUserProp LogicObjects[i] "linked_mirrors"
						if (l_LinkedMirrors==undefined or l_LinkedMirrors=="undefined") then
						(
							l_LinkedMirrors = false
						)
						
						-- Escritura al xml
						format "\t<logic_object name=\"%\" logic_type=\"%\" renderable_object=\"%\" layer=\"%\" spawn_point=\"%\" angle_rot=\"%\" linked=\"%\" " l_Name l_GenerateLogic l_RenderableObject l_Layer l_SpawnPoint l_AngleRot  l_LinkedMirrors to:file1

						-- Coger vertices plane asociado al mirror
						local l_MirrorPlane = getUserProp LogicObjects[i] "mirror_plane"
						if (l_MirrorPlane != undefined and l_MirrorPlane != "undefined" and l_MirrorPlane!="") then
						(
							local l_PlaneName = "$" + l_MirrorPlane
							local l_Plane = execute (l_PlaneName)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
							local l_face1 = getFace l_Plane 1
							local l_face2 = getFace l_Plane 2
							
							local mat=inverse(l_Plane.transform)
							
							local l_v11 = getVert l_Plane l_face1.x
							l_v11=l_v11*mat
							l_v11=RHTranslationToLH l_v11
							
							local l_v12 = getVert l_Plane l_face1.y
							l_v12=l_v12*mat
							l_v12=RHTranslationToLH l_v12
							
							local l_v13 = getVert l_Plane l_face1.z
							l_v13=l_v13*mat
							l_v13=RHTranslationToLH l_v13
							
							local l_v21 = getVert l_Plane l_face2.x
							l_v21=l_v21*mat
							l_v21=RHTranslationToLH l_v21
							
							format " p0=\"% % %\" " l_v11.x l_v11.y l_v11.z to:file1
							format " p1=\"% % %\" " l_v12.x l_v12.y l_v12.z to:file1
							format " p2=\"% % %\" " l_v13.x l_v13.y l_v13.z to:file1
							format " p3=\"% % %\" " l_v21.x l_v21.y l_v21.z to:file1
						)
						else
						(
							format " use_plane =\"false\" " to:file1
						)
						
						-- Escritura al xml
						format "/>\n" to:file1
					)
					
			)
			else if (l_GenerateLogic=="Spawn Point") then
			(
				local l_Position_x = LogicObjects[i].position.x
				local l_Position_y = LogicObjects[i].position.z + 0.01
				local l_Position_z = LogicObjects[i].position.y			
				format "\t<logic_object name=\"%\" logic_type=\"%\" pos=\"% % %\"/>\n" l_Name l_GenerateLogic l_Position_x l_Position_y l_Position_z to:file1
			)
			else if (l_GenerateLogic=="Tutorial") then
			(
				local l_Position_x = LogicObjects[i].position.x
				local l_Position_y = LogicObjects[i].position.z + 0.01
				local l_Position_z = LogicObjects[i].position.y	
				local l_Texture = getUserProp LogicObjects[i] "texture"	
				local l_FSTexture = getUserProp LogicObjects[i] "FS_texture"
				
				format "\t<logic_object name=\"%\" logic_type=\"%\" xml_file=\"%\" auto=\"%\"/>\n" l_Name l_GenerateLogic l_Texture l_FSTexture to:file1
			)	
			else if (l_GenerateLogic=="Door") then
			(
				local l_PuertaMR = getUserProp LogicObjects[i] "puerta_MR"
				local l_PuertaME = getUserProp LogicObjects[i] "puerta_ME"
			
				if (l_PuertaMR == undefined or l_PuertaMR == "undefined" or l_PuertaMR=="" or l_PuertaME == undefined or l_PuertaME == "undefined" or l_PuertaME=="") then
				(
					local l_MsgError = "AVISO: Es necesario asignar un renderable_object a " + l_Name + " !\n"
					Messagebox l_MsgError					
				)
				else
				(
					local l_TiempoPuerta = getUserProp LogicObjects[i] "close_time"
					if (l_TiempoPuerta=="" or l_TiempoPuerta==undefined) then
					(
						l_AngleRot=0.0
					)	
					
					format "\t<logic_object name=\"%\" logic_type=\"%\" close_time=\"%\" " l_Name l_GenerateLogic l_TiempoPuerta to:file1
					
					local l_ObjName1 = "$" + l_PuertaMR
					--format "obj name 1 = %\n" l_ObjName1
					local l_Obj1 = execute (l_ObjName1)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj1!="" and l_Obj1!=undefined and l_Obj1!="undefined") then
					(
						local l_Layer1 = getUserProp l_Obj1 "layer"
						if l_Layer1 == undefined or l_Layer1==false then
						(
							l_Layer1 = g_ddlLayers[1]
						)
						format "renderable_object1=\"%\" layer1=\"%\" " l_PuertaMR l_Layer1 to:file1
					)						

					local l_ObjName2 = "$" + l_PuertaME
					--format "obj name 2 = %\n" l_ObjName2
					local l_Obj2 = execute (l_ObjName2)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj2!="" and l_Obj2!=undefined and l_Obj2!="undefined") then
					(						
						local l_Layer2 = getUserProp l_Obj2 "layer"
						if l_Layer2 == undefined or l_Layer2==false then
						(
							l_Layer2 = g_ddlLayers[1]
						)	
						format "renderable_object2=\"%\" layer2=\"%\" " l_PuertaME l_Layer2 to:file1							
					)
					
					local l_PuertaAuto = getUserProp LogicObjects[i] "puerta_auto"
					if (l_PuertaAuto!="" and l_PuertaAuto!=undefined and l_PuertaAuto!="undefined") then
					(
						format "auto=\"%\" " l_PuertaAuto  to:file1
					)
				
					local l_MainSwitch = getUserProp LogicObjects[i] "interruptor"
					if (l_MainSwitch!="" and l_MainSwitch!=undefined and l_MainSwitch!="undefined") then
					(
						format "switch=\"%\" " l_MainSwitch  to:file1
					)
					
					--format "\t<logic_object name=\"%\" logic_type=\"%\" renderable_object1=\"%\" layer1=\"%\" renderable_object2=\"%\" layer2=\"%\" close_time=\"%\" " l_Name l_GenerateLogic l_PuertaMR l_Layer1 l_PuertaME l_Layer2  l_TiempoPuerta to:file1
					format "/>\n" to:file1
				)
			)	
			else if (l_GenerateLogic=="Elevator") then
			(
				format "\t<logic_object name=\"%\" logic_type=\"%\" " l_Name l_GenerateLogic to:file1
				local l_AnimModel1 = LogicObjects[i].name
				local l_Layer1 = getUserProp LogicObjects[i] "layer"
				format "animated_model_instance=\"%\" layer=\"%\" " l_AnimModel1 l_Layer1 to:file1
				
				local l_LeftDoor = getUserProp LogicObjects[i] "puerta_izq"
				local l_RightDoor = getUserProp LogicObjects[i] "link_mundo"
				--format "left_door_static_mesh=\"%\" right_door_static_mesh=\"%\" " l_LeftDoor l_RightDoor to:file1
				format "door_physic_mesh=\"%\" link=\"%\" " l_LeftDoor l_RightDoor to:file1
				
				format "/>\n" to:file1
			)
			else if (l_GenerateLogic=="Fan") then
			(
				local l_PuertaMR = getUserProp LogicObjects[i] "puerta_MR"
				--local l_PuertaME = getUserProp LogicObjects[i] "puerta_ME"
			
				if (l_PuertaMR == undefined or l_PuertaMR == "undefined" or l_PuertaMR=="") then
				(
					local l_MsgError = "AVISO: Es necesario asignar un renderable_object a " + l_Name + " !\n"
					Messagebox l_MsgError					
				)
				else
				(
					local l_TiempoPuerta = getUserProp LogicObjects[i] "close_time"
					if (l_TiempoPuerta=="" or l_TiempoPuerta==undefined) then
					(
						l_AngleRot=0.0
					)	
					
					format "\t<logic_object name=\"%\" logic_type=\"%\" spin_vel=\"%\" " l_Name l_GenerateLogic l_TiempoPuerta to:file1
					
					local l_ObjName1 = "$" + l_PuertaMR
					--format "obj name 1 = %\n" l_ObjName1
					local l_Obj1 = execute (l_ObjName1)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj1!="" and l_Obj1!=undefined and l_Obj1!="undefined") then
					(
						local l_Layer1 = getUserProp l_Obj1 "layer"
						if l_Layer1 == undefined or l_Layer1==false then
						(
							l_Layer1 = g_ddlLayers[1]
						)
						format "renderable_object=\"%\" layer=\"%\" " l_PuertaMR l_Layer1 to:file1
					)						

					--local l_ObjName2 = "$" + l_PuertaME
					--format "obj name 2 = %\n" l_ObjName2
					--local l_Obj2 = execute (l_ObjName2)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					--if (l_Obj2!="" and l_Obj2!=undefined and l_Obj2!="undefined") then
					--(						
					--	local l_Layer2 = getUserProp l_Obj2 "layer"
					--	if l_Layer2 == undefined or l_Layer2==false then
					--	(
					--		l_Layer2 = g_ddlLayers[1]
					--	)	
					--	format "renderable_object2=\"%\" layer2=\"%\" " l_PuertaME l_Layer2 to:file1							
					--)
				
					--format "\t<logic_object name=\"%\" logic_type=\"%\" renderable_object1=\"%\" layer1=\"%\" renderable_object2=\"%\" layer2=\"%\" close_time=\"%\" " l_Name l_GenerateLogic l_PuertaMR l_Layer1 l_PuertaME l_Layer2  l_TiempoPuerta to:file1
					format "/>\n" to:file1
				)
			)
			else if (l_GenerateLogic=="Lasers") then
			(
				format "\t<logic_object name=\"%\" logic_type=\"%\" " l_Name l_GenerateLogic to:file1
				format "/>\n" to:file1
			)
			else if (l_GenerateLogic=="Life" or l_GenerateLogic=="Energy") then
			(
				format "\t<logic_object name=\"%\" logic_type=\"%\" " l_Name l_GenerateLogic to:file1
				
				local l_MeshName = getUserProp LogicObjects[i] "mesh_name"
				if (l_MeshName != undefined and l_MeshName != "undefined" and l_MeshName!="") then
				(
					local l_ObjName1 = "$" + l_MeshName

					local l_Obj1 = execute (l_ObjName1)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj1!="" and l_Obj1!=undefined and l_Obj1!="undefined") then
					(
						local l_Layer1 = getUserProp l_Obj1 "layer"
						if l_Layer1 == undefined or l_Layer1==false then
						(
							l_Layer1 = g_ddlLayers[1]
						)
						format "static_mesh=\"%\" layer=\"%\" " l_MeshName l_Layer1 to:file1
						
						local l_Light = getUserProp LogicObjects[i] "life_light"
						format "light=\"%\" " l_Light to:file1
					)						
				
				)				
				
				local l_TextureON = getUserProp LogicObjects[i] "texture_on"
				local l_TextureOFF = getUserProp LogicObjects[i] "texture_off"
				
				if l_TextureON!=undefined and l_TextureON!="undefined" and l_TextureON!="" and l_TextureON!=false then
				(
					local l_TextON = g_MeshesPath+l_TextureON
					format " on_texture=\"%\"" l_TextON to:file1
				)

				if l_TextureOFF!=undefined and l_TextureOFF!="undefined" and l_TextureOFF!="" and l_TextureOFF!=false then
				(
					local l_TextOFF = g_TexturesPath+l_TextureOFF
					format " off_texture=\"%\"" l_TextOFF to:file1
				)		
				
				format "/>\n" to:file1
			)
			else if (l_GenerateLogic=="Mirror piece") then
			(
				format "\t<logic_object name=\"%\" logic_type=\"%\" " l_Name l_GenerateLogic to:file1
				
				local l_MeshName = getUserProp LogicObjects[i] "mesh_name"
				if (l_MeshName != undefined and l_MeshName != "undefined" and l_MeshName!="") then
				(
					local l_ObjName1 = "$" + l_MeshName

					local l_Obj1 = execute (l_ObjName1)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj1!="" and l_Obj1!=undefined and l_Obj1!="undefined") then
					(
						local l_Layer1 = getUserProp l_Obj1 "layer"
						if l_Layer1 == undefined or l_Layer1==false then
						(
							l_Layer1 = g_ddlLayers[1]
						)
						format "renderable_object=\"%\" layer=\"%\" " l_MeshName l_Layer1 to:file1
					)						
				
				)				
				
				format "/>\n" to:file1
			)
			else if (l_GenerateLogic=="Broken mirror") then
			(
				format "\t<logic_object name=\"%\" logic_type=\"%\" " l_Name l_GenerateLogic to:file1
				
				local l_MeshName = getUserProp LogicObjects[i] "broken_mirror_mesh"
				if (l_MeshName != undefined and l_MeshName != "undefined" and l_MeshName!="") then
				(
					local l_ObjName1 = "$" + l_MeshName

					local l_Obj1 = execute (l_ObjName1)		-- obtiene objeto 3dsmax a partir de su nombre (cogido de la propiedad del objeto)
					if (l_Obj1!="" and l_Obj1!=undefined and l_Obj1!="undefined") then
					(
						local l_Layer1 = getUserProp l_Obj1 "layer"
						if l_Layer1 == undefined or l_Layer1==false then
						(
							l_Layer1 = g_ddlLayers[1]
						)
						format "renderable_object=\"%\" layer=\"%\" " l_MeshName l_Layer1 to:file1
					)						
				)				
				
				local l_TriggerName = getUserProp LogicObjects[i] "final_mirror_object"
				if (l_TriggerName != undefined and l_TriggerName != "undefined" and l_TriggerName!="") then
				(
					format "final_mirror_trigger=\"%\" " l_TriggerName to:file1
				)
				
				local l_OtherMirrorName = getUserProp LogicObjects[i] "other_world_mirror"
				if (l_OtherMirrorName != undefined and l_OtherMirrorName != "undefined" and l_OtherMirrorName!="") then
				(
					format "link=\"%\" " l_OtherMirrorName to:file1
				)				
				format "/>\n" to:file1
			)			
			else
			(
				local l_MsgError = "AVISO: Tipo de logica " + l_GenerateLogic + "desconocida!\n"
				Messagebox l_MsgError				
			)
		)
	)
	format "</Logic_Objects>\n" to:file1

	close file1	
)

fn CreateParticlesXml ParticlesObjects DataPath Level =
(
	-- Creamos fichero logic_objects.xml
	local file1=createfile (DataPath+"\\"+g_XMLPath+Level+"\\"+"particles.xml")
	local l_CoreNames=#()
	local l_InstancesPos=#()
	local l_InstanceId=#()
	local l_InstanceDesp=#()
	
	--format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file1
	format "<ParticleManager>\n" to:file1
		
	format "\t<ParticleEmitters>\n" to:file1
	for i=1 to ParticlesObjects.count do
	(
		local l_GenerateParticles = getUserProp ParticlesObjects[i] "generate_particles"
		if (l_GenerateParticles!=false and l_GenerateParticles!=undefined) then
		(
			local l_Name = getUserProp ParticlesObjects[i] "name"
			if((findItem l_CoreNames l_Name)==0) then
			(
				format "\t\t<PointEmitter type=\"%\" name=\"%\" gravity=\"%\" " l_GenerateParticles l_Name (getUserProp ParticlesObjects[i] "gravity") to:file1
				format "life_time_min=\"%\" life_time_max=\"%\" emit_rate_min=\"%\"\n"(getUserProp ParticlesObjects[i] "life_time_min") (getUserProp ParticlesObjects[i] "life_time_max") (getUserProp ParticlesObjects[i] "emit_rate_min") to:file1
				
				--g_TexturesPath
				local l_FileName = (getUserProp ParticlesObjects[i] "tex")	
				local l_newFileName=DataPath+"\\"+g_TexturesPath+(filenameFromPath l_FileName)
				format "File name: %\n%\n" l_FileName l_newFileName
				copyFile l_FileName l_newFileName
				
				format "\t\t\temit_rate_max=\"%\" tex=\"%\" angle=\"2.0\">\n" (getUserProp ParticlesObjects[i] "emit_rate_max") (g_TexturesPath+(filenameFromPath l_FileName)) to:file1
				local l_aux = (filterString (getUserProp ParticlesObjects[i] "color1") ",[]")
				format "\t\t\t<color time=\"0.0\" color1=\"% % % %\" " l_aux[1] l_aux[2] l_aux[3] l_aux[4] to:file1
				l_aux = (filterString (getUserProp ParticlesObjects[i] "color2") ",[]")
				format "color2=\"% % % %\"/>\n" l_aux[1] l_aux[2] l_aux[3] l_aux[4] to:file1
				
				l_aux = (filterString (getUserProp ParticlesObjects[i] "dir1") ",[]")
				format "\t\t\t<direction time=\"0.0\" dir1=\"% % %\" " l_aux[1] l_aux[2] l_aux[3] to:file1
				l_aux = (filterString (getUserProp ParticlesObjects[i] "dir2") ",[]")
				format "dir2=\"% % %\"/>\n" l_aux[1] l_aux[2] l_aux[3] to:file1
				
				format "\t\t\t<size time=\"0.0\" mins=\"%\" maxs=\"%\"/>\n" (getUserProp ParticlesObjects[i] "size1") (getUserProp ParticlesObjects[i] "size2") to:file1
				
				l_aux = (filterString (getUserProp ParticlesObjects[i] "acel1") ",[]")
				format "\t\t\t<aceleration time=\"0.0\" acel1=\"% % %\" " l_aux[1] l_aux[2] l_aux[3] to:file1
				l_aux = (filterString (getUserProp ParticlesObjects[i] "acel2") ",[]")
				format "acel2=\"% % %\"/>\n" l_aux[1] l_aux[2] l_aux[3] to:file1
				
				format "\t\t</PointEmitter>\n" to:file1
				append l_CoreNames l_Name
			)
			
			local l_desp = (filterString (getUserProp ParticlesObjects[i] "despl") ",[]")
			append l_InstanceDesp (l_desp[1] as float)  
			append l_InstanceDesp (l_desp[2] as float)
			append l_InstanceDesp (l_desp[3] as float)
			
			append l_InstancesPos ParticlesObjects[i].position.x
			append l_InstancesPos ParticlesObjects[i].position.y
			append l_InstancesPos ParticlesObjects[i].position.z
			
			append l_InstanceId l_Name
		)
	)
	format "\t</ParticleEmitters>\n" to:file1
	
	format"\t<Instances>\n" to:file1
	for i=1 to l_InstanceId.count do
	(
		local l_pos = point3 l_InstancesPos[(i-1)+1] l_InstancesPos[(i-1)+2] l_InstancesPos[(i-1)+3]
		local l_despl = point3 l_InstanceDesp[(i-1)+1] l_InstanceDesp[(i-1)+2] l_InstanceDesp[(i-1)+3]
		l_despl.x = l_despl.x + l_pos.x
		l_despl.y = l_despl.y + l_pos.z
		l_despl.z = l_despl.z + l_pos.y
		--format "pos1=%\n" l_pos
		--format "pos2=%\n" l_despl
		
		format "\t\t<Instance id=\"%\" pos1=\"% % %\" pos2=\"% % %\"" l_InstanceId[i] l_pos.x l_pos.z l_pos.y l_despl.x l_despl.y l_despl.z to:file1
		local l_GenerateParticles = getUserProp ParticlesObjects[i] "generate_particles"
		if(l_GenerateParticles!="point" and l_GenerateParticles!="line")then
		(
			format " size=\"%\"/>\n" (getUserProp ParticlesObjects[i] "size") to:file1
		)
		else
		(
			format "/>\n" to:file1
		)
	)
	format"\t</Instances>\n" to:file1
	
	format "</ParticleManager>\n" to:file1

	close file1
)

fn PrintObjectsDebugInfo LogicObjects =
(
	local l_NumObjects = LogicObjects.count
	local l_NumTriggers = 0
	
	for i=1 to l_NumObjects do
	(
		local l_GenerateLogic = getUserProp LogicObjects[i] "generate_logic"
		if (isLogic l_GenerateLogic) then
		(
			format "Trigger[%] : % \n" (l_NumTriggers+1) LogicObjects[i].name
			l_NumTriggers = l_NumTriggers + 1
		)
		else
		(
			format "Logic Object[%] : % \n" (i-l_NumTriggers) LogicObjects[i]
		)
	)
	
	format "DEBUG> Num. triggers = %\n" l_NumTriggers
	format "DEBUG> Num. logic objects = %\n" (l_NumObjects	- l_NumTriggers)
	
	format "\n"
)

fn ExportObjects DataPath Level Overwrite =
(
	local l_LogicObjects=#()
	local l_Particles=#()
	local l_Objs = $*
	local l_NumObjects = l_Objs.count
	
	for i= 1 to l_NumObjects do
	(
		l_Object = l_Objs[i]
		
		local l_GenerateLogic = getUserProp l_Object "generate_logic"
		if (l_GenerateLogic!=false and l_GenerateLogic!=undefined) then
		(
			local pers = isCharacter l_Object
			if (pers==false) then
			(
				-- Objeto tiene propiedades logicas (sin incluir los enemigos)
				append l_LogicObjects l_Object
			)
		)
		else
		(
			local l_GenerateParticle = getUserProp l_Object "generate_particles"
			if(l_GenerateParticle!=false and l_GenerateParticle!=undefined)then
			(
				local pers1 =isCharacter l_Object
				if(pers1==false)then
				(
					append l_Particles l_Object
				)
			)
		)
		
		local l_ActiveTrigger = getUserProp l_Object "active_trigger"
		if (l_ActiveTrigger==true) then
		(
			--format "append trigger !!\n"
			append l_LogicObjects l_Object
		)
	)

	--PrintObjectsDebugInfo l_LogicObjects			-- DEGUG !!!
	CreateObjectFileXml l_LogicObjects DataPath Level
	CreateTriggersXml l_LogicObjects DataPath Level
	if(Overwrite)then
	(
		CreateParticlesXml l_Particles DataPath Level
	)
)