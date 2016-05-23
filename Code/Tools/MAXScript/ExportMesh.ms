fileIn "VertexTypes.ms"
fileIn "functions.ms"


function AddToIndexBuffer IB idx IDMaterial=
(
	while IB.count<IDMaterial do
	(
		local l_Idxs=#()
		append IB l_Idxs
	)

	append IB[IDMaterial] idx
)

function CreateVertex Material Vtx Normal Tuv Tuv2 VertexType =
(
	local l_Vtx=#()
	append l_Vtx Vtx.x
	append l_Vtx Vtx.y
	append l_Vtx Vtx.z
	
	append l_Vtx Normal.x
	append l_Vtx Normal.y
	append l_Vtx Normal.z
	
	if (bit.and VertexType VERTEX_TYPE_BINORMAL)==VERTEX_TYPE_BINORMAL and (bit.and VertexType VERTEX_TYPE_TANGENT)==VERTEX_TYPE_TANGENT then
	(
		append l_Vtx 0	--Normal.w
		append l_Vtx 0 	--Tangent.x
		append l_Vtx 0 	--Tangent.y
		append l_Vtx 0 	--Tangent.z
		append l_Vtx 0 	--Tangent.w
		append l_Vtx 0 	--Binormal.x
		append l_Vtx 0 	--Binormal.y
		append l_Vtx 0 	--Binormal.z
		append l_Vtx 0 	--Binormal.w
		
	)
	if (bit.and VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
	(
		append l_Vtx Tuv.x
		append l_Vtx Tuv.y
	)
	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
	(
		append l_Vtx Tuv2.x
		append l_Vtx Tuv2.y
	)
	
	if (bit.and VertexType VERTEX_TYPE_DIFFUSE)==VERTEX_TYPE_DIFFUSE then
	(
		local l_r=bit.shift (Material.diffuse.r as integer) 24
		local l_g=bit.shift (Material.diffuse.g as integer) 16
		local l_b=bit.shift (Material.diffuse.b as integer) 8
		local l_rg = bit.or l_r l_g
		local l_rgb = bit.or l_rg l_b
		local l_Diffuse= bit.or l_rgb 0xff
		
		--local l_Diffuse=bit.or (bir.or (bit.or l_r l_g) l_b) 0xff
		append l_Vtx l_Diffuse
	)
	
	
	return l_Vtx
)

function GetVertexType material =
(
	-- Coordendas geometricas y de normal siempre
	local l_VertexType =bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	
	if material != undefined then
	(
		-- Miramos si tiene coordenadas de textura
		if material.diffuseMapEnable==true and (classof material.diffuseMap)==Bitmaptexture then
		(
			l_VertexType=bit.or l_VertexType VERTEX_TYPE_TEXTURE1
			
			-- Miramos si tiene coordenadas lightmap
			if material.selfIllumMapEnable==true and (classof material.selfIllumMap)==Bitmaptexture then
			(
				l_VertexType=bit.or l_VertexType VERTEX_TYPE_TEXTURE2
			)

			-- Miramos si tiene coordenadas bumpmap
			if material.bumpMapEnable==true and (classof material.bumpMap)==Bitmaptexture then
			(
				l_VertexType=bit.or l_VertexType VERTEX_TYPE_BINORMAL
				l_VertexType=bit.or l_VertexType VERTEX_TYPE_TANGENT
			)

			-- Miramos si tiene radiosity normal map
			if material.selfIllumMapEnable==true and classof (material.selfIllumMap) == Multi_Sub_Map then
			(
				l_VertexType=bit.or l_VertexType VERTEX_TYPE_TEXTURE2
				l_VertexType=bit.or l_VertexType VERTEX_TYPE_RADIOSITY
				if material.selfIllumMap.id_3_color_shader != undefined then
				(
					l_VertexType=bit.or l_VertexType VERTEX_TYPE_RADIOSITY_SPEC
				)				
			)
		)
		else
		(
			l_VertexType=bit.or l_VertexType VERTEX_TYPE_DIFFUSE
		)
	)
	
	return l_VertexType
)

function WriteBitmapName File FileName =
(
	--format "DEBUG> Filename.count: %\n" Filename.count
	WriteShort File Filename.count #unsigned 
	WriteString File Filename
)


function WriteMaterialToFile File VertexType Material=
(
	local l_Radiosity = false
	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
	(
		--format "Has texture!\n"
		local l_FileName =  filenameFromPath (openBitmap Material.diffuseMap.bitmap.filename).filename	
		--format "File name: %\n" (g_MeshesPath + l_FileName)
		WriteBitmapName File (g_MeshesPath+l_FileName)
	)
	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
	(
		if (bit.and VertexType VERTEX_TYPE_RADIOSITY)==VERTEX_TYPE_RADIOSITY then
		(
			--format "Has Radiosity Normal Map!\n"
			local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_0_color_shader.bitmap.filename).filename
			--format "File name lightmap x: %\n" (g_MeshesPath + l_FileName)
			WriteBitmapName File (g_MeshesPath+l_FileName)
			local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_1_color_shader.bitmap.filename).filename
			--format "File name lightmap y: %\n" (g_MeshesPath + l_FileName)
			WriteBitmapName File (g_MeshesPath+l_FileName)		
			local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_2_color_shader.bitmap.filename).filename
			--format "File name lightmap z: %\n" (g_MeshesPath + l_FileName)
			WriteBitmapName File (g_MeshesPath+l_FileName)	
			
			if (bit.and VertexType VERTEX_TYPE_RADIOSITY_SPEC)==VERTEX_TYPE_RADIOSITY_SPEC then
			(
				local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_3_color_shader.bitmap.filename).filename
				--format "File name lightmap S: %\n" (g_MeshesPath + l_FileName)
				WriteBitmapName File (g_MeshesPath+l_FileName)
				l_Radiosity = true
			)
			else
			(
				--Error, falta asignar lightmap normal en canal S del radiosity
				local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_2_color_shader.bitmap.filename).filename
				local l_MsgError = "AVISO: Falta asignar el lightmap en el canal 'S' del radiosity normal map despues de " + l_FileName
				Messagebox l_MsgError
			)
		)
		else
		(
			--format "Has lightmap!\n"
			local l_FileName =  filenameFromPath (openBitmap Material.selfIllumMap.bitmap.filename).filename
			--format "File name: %\n" (g_MeshesPath + l_FileName)
			WriteBitmapName File (g_MeshesPath+l_FileName)
		)
	)
	
	if (bit.and VertexType VERTEX_TYPE_BINORMAL)==VERTEX_TYPE_BINORMAL then
	(
		--format "Has bumpmap!\n"
		local l_FileName = filenameFromPath  (openBitmap Material.bumpMap.bitmap.filename).filename
		--format "File name: %\n" (g_MeshesPath + l_FileName)
		WriteBitmapName File (g_MeshesPath+l_FileName)
	)
	else
	(
		if (l_Radiosity==true) then
		(
			--Error: material con radiosity tiene que tener bumpmap tambien
			local l_FileName = filenameFromPath  (openBitmap Material.selfIllumMap.id_0_color_shader.bitmap.filename).filename
			local l_MsgError = "AVISO: Material con radiosity normal map tiene que tener bumpmap en " + l_FileName
			Messagebox l_MsgError
		)
	)
	
)

