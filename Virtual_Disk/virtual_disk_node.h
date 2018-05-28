#pragma once
#include<string>
#include<exception>
#include<map>
#include"global.h"
#include <fstream>

namespace houchenxi 
{
	class Base;
	extern Base* g_cur_node;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//序列化基础函数
	template<typename T>
	void Serialize(std::fstream& stream, const T& v);

	template<typename value_type, typename size_type>
	void Serialize(std::fstream& stream, const value_type* val, size_type size);

	//反序列化基础函数
	template<typename T>
	void Deserialize(std::fstream& stream, T& v);

	template<typename value_type, typename size_type>
	void Deserialize(std::fstream& stream, value_type* val, size_type size);

	//以上函数只适用于定长结构的序列化和反序列化，下面是对不定长结构string的序列化反序列化
	void str_Serialize(std::fstream& stream, std::string& str);

	void str_Deserialize(std::fstream& stream, std::string& str);
	
	class Base 
	{
	public:

		Base(Base* parent, std::string name,std::string& time) :m_parent(parent), m_name(name) 
		{
			m_file_mode = INVALID_FILE_MODE;
			m_time = time;
			getAbsolute_Path();
		}
		virtual ~Base()
		{

		}
		std::string getName()
		{
			return m_name;
		}

		Base* getParent()
		{
			return m_parent;
		}

		int getFile_Mode()
		{
			return m_file_mode;
		}

		uint64_t getSize()
		{
			return m_size;
		}

		std::string getTime()
		{
			return m_time;
		}

		virtual int setSize_and_Buffer(uint64_t size)
		{
			return 0;
		}

		bool setName(std::string name)
		{
			try
			{
				m_name = name;
			}
			catch (std::exception& )
			{
				return false;
			}
			return true;
		}
		
		int getAbsolute_Path()
		{
			m_absolute_path = "";
			if (m_parent == this)
			{
				m_absolute_path = m_name ;
				return SUCESS;
			}
			m_absolute_path = "\\" + m_name;
			Base* tmp = m_parent;
			if (!tmp)
			{
				return NULL_PTR_ERROR;
			}
			while (tmp != tmp->m_parent)
			{
				m_absolute_path = "\\" + tmp->getName()+ m_absolute_path;
				tmp = tmp->m_parent;
				if (!tmp)
				{
					return NULL_PTR_ERROR;
				}
			}
			m_absolute_path = tmp->m_name+ m_absolute_path;
			return SUCESS;
		}

		int Base_Serialize(std::fstream& stream)
		{
			//首先序列定长变量
			Serialize(stream,m_size);
			Serialize(stream,m_file_mode);

			//接着序列不定长变量
			str_Serialize(stream,m_absolute_path);
			str_Serialize(stream, m_name);
			str_Serialize(stream, m_time);

			//不用序列map，该结构只交给Dir来序列
			return SUCESS;
		}

		int Base_DeSerialize(std::fstream& stream)
		{
			Deserialize(stream, m_size);
			Deserialize(stream, m_file_mode);

			str_Deserialize(stream, m_absolute_path);
			str_Deserialize(stream, m_name);
			str_Deserialize(stream, m_time);

			return SUCESS;
		}


		std::map<std::string, Base*> m_children;
		Base* m_parent;
		std::string m_absolute_path;
	protected:
		std::string m_name;
		uint64_t m_size;
		int m_file_mode;
		std::string m_time;
	};

	

	void map_Serialize(std::fstream& stream, std::map<std::string, Base*> & m_children);

	void map_Deserialize(std::fstream& stream, std::map<std::string, Base*>& m_children);

	class File : public Base 
	{
	public:
		enum 
		{
			INVALID_SIZE=1,
			INVALID_PTR=2,
			MEM_ALLOC_FAILURE=3
		};

		File(Base* parent, std::string& name, std::string& time,uint64_t size) :Base(parent, name,time)
		{
			m_size = size;
			if (m_size > 0)
			{
				m_buffer = (char*)malloc((int)m_size);
				memset(m_buffer, 0, (int)m_size);
			}
			else
			{
				m_buffer = NULL;
			}
			
			m_file_mode = FILE;
		}

		virtual ~File()
		{
		
			free(m_buffer);
			m_buffer = NULL;
		}

