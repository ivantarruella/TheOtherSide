#pragma once
#ifndef __CLASS_VECTORMAPMANAGER_H__
#define __CLASS_VECTORMAPMANAGER_H__

#include <vector>
#include <string>
#include <map>

template<class T>
class CVectorMapManager
{
public:
	class CMapResourceValue
	{
	public:
		T *m_Value;
		size_t m_Id;
		CMapResourceValue()
			: m_Value(NULL)
			, m_Id(0)
		{
		}
		CMapResourceValue(T *Value, size_t Id)
			: m_Value(Value), m_Id(Id)
		{
		}
	};
	typedef std::vector<T *> TVectorResources;
	typedef std::map<std::string, CMapResourceValue> TMapResources;

protected:
	TVectorResources m_ResourcesVector;
	TMapResources m_ResourcesMap;

public:
	void RemoveResource(const std::string &Name)
	{
		TMapResources::const_iterator it =m_ResourcesMap.find(Name);
		if (it != m_ResourcesMap.end()) {
			m_ResourcesVector.erase(it->second.m_Id);
			m_ResourcesMap.erase(it);
		}
	}

	virtual T * GetResourceById(size_t Id)
	{
		return m_ResourcesVector[Id];
	}

	virtual T * GetResource(const std::string &Name)
	{
		TMapResources::const_iterator it = m_ResourcesMap.find(Name);
		if (it != m_ResourcesMap.end())
			return it->second.m_Value;

		return NULL;
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		//TMapResources::const_iterator it = m_ResourcesMap.find(Name);
		//if (it == m_ResourcesMap.end())
		{
			m_ResourcesMap.insert(std::pair<std::string, CMapResourceValue>(Name, CMapResourceValue(Resource, m_ResourcesVector.size())));
			m_ResourcesVector.push_back(Resource);
			return true;
		}
		//return false;
	}

	virtual void Destroy()
	{
		TMapResources::iterator it = m_ResourcesMap.begin();
		TMapResources::iterator it_end = m_ResourcesMap.end();

		for (; it != it_end; ++it)
		{
			if (it->second.m_Value!=NULL)
				delete it->second.m_Value;
			it->second.m_Value=NULL;
		}
		m_ResourcesMap.clear(); 
		m_ResourcesVector.clear();
	}

	TMapResources & GetResourcesMap()
	{
		return m_ResourcesMap;
	}

	TVectorResources & GetResourcesVector()
	{
		return m_ResourcesVector;
	}
};

#endif //__CLASS_VECTORMAPMANAGER_H__