function WriteVertexToFile FileName Vtx VertexType =
(
	--format "DEBUG> VertexType: %\n" VertexType
	if (bit.and VertexType VERTEX_TYPE_DIFFUSE)==VERTEX_TYPE_DIFFUSE then
	(
		WriteFloat FileName Vtx[1]
		WriteFloat FileName Vtx[2]
		WriteFloat FileName Vtx[3]
		WriteFloat FileName Vtx[4]
		WriteFloat FileName Vtx[5]
		WriteFloat FileName Vtx[6]
		--format "DEBUG> Vertex color: %\n" Vtx[4]
		--format "DEBUG> Vertex color: %\n" Vtx[5]
		--format "DEBUG> Vertex color: %\n" Vtx[6]
		--format "DEBUG> Vertex color: %\n" Vtx[7]
		WriteLong FileName Vtx[7] #unsigned
	)
	else
	(
		for i=1 to Vtx.count do
		(
			WriteFloat FileName Vtx[i]
		)
	)
)


/*
function VertexCompare vtx1 vtx2 =
(
	if vtx1.count != vtx2.count then
		return false
	
	for i=1 to vtx1.count do
	(
			if vtx1[i] != vtx2[i] then
			(
				return false
			)
	)
	
	return true
)

function FindVtx VB Vtx IDMaterial =
(
	local index = -1
	for i=1 to VB[IDMaterial].count do
	(
		if (VertexCompare VB[IDMaterial][i] vtx) == true and index==-1 then
		(
			index = i
			--return i		-- no compatible con 3dsmax 2012
		)
	)
	
	return index
)

function AddToVertexBuffer VB vtx IDMaterial =
(
	while VB.count<IDMaterial do
	(
		local l_Vtxs=#()
		append VB l_Vtxs
	)

	local idx = FindVtx VB vtx IDMaterial
	if  idx == -1 then
	(
		append VB[IDMaterial] vtx
		idx=VB[IDMaterial].count
	)
	idx = idx - 1 
	return idx
)
*/

