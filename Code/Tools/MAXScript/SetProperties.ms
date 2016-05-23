fileIn "functions.ms"

global texture_path=undefined 

rollout GetSetObjProperties "GetSetObjProperties" width:1133 height:711
(
	GroupBox grpRender "Render" pos:[18,8] width:254 height:166
	checkbox chkExport "Export" pos:[34,33] width:64 height:24
	checkbox chkVisible "Visible" pos:[34,136] width:64 height:24
	checkbox chkLayerRender "Layer" pos:[34,96] width:75 height:24
	dropdownList ddlExport "" pos:[120,31] width:135 height:21 items:#("mesh_instance", "animated_model_instance")
	dropdownList ddlLayerRender "" pos:[120,96] width:150 height:21 items:#("solid","solid_MR","solid_ME","alpha_blend_objects_MR","alpha_blend_objects_ME","particles")
	label lblAnim "Anim. Model:" pos:[42,62] width:64 height:16
	checkbox chkCastShadows "Cast shadows" pos:[128,136] width:120 height:24
	GroupBox grpPhysics "Physics" pos:[16,184] width:256 height:98
	checkbox chkGeneratePhysics "Generate" pos:[34,209] width:76 height:21
	checkbox chkLayerPhysics "Layer" pos:[34,244] width:64 height:21
	dropdownList ddlGeneratePhysics "" pos:[120,207] width:135 height:21 items:#("triangle_mesh", "convex_hull", "bounding_box", "bounding_sphere", "plane", "capsule")
	dropdownList ddlLayerPhysics "" pos:[120,244] width:135 height:21 items:#("logic", "pushable", "pullable", "breakable", "collision")
	GroupBox grpLogic "Logic" pos:[290,9] width:545 height:607
	checkbox chkGenerateLogic "Generate" pos:[310,27] width:76 height:21
	dropdownList ddlGenerateLogic "" pos:[397,25] width:135 height:21 items:#("Player", "Soldier", "Monster", "Mirror", "Spawn Point", "Tutorial", "Door", "Elevator", "Fan", "Lasers", "Life", "Energy", "Mirror piece", "Broken mirror")
	button btnGet "Get" pos:[328,640] width:85 height:24
	button btnSet "Set" pos:[664,640] width:85 height:24
	edittext editAnim "" pos:[120,56] width:133 height:23	
	
	edittext edtSpawnPoint "" pos:[401,120] width:121 height:23
	label lbl8 "Spawn point espejo:" pos:[323,120] width:69 height:25
	label lbl9 "Espejo destino:" pos:[323,153] width:78 height:21
	edittext edtTargetMirror "" pos:[400,145] width:121 height:23
	label lbl7 "Malla espejo:" pos:[323,89] width:84 height:24
	edittext edtRenderObj "" pos:[400,86] width:122 height:24
	GroupBox grp7 "Mirror Properties:" pos:[304,63] width:240 height:327
	GroupBox grp8 "Rotable" pos:[312,221] width:224 height:154
	label lbl19 "Espejo destino rotado:" pos:[325,243] width:72 height:24
	edittext edtRotatedMirror "" pos:[405,243] width:120 height:24
	label lbl10 "Angulo rotación espejo:" pos:[325,275] width:130 height:25
	edittext edtAngleRot "" pos:[453,275] width:72 height:24
	checkbox chk_2mundos "Rotación afecta espejos ambos mundos" pos:[317,348] width:208 height:16
	GroupBox grp6 "Tutorial:" pos:[301,395] width:238 height:84
	label lblTextura "Fichero XML:" pos:[315,422] width:54 height:16
	edittext edtTextura "" pos:[390,418] width:140 height:23
	checkbox chkFSTexture "Automatico" pos:[315,449] width:159 height:15
	GroupBox grpParticles "Particles" pos:[855,7] width:254 height:536
	checkbox chkGenerateParticles "Generate" pos:[874,24] width:76 height:21	
	
	edittext edtParticleName "Core Name:" pos:[960,24] width:120 height:21
	dropdownList ddlGenerateParticles "" pos:[960,49] width:135 height:21 items:#("point", "line", "plane", "box", "sphere", "circle", "torus")
	checkbox chkGravityParticles "Gravity" pos:[874,49] width:76 height:21
	spinner spnParticleMinTime "Min Life Time:" pos:[929,73] width:42 height:16
	spinner spnParticleMaxTime "Max Life Time:" pos:[1050,74] width:45 height:16
	spinner spnParticleMinRate "Frecuen. Min:" pos:[934,99] width:45 height:16 range:[0.1,100,1]
	spinner spnParticleMaxRate "Frecuen. Max:" pos:[1050,99] width:45 height:16 range:[0.1,100,1]
	button btn1 "Texture Path" pos:[924,124] width:129 height:20
	label lbl1 "Path" pos:[874,149] width:200 height:40
	GroupBox grpColor "Color:(RGBA min - RGBA max) 0.0 - 1.0" pos:[864,174] width:240 height:70
	spinner spnParticleR1 "R1:" pos:[885,194] width:37 height:16 range:[0,1,0]
	spinner spnParticleG1 "G1:" pos:[943,194] width:37 height:16 range:[0,1,0]
	spinner spnParticleB1 "B1:" pos:[1001,194] width:37 height:16 range:[0,1,0]
	spinner spnParticleA1 "A1:" pos:[1059,194] width:37 height:16 range:[0,1,0]
	spinner spnParticleR2 "R2:" pos:[885,219] width:37 height:16 range:[0,1,0]
	spinner spnParticleG2 "G2:" pos:[943,219] width:37 height:16 range:[0,1,0]
	spinner spnParticleB2 "B2:" pos:[1001,219] width:37 height:16 range:[0,1,0]
	spinner spnParticleA2 "A2:" pos:[1059,219] width:37 height:16 range:[0,1,0]
	GroupBox grpDirection "Direction:(XYZ min - XYZ max) valores -/+" pos:[864,244] width:240 height:70
	spinner spnParticleX1 "X1:" pos:[885,264] width:50 height:16 range:[-100,100,0]
	spinner spnParticleY1 "Y1:" pos:[965,264] width:50 height:16 range:[-100,100,0]
	spinner spnParticleZ1 "Z1:" pos:[1045,264] width:50 height:16 range:[-100,100,0]
	spinner spnParticleX2 "X2:" pos:[885,289] width:50 height:16 range:[-100,100,0]
	spinner spnParticleY2 "Y2:" pos:[965,289] width:50 height:16 range:[-100,100,0]
	spinner spnParticleZ2 "Z2:" pos:[1045,289] width:50 height:16 range:[-100,100,0]
	GroupBox grpAceleration "Aceleracion:(XYZ min - XYZ max) valores -/+" pos:[864,314] width:240 height:70
	spinner spnParticleAcelX1 "X1:" pos:[885,334] width:50 height:16 range:[-100,100,0]
	spinner spnParticleAcelY1 "Y1:" pos:[965,334] width:50 height:16 range:[-100,100,0]
	spinner spnParticleAcelZ1 "Z1:" pos:[1045,334] width:50 height:16 range:[-100,100,0]
	spinner spnParticleAcelX2 "X2:" pos:[885,359] width:50 height:16 range:[-100,100,0]
	spinner spnParticleAcelY2 "Y2:" pos:[965,359] width:50 height:16 range:[-100,100,0]
	spinner spnParticleAcelZ2 "Z2:" pos:[1045,359] width:50 height:16 range:[-100,100,0]
	GroupBox grpSize "Size:(Size min - Size max) val positivos > 0" pos:[864,384] width:240 height:40
	spinner spnParticleSize1 "Size min:" pos:[915,404] width:50 height:16 range:[0.01,100,0.1]
	spinner spnParticleSize2 "Size max:" pos:[1030,404] width:50 height:16 range:[0.01,100,0.1]
	spinner spnParticleInstanceSize "Instance Size: (P.E. Radio Circle)" pos:[1045,454] width:50 height:16 range:[0,100,0]
	GroupBox grpDesplazamiento "Desplazamiento Posicion 2:" pos:[864,483] width:240 height:40
	spinner spnParticleDespX1 "X1:" pos:[885,503] width:50 height:16 range:[-100,100,0]
	spinner spnParticleDespY1 "Y1:" pos:[965,503] width:50 height:16 range:[-100,100,0]
	spinner spnParticleDespZ1 "Z1:" pos:[1045,503] width:50 height:16 range:[-100,100,0]
	label lbl17 "Plane espejo:" pos:[323,183] width:76 height:13
	edittext edtPlane "" pos:[399,179] width:125 height:22
	--label lbl18 "Plane espejo rot:" pos:[324,312] width:90 height:16
	--edittext edtPlaneRot "" pos:[426,309] width:97 height:21
	label lblDoorMR "Door/Fan 1:" pos:[573,52] width:59 height:18
	label lblDoorME "Door/Fan 2:" pos:[573,87] width:59 height:18
	edittext edtPuertaMR "" pos:[647,47] width:160 height:24
	edittext edtPuertaME "" pos:[647,79] width:160 height:24
	label lbl32 "T. auto cerrado puertas (seg) / Vel. giro ventilador (vueltas/seg):" pos:[572,119] width:175 height:32
	edittext edtTiempoPuerta "" pos:[742,119] width:64 height:24
	GroupBox grp37 "Ascensor:" pos:[563,227] width:261 height:158
	label lbl33 "Trigger:" pos:[583,252] width:59 height:18
	label lbl34 "Puerta:" pos:[583,287] width:67 height:16
	label lbl35 "Ascensor otro mundo:" pos:[583,319] width:120 height:16
	edittext edtCabina "" pos:[657,247] width:160 height:24
	edittext edtPuertaIzq "" pos:[657,279] width:160 height:24
	edittext edtPuertaDer "" pos:[695,311] width:120 height:24
	label lbl36 "Orientación:" pos:[583,351] width:136 height:22
	edittext edtTiempoSubida "" pos:[746,349] width:64 height:24
	GroupBox grp38 "Puertas / Ventiladores:" pos:[562,25] width:256 height:185
	checkbox chkDoorsAuto "Puertas automaticas" pos:[571,160] width:201 height:14
	label lblSwitch "Interruptor Principal:" pos:[571,180] width:108 height:19
	edittext edtMainSwitch "" pos:[688,176] width:119 height:22
	label lblOnEnter "OnEnter:" pos:[24,358] width:54 height:17
	label lblOnLeave "OnLeave:" pos:[24,390] width:48 height:16
	label lblOnTrigger "OnTrigger:" pos:[24,422] width:56 height:16
	edittext edtOnEnter "" pos:[88,350] width:176 height:24
	edittext edtOnLeave "" pos:[88,382] width:176 height:24
	edittext edtOnTrigger "" pos:[88,414] width:176 height:24
	GroupBox grp15 "Triggers:" pos:[16,297] width:256 height:155
	checkbox chkActiveTrigger "Activar Trigger" pos:[28,320] width:132 height:17
	label lbl42 "Malla punto vida/energia/trozo espejo:" pos:[570,422] width:129 height:25
	edittext edtLifeEnergyMesh "" pos:[701,421] width:119 height:25
	label lbl44 "Textura ON:" pos:[571,458] width:87 height:17
	edittext edtLifeEnergy_TextureON "" pos:[659,458] width:159 height:20
	label lbl45 "Textura OFF:" pos:[572,488] width:75 height:14
	edittext edtLifeEnergy_TextureOFF "" pos:[659,486] width:158 height:20
	GroupBox grp32 "Life / Energy / Mirror Piece:" pos:[564,394] width:266 height:149
	label lbl47 "Luz:" pos:[577,517] width:57 height:16
	edittext edtLight "" pos:[660,512] width:158 height:20
	label lbl26 "Malla espejo roto:" pos:[305,508] width:90 height:19
	label lbl27 "Trigger espejo final:" pos:[306,540] width:102 height:15
	edittext edtBrokenMirror "" pos:[418,503] width:112 height:23
	edittext edtFinalMirror "" pos:[420,539] width:109 height:21
	GroupBox grp17 "Espejo roto:" pos:[300,491] width:240 height:117
	label lbl28 "Espejo roto otro mundo:" pos:[305,574] width:106 height:29
	editText edtEspejoRotoOtroMundo "" pos:[424,571] width:103 height:25
	
	
	-- Get Button	
	on btnGet pressed do
	(
		--format "Get Object properties\n"
		
		
		-- Export type dropdown list
		local l_ExportType = getUserProp $ "export_type"
		local l_AnimModel = getUserProp $ "anim_model"
		if l_ExportType==false then
		(
			chkExport.checked = false
			ddlExport.Selection = 1
			--format "GET::ExportType = false\n" 
		)
		else 
		(
			chkExport.checked = true
			-- by default
			ddlExport.Selection = 1
			for i=1 to ddlExport.items.count do
			(
				if ddlExport.items[i] == l_ExportType then
					ddlExport.Selection = i
			)
			if (l_AnimModel != undefined) then
			(
				editAnim.text = l_AnimModel
			)
			--format "GET::ExportType = %\n" ddlExport.items[ddlExport.Selection]
			--format "GET::Anim.Model = %\n" l_AnimModel
		)
		
		
	
		-- Visible
		local l_Visible = getUserProp $ "visible"
		if l_Visible==false then
		(
			chkVisible.checked = false
		)
		else
		(
			chkVisible.checked = true
		)
		--format "GET::Visible = %\n" chkVisible.checked
		
		
		
		-- Cast shadows
		local l_CastShadows = getUserProp $ "cast_shadows"
		if l_CastShadows==true or l_CastShadows==undefined then
		(
			chkCastShadows.checked = true
		)
		else
		(
			chkCastShadows.checked = false
		)
		--format "GET::Cast Shadows = %\n" chkCastShadows.checked
		
		
		-- Layer dropdown list
		local l_Layer = getUserProp $ "layer"
		if l_Layer==false then
		(
			chkLayerRender.checked = false
			ddlLayerRender.Selection = 1
			--format "GET::Layer = false\n" 
		)
		else 
		(
			chkLayerRender.checked = true
			-- by default
			ddlLayerRender.Selection = 1
			for i=1 to ddlLayerRender.items.count do
			(
				if ddlLayerRender.items[i] == l_Layer then
					ddlLayerRender.Selection = i
			)
			--format "GET::Layer = %\n" ddlLayerRender.items[ddlLayerRender.Selection]
		)
		
		-- Physics 
		-- Generate
		local l_GeneratePhysics = getUserProp $ "generate_physics"
		if l_GeneratePhysics==false then
		(
			chkGeneratePhysics.checked = false
			ddlGeneratePhysics.Selection = 1
			--format "GET::Generate Physics = false\n" 
		) 
		else
		(
			chkGeneratePhysics.checked = true
			-- by default
			ddlGeneratePhysics.Selection = 1
			for i=1 to ddlGeneratePhysics.items.count do
			(
				if ddlGeneratePhysics.items[i] == l_GeneratePhysics then
				(
					ddlGeneratePhysics.Selection = i
				)
			)		
			--format "GET::Generate Physics = %\n" ddlGeneratePhysics.items[ddlGeneratePhysics.Selection]
		)
		
		-- Layer
		local l_LayerPhysics = getUserProp $ "layer_physics"
		if l_LayerPhysics==false or l_LayerPhysics==undefined then
		(
			chkLayerPhysics.checked = false
			ddlLayerPhysics.Selection = 1
			--format "GET::Layer Physics = false\n" 
		) 
		else
		(
			chkLayerPhysics.checked = true
			-- by default
			ddlLayerPhysics.Selection = 1
			for i=1 to ddlLayerPhysics.items.count do
			(
				if ddlLayerPhysics.items[i] == l_LayerPhysics then
				(
					ddlLayerPhysics.Selection = i
				)
			)		
			--format "GET::Layer Physics = %\n" ddlLayerPhysics.items[ddlLayerPhysics.Selection]
		)
		
		-- Logic
		local l_GenerateLogic = getUserProp $ "generate_logic"
		if l_GenerateLogic==false or l_GenerateLogic==undefined then
		(
			chkGenerateLogic.checked = false
			ddlGenerateLogic.Selection = 1
			--format "GET::Generate Logic = false\n" 
		)
		else 
		(
			chkGenerateLogic.checked = true
			-- by default
			ddlGenerateLogic.Selection = 1
			for i=1 to ddlGenerateLogic.items.count do
			(
				if ddlGenerateLogic.items[i] == l_GenerateLogic then
					ddlGenerateLogic.Selection = i
			)
			--format "GET::Generate Logic = %\n" ddlGenerateLogic.items[ddlGenerateLogic.Selection]
			
			-- Mirror
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Mirror") then
			(
				local l_RenderableObject = getUserProp $ "renderable_object"
				local l_TargetMirror = getUserProp $ "target_mirror"
				local l_RotatedTargetMirror = getUserProp $ "rotated_target_mirror"
				local l_AngleRot = getUserProp $ "angle_rot"
				local l_SpawnPoint = getUserProp $ "spawn_point"
				local l_LinkedMirrors = getUserProp $ "linked_mirrors"
				local l_MirrorPlane = getUserProp $ "mirror_plane"
				--local l_MirrorPlaneRot = getUserProp $ "mirror_plane_rot"
				
				if l_RenderableObject != "" and l_RenderableObject!=undefined then
				(
					edtRenderObj.text = l_RenderableObject
					--format "GET::Renderable Object = %\n" l_RenderableObject
				)
				if l_TargetMirror!="" and l_TargetMirror!=undefined then
				(
					edtTargetMirror.text = l_TargetMirror
					--format "GET::Target Mirror = %\n" l_TargetMirror
				)
				if l_RotatedTargetMirror!="" and l_RotatedTargetMirror!=undefined then
				(
					edtRotatedMirror.text = l_RotatedTargetMirror
					--format "GET::Rotated Mirror = %\n" l_RotatedTargetMirror
				)				
				if l_AngleRot!="" and l_AngleRot!=undefined then
				(
					edtAngleRot.text = l_AngleRot as string
					--format "GET::Angle Rot = %\n" l_AngleRot
				)				
				if l_SpawnPoint!="" and l_SpawnPoint!=undefined then
				(
					edtSpawnPoint.text = l_SpawnPoint
					--format "GET::Spawn Point = %\n" l_SpawnPoint
				)
				if l_LinkedMirrors!="" and l_LinkedMirrors!=undefined then
				(
					if (l_LinkedMirrors==true) then
						chk_2mundos.checked = true
					else
						chk_2mundos.checked = false
					
					--format "GET::Ambos mundos = %\n" l_LinkedMirrors
				)		
				if l_MirrorPlane != "" and l_MirrorPlane!=undefined then
				(
					edtPlane.text = l_MirrorPlane
					--format "GET::Mirror plane = %\n" l_MirrorPlane
				)		
				--if l_MirrorPlaneRot != "" and l_MirrorPlaneRot!=undefined then
				--(
				--	edtPlaneRot.text = l_MirrorPlaneRot
					--format "GET::Mirror plane rot = %\n" l_MirrorPlaneRot
				--)					
			)
			
			-- Tutorial
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Tutorial") then
			(
				local l_Texture = getUserProp $ "texture"
				if l_Texture != "" and l_Texture!=undefined then
				(
					edtTextura.text = l_Texture
					--format "GET::Texture = %\n" l_Texture
					
					local l_FS = getUserProp $ "FS_texture"
					if l_FS != "" and l_FS!=undefined then
					(
						--format "GET::Una sola vez = %\n" l_FS
						if (l_FS == true) then
							chkFSTexture.checked = true
						else
							chkFSTexture.checked = false
					)
				)
			)
			
			-- Puertas, Ventiladores
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Door" or ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Fan" or ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Elevator") then
			(
				local l_PuertaMR = getUserProp $ "puerta_MR"
				local l_PuertaME = getUserProp $ "puerta_ME"
				local l_CloseTime = getUserProp $ "close_time"
				local l_PuertaAuto = getUserProp $ "puerta_auto"
				local l_MainSwitch = getUserProp $ "interruptor"
				
				if l_PuertaMR != "" and l_PuertaMR!=undefined  and l_PuertaMR!="undefined" then
				(
					edtPuertaMR.text = l_PuertaMR
				)
				if l_PuertaME != "" and l_PuertaME!=undefined  and l_PuertaME!="undefined" then
				(
					edtPuertaME.text = l_PuertaME
				)
				if l_CloseTime != "" and l_CloseTime!=undefined  and l_CloseTime!="undefined" then
				(
					edtTiempoPuerta.text = l_CloseTime as string
				)
				if l_PuertaAuto != "" and l_PuertaAuto!=undefined  and l_PuertaAuto!="undefined" then
				(
					chkDoorsAuto.checked = l_PuertaAuto
				)
				if l_MainSwitch != "" and l_MainSwitch!=undefined  and l_MainSwitch!="undefined" then
				(
					edtMainSwitch.text = l_MainSwitch
				)		
			)
			
			-- Ascensor
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Elevator") then
			(
				local l_TriggerName = getUserProp $ "elevator_trigger"
				if l_TriggerName != "" and l_TriggerName!=undefined  and l_TriggerName!="undefined" then
				(
					edtCabina.text = l_TriggerName
				)
				
				local l_ElevatorOrientation = getUserProp $ "elevator_orientation"
				if l_ElevatorOrientation != "" and l_ElevatorOrientation!=undefined  and l_ElevatorOrientation!="undefined" then
				(
					edtTiempoSubida.text = l_ElevatorOrientation as string
				)
	
				local l_LeftDoor = getUserProp $ "puerta_izq"
				if l_LeftDoor != "" and l_LeftDoor!=undefined  and l_LeftDoor!="undefined" then
				(
					edtPuertaIzq.text = l_LeftDoor
				)	
	
				local l_RightDoor = getUserProp $ "link_mundo"
				if l_RightDoor != "" and l_RightDoor!=undefined  and l_RightDoor!="undefined" then
				(
					edtPuertaDer.text = l_RightDoor
				)					
			)
			
			-- Life / Energy Machines
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Life" or ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Energy") then
			(
				local l_MeshName = getUserProp $ "mesh_name"
				if l_MeshName != "" and l_MeshName!=undefined  and l_MeshName!="undefined" then
				(
					edtLifeEnergyMesh.text = l_MeshName
				)
				
				local l_TextureON = getUserProp $ "texture_on"
				if l_TextureON != "" and l_TextureON!=undefined  and l_TextureON!="undefined" then
				(
					edtLifeEnergy_TextureON.text = l_TextureON 
				)
	
				local l_TextureOFF = getUserProp $ "texture_off"
				if l_TextureOFF != "" and l_TextureOFF!=undefined  and l_TextureOFF!="undefined" then
				(
					edtLifeEnergy_TextureOFF.text = l_TextureOFF
				)	
				
				local l_Light = getUserProp $ "life_light"
				if l_Light != "" and l_Light!=undefined  and l_Light!="undefined" then
				(
					edtLight.text = l_Light
				)
			)
	
			-- Mirror Piece
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Mirror piece") then
			(
				local l_MeshName = getUserProp $ "mesh_name"
				if l_MeshName != "" and l_MeshName!=undefined  and l_MeshName!="undefined" then
				(
					edtLifeEnergyMesh.text = l_MeshName
				)
			)	
	
			-- Broken Mirror
			if (ddlGenerateLogic.items[ddlGenerateLogic.Selection]=="Broken mirror") then
			(
				local l_MeshName = getUserProp $ "broken_mirror_mesh"
				if l_MeshName != "" and l_MeshName!=undefined  and l_MeshName!="undefined" then
				(
					edtBrokenMirror.text = l_MeshName
				)
	
				local l_TriggerName = getUserProp $ "final_mirror_object"
				if l_TriggerName != "" and l_TriggerName!=undefined  and l_TriggerName!="undefined" then
				(
					edtFinalMirror.text = l_TriggerName
				)
				
				local l_OtherWorldMirror = getUserProp $ "other_world_mirror"
				if l_OtherWorldMirror != "" and l_OtherWorldMirror!=undefined  and l_OtherWorldMirror!="undefined" then
				(
					edtEspejoRotoOtroMundo.text = l_OtherWorldMirror
				)				
			)			
		)
		
		-- Scripting
		local l_ActiveTrigger = getUserProp $ "active_trigger"
		if l_ActiveTrigger==false or l_ActiveTrigger==undefined then
		(
			chkActiveTrigger.checked = false
		)
		else
		(
			chkActiveTrigger.checked = true
			
			local l_OnEnter = getUserProp $ "onEnter_trigger"
			local l_OnLeave = getUserProp $ "onLeave_trigger"
			local l_OnTrigger = getUserProp $ "onTrigger_trigger"
			
			if l_OnEnter != "" and l_OnEnter!=undefined  and l_OnEnter!="undefined" then
			(
				edtOnEnter.text = l_OnEnter
			)	
			if l_OnLeave != "" and l_OnLeave!=undefined  and l_OnLeave!="undefined" then
			(
				edtOnLeave.text = l_OnLeave
			)	
			if l_OnTrigger != "" and l_OnTrigger!=undefined  and l_OnTrigger!="undefined" then
			(
				edtOnTrigger.text = l_OnTrigger
			)	
		)
		
		-- Particles
		local l_GenerateParticles = getUserProp $ "generate_particles"
		if l_GenerateParticles==false or l_GenerateParticles==undefined then
		(
			chkGenerateParticles.checked = false
			ddlGenerateParticles.Selection = 1
			--format "GET::Generate Particles = false\n" 
		)
		else 
		(
			chkGenerateParticles.checked = true
			
			ddlGenerateParticles.Selection = 1
			for i=1 to ddlGenerateParticles.items.count do
			(
				if ddlGenerateParticles.items[i] == l_GenerateParticles then
					ddlGenerateParticles.Selection = i
			)
			--format "GET::Generate Logic = %\n" ddlGenerateParticles.items[ddlGenerateParticles.Selection]
			
			local l_aux = (filterString (getUserProp $ "color1") ",[]")
			local l_RGB1 =point4 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float) (l_aux[4] as float) 
			l_aux = (filterString (getUserProp $ "color2") ",[]")
			local l_RGB2 =point4 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float) (l_aux[4] as float) 
			l_aux = (filterString (getUserProp $ "dir1") ",[]")
			local l_XYZ1 =point3 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float)
			l_aux = (filterString (getUserProp $ "dir2") ",[]")
			local l_XYZ2 =point3 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float)
			l_aux = (filterString (getUserProp $ "acel1") ",[]")
			local l_AcelXYZ1 =point3 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float)
			l_aux = (filterString (getUserProp $ "acel2") ",[]")
			local l_AcelXYZ2 =point3 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float)
			l_aux = (filterString (getUserProp $ "despl") ",[]")
			local l_DespXYZ =point3 (l_aux[1] as float) (l_aux[2] as float) (l_aux[3] as float)
			
			edtParticleName.text = getUserProp $ "name"
			chkGravityParticles.checked = getUserProp $ "gravity"
			spnParticleMinTime.value = getUserProp $ "life_time_min"
			spnParticleMaxTime.value = getUserProp $ "life_time_max"
			spnParticleMinTime.value = getUserProp $ "emit_rate_min"
			spnParticleMaxRate.value = getUserProp $ "emit_rate_max"
			lbl1.text = getUserProp $ "tex"
			
			spnParticleR1.value = l_RGB1.x
			spnParticleG1.value = l_RGB1.y
			spnParticleB1.value = l_RGB1.z
			spnParticleA1.value = l_RGB1.w
			spnParticleR2.value = l_RGB2.x
			spnParticleG2.value = l_RGB2.y
			spnParticleB2.value = l_RGB2.z
			spnParticleA2.value = l_RGB2.w
			spnParticleX1.value = l_XYZ1.x
			spnParticleY1.value = l_XYZ1.y
			spnParticleZ1.value = l_XYZ1.z
			spnParticleX2.value = l_XYZ2.x
			spnParticleY2.value = l_XYZ2.y
			spnParticleZ2.value = l_XYZ2.z
			spnParticleAcelX1.value = l_AcelXYZ1.x
			spnParticleAcelY1.value = l_AcelXYZ1.y
			spnParticleAcelZ1.value = l_AcelXYZ1.z
			spnParticleAcelX2.value = l_AcelXYZ2.x
			spnParticleAcelY2.value = l_AcelXYZ2.y
			spnParticleAcelZ2.value = l_AcelXYZ2.z
			spnParticleDespX1.value = l_DespXYZ.x
			spnParticleDespY1.value = l_DespXYZ.y
			spnParticleDespZ1.value = l_DespXYZ.z
			
			spnParticleSize1.value = getUserProp $ "size1"
			spnParticleSize2.value = getUserProp $ "size2"
			spnParticleInstanceSize.value = getUserProp $ "size"
			
			--format "GET::Type = %\n" l_GenerateParticles
			--format "GET::Name = %\n" edtParticleName.text
			--format "GET::Gravity = %\n" chkGravityParticles.checked
			--format "GET::Life Time Min = %\n" spnParticleMinTime.value
			--format "GET::Life Time Max = %\n" spnParticleMaxTime.value
			--format "GET::Emit Rate Min = %\n" spnParticleMinRate.value
			--format "GET::Emit Rate Max = %\n" spnParticleMaxRate.value
			--format "GET::Texture Path = %\n" lbl1.text
			--format "GET::Color1 = %\n" l_RGB1
			--format "GET::Color2 = %\n" l_RGB2
			--format "GET::Direction1 = %\n" l_XYZ1
			--format "GET::Direction2 = %\n" l_XYZ2
			--format "GET::Size1 = %\n" spnParticleSize1.value
			--format "GET::Size2 = %\n" spnParticleSize2.value
			--format "GET::Aceleration1 = %\n" l_AcelXYZ1
			--format "GET::Aceleration2 = %\n" l_AcelXYZ2
			--format "GET::InstanceSize = %\n" spnParticleInstanceSize.value
			--format "GET::Desplazamiento = %\n" l_DespXYZ
		)
		
		--format "\n"
	)
	
	
	-- Set Button	
	on btnSet pressed do
	(
		--format "Set Object properties:\n"
		
		-- Export type dropdown list
		local l_ExportType
		local l_AnimModel
		if chkExport.checked then
		(
			l_ExportType = ddlExport.items[ddlExport.Selection]
			l_AnimModel = editAnim.text
		) else (
			l_ExportType=false
		)
		--format "SET::ExportType = %\n" l_ExportType
		--format "SET::AnimModel = %\n" l_AnimModel
		setUserProp $ "export_type" l_ExportType
		setUserProp $ "anim_model" l_AnimModel
		
		-- Visible
		setUserProp $ "visible" chkVisible.checked
		--format "SET::Visible = %\n" chkVisible.checked
		
		-- Cast shadows
		setUserProp $ "cast_shadows" chkCastShadows.checked
		--format "SET::Cast Shadows = %\n" chkCastShadows.checked
		
		-- Layer dropdown list
		local l_Layer
		if chkLayerRender.checked then
		(
			l_Layer = ddlLayerRender.items[ddlLayerRender.Selection]
		) else (
			l_Layer=false
		)	
		--format "SET::Layer = %\n" l_Layer
		setUserProp $ "layer" l_Layer
	
		--  Physics
		local l_GeneratePhysics
		if chkGeneratePhysics.checked then
		(	
			l_GeneratePhysics=ddlGeneratePhysics.items[ddlGeneratePhysics.Selection]
		) else (
		
			l_GeneratePhysics=false
		)
		--format "SET::Generate Physics = %\n" l_GeneratePhysics
		setUserProp $ "generate_physics" l_GeneratePhysics
		
		
		--  Layer Physics
		local l_LayerPhysics
		if chkLayerPhysics.checked then
		(
			 l_LayerPhysics = ddlLayerPhysics.items[ddlLayerPhysics.Selection]
		) else (
			l_LayerPhysics=false
		)
		--format "SET::Layer Physics = %\n" l_LayerPhysics
		setUserProp $ "layer_physics" l_LayerPhysics			
		
		--  Logic
		local l_GenerateLogic = ddlGenerateLogic.items[ddlGenerateLogic.Selection]
		--format "SET::Generate Logic = %\n" l_GenerateLogic
		
		if chkGenerateLogic.checked then
		(
			local l_RenderableObject = edtRenderObj.text
			local l_TargetMirror = edtTargetMirror.text
			local l_RotatedTargetMirror = edtRotatedMirror.text
			local l_AngleRot = edtAngleRot.text
			local l_SpawnPoint = edtSpawnPoint.text
			local l_AmbosMundos = chk_2mundos.checked
			local l_Texture = edtTextura.text
			local l_FS = chkFSTexture.checked
			local l_MirrorPlane = edtPlane.text
			--local l_MirrorPlaneRot = edtPlaneRot.text
	
			l_GenerateLogic = ddlGenerateLogic.items[ddlGenerateLogic.Selection]
			if (l_GenerateLogic=="Mirror") then
			(
				setUserProp $ "renderable_object" l_RenderableObject	
				setUserProp $ "target_mirror" l_TargetMirror	
				setUserProp $ "rotated_target_mirror" 	l_RotatedTargetMirror
				setUserProp $ "angle_rot" l_AngleRot
				setUserProp $ "spawn_point" l_SpawnPoint
				setUserProp $ "linked_mirrors" l_AmbosMundos
				setUserProp $ "mirror_plane" l_MirrorPlane
				--setUserProp $ "mirror_plane_rot" l_MirrorPlaneRot
				
				--format "SET::Mesh = %\n" l_RenderableObject
				--format "SET::Spawn Point = %\n" l_SpawnPoint
				--format "SET::Target Mirror = %\n" l_TargetMirror
				--format "SET::Rotated Target Mirror = %\n" l_RotatedTargetMirror
				--format "SET::Angle Rot = %\n" l_AngleRot
				--format "SET::Ambos Mundos = %\n" l_AmbosMundos
				--format "SET::Mirror plane = %\n" l_MirrorPlane
				--format "SET::Mirror plane rot = %\n" l_MirrorPlaneRot
			)
			else if (l_GenerateLogic=="Tutorial") then
			(
				setUserProp $ "texture" l_Texture
				setUserProp $ "FS_texture" l_FS
				
				--format "SET::Texture = %\n" l_Texture
				--format "SET::Una sola vez = %\n" l_FS				
			)
			else if (l_GenerateLogic=="Door" or l_GenerateLogic=="Fan") then
			(
				local l_PuertaMR = edtPuertaMR.text
				local l_PuertaME = edtPuertaME.text
				local l_CloseTime = edtTiempoPuerta.text
				local l_PuertaAuto = chkDoorsAuto.checked
				local l_MainSwitch = edtMainSwitch.text
				
				setUserProp $ "puerta_MR" l_PuertaMR	
				setUserProp $ "puerta_ME" l_PuertaME	
				setUserProp $ "close_time" l_CloseTime
				setUserProp $ "puerta_auto" l_PuertaAuto
				setUserProp $ "interruptor" l_MainSwitch
			)
			else if (l_GenerateLogic=="Elevator") then
			(
				local l_TriggerName = edtCabina.text
				setUserProp $ "elevator_trigger" l_TriggerName	
	
				local l_ElevatorOrientation = edtTiempoSubida.text	
				setUserProp $ "elevator_orientation" l_ElevatorOrientation	
	
				local l_LeftDoor = edtPuertaIzq.text
				setUserProp $ "puerta_izq" l_LeftDoor
				
				local l_RightDoor = edtPuertaDer.text
				setUserProp $ "link_mundo" l_RightDoor				
			)
			else if (l_GenerateLogic=="Life" or l_GenerateLogic=="Energy") then
			(
				local l_MeshName = edtLifeEnergyMesh.text
				setUserProp $ "mesh_name" l_MeshName	
	
				local l_TextureON = edtLifeEnergy_TextureON.text	
				setUserProp $ "texture_on" l_TextureON	
	
				local l_TextureOFF = edtLifeEnergy_TextureOFF.text
				setUserProp $ "texture_off" l_TextureOFF
				
				local l_Light = edtLight.text
				setUserProp $ "life_light" l_Light
			)
			else if (l_GenerateLogic=="Mirror piece") then
			(
				local l_MeshName = edtLifeEnergyMesh.text
				setUserProp $ "mesh_name" l_MeshName	
			)
			else if (l_GenerateLogic=="Broken mirror") then
			(
				local l_MeshName = edtBrokenMirror.text
				setUserProp $ "broken_mirror_mesh" l_MeshName	
	
				local l_TriggerName = edtFinalMirror.text
				setUserProp $ "final_mirror_object" l_TriggerName	
				
				local l_OtherWorldMirror = edtEspejoRotoOtroMundo.text
				setUserProp $ "other_world_mirror" l_OtherWorldMirror	
			)			
		) 
		
		else 
		(
			l_GenerateLogic=false
		)
		
		setUserProp $ "generate_logic" l_GenerateLogic			
		
		-- Scripting
		local l_ActiveTrigger = chkActiveTrigger.checked
		local l_OnEnter = edtOnEnter.text
		local l_OnLeave = edtOnLeave.text
		local l_OnTrigger = edtOnTrigger.text
		
		if (l_ActiveTrigger==false) then
		(
			l_OnEnter=""
			l_OnLeave=""
			l_OnTrigger=""
		)
		
		setUserProp $ "active_trigger" l_ActiveTrigger
		setUserProp $ "onEnter_trigger" l_OnEnter
		setUserProp $ "onLeave_trigger" l_OnLeave
		setUserProp $ "onTrigger_trigger" l_OnTrigger
		
		--  Particles
		local l_GenerateParticles = ddlGenerateParticles.items[ddlGenerateParticles.Selection]
		--format "SET::Generate Particles = %\n" l_GenerateParticles
		
		if chkGenerateParticles.checked then
		(	
			if(lbl1.text==undefined or lbl1.text=="undefined" or edtParticleName.text=="") then 
			(
				Messagebox "ERROR: Es necesario poner un nombre de Core y un path de textura!"
			)
			else
			(
				local l_RGB1 = point4 spnParticleR1.value spnParticleG1.value spnParticleB1.value spnParticleA1.value
				local l_RGB2 = point4 spnParticleR2.value spnParticleG2.value spnParticleB2.value spnParticleA2.value
				local l_XYZ1 = point3 spnParticleX1.value spnParticleY1.value spnParticleZ1.value
				local l_XYZ2 = point3 spnParticleX2.value spnParticleY2.value spnParticleZ2.value
				local l_AcelXYZ1 = point3 spnParticleAcelX1.value spnParticleAcelY1.value spnParticleAcelZ1.value
				local l_AcelXYZ2 = point3 spnParticleAcelX2.value spnParticleAcelY2.value spnParticleAcelZ2.value
				local l_DespXYZ = point3 spnParticleDespX1.value spnParticleDespY1.value spnParticleDespZ1.value
			
				--setUserProp $ "type" l_GenerateParticles
				setUserProp $ "name" edtParticleName.text
				setUserProp $ "gravity" chkGravityParticles.checked
				setUserProp $ "life_time_min" spnParticleMinTime.value
				setUserProp $ "life_time_max" spnParticleMaxTime.value
				setUserProp $ "emit_rate_min" spnParticleMinRate.value
				setUserProp $ "emit_rate_max" spnParticleMaxRate.value
				setUserProp $ "tex" texture_path
				setUserProp $ "color1" l_RGB1		
				setUserProp $ "color2" l_RGB2	 
				setUserProp $ "dir1" l_XYZ1		
				setUserProp $ "dir2" l_XYZ2
				setUserProp $ "size1" spnParticleSize1.value		
				setUserProp $ "size2" spnParticleSize2.value		
				setUserProp $ "acel1" l_AcelXYZ1		
				setUserProp $ "acel2" l_AcelXYZ2
				setUserProp $ "size" spnParticleInstanceSize.value
				setUserProp $ "despl" l_DespXYZ
			
				--format "SET::Type = %\n" l_GenerateParticles
				--format "SET::Name = %\n" edtParticleName.text
				--format "SET::Gravity = %\n" chkGravityParticles.checked
				--format "SET::Life Time Min = %\n" spnParticleMinTime.value
				--format "SET::Life Time Max = %\n" spnParticleMaxTime.value
				--format "SET::Emit Rate Min = %\n" spnParticleMinRate.value
				--format "SET::Emit Rate Max = %\n" spnParticleMaxRate.value
				--format "SET::Texture Path = %\n" texture_path
				--format "SET::Color1 = %\n" l_RGB1
				--format "SET::Color2 = %\n" l_RGB2
				--format "SET::Direction1 = %\n" l_XYZ1
				--format "SET::Direction2 = %\n" l_XYZ2
				--format "SET::Size1 = %\n" spnParticleSize1.value
				--format "SET::Size2 = %\n" spnParticleSize2.value
				--format "SET::Aceleration1 = %\n" l_AcelXYZ1
				--format "SET::Aceleration2 = %\n" l_AcelXYZ2
				--format "SET::InstanceSize = %\n" spnParticleInstanceSize.value
				--format "SET::Desplazamiento = %\n" l_DespXYZ
			)
		) 		
		else 
		(
			l_GenerateParticles=false
		)
		
		setUserProp $ "generate_particles" l_GenerateParticles
		
		--format "\n"
		destroyDialog GetSetObjProperties
	)
	on btn1 pressed do
	(
		texture_path= getOpenFileName caption: "Data Path" initialDir:"d:\\"
		if texture_path!=undefined then 
			lbl1.text=texture_path
		format "Path de la textura: %\n" texture_path
	)
)



--createDialog UABSetProperties