fileIn "VertexTypes.ms"
fileIn "ExportMesh.ms"

fn existDirectory fname = (getDirectories fname).count != 0

function CopyMaterial Path  Material  =
(
	l_VertexType=GetVertexType Material
	
	if (bit.and l_VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
	(
		local l_FileName = (openBitmap Material.diffuseMap.bitmap.filename).filename	
		local l_newFileName=Path+(filenameFromPath l_FileName)
		--format "File name: %\n%\n" l_FileName l_newFileName
		copyFile l_FileName l_newFileName
	)
	if (bit.and l_VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
	(
		if (bit.and l_VertexType VERTEX_TYPE_RADIOSITY)==VERTEX_TYPE_RADIOSITY then
		(
			local l_FileName = (openBitmap Material.selfIllumMap.id_0_color_shader.bitmap.filename).filename
			local l_newFileName=Path+(filenameFromPath l_FileName)
			--format "File name lightmap x: %\n%\n" l_FileName l_newFileName
			copyFile l_FileName l_newFileName	
			
			local l_FileName = (openBitmap Material.selfIllumMap.id_1_color_shader.bitmap.filename).filename
			local l_newFileName=Path+(filenameFromPath l_FileName)
			--format "File name lightmap y: %\n%\n" l_FileName l_newFileName
			copyFile l_FileName l_newFileName	
			
			local l_FileName = (openBitmap Material.selfIllumMap.id_2_color_shader.bitmap.filename).filename
			local l_newFileName=Path+(filenameFromPath l_FileName)
			--format "File name lightmap z: %\n%\n" l_FileName l_newFileName
			copyFile l_FileName l_newFileName	
			
			if (bit.and l_VertexType VERTEX_TYPE_RADIOSITY_SPEC)==VERTEX_TYPE_RADIOSITY_SPEC then
			(
				local l_FileName = (openBitmap Material.selfIllumMap.id_3_color_shader.bitmap.filename).filename
				local l_newFileName=Path+(filenameFromPath l_FileName)
				--format "File name lightmap s: %\n%\n" l_FileName l_newFileName
				copyFile l_FileName l_newFileName					
			)
		)
		else
		(
			local l_FileName = (openBitmap Material.selfIllumMap.bitmap.filename).filename
			local l_newFileName=Path+(filenameFromPath l_FileName)
			--format "File name: %\n%\n" l_FileName l_newFileName
			copyFile l_FileName l_newFileName
		)
	)
	if (bit.and l_VertexType VERTEX_TYPE_BINORMAL)==VERTEX_TYPE_BINORMAL then
	(
		local l_FileName = (openBitmap Material.bumpMap.bitmap.filename).filename
		local l_newFileName=Path+(filenameFromPath l_FileName)
		--format "File name: %\n%\n" l_FileName l_newFileName
		copyFile l_FileName l_newFileName
	)
)

fn ExportTextures  DataPath Obj ExportMeshChecked =
(
	local l_numMat=undefined
	local l_IsMultimaterial=undefined
	local l_Material=undefined
	
	if classof(Obj.material) == Multimaterial then
	(
		l_numMat = Obj.material.numsubs
		l_IsMultimaterial=true
	)
	else
	(
		l_numMat=1
		l_IsMultimaterial=false
	)

	local l_path =undefined 
	
	if 	ExportMeshChecked == true then
	(
		--Cogemos el directorio a partir del mesh
		--creamos directorio Textures si no existe
		--l_path=(getFilenamePath DataPath)+"Textures"
		l_path=(getFilenamePath DataPath)
		--format " %\n" l_path
		if (existDirectory l_path)==false then
		(
			--crea el directorio si existe la ruta
			local itsok = makeDir l_path all:false
			--format "%\n" itsok
			if itsok==false then
			(
				l_path=getFilenamePath DataPath
			)
		)
	)
	else
	(
		--Solo esta activada la opcion textures
		l_path=DataPath
		--format "DEBUG> Path: %\n" DataPath
	)

	l_Material=Obj.material
	for i=1 to l_numMat do
	(
		if l_IsMultimaterial==true then
			l_Material=Obj.material[i]
		 
		--format "DEBUG> Data Path 2: %\n" l_path to:filedebug
		--format "DEBUG> Material: %\n" l_Material to:filedebug
		--format "DEBUG> Objeto: %\n" Obj to:filedebug
		
		CopyMaterial l_path l_Material
	)
	
)

fn ExportAllTextures DataPath ExportMeshChecked =
(	
	--global filedebug=createfile (DataPath+"\\"+g_XMLPath+"debug.xml")
	local l_Object=undefined

	local l_Objs = $*
	local l_NumObjects = l_Objs.count

	for i= 1 to l_NumObjects do
	(
		l_Object = l_Objs[i]

		--if classof l_Object==Editable_Mesh or classof l_Object==Editable_Poly then
		if isMesh l_Object then
		(
			local l_DataPath = DataPath+"\\"+g_MeshesPath
			--format "DEBUG> Data Path : %\n" l_DataPath
			ExportTextures l_DataPath l_Object ExportMeshChecked
		)
	)
) 
				
