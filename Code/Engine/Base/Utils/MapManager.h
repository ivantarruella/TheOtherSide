#pragma once
#ifndef __CLASS_MAPMANAGER_H__
#define __CLASS_MAPMANAGER_H__

//#include "Base.h"		// itb: provoca muchos problemas ya que MemLeaks.h redefine el new

#include <string>
#include <map>

template<class T>
class CMapManager
{
protected:
	typedef std::map<std::string, T*> TMapResource;
	TMapResource m_Resources;

public:
	virtual T * GetResource(const std::string &Name) const
	{
		TMapResource::const_iterator it =m_Resources.find(Name);
		if (it != m_Resources.end())
			return it->second;

		return NULL;
	}

	virtual T * GetResource(int i) const
	{
		/*TMapResource::const_iterator it =m_Resources.find(Name);
		if (it != m_Resources.end())
			return it->second;

		return NULL;*/

		TMapResource::const_iterator it =m_Resources.begin();
		unsigned int l_i = i;
		if(l_i >= m_Resources.size() || l_i < 0)
			return NULL;

		int n=0;
		for(it=m_Resources.begin(); it!=m_Resources.end(); it++)
		{
			if(n==i)
			{
				return it->second;
			}
			n++;
		}
		return NULL;
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		//TMapResource::const_iterator it =m_Resources.find(Name);
		//if (it == m_Resources.end())
		{
			m_Resources[Name] =  Resource;
			return true;
		}
		//return false;
	}

	void Destroy()
	{
		TMapResource::iterator it = m_Resources.begin();
		TMapResource::iterator it_end = m_Resources.end();

		for (; it != it_end; ++it)
		{
			//CHECKED_DELETE(it->second);
			if (it->second!=NULL)
				delete it->second;
			it->second=NULL;
		}
		m_Resources.clear();
	}
	bool RemoveResource(const std::string &Name)
	{
		TMapResource::iterator it =m_Resources.find(Name);
		if (it != m_Resources.end())
		{	
			m_Resources.erase(it);
			return true;
		}
		return false;
	}
	bool DestroyResource(const std::string &Name)
	{
		TMapResource::iterator it =m_Resources.find(Name);
		if (it != m_Resources.end())
		{
			if (it->second!=NULL)
				delete it->second;
			it->second=NULL;	
			m_Resources.erase(it);
			return true;
		}
		return false;
	}
};

#endif //__CLASS_MAPMANAGER_H__