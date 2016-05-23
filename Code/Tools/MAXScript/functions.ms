fileIn "globals.ms" 

-- Funciones globales

fn isMesh Obj =
(
	if classof Obj==Editable_Mesh or classof Obj==Editable_Poly or classof Obj==PolyMeshObject then
		return true
	else
		return false
)

fn isBone Obj =
(
	if classof Obj==BoneGeometry then
		return true
	else
		return false	
)

fn isPointHelper Obj =
(
	if classof Obj == Point then
		return true
	else
		return false
)

fn isLogic logicObj =
(
	if (logicObj == "Mirror" or logicObj=="Tutorial" or logicObj=="Door" or logicObj=="Elevator" or logicObj=="Lasers" or logicObj=="Life" or logicObj=="Energy" or logicObj=="Mirror piece" or logicObj=="Broken mirror") then
		return true
	
	return false
)

fn isTrigger logicObj =
(
	local l_ActiveTrigger = getUserProp logicObj "active_trigger"
	
	if (l_ActiveTrigger==true) then
		return true
	
	return false
)

fn isLight Obj =
(
	if classof Obj == Omnilight or classof Obj == targetSpot or classof Obj == TargetDirectionallight then
		return true
	else
		return false
)

fn isCharacter Obj =
(
	local l_GenerateLogic = getUserProp Obj "generate_logic"
	
	if (l_GenerateLogic=="Player" or l_GenerateLogic=="Soldier" or l_GenerateLogic=="Monster") then
		return true
	else
		return false
)

function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)

function GetYaw transform =
(
	return (transform as eulerangles).z
)

function GetPitch transform =
(
	return (transform as eulerangles).x
)

function GetRoll transform =
(
	return (transform as eulerangles).y
)

function ActualizaMin Vtx VtxMin  =
(
	if VtxMin==undefined then
	(
		VtxMin = point3 Vtx.x Vtx.y Vtx.z
	)
	
	if Vtx.x < VtxMin.x then
	(
		VtxMin.x = Vtx.x
	)
	if Vtx.y < VtxMin.y then
	(	
		VtxMin.y = Vtx.y
	)
	if Vtx.z < VtxMin.z then
	(
		VtxMin.z = Vtx.z
	)
	
	return VtxMin
)

function ActualizaMax Vtx VtxMax =
(
	if VtxMax==undefined then
	(
		VtxMax = point3 Vtx.x Vtx.y Vtx.z
	)

	if Vtx.x > VtxMax.x then
	(
		VtxMax.x = Vtx.x
	)
	if Vtx.y > VtxMax.y then
	(	
		VtxMax.y = Vtx.y
	)
	if Vtx.z > VtxMax.z then
	(
		VtxMax.z = Vtx.z
	)
	
	return VtxMax
)

global UABVtxsNormals=#()

function UABClearNormalsArray =
(
	while UABVtxsNormals.count>0 do
	(
		deleteItem UABVtxsNormals 1
	)
)

function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask=2^(IdGroup-1)
	return (bit.and Value ValueMask)==ValueMask
)

function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal=false
	local Normal=point3 0 0 0
	for b=1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal=Normal+UABVtxsNormals[IdVertex][b]
			HasNormal=true
		)
	)
	if HasNormal==false then
	(
		Normal=FaceNormal
	)
	return Normal
)

function UABCalcVertexsNormals obj =
(
	UABClearNormalsArray()
	local NumVtxs=getNumVerts obj
	for b=1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		for t=1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)
	local NumFaces=getNumFaces obj
	local InvTransform=inverse obj.transform
	for IdFace=1 to NumFaces do
	(
		local IdxsFace=getFace obj IdFace
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform
		local FaceNormal=getFaceNormal obj IdFace
		local SmoothValue=getFaceSmoothGroup obj IdFace
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] +FaceNormal
			)
		)
	)
)


function GetBoundingBoxSize  Obj =
(
	local l_Size = point3 0 0 0
	local l_Vtx_Min=undefined
	local l_Vtx_Max=undefined
	
	if isMesh Obj then
	(
		for IdFace=1 to getNumFaces Obj do
		(
			local IdxsFace=getFace Obj IdFace
			
			-- Geometria
			local mat=inverse(Obj.transform)
			
			local Vtx1=getVert Obj IdxsFace.x
			Vtx1=Vtx1*mat
			Vtx1=RHTranslationToLH Vtx1
					
			local Vtx2=getVert Obj IdxsFace.y
			Vtx2=Vtx2*mat
			Vtx2=RHTranslationToLH Vtx2
			
			local Vtx3=getVert Obj IdxsFace.z
			Vtx3=Vtx3*mat
			Vtx3=RHTranslationToLH Vtx3
			
			-- Actualizamos vertice minimo
			l_Vtx_Min=ActualizaMin Vtx1 l_Vtx_Min
			l_Vtx_Min=ActualizaMin Vtx2 l_Vtx_Min
			l_Vtx_Min=ActualizaMin Vtx3 l_Vtx_Min

			-- Actualizamos vertice maximo
			l_Vtx_Max=ActualizaMax Vtx1 l_Vtx_Max
			l_Vtx_Max=ActualizaMax Vtx2 l_Vtx_Max
			l_Vtx_Max=ActualizaMax Vtx3 l_Vtx_Max
		)
		
		-- Calculamos bounding box size (Vmax - Vmin)
		l_Size = l_Vtx_Max - l_Vtx_Min
		--format "DEBUG> l_Size = %\n" l_Size
	)
	
	return l_Size 
)