		int File_Serialize(std::fstream& stream)
		{
			Base_Serialize(stream);

			Serialize(stream,m_buffer, m_size);

			return SUCESS;
		}

		int File_DeSerialize(std::fstream& stream)
		{
			Base_DeSerialize(stream);

			m_buffer = (char*)malloc((int)m_size);
			Deserialize(stream, m_buffer, m_size);

			return SUCESS;
		}

		virtual int setSize_and_Buffer(uint64_t size)
		{
			if (size < 0)
				return INVALID_SIZE;
			if (!m_buffer)
			{
				m_buffer = (char*)malloc((int)size);
			}
			else
			{
				char* tmp = m_buffer;
				tmp = (char*)realloc(tmp, (int)size);
				if (!tmp)
					return MEM_ALLOC_FAILURE;
				m_buffer = tmp;
			}
			m_size = size;
			return 0;
		
		}

		char* m_buffer;
	
	};

	/*
	符号链接特点：
		在系统中不占用空间
		在文件系统中不是一个单独的文件
		如果源文件被删除了，链接就没用了
		移除源文件不会影响符号链接
		移除链接文件也不会影响源文件
		文件大小为0字节和不占用空间
		文件属性的创建时间和修改时间都是软链接创建和修改时的时间
	*/
	class Slink :public Base
	{
	public:
		enum
		{
			SYMBOLIC_LINK_SIZE = 0,
			
		};
		Slink(Base* parent, std::string name, std::string& time,int slink_type) :Base(parent, name,time)
		{
			m_size = SYMBOLIC_LINK_SIZE;
			m_file_mode = slink_type;
		}

		int Slink_Serialize(std::fstream& stream)
		{
			Base_Serialize(stream);

			str_Serialize(stream,m_link_path);
			
			return SUCESS;
		}

		int Slink_DeSerialize(std::fstream& stream)
		{
			Base_DeSerialize(stream);

			str_Deserialize(stream,m_link_path);

			return SUCESS;
		}

		std::string m_link_path;
	};

	class Dir :public Base 
	{
	public:
		enum exception
		{
			NULL_PTR_ERROR = -1,
			SAME_NAME_ERROR,
			DEFAULT_DIR_SIZE ,
			GET_CHILDREN_SIZE_SUCESS 
		};

		Dir(Base* parent, std::string name, std::string& time) :Base(parent, name,time)
		{
			m_file_mode = DIROCTRY;
			setSize_and_Buffer();
		}

		virtual ~Dir()
		{
			std::map<std::string, Base*>::iterator iter = m_children.begin();
			while (iter != m_children.end())
			{
				std::map<std::string, Base*>::iterator tmp = iter;
				iter++;

				if (tmp->second)
				{
					delete tmp->second;
				}
				m_children.erase(tmp);
			}
		}

		int Dir_Serialize(std::fstream& stream)
		{
			Base_Serialize(stream);
			Serialize(stream,m_children_size);
			map_Serialize(stream,m_children);

			return SUCESS;
		}

		int Dir_DeSerialize(std::fstream& stream)
		{
			Base_DeSerialize(stream);
			Deserialize(stream,m_children_size);
			map_Deserialize(stream,m_children);

			return SUCESS;
		}

		virtual int setSize_and_Buffer()
		{
			if (getAll_Children_Size() == GET_CHILDREN_SIZE_SUCESS)
				m_size = m_children_size;
			else
				m_size = DEFAULT_DIR_SIZE;
			return 0;
		}

		uint64_t getAll_Children_Size()
		{
			m_children_size = 0;
			std::map<std::string, Base*>::iterator iter = m_children.begin();
			for (; iter != m_children.end(); iter++)
			{
				if (iter->second)
				{
					m_children_size += iter->second->getSize();
				}
				else
				{
					return NULL_PTR_ERROR;
				}
			}
			return GET_CHILDREN_SIZE_SUCESS;
		}

		int addNew_Child(Base* base)
		{
			if (!base)
				return NULL_PTR_ERROR;
			base->m_parent = this;
			std::string name = base->getName();
			std::map<std::string, Base*>::iterator iter = m_children.find(name);
			if (iter != m_children.end())
			{
				return SAME_NAME_ERROR;
			}
			else
			{
				m_children[name] = base;
			}
			return SUCESS;
		}

	
		uint64_t m_children_size;
	};
	
};