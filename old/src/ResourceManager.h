#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

//Helper interface for Resource
class Resource
{
  public:

	virtual ~Resource();
};

template <typename T>
class ResourceManager
{
	std::vector<T *> m_list;

	//Adds a resource to the list. Returns the index the resource was added to
	int addResource(T * _resource)
	{
		for(int i = 0;i < m_list.size();i ++)
		{
			if(m_list[i] == 0)
			{
				m_list[i] = _resource;
				return i;
			}
		}

		m_list.push_back(_resource);
		return m_list.size() - 1;
	}

  public:

	ResourceManager()
	{
	}
	~ResourceManager()
	{
		//Delete all resources
		while(!m_list.empty())
		{
			if(m_list.back())
				delete m_list.back();

			m_list.pop_back();
		}
	}

	//Loads a resource from a file. Returns a handle to the resource.
	int loadResource(const std::string & _filename)
	{
		T * t = new T();

		t->loadFromFile(_filename);

		return addResource(t) + 1;
	}

	//Finds a handle, doesn't try to load if it doesn't exist
	int findHandle(const std::string & _filename)
	{
		for(int i = 0;i < m_list.size();i ++)
		{
			T * resource = m_list[i];
			if(resource && resource->getFilename() == _filename)
			{
				return i + 1;
			}
		}

		return 0;
	}

	//Tries to find a handle. If it doesn't exist, we load the resource from the given filename
	int getHandle(const std::string & _filename)
	{
		int handle = findHandle(_filename);

		if(handle)
			return handle;

		loadResource(_filename);
	}

	//Gets a resource from a handle
	T * getResource(int _handle)
	{
		try
		{
			return m_list.at(_handle - 1).get();
		}
		catch(std::out_of_range & e)
		{
			//std::cout << "ResourceManager: Couldn't find resource with handle " << _handle << std::endl;

			return 0;
		}
	}
};

#endif