function ConvertToString Vtx IdMaterial =
(
	local l_Text = stringstream ""
	local l_Value=""
	for i=1 to Vtx.count do
	(
		format "%," Vtx[i] to:l_Text
	)
	format "%," IdMaterial to:l_Text
	
	return (l_Text as string)
)

function DotNetAddVertex VB FullVtx VtxMap IdMaterial=
(
	while VB.count<IdMaterial do
	(
		local l_Vtxs=#()
		append VB l_Vtxs
	)
	
	local l_VtxString=ConvertToString FullVtx IdMaterial
	local l_Idx=1
	if (VtxMap.ContainsKey (l_VtxString))==false then
	(
		append VB[IdMaterial] FullVtx
		l_Idx=VB[IdMaterial].count-1
		VtxMap.Add l_VtxString l_Idx
	)
	else
		l_Idx=VtxMap.Item[l_VtxString]
	
	return l_Idx
)

function CalcularDistancia Vtx1 Vtx2 =
(
	local l_Dist = sqrt(((Vtx2.x - Vtx1[1])*(Vtx2.x - Vtx1[1])) +( (Vtx2.y - Vtx1[2])*(Vtx2.y - Vtx1[2])) + ((Vtx2.z - Vtx1[3])*(Vtx2.z - Vtx1[3])))
	return l_Dist
)

function CalcularRadioBoundingSphere VB centre =
(
	local l_DistMax=0
	for i=1 to VB.count do
	(
		for j=1 to VB[i].count do
		(
			local l_Dist = CalcularDistancia VB[i][j] centre
			if l_Dist>l_DistMax then
			(
				l_DistMax = l_Dist
			)
		)
	)
	return l_DistMax
)

function WriteMeshToFile filename Obj VB IB VtxMin VtxMax Center Radius =
(
	local file=fopen filename "wb"
	
	if file!=undefined then
	(
		-- Escribimos header
		WriteShort file 0xcaca #unsigned
		
		-- Escribimos numero de Renderable Vertexs
		local l_RenderableVtxs = undefined
		local l_MultiMaterial = false
		if classof(Obj.material) == Multimaterial then
		(
			l_RenderableVtxs = Obj.material.numsubs
			l_MultiMaterial = true
		)
		else
		(
			l_RenderableVtxs = 1
		)
		WriteShort file l_RenderableVtxs #unsigned

		local l_Material=Obj.material
		for IdRenderableVtx=1 to l_RenderableVtxs do
		(
			if l_MultiMaterial==true then
				l_Material=Obj.material[IdRenderableVtx]
			
			-- Escribimos tipo de vertices
			local l_VertexType=GetVertexType l_Material
			WriteShort file l_VertexType #unsigned

			-- Escribimos nombre del material para cada tipo de vertice
			WriteMaterialToFile file l_VertexType l_Material	

			-- Escribimos núm de vertices del material
			if (VB[IdRenderableVtx]  != undefined) then
			(
				WriteShort file VB[IdRenderableVtx].count #unsigned
				-- Escribimos Vertex Buffer del material
				for i=1 to VB[IdRenderableVtx].count do
				(
					WriteVertexToFile file VB[IdRenderableVtx][i] l_VertexType
				)				
			)
			else
			(
				WriteShort file 0 #unsigned
			)

			-- Escribimos núm de indices del material
			if (VB[IdRenderableVtx]  != undefined) then
			(
				WriteShort file IB[IdRenderableVtx].count #unsigned
				
				-- Escribimos Index Buffer del material
				for i=1 to IB[IdRenderableVtx].count do
				(
					WriteShort file IB[IdRenderableVtx][i] #unsigned
				)
			)
			else
			(
				WriteShort file 0 #unsigned
			)
		
		)
		
		-- Escribimos vertice min
		WriteFloat file VtxMin.x
		WriteFloat file VtxMin.y
		WriteFloat file VtxMin.z
		
		-- Escribimos vertice max
		WriteFloat file VtxMax.x
		WriteFloat file VtxMax.y
		WriteFloat file VtxMax.z
		
		-- Escribimos vertice centro
		WriteFloat file Center.x
		WriteFloat file Center.y
		WriteFloat file Center.z

		-- Escribimos radio de la esfera contenedora
		WriteFloat file Radius
		
		-- Escribimos footer
		WriteShort file 0xacac #unsigned
		
		fclose file
	)
)

