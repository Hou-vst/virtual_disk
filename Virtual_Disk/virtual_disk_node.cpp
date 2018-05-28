#include"virtual_disk_node.h"


houchenxi::Base* houchenxi::g_cur_node = NULL;

template<typename T>
void houchenxi::Serialize(std::fstream& stream, const T& v)
{
	stream.write(reinterpret_cast<const char *>(&v), sizeof(T));
}

template<typename value_type, typename size_type>
void houchenxi::Serialize(std::fstream& stream, const value_type* val, size_type size)
{
	if (size <= 0)
		return;
	stream.write(reinterpret_cast<const char *>(val), size * sizeof(value_type));
}

//�����л���������
template<typename T>
void houchenxi::Deserialize(std::fstream& stream, T& v)
{
	stream.read(reinterpret_cast<char*>(&v), sizeof(T));
}

template<typename value_type, typename size_type>
void houchenxi::Deserialize(std::fstream& stream, value_type* val, size_type size)
{
	if (size <= 0)
		return;
	stream.read(reinterpret_cast<char*>(val), size * sizeof(value_type));
}

//���Ϻ���ֻ�����ڶ����ṹ�����л��ͷ����л��������ǶԲ������ṹstring�����л������л�
void houchenxi::str_Serialize(std::fstream& stream, std::string& str)
{
	Serialize(stream, str.size());
	Serialize(stream, str.c_str(), str.size());
}

void houchenxi::str_Deserialize(std::fstream& stream, std::string& str)
{
	std::string::size_type size;
	Deserialize(stream, size);
	char* buffer = (char*)malloc(size + 1);
	memset(buffer, 0, size + 1);

	Deserialize(stream, buffer, size);
	str = buffer;

	free(buffer);
}

//��map�����л��ͷ����л�
void houchenxi::map_Serialize(std::fstream& stream, std::map<std::string, Base*> & m_children)
{
	//�ȴ���map�ĳ���
	int size = m_children.size();
	Serialize(stream, size);

	//�������map��Ԫ��
	std::map<std::string, Base*>::iterator iter = m_children.begin();
	while (iter != m_children.end())
	{
		std::string key = iter->first;
		//���л�key���ȴ�key�Ĵ�С���ٴ�key������
		str_Serialize(stream, key);
		if (iter->second == NULL)
		{
			Serialize(stream, INVALID_FILE_MODE);
		}
		else
		{
			switch (iter->second->getFile_Mode())
			{
			case DIROCTRY:
			{
				Dir * dir = (Dir*)iter->second;
				int file_mode = DIROCTRY;
				Serialize(stream, file_mode);
				dir->Dir_Serialize(stream);
				break;
			}

			case FILE:
			{
				File * file = (File*)iter->second;
				int file_mode = FILE;
				Serialize(stream, file_mode);
				file->File_Serialize(stream);
				break;
			}
			case SYMLINK:
			{
				Slink* slink = (Slink*)iter->second;
				int file_mode = SYMLINK;
				Serialize(stream, file_mode);
				slink->Slink_Serialize(stream);
				break;
			}
			case SYMLINKD:
			{
				Slink* slink = (Slink*)iter->second;
				int file_mode = SYMLINKD;
				Serialize(stream, file_mode);
				slink->Slink_Serialize(stream);
				break;
			}
			default:
				break;
			}
		}
		iter++;
	}
}

void houchenxi::map_Deserialize(std::fstream& stream, std::map<std::string, Base*>& m_children)
{
	int size = 0;
	Deserialize(stream, size);
	std::string time = "";

	for (int i = 0; i < size; i++)
	{
		std::string key = "";
		str_Deserialize(stream, key);
		int file_mode = 0;
		Deserialize(stream, file_mode);
		switch (file_mode)
		{
		case DIROCTRY:
		{
			Dir* dir = new Dir(NULL, key, time);
			dir->Dir_DeSerialize(stream);
			m_children[key] = dir;
			break;
		}

		case FILE:
		{
			File* file = new File(NULL, key, time, 0);
			file->File_DeSerialize(stream);
			m_children[key] = file;
			break;
		}
		case SYMLINK:
		{
			Slink* slink = new Slink(NULL, key, time, SYMLINK);
			slink->Slink_DeSerialize(stream);
			m_children[key] = slink;
			break;
		}
		case SYMLINKD:
		{
			Slink* slink = new Slink(NULL, key, time, SYMLINKD);
			slink->Slink_DeSerialize(stream);
			m_children[key] = slink;
			break;
		}
		default:
			break;
		}

	}
}