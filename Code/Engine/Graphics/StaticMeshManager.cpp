#include <time.h>
#include <sstream> 
#include "StaticMeshManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "TextureManager.h"
#include "Base.h"


CStaticMeshManager::CStaticMeshManager() : bMeshesPreLoaded(false)
{
}


CStaticMeshManager::~CStaticMeshManager()
{
	Destroy();
}

bool CStaticMeshManager::Load (const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CStaticMeshManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_FileName = FileName;

	// Obtenemos el nodo "static_meshes"
	CXMLTreeNode  l_StaticMeshesNode = parser["Static_Meshes"];
	if (l_StaticMeshesNode.Exists())
	{
		// Obtenemos los datos para todos los meshes
		int l_count_meshes = l_StaticMeshesNode.GetNumChildren();
		for (int i = 0; i < l_count_meshes; ++i)
		{
			if (!l_StaticMeshesNode(i).IsComment())
			{
				// Obtenemos los datos de cada mesh
				std::string l_name = l_StaticMeshesNode(i).GetPszProperty("name", "");
				std::string l_filename = l_StaticMeshesNode(i).GetPszProperty("filename", "");

				if (1/*GetResource(l_name) == NULL*/)
				{
					CStaticMesh* l_StaticMesh;
					l_StaticMesh = new CStaticMesh();
					if (l_StaticMesh->Load(l_filename))
					{
						AddResource(l_name, l_StaticMesh);
					}
					else
					{
						CHECKED_DELETE(l_StaticMesh);
						std::string msg_error = "CStaticMeshManager::Load()-> Imposible cargar core mesh " + l_name + " !\n";
						LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
					}
				}
				else
				{
					std::string msg_error = "CStaticMeshManager::Load()-> Core mesh " + l_name + " repetida! IMPORTANTE: CAMBIAR SU NOMBRE!\n";
					LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
				}

			}
		}
	}

	return true;
}

void CStaticMeshManager::LoadFolder (const std::string &FolderName)
{
	bMeshesPreLoaded = true;
	std::string search_path = FolderName + "/*.*";
    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
		do { 
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				std::string l_name = fd.cFileName;
				std::string l_ext = ".m3d";
				std::string::size_type i = l_name.find(l_ext);

				if (i != std::string::npos)
				{
					l_name.erase(i, l_ext.length());

					CStaticMesh* l_StaticMesh;
					l_StaticMesh = new CStaticMesh();

					if (l_StaticMesh->Load(FolderName + "\\" + l_name + l_ext))
					{
						AddResource(l_name, l_StaticMesh);
					}
					else
					{
						CHECKED_DELETE(l_StaticMesh);
					}
				}
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
}

bool CStaticMeshManager::Reload ()
{
	if (m_FileName != "")
	{
		Destroy();
		return Load (m_FileName);
	}

	return false;
}

void CStaticMeshManager::SetMeshTexture(const std::string& mesh, const std::string texture, int stage)
{
	CStaticMesh* l_mesh = GetResource(mesh);
	if (l_mesh != NULL)
	{
		CTexture* l_text = CORE->GetTextureManager()->GetResource(texture);
		if (l_text != NULL)
			l_mesh->SetTexture(l_text, stage);
	}
}