function PrintMeshDebugInfo FileName Obj VB IB VtxMin VtxMax Center Radius =
(
	local l_NumRenderableVertexs = 0
	if classof(Obj.material) == Multimaterial then
	(
		l_NumRenderableVertexs = Obj.material.numsubs
	)
	else
	(
		l_NumRenderableVertexs = 1
	)
	
	format "\n"
	format "Fichero de salida: %\n" FileName
	format "Num materiales: %\n" l_NumRenderableVertexs
	for i=1 to l_NumRenderableVertexs do
	(
		format "DATA Material %:\n" i
		format "VB[%]: %\n" i VB[i]
		if (VB[i] != undefined) then
		(
			format "VB[%].Size: %\n" i VB[i].count
		)
		format "IB[%]: %\n" i IB[i]
		if (IB[i] != undefined) then
		(
			format "IB[%].Size: %\n" i IB[i].count
		)
	)	
	
	format "Vtx Min x=% y=% z=%\n" VtxMin.x VtxMin.y VtxMin.z
	format "Vtx Max x=% y=% z=%\n" VtxMax.x VtxMax.y VtxMax.z
	format "Centro x=% y=% z=%\n" Center.x Center.y Center.z
	format "Radio: %\n" Radius
)

function CreateMesh  Obj filename =
(
	l_VertexBuffer=#()
	l_IndexBuffer=#()
	
	local l_Vtx_Min=undefined
	local l_Vtx_Max=undefined
	local l_VtxsMap=dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
	
	if isMesh Obj then
	(
		local l_ObjCopy=undefined
		if classof Obj==Editable_Poly or classof Obj==PolyMeshObject then 
		(
			l_ObjCopy=copy Obj
			l_ObjCopy.name=Obj.name+"_copia_BORRAR"
			convertToMesh l_ObjCopy
		)
		else
		(
			l_ObjCopy=Obj
		)
			
		UABCalcVertexsNormals l_ObjCopy
		
		local l_Error = false
		--format "DEBUG> Creating mesh from object: %\n" Obj
		for IdFace=1 to getNumFaces l_ObjCopy do
		(
			if l_Error==false then
			(
				local IdxsFace=getFace l_ObjCopy IdFace
				
				-- Geometria
				local Vtx1=getVert l_ObjCopy IdxsFace.x
				local mat=inverse(l_ObjCopy.transform)
				Vtx1=Vtx1*mat
				Vtx1=RHTranslationToLH Vtx1
						
				local Vtx2=getVert l_ObjCopy IdxsFace.y
				--local mat=inverse(l_ObjCopy.transform)
				Vtx2=Vtx2*mat
				Vtx2=RHTranslationToLH Vtx2
				
				local Vtx3=getVert l_ObjCopy IdxsFace.z
				--local mat=inverse(l_ObjCopy.transform)
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

				-- Normales
				--local Normal1=getNormal l_ObjCopy IdxsFace.x
				--local Normal2=getNormal l_ObjCopy IdxsFace.y
				--local Normal3=getNormal l_ObjCopy IdxsFace.z	
				local FaceNormal=getFaceNormal l_ObjCopy IdFace
				local SmoothValue=getFaceSmoothGroup l_ObjCopy IdFace
				
				local Normal1=normalize(UABGetVertexNormal l_ObjCopy IdxsFace.x SmoothValue FaceNormal)
				local Normal2=normalize(UABGetVertexNormal l_ObjCopy IdxsFace.y SmoothValue FaceNormal)
				local Normal3=normalize(UABGetVertexNormal l_ObjCopy IdxsFace.z SmoothValue FaceNormal)
				
				Normal1=RHTranslationToLH Normal1
				Normal2=RHTranslationToLH Normal2
				Normal3=RHTranslationToLH Normal3
				
				-- Obtenemos (multi)material 
				local l_MaterialID=getFaceMatID l_ObjCopy IdFace
				local l_Material=undefined
				if classof(l_ObjCopy.material) == Multimaterial then
				(
					l_Material=l_ObjCopy.material[l_MaterialID]
					--format "DEBUG> IdFace: %  l_Material : %\n" idFace l_Material
				)
				else
				(
					l_Material=l_ObjCopy.material
					l_MaterialID=1
				)
				if l_Material==undefined then
				(
					local l_MsgError = "AVISO: Detectado un vértice con un índice de material incorrecto " + (l_MaterialID as String)+" en malla "+Obj.name
					Messagebox l_MsgError
					l_Error = true
				)

				if (classof l_Material == Multimaterial) then
				(
					local l_ObjName = l_ObjCopy.name
					local l_MsgError = "AVISO: Detectado un multimaterial dentro de otro en el objeto " + l_ObjName
					Messagebox l_MsgError
					l_Error = true
				)
				else
				(
					-- Obtenemos tipo de vertice
					local l_VertexType=GetVertexType l_Material
					
					-- Coordenadas de textura1(diffusemap) y textura2(lightmap)
					local l_TUV11=point3 0 0 0
					local l_TUV12=point3 0 0 0
					local l_TUV13=point3 0 0 0

					local l_TUV21=point3 0 0 0
					local l_TUV22=point3 0 0 0
					local l_TUV23=point3 0 0 0

					if (bit.and l_VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
					(
						if (meshop.getMapSupport l_ObjCopy 1) then
						(
							local l_TUFaceIndexs=meshop.getMapFace l_ObjCopy 1 idFace
							
							l_TUV11=meshop.getMapVert l_ObjCopy 1 l_TUFaceIndexs.x
							l_TUV11.y = 1 - l_TUV11.y
							l_TUV12=meshop.getMapVert l_ObjCopy 1 l_TUFaceIndexs.y
							l_TUV12.y = 1 - l_TUV12.y
							l_TUV13=meshop.getMapVert l_ObjCopy 1 l_TUFaceIndexs.z
							l_TUV13.y = 1 - l_TUV13.y
						)
						else
						(
							-- Comentar con Jordi el caso especial de los cables
						)
					)
				
					if (bit.and l_VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
					(
						local l_TUFaceIndexs2=meshop.getMapFace l_ObjCopy 2 idFace
						
						l_TUV21=meshop.getMapVert l_ObjCopy 2 l_TUFaceIndexs2.x
						l_TUV21.y = 1 - l_TUV21.y
						l_TUV22=meshop.getMapVert l_ObjCopy 2 l_TUFaceIndexs2.y
						l_TUV22.y = 1 - l_TUV22.y
						l_TUV23=meshop.getMapVert l_ObjCopy 2 l_TUFaceIndexs2.z
						l_TUV23.y = 1 - l_TUV23.y
					)

					-- Creamos los 3 vertices de esta cara a partir de los datos de geometria, normal y textura
					local total_vertex1 = CreateVertex l_Material Vtx1 Normal1 l_TUV11 l_TUV21 l_VertexType 
					local total_vertex2 = CreateVertex l_Material Vtx2 Normal2 l_TUV12 l_TUV22 l_VertexType
					local total_vertex3 = CreateVertex l_Material Vtx3 Normal3 l_TUV13 l_TUV23 l_VertexType
					
					-- Los añadimos al vertex buffer (si no existian ya)
					/* Metodo muy lento
					local Idx1=AddToVertexBuffer l_VertexBuffer total_vertex1 l_MaterialID
					local Idx2=AddToVertexBuffer l_VertexBuffer total_vertex3 l_MaterialID
					local Idx3=AddToVertexBuffer l_VertexBuffer total_vertex2 l_MaterialID
					*/
					local Idx1=DotNetAddVertex l_VertexBuffer total_vertex1 l_VtxsMap l_MaterialID
					local Idx2=DotNetAddVertex l_VertexBuffer total_vertex3 l_VtxsMap l_MaterialID
					local Idx3=DotNetAddVertex l_VertexBuffer total_vertex2 l_VtxsMap l_MaterialID

					-- Añadimos sus respectivos indices al indexBuffer
					AddToIndexBuffer l_IndexBuffer Idx1 l_MaterialID
					AddToIndexBuffer l_IndexBuffer Idx2 l_MaterialID
					AddToIndexBuffer l_IndexBuffer Idx3 l_MaterialID
				)
			)
		)
		
		if (l_Error==false) then
		(
			-- Calculamos centro
			local l_Center=point3 ((l_Vtx_Min.x + l_Vtx_Max.x)/2) ((l_Vtx_Min.y + l_Vtx_Max.y)/2) ((l_Vtx_Min.z + l_Vtx_Max.z)/2)
			
			-- Calculamos radio de la esfera contenedora del mesh
			local l_Radius = CalcularRadioBoundingSphere l_VertexBuffer l_Center 
			
			--PrintMeshDebugInfo filename l_ObjCopy l_VertexBuffer l_IndexBuffer l_Vtx_Min l_Vtx_Max l_Center l_Radius
			WriteMeshToFile filename l_ObjCopy l_VertexBuffer l_IndexBuffer l_Vtx_Min l_Vtx_Max l_Center l_Radius
		)
		
		-- Borramos copia del objeto creada
		if classof Obj==Editable_Poly or classof Obj==PolyMeshObject then 
		(
			delete l_ObjCopy
		)
	)
)

--CreateMesh $ "C:\\Users\\IVAN\\Desktop\\paternon.m3d"