#include"virtual_disk_command.h"
#include <io.h>
#include <iostream>
#include <fstream>


std::string houchenxi::getCur_time()
{
	std::string time_str = "";
	struct tm t;    //tm结构指针
	time_t now;     //声明time_t类型变量
	time(&now);     //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间

	char buffer[19];
	memset(buffer, 0, 18);
	snprintf(buffer, 18, "%d/%02d/%02d  %02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min);
	time_str = buffer;

	return time_str;
}

std::string houchenxi::get_time(time_t tt)
{
	std::string time_str = "";
	struct tm t;    //tm结构指针
	localtime_s(&t, &tt);

	char buffer[19];
	memset(buffer, 0, 18);
	snprintf(buffer, 18, "%d/%02d/%02d  %02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min);
	time_str = buffer;

	return time_str;
}

int houchenxi::error_deal(int ERROR_ID)
{
	switch (ERROR_ID)
	{
	case INVALID_CMD_ERROR:
		std::cout << "无效的命令" << std::endl;
		break;
	case MUST_VIR_FILE_ERROR:
		std::cout << "load的文件必须是序列化文件" << std::endl;
		break;
	case MUST_DISK_PATH_ERROR:
		std::cout << "路径必须是真实磁盘的路径" << std::endl;
		break;
	case LINK_NULL_ERROR:
		std::cout << "链接的目录或文件不存在" << std::endl;
		break;
	case MOVE_MUST_DIR_ERROR:
		std::cout << "移动多个文件的时候，目标文件必须是文件夹形式" << std::endl;
		break;
	case ILLEGAL_NAME_ERROR:
		std::cout << "非法的名字，不能包含 \\ /,不能是 . 或者 .." << std::endl;
		break;
	case NO_DST_FILE_ERROR:
		std::cout << "要修改的文件不存在" << std::endl;
		break;
	case DST_NAME_EXIST_ERROR:
		std::cout << "该名字已被其他文件占用" << std::endl;
		break;
	case MK_DIR_ERROR:
		std::cout << "创建虚拟磁盘文件夹失败" << std::endl;
		break;
	case OPEN_DISK_FILE_ERROR:
		std::cout << "无效的真实磁盘路径" << std::endl;
		break;
	case DST_MUST_DIR_ERROR:
		std::cout << "目标必须为目录形式" << std::endl;
		break;
	case ONLY_DEL_EMPTY_DIR_ERROR:
		std::cout << "只能删除空文件夹" << std::endl;
		break;
	case ONLY_DEL_DIR_ERROR:
		std::cout << "不能删除文件，只能删除文件夹" << std::endl;
		break;
	case INPUT_ERROR:
		std::cout << "无效的输入" << std::endl;
		break;
	case INVALID_OP_ERROR:
		std::cout << "无效的命令选项" << std::endl;
		break;
	case INVALID_PATH_ERROR:
		std::cout << "无效的路径" << std::endl;
		break;
	case NULL_PTR_ERROR:
		std::cout << "空指针异常" << std::endl;
		break;
	case NAME_CONFLICT_ERROR:
		std::cout << "子目录或文件已经存在" << std::endl;
		break;
	default:
		break;
	}
	return ERROR_ID;
}

std::string houchenxi::delete_space(std::string& str)
{
	std::string::size_type s = 0;
	for (; s < str.size(); s++)
	{
		if (str[s] == ' ')
			continue;
		break;
	}
	int e1 = str.size() - 1;
	if (e1 < 0)
	{
		return "";
	}
	unsigned int e = e1;
	for (; e >= s; e--)
	{
		if (str[e] == ' ')
			continue;
		break;
	}
	std::string tmp = str.substr(s, e - s + 1);
	return tmp;
}
/*------------------------CommandBase-----------------------------*/
std::string houchenxi::CommandBase::getFile(std::string& path)
{
	std::string t1 = delete_space(path);
	std::string t = delete_backslash(t1);
	int i = t.size()-1;
	for (; i >= 0; i--)
	{
		if (t[i] == '\\' || t[i] == '/')
			break;
	}
	if (i == -1)
		return path;
	return t.substr(i + 1);
}

bool houchenxi::CommandBase::is_Disk_Path(std::string& path)
{
	path = delete_space(path);
	if (path.size() == 0)
		return 0;
	if (path[0] == '@')
	{
		path = path.substr(1);
		return 1;
	}
	return 0;
}

std::string houchenxi::CommandBase::getDir(std::string& path)
{
	std::string t = delete_space(path);
	std::string tmp = delete_backslash(t);
	std::string ret = "";
	int i = tmp.size()-1;
	for (; i >= 0; i--)
	{
		if (tmp[i] == '\\' || tmp[i] == '/')
			break;
	}
	if (i == -1)
		return ret;
	ret = t.substr(0, i);
	return ret;
}

int houchenxi::CommandBase::deal_args_op(std::vector<std::string>& args)
{
	unsigned int i = 0;
	for (; i < args.size(); i++)
	{
		if (args[i] == "")
			continue;
		if (args[i][0] == '/'&&args[i].size()<=3)
		{
			m_option.push_back(args[i]);
			continue;
		}
		break;
	}

	return deal_op();
}

std::string houchenxi::CommandBase::str_pre_deal(std::string str)
{
	std::string t = delete_space(str);
	std::string tmp = delete_backslash(t);
	return tmp;
}

std::string houchenxi::CommandBase::delete_backslash(std::string& str)
{
	std::string result="";
	if (str == "")
		return result;
	for (unsigned int i = 0; i < str.size() - 1; i++)
	{
		if ((str[i] == '\\' || str[i] == '/') && (str[i + 1] == '\\' || str[i + 1] == '/'))
			continue;
		result += str[i];
	}
	if (str[str.size() - 1] != '\\'&&str[str.size() - 1] != '/')
		result += str[str.size() - 1];
	return result;
}

houchenxi::Base* houchenxi::CommandBase::get_root()
{
	//std::string root_path = "c:\\";
	//Base* root = Path_to_Node(root_path);
	Base* root = m_cur_node;
	while (root != root->m_parent)
	{
		root = root->m_parent;
	}
	return root;
}

houchenxi::Base* houchenxi::CommandBase::Path_to_Node(std::string& path)
{
	std::string t = delete_space(path);
	std::string tmp = delete_backslash(t);
	if (tmp == "c:" || tmp == "C:")
		return get_root();

	if (tmp == "")
	{
		return NULL;
	}
	if (tmp.size() < 2)
	{
		//处理相对路径
		return Relative_path_to_Node(tmp);
	}
	//处理绝对路径
	if ((tmp[0] == 'C' || tmp[0] == 'c') && (tmp[1] == ':' || tmp[1] == '：'))
	{
		if (tmp[2] != '\\' && tmp[2] != '/')
		{
			error_deal(INVALID_PATH_ERROR);
			return NULL;
		}
		std::vector<std::string> path_node;
		for (unsigned int i = 2; i < tmp.size(); i++)
		{
			if (tmp[i] == '\\' || tmp[i] == '/')
			{
				int len = 0;
				int s = i + 1;
				while ((tmp[++i] != '\\'&& tmp[i] != '/') && i < tmp.size())
				{
					len++;
				}
				if (len > 0)
				{
					std::string tmp1 = tmp.substr(s, len);
					path_node.push_back(delete_space(tmp1));
				}
				i--;
			}

		}
		Base* root = get_root();
		if (!root)
		{
			error_deal(NULL_PTR_ERROR);
			return NULL;

			if (root->getFile_Mode() != DIROCTRY)
			{
				error_deal(MUST_DIR_ERROR);
				return NULL;
			}
		}
		Base* result = root;
		for (unsigned int i = 0; i < path_node.size(); i++)
		{
			if (path_node[i] == ".")
				continue;

			if (path_node[i] == "..")
			{
				result = result->m_parent;
				continue;
			}
			std::map<std::string, Base*>::iterator iter = result->m_children.find(path_node[i]);
			if (iter != result->m_children.end())
			{
				result = result->m_children[path_node[i]];
			}
			else
			{
				error_deal(INVALID_PATH_ERROR);
				return NULL;
			}
		}

		return result;
	}
	//处理相对路径
	return Relative_path_to_Node(tmp);
}

bool houchenxi::CommandBase::isMatch(std::string s, std::string p) 
{
	int ss = s.size();
	int ps = p.size();
	int smatch = -1;
	int idx = -1;
	int si = 0;
	int pi = 0;
	while (si<ss)
	{
		if (pi<ps&&p[pi] == '*')
		{
			smatch = si;
			while (pi<ps&&p[pi] == '*')
				pi++;
			idx = pi - 1;
		}
		if (pi<ps && (s[si] == p[pi] || p[pi] == '?'))
		{
			si++;
			pi++;
			continue;
		}
		if (smatch >= 0)
		{
			si = ++smatch;
			pi = idx + 1;
		}
		else
			return false;
	}
	while (pi<ps&&p[pi] == '*')
		pi++;
	return pi == ps;
}

bool houchenxi::CommandBase::isAbsolutePath(std::string& tmp)
{
	if (tmp.size() >= 3 && (tmp[0] == 'C' || tmp[0] == 'c') && (tmp[1] == ':' || tmp[1] == '：') && (tmp[2] != '\\' || tmp[2] != '/'))
		return true;
	return false;
}

bool houchenxi::CommandBase::need_wildcard_match(std::string& path)
{
	for (unsigned int i = 0; i < path.size(); i++)
	{
		if (path[i] == '*' || path[i] == '?')
			return true;
	}
	return false;
}

std::vector<std::string> houchenxi::CommandBase::wildcard_match(std::string& path)
{
	std::vector<std::string> m_match_path;//用于存储通配符匹配出来的路径，只存储绝对路径
	std::string t = delete_space(path);
	std::string tmp = delete_backslash(t);
	Base* base = NULL;
	std::string file;
	//path为相对路径的处理
	int i = tmp.size() - 1;
	for (; i >= 0; i--)
	{
		if (tmp[i] == '\\' || tmp[i] == '/')
		{
			break;
		}
	}

	if (i == -1)
	{
		base = m_cur_node;
		file = tmp;
	}
	else
	{
		std::string dir = tmp.substr(0, i);
		base = Path_to_Node(dir);
		file = tmp.substr(i + 1);
	}

	if (base == NULL)
	{
		error_deal(INVALID_PATH_ERROR);
		return m_match_path;
	}

	std::map<std::string, Base*>::iterator iter = base->m_children.begin();
	for (; iter != base->m_children.end(); iter++)
	{
		if (iter->second)
		{
			std::string source_name = iter->second->getName();
			if (isMatch(source_name, file))
			{
				m_match_path.push_back(iter->second->m_absolute_path);
			}
		}
		else
		{
			error_deal(NULL_PTR_ERROR);
			return m_match_path;
		}
	}

	return m_match_path;
}

houchenxi::Base* houchenxi::CommandBase::Relative_path_to_Node(std::string& tmp)
{
	//处理相对路径
	std::vector<std::string> path_node;
	unsigned int i = 0;
	int len = 0;
	while ((tmp[i] != '\\'&&tmp[i] != '/') && i<tmp.size())
	{
		i++;
		len++;
	}
	if (len > 0)
	{
		std::string tmp1 = tmp.substr(0, len);
		path_node.push_back(delete_space(tmp1));
	}

	for (; i < tmp.size(); i++)
	{

		if (tmp[i] == '\\' || tmp[i] == '/')
		{
			int len = 0;
			int s = i + 1;
			while ((tmp[++i] != '\\'&& tmp[i] != '/') && i < tmp.size())
			{
				len++;
			}
			if (len > 0)
			{
				std::string tmp1 = tmp.substr(s, len);
				path_node.push_back(delete_space(tmp1));
			}
			i--;
		}
	}
	Base* result = m_cur_node;
	for (unsigned int i = 0; i < path_node.size(); i++)
	{
		if (path_node[i] == ".")
			continue;

		if (path_node[i] == "..")
		{
			result = result->m_parent;
			continue;
		}
		std::map<std::string, Base*>::iterator iter = result->m_children.find(path_node[i]);
		if (iter != result->m_children.end())
		{
			result = result->m_children[path_node[i]];
		}
		else
		{
			//error_deal(INVALID_PATH_ERROR);
			return NULL;
		}
	}

	return result;
}
/*------------------------CommandBase-----------------------------*/

/*------------------------Command_dir-----------------------------*/
int houchenxi::Command_dir::exec_command(std::vector<std::string>& args)
{
	if (m_cur_node == NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}

	if (deal_args_op(args) != SUCESS)
	{
		return error_deal(INVALID_OP_ERROR);
	}
	unsigned int i = m_option.size();
	int ret = 0;
	if (i >= args.size())
	{
		ret = show_diroctry_with_op(m_cur_node);
		if (ret != SUCESS)
			return ret;
	}
	else
	{
		for (; i < args.size(); i++)
		{
			Base *base = Path_to_Node(args[i]);
			if (base == NULL)
				return NULL_PTR_ERROR;
			if (base->getFile_Mode() == DIROCTRY || base->getFile_Mode() == SYMLINKD)
			{
				ret = show_diroctry_with_op(base);
				if (ret != SUCESS)
					return ret;
			}
			else
			{
				ret = show(base);
				if (ret != SUCESS)
					return ret;
			}
		}
	}

	return SUCESS;
}

int houchenxi::Command_dir::show_diroctry_with_op(Base* base)
{
	if (base == NULL)
		return NULL_PTR_ERROR;

	if (base->getFile_Mode() == SYMLINKD)
	{
		Slink* link = (Slink*)base;
		Base* tmp = Path_to_Node(link->m_link_path);
		return show_diroctry_with_op(tmp);
	}

	if (base->getFile_Mode() != DIROCTRY)
		return INVALID_FILE_MODE_ERROR;

	std::cout << '\n' << base->m_absolute_path.c_str() << " 的目录" << '\n' << std::endl;

	if (m_op_ad == false && m_op_s == false)
	{
		return show_diroctry(base, m_op_ad);
	}
	else if (m_op_ad == true && m_op_s == true)
	{
		int ret = show_diroctry(base, m_op_ad);
		if (ret == SUCESS)
		{
			Dir* dir = (Dir*)base;
			std::map<std::string, Base*>::iterator iter = dir->m_children.begin();
			for (; iter != dir->m_children.end(); iter++)
			{
				if (iter->second&&iter->second->getFile_Mode() == DIROCTRY)
				{
					show_diroctry_with_op(iter->second);
				}
				/*else
				{
					std::cout << iter->first << "  无对象或者对象不是目录" << std::endl;
					return FAILURE;
				}*/
			}
		}
		else
			return ret;

	}
	else if (m_op_ad == true)
	{
		return show_diroctry(base, m_op_ad);
	}
	else if (m_op_s == true)
	{
		int ret = show_diroctry(base, m_op_ad);
		if (ret == SUCESS)
		{
			Dir* dir = (Dir*)base;
			std::map<std::string, Base*>::iterator iter = dir->m_children.begin();
			for (; iter != dir->m_children.end(); iter++)
			{
				if (iter->second&&iter->second->getFile_Mode() == DIROCTRY)
				{
					show_diroctry_with_op(iter->second);
				}
				/*else
				{
					std::cout << iter->first << "  无对象或者对象不是目录" << std::endl;
					return FAILURE;
				}*/
			}
		}
		else
			return ret;
	}
	return SUCESS;
}

int houchenxi::Command_dir::show_diroctry(Base* base, bool ad)
{
	if (base == NULL)
		return NULL_PTR_ERROR;

	if (base->getFile_Mode() == SYMLINKD)
	{
		Slink* link = (Slink*)base;
		Base* tmp = Path_to_Node(link->m_link_path);
		return show_diroctry(tmp, ad);
	}

	if (base->getFile_Mode() != DIROCTRY)
		return INVALID_FILE_MODE_ERROR;
	show_default(base);
	Dir* dir = (Dir*)base;
	std::map<std::string, Base*>::iterator iter = dir->m_children.begin();
	for (; iter != dir->m_children.end(); iter++)
	{
		if (ad)
		{
			if (iter->second&&iter->second->getFile_Mode() == DIROCTRY)
			{
				show(iter->second);
			}
			/*else
			{
				std::cout << iter->first << "  无对象或者对象不是目录" << std::endl;
				return FAILURE;
			}*/
		}
		else
		{
			if (iter->second)
			{
				show(iter->second);
			}
			/*else
			{
				std::cout << iter->first << "  无对象" << std::endl;
				return NULL_PTR_ERROR;
			}*/
		}
	}

	return SUCESS;
}

int houchenxi::Command_dir::show(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	int file_mode = base->getFile_Mode();
	int ret = 0;
	switch (file_mode)
	{
	case FILE:
		ret = show_file(base);
		break;
	case SYMLINK:
		ret = show_symlink(base);
		break;
	case SYMLINKD:
		ret = show_symlinkd(base);
		break;
	case DIROCTRY:
		ret = show_dir(base);
		break;
	default:
		return INVALID_FILE_MODE_ERROR;
	}

	if (ret == SUCESS)
		return SUCESS;
	else
		return FAILURE;
}

int houchenxi::Command_dir::show_default(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	char buffer[300];
	memset(buffer, 0, 300);
	std::string filemode = "<DIR>";
	std::string name = ".";
	int ret = snprintf(buffer, 300, "%s%9s          %s", base->getTime().c_str(), filemode.c_str(), name.c_str());
	std::cout << buffer << std::endl;
	if (ret <= 0)
		return FAILURE;

	if (base->m_parent == NULL)
	{
		return NULL_PTR_ERROR;
	}
	memset(buffer, 0, 300);
	name = "..";
	ret = snprintf(buffer, 300, "%s%9s          %s", base->m_parent->getTime().c_str(), filemode.c_str(), name.c_str());
	std::cout << buffer << std::endl;
	if (ret <= 0)
		return FAILURE;

	return SUCESS;
}

int houchenxi::Command_dir::show_file(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	char buffer[300];
	memset(buffer, 0, 300);
	std::string name = base->getName();
	int ret = snprintf(buffer, 300, "%s%18d ", base->getTime().c_str(), (int)base->getSize() );
	std::string bu = buffer + name;
	std::cout << bu.c_str() << std::endl;
	if (ret > 0)
		return SUCESS;
	else
		return FAILURE;
}

int houchenxi::Command_dir::show_symlink(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	char buffer[255];
	memset(buffer, 0, 255);
	std::string filemode = "<SYMLINK>";
	int ret = snprintf(buffer, 255, "%s%13s      %s", base->getTime().c_str(), filemode.c_str(), base->getName().c_str());
	std::cout << buffer << std::endl;
	if (ret > 0)
		return SUCESS;
	else
		return FAILURE;
}

int houchenxi::Command_dir::show_symlinkd(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	char buffer[255];
	memset(buffer, 0, 255);
	std::string filemode = "<SYMLINKD>";
	int ret = snprintf(buffer, 255, "%s%14s     %s", base->getTime().c_str(), filemode.c_str(), base->getName().c_str());
	std::cout << buffer << std::endl;
	if (ret > 0)
		return SUCESS;
	else
		return FAILURE;
}

int houchenxi::Command_dir::show_dir(Base* base)
{
	if (base == NULL)
	{
		return NULL_PTR_ERROR;
	}
	char buffer[255];
	memset(buffer, 0, 255);
	std::string filemode = "<DIR>";
	int ret = snprintf(buffer, 255, "%s%9s          %s", base->getTime().c_str(), filemode.c_str(), base->getName().c_str());
	std::cout << buffer << std::endl;
	if (ret > 0)
		return SUCESS;
	else
		return FAILURE;
}

int houchenxi::Command_dir::deal_op()
{
	for (unsigned int i = 0; i < m_option.size(); i++)
	{
		if (m_option[i] == "/ad")
		{
			m_op_ad = true;
		}
		else if (m_option[i] == "/s")
		{
			m_op_s = true;
		}
		else
		{
			return error_deal(INVALID_OP_ERROR);
		}
	}
	return  SUCESS;
}
/*------------------------Command_dir-----------------------------*/

/*------------------------Command_md-----------------------------*/
int houchenxi::Command_md::exec_command(std::vector<std::string>& args) 
{
	if (args.size() != 1)
	{
		return error_deal(INPUT_ERROR);
	}

	std::string t = delete_space(args[0]);
	std::string tmp = delete_backslash(t);

	if(tmp=="")
		return error_deal(INPUT_ERROR);

	if (tmp.size() < 3)
	{
		return Relative_path_md(tmp);
	}

	//处理绝对路径
	if ((tmp[0] == 'C' || tmp[0] == 'c') && (tmp[1] == ':' || tmp[1] == '：'))
	{
		if (tmp[2] != '\\' && tmp[2] != '/')
		{
			error_deal(INVALID_PATH_ERROR);
			return NULL;
		}
		//获取路径中各个结点名
		std::vector<std::string> path_node;
		for (unsigned int i = 2; i < tmp.size(); i++)
		{
			if (tmp[i] == '\\')
			{
				int len = 0;
				int s = i + 1;
				while ((tmp[++i] != '\\'&& tmp[i] != '/') && i < tmp.size())
				{
					len++;
				}
				if (len > 0)
				{
					std::string tmp1 = tmp.substr(s, len);
					path_node.push_back(delete_space(tmp1));
				}
				i--;
			}
		}
		if (path_node.size() == 0)
			return error_deal(INPUT_ERROR);
		//生成目录
		Base* dir = get_root();
		if (!dir)
		{
			return error_deal(NULL_PTR_ERROR);
		}
		unsigned int i = 0;
		for (; i < path_node.size() - 1; i++)
		{
			//路径不存在直接创建
			if (path_node[i] == ".")
				continue;

			if (path_node[i] == "..")
			{
				dir = dir->m_parent;
				continue;
			}
			std::map<std::string, Base*>::iterator iter = dir->m_children.find(path_node[i]);
			if (iter == dir->m_children.end())
			{
				std::string time = getCur_time();
				dir->m_children[path_node[i]] = new Dir(dir, path_node[i],time);
				dir = dir->m_children[path_node[i]];
			}
			else
			{
				if (iter->second)
				{
					if (iter->second->getFile_Mode() != DIROCTRY)
					{
						return error_deal(NAME_CONFLICT_ERROR);
					}
					dir = iter->second;
				}
				return error_deal(NULL_PTR_ERROR);
			}
		}
		//处理最后一个结点
		if (path_node[i] == "." || path_node[i] == "..")
		{
			return error_deal(INPUT_ERROR);
		}
		std::map<std::string, Base*>::iterator iter = dir->m_children.find(path_node[i]);
		if (iter == dir->m_children.end())
		{
			std::string time = getCur_time();
			dir->m_children[path_node[i]] = new Dir(dir, path_node[i], time);
		}
		else
		{
			return error_deal(NAME_CONFLICT_ERROR);
		}
	}
	//处理相对路径
	else
	{
		return Relative_path_md(tmp);
	}
	return SUCESS;
}

int houchenxi::Command_md::deal_op() { return SUCESS; }

int houchenxi::Command_md::Relative_path_md(std::string& tmp)
{
	std::vector<std::string> path_node;
	unsigned int i = 0;
	int len = 0;
	while ((tmp[i] != '\\'&&tmp[i] != '/') && i<tmp.size())
	{
		i++;
		len++;
	}
	if (len > 0)
	{
		std::string tmp1 = tmp.substr(0, len);
		path_node.push_back(delete_space(tmp1));
	}

	for (; i < tmp.size(); i++)
	{

		if (tmp[i] == '\\' || tmp[i] == '/')
		{
			int len = 0;
			int s = i + 1;
			while ((tmp[++i] != '\\'&& tmp[i] != '/') && i < tmp.size())
			{
				len++;
			}
			if (len > 0)
			{
				std::string tmp1 = tmp.substr(s, len);
				path_node.push_back(delete_space(tmp1));
			}
			i--;
		}
	}
	if (path_node.size() == 0)
		return error_deal(INPUT_ERROR);

	Base* base = m_cur_node;
	i = 0;
	for (; i < path_node.size() - 1; i++)
	{
		//路径不存在直接创建
		if (path_node[i] == ".")
			continue;

		if (path_node[i] == "..")
		{
			base = base->m_parent;
			continue;
		}
		std::map<std::string, Base*>::iterator iter = base->m_children.find(path_node[i]);
		if (iter == base->m_children.end())
		{
			std::string time = getCur_time();
			base->m_children[path_node[i]] = new Dir(base, path_node[i], time);
			base = base->m_children[path_node[i]];
		}
		else
		{
			if (iter->second)
			{
				if (iter->second->getFile_Mode() != DIROCTRY)
				{
					return error_deal(NAME_CONFLICT_ERROR);
				}
				base = iter->second;
			}
			else
			{
				return error_deal(NULL_PTR_ERROR);
			}
		}
	}
	//处理最后一个结点
	if (path_node[i] == "." || path_node[i] == "..")
	{
		return error_deal(INPUT_ERROR);
	}
	std::map<std::string, Base*>::iterator iter = base->m_children.find(path_node[i]);
	if (iter == base->m_children.end())
	{
		std::string time = getCur_time();
		base->m_children[path_node[i]] = new Dir(base, path_node[i], time);
	}
	else
	{
		return error_deal(NAME_CONFLICT_ERROR);
	}
	return SUCESS;
}
/*------------------------Command_md-----------------------------*/

/*------------------------Command_rd-----------------------------*/
int houchenxi::Command_rd::exec_command(std::vector<std::string>& args)
{
	if (m_cur_node == NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}

	if (deal_args_op(args) != SUCESS)
	{
		return error_deal(INVALID_OP_ERROR);
	}

	unsigned int i = m_option.size();
	if (i >= args.size())
	{
		return error_deal(INVALID_PATH_ERROR);
	}
	else
	{
		for (; i < args.size(); i++)
		{
			Base* base = Path_to_Node(args[i]);
			int ret=rd(base);
			if (ret != SUCESS)
				return ret;
		}
	}
	return SUCESS;
}

int houchenxi::Command_rd::deal_op()
{
	for (unsigned int i = 0; i < m_option.size(); i++)
	{
		if (m_option[i] == "/s")
		{
			m_op_s = true;
		}
		else
		{
			return error_deal(INVALID_OP_ERROR);
		}
	}
	return  SUCESS;
}

int houchenxi::Command_rd::del_single_file(Base* base)
{
	if (base == NULL)
		return error_deal(NULL_PTR_ERROR);
	
	Base* parent = base->getParent();
	if (parent == NULL)
		return error_deal(NULL_PTR_ERROR);
	std::map<std::string, Base*>::iterator iter = parent->m_children.find(base->getName());
	if (iter != parent->m_children.end())
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = NULL;
			parent->m_children.erase(iter);
		}
		else
		{
			return error_deal(NULL_PTR_ERROR);
		}
	}
	else
	{
		return error_deal(NULL_PTR_ERROR);
	}
	return SUCESS;

}
//只删除文件夹
//可以删除文件链接，但是不会对链接的文件造成影响
int houchenxi::Command_rd::rd(Base* base)
{
	if (base == NULL)
		return error_deal(NULL_PTR_ERROR);

	if (base->getFile_Mode() == SYMLINKD)
	{
		return del_single_file(base);
	}

	//只删除空文件
	if (!m_op_s)
	{
		if (base->getFile_Mode() != DIROCTRY && base->getFile_Mode() != SYMLINKD)
		{
			return error_deal(ONLY_DEL_DIR_ERROR);
		}

		if (base->m_children.size() > 0)
		{
			return error_deal(ONLY_DEL_EMPTY_DIR_ERROR);
		}
	}
	//强制删除
	else
	{
		//删子目录和子文件
		std::map<std::string, Base*>::iterator iter = base->m_children.begin();
		for (; iter != base->m_children.end();)
		{
			if (iter->second)
			{
				if (iter->second->getFile_Mode() != DIROCTRY)
				{
					std::map<std::string, Base*>::iterator tmp_iter = iter;
					iter++;
					delete tmp_iter->second;
					tmp_iter->second = NULL;
					base->m_children.erase(tmp_iter);
					continue;
				}
				else
				{
					std::map<std::string, Base*>::iterator tmp_iter = iter;
					iter++;
					int ret=rd(tmp_iter->second);
					if (ret != SUCESS)
					{
						return error_deal(ret);
					}
				}
			}
			else
				return error_deal(NULL_PTR_ERROR);
		}
	}
	//删除自己
	return del_single_file(base);
}
/*------------------------Command_rd-----------------------------*/

/*------------------------Command_cd-----------------------------*/
int houchenxi::Command_cd::exec_command(std::vector<std::string>& args)
{
	//参数无，显示当前的绝对路径
	if (args.size() == 0)
	{
		if (m_cur_node == NULL)
			return error_deal(NULL_PTR_ERROR);
		if (m_cur_node->getAbsolute_Path() == SUCESS)
		{
			g_cur_node = m_cur_node;
			std::cout << m_cur_node->m_absolute_path.c_str() << std::endl;
			return SUCESS;
		}
		std::cout << "获取绝对路径失败" << std::endl;
		return FAILURE;
	}

	if (args.size() > 1)
	{
		return error_deal(INVALID_PATH_ERROR);
	}

	Base* tmp = Path_to_Node(args[0]);
	if (tmp == NULL)
		return error_deal(INVALID_PATH_ERROR);
	//更改manager当前所属结点
	
	if (tmp->getFile_Mode() == DIROCTRY)
	{
		g_cur_node = tmp;
	}
	else if (tmp->getFile_Mode() == SYMLINKD)
	{
		Slink* slink = (Slink*)tmp;
		tmp = Path_to_Node(slink->m_link_path);
		if (tmp == NULL)
			return error_deal(NULL_PTR_ERROR);
		g_cur_node = tmp;
	}
	else
	{
		return error_deal(INVALID_PATH_ERROR);
	}
	return SUCESS;
}
/*------------------------Command_cd-----------------------------*/

/*------------------------Command_del-----------------------------*/
int houchenxi::Command_del::exec_command(std::vector<std::string>& args)
{
	if (m_cur_node == NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}

	if (deal_args_op(args) != SUCESS)
	{
		return error_deal(INVALID_OP_ERROR);
	}

	unsigned int i = m_option.size();
	int ret = -1;
	//删除子目录下的全部文件
	if (i == args.size())
	{
		return del(m_cur_node);
	}

	for (; i < args.size(); i++)
	{
		if (need_wildcard_match(args[i]))
		{
			std::vector<std::string> match_path = wildcard_match(args[i]);
			for (unsigned int j = 0; j < match_path.size(); j++)
			{
				ret = del(Path_to_Node(match_path[j]));
				if (ret != SUCESS)
				{
					return ret;
				}
			}
		}
		else
		{
			ret = del(Path_to_Node(args[i]));
			if (ret != SUCESS)
			{
				return ret;
			}
		}
	}

	return SUCESS;
}

//只删文件不删文件夹
//删除文件链接的时候，不会删除链接文件本身，会删除链接的文件里面的文件
int houchenxi::Command_del::del(Base* base)
{
	if (base == NULL)
		return error_deal(NULL_PTR_ERROR);

	//不删除目录链接，删除目录链接指向的目录位置的文件
	if (base->getFile_Mode() == SYMLINKD)
	{
		Slink* link = (Slink*)base;
		Base* tmp = Path_to_Node(link->m_link_path);
		return del(tmp);
	}
	//不是文件夹，直接删除
	if (base->getFile_Mode() != DIROCTRY)
	{
		return del_single_file(base);
	}
	//是文件夹，无选项，删除子目录的全部文件，跳过文件夹和目录链接
	//有选项，递归删除目录里的全部文件
	else
	{
		if (!m_op_s)
		{
			Dir* dir = (Dir*)base;
			std::map<std::string, Base*>::iterator iter = dir->m_children.begin();
			for (; iter != dir->m_children.end();)
			{
				if (iter->second)
				{
					if (iter->second->getFile_Mode() != DIROCTRY && iter->second->getFile_Mode() != SYMLINKD)
					{
						std::map<std::string, Base*>::iterator tmp_iter = iter;
						iter++;
						//注意下面的函数会删除当前的迭代器从而使迭代器失效，故在迭代器失效之前要提前指向下一个元素
						int ret=del_single_file(tmp_iter->second);

						if (ret != SUCESS)
						{
							return ret;
						}

						continue;
					}
				}
				else
				{
					return error_deal(NULL_PTR_ERROR);
				}
				iter++;
			}
		}
		else
		{
			Dir* dir = (Dir*)base;
			std::map<std::string, Base*>::iterator iter = dir->m_children.begin();
			for (; iter != dir->m_children.end();)
			{
				if (iter->second)
				{
					std::map<std::string, Base*>::iterator tmp_iter = iter;
					iter++;
					if (tmp_iter->second->getFile_Mode() != DIROCTRY && tmp_iter->second->getFile_Mode() != SYMLINKD)
					{
						//注意下面的函数会删除当前的迭代器从而使迭代器失效，故在迭代器失效之前要提前指向下一个元素
						int ret = del_single_file(tmp_iter->second);

						if (ret != SUCESS)
						{
							return ret;
						}
					}
					else
					{
						int ret = del(tmp_iter->second);
						if (ret != SUCESS)
						{
							return ret;
						}
					}
					continue;
				}
				else
				{
					return error_deal(NULL_PTR_ERROR);
				}
				iter++;
			}
		}
	}
	return SUCESS;
}

int houchenxi::Command_del::del_single_file(Base* base)
{
	if (base == NULL)
		return error_deal(NULL_PTR_ERROR);

	Base* parent = base->getParent();
	if (parent == NULL)
		return error_deal(NULL_PTR_ERROR);
	std::map<std::string, Base*>::iterator iter = parent->m_children.find(base->getName());
	if (iter != parent->m_children.end())
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = NULL;
			parent->m_children.erase(iter);
		}
		else
		{
			return error_deal(NULL_PTR_ERROR);
		}
	}
	else
	{
		return error_deal(NULL_PTR_ERROR);
	}
	return SUCESS;

}

int houchenxi::Command_del::deal_op()
{
	for (unsigned int i = 0; i < m_option.size(); i++)
	{
		if (m_option[i] == "/s")
		{
			m_op_s = true;
		}
		else
		{
			return error_deal(INVALID_OP_ERROR);
		}
	}
	return  SUCESS;
}
/*------------------------Command_del-----------------------------*/

/*------------------------Command_copy-----------------------------*/
int houchenxi::Command_copy::exec_command(std::vector<std::string>& args)
{
	if (m_cur_node == NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}

	if (deal_args_op(args) != SUCESS)
	{
		return error_deal(INVALID_OP_ERROR);
	}

	unsigned int i = m_option.size();
	if ((args.size() - i) < 2)
	{
		return error_deal(INPUT_ERROR);
	}

	//目标路径必须是文件夹
	Base* dst_node = Path_to_Node(args[args.size()-1]);
	if (!dst_node)
	{
		return error_deal(NULL_PTR_ERROR);
	}
	if (dst_node->getFile_Mode() != DIROCTRY && dst_node->getFile_Mode() != SYMLINKD)
	{
		return error_deal(DST_MUST_DIR_ERROR);
	}
	else
	{
		for (; i < args.size() - 1; i++)
		{
			int ret=copy(args[i], args[args.size() - 1]);
			if (ret != SUCESS)
			{
				return ret;
			}
		}
	}
	return SUCESS;
}

int houchenxi::Command_copy::deal_op()
{
	for (unsigned int i = 0; i < m_option.size(); i++)
	{
		if (m_option[i] == "/r")
		{
			m_op_r = true;
		}
		else
		{
			return error_deal(INVALID_OP_ERROR);
		}
	}
	return  SUCESS;
}

int houchenxi::Command_copy::copy(std::string src_path, std::string dst_path)
{
	bool src_is_disk_path = is_Disk_Path(src_path);
	bool dst_is_disk_path = is_Disk_Path(dst_path);
	if ((!src_is_disk_path) && (!dst_is_disk_path))
	{
		std::vector<std::string> match_path = wildcard_match(src_path);
		for (unsigned int i = 0; i < match_path.size(); i++)
		{
			int ret=copy_vir2vir(match_path[i], dst_path);
			if (ret != SUCESS)
				return ret;
		}
	}
	else if ((src_is_disk_path) && (!dst_is_disk_path))
	{
		return copy_dis2vir(src_path, dst_path);
	}
	else
		return error_deal(INVALID_PATH_ERROR);

	return SUCESS;
}

int houchenxi::Command_copy::copy_vir2vir(std::string& src_path, std::string dst_path)
{
	Base* dst = Path_to_Node(dst_path);
	if (dst== NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}
	if (dst->getFile_Mode() != DIROCTRY)
	{
		return error_deal(MUST_DIR_ERROR);
	}
	Base* src = Path_to_Node(src_path);
	Base* copy = vir_r_copy(src,dst);
	if(copy==NULL)
		return error_deal(NULL_PTR_ERROR);
	std::map<std::string, Base*>::iterator tmp = dst->m_children.find(src->getName());
	if (tmp != dst->m_children.end())
	{
		delete tmp->second;
		dst->m_children.erase(tmp);
	}

	dst->m_children[src->getName()] = copy;
	copy->getAbsolute_Path();

	return SUCESS;

}

houchenxi::Base* houchenxi::Command_copy::vir_r_copy(Base* base,Base* parent)
{
	if (base == NULL)
	{
		error_deal(NULL_PTR_ERROR);
		return NULL;
	}
	std::string name = base->getName();
	std::string time = base->getTime();
	Base* result=NULL;
	if (base->getFile_Mode() == FILE)
	{
		result = new File(parent,name,time,base->getSize());
		File* dst = (File*)result;
		File* src = (File*)base;
		memcpy(dst->m_buffer,src->m_buffer, (int)base->getSize());
	}
	else if(base->getFile_Mode()==SYMLINK||base->getFile_Mode()==SYMLINKD)
	{
		result = new Slink(parent,name,time,base->getFile_Mode());
		Slink* dst = (Slink*)result;
		Slink* src = (Slink*)base;
		dst->m_link_path = src->m_link_path;
	}
	else//递归处理目录
	{
		result = new Dir(parent,name,time);
		Dir* dst = (Dir*)result;
		Dir* src = (Dir*)base;
		
		std::map<std::string, Base*>::iterator iter = src->m_children.begin();
		for (;iter!=src->m_children.end();iter++)
		{
			dst->m_children[iter->first] = vir_r_copy(iter->second,dst);
		}
	}
	return result;
}

int houchenxi::Command_copy::copy_dis2vir(std::string& src_path, std::string dst_path)
{
	std::vector<std::string> ab_disk_path = read_disk_file(src_path);
	if (ab_disk_path.size() == 0)
	{
		return error_deal(INVALID_PATH_ERROR);
	}
	
	Base* dst = Path_to_Node(dst_path);

	if (dst->getFile_Mode() != DIROCTRY && dst->getFile_Mode() != SYMLINKD)
	{
		return error_deal(DST_MUST_DIR_ERROR);
	}

	for (unsigned int i = 0; i < ab_disk_path.size(); i++)
	{

		Base* base=creat_relative_path_file(ab_disk_path[i], src_path, dst_path);
		if (base != NULL)
		{
			std::ifstream in;
		
			in.open(ab_disk_path[i], std::ios::binary);//打开源文件
			if (in.fail())//打开源文件失败
			{
				in.close();
				std::cout << "open " << ab_disk_path[i].c_str() << " error" << std::endl;
				return error_deal(OPEN_DISK_FILE_ERROR);
			}
			in.seekg(0,std::ios::end);
			uint64_t size = in.tellg();
			File* file = (File*)base;
			file->setSize_and_Buffer(size);
			in.seekg(0, std::ios::beg);
			in.read(file->m_buffer,size);

			in.close();

		}
	}
	return SUCESS;
}

int houchenxi::Command_copy::list_Disk_Files(const char * dir, std::vector<std::string>& ab_disk_path)
{
	char dirNew[200];
	strcpy_s(dirNew, dir);
	ab_disk_path.push_back(dir);
	strcat_s(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return -1;
	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			strcpy_s(dirNew, dir);
			strcat_s(dirNew, "\\");
			strcat_s(dirNew, findData.name);
			
			list_Disk_Files(dirNew, ab_disk_path);
		}
		else
		{
			//cout << findData.name << "\t" << findData.size << " bytes.\n";
			//cout << dir << "\\" << findData.name << "\n";
			std::string path = std::string(dir) +"\\"+ findData.name;
			ab_disk_path.push_back(path);
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
	return SUCESS;
}

std::vector<std::string> houchenxi::Command_copy::read_disk_file(std::string& path)
{
	std::vector<std::string> ab_disk_path;
	intptr_t handle;
	_finddata_t findData;
	handle = _findfirst(path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
	{
		return ab_disk_path;
	}
	if (findData.attrib & _A_SUBDIR)
	{
		//ab_disk_path.push_back(path.c_str());
		list_Disk_Files(path.c_str(), ab_disk_path);
	}
	else
	{
		ab_disk_path.push_back(path);
	}
	return ab_disk_path;
}

int houchenxi::Command_copy::get_DiskFile_mode(std::string& src_path)
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(src_path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
		return INVALID_FILE_MODE;

	if (findData.attrib & _A_SUBDIR)
		return DIROCTRY;

	return FILE;
}

std::string houchenxi::Command_copy::get_DiskFile_time(std::string& src_path)
{
	std::string time_str = "";
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(src_path.c_str(), &findData);
	if (handle == -1)
		return time_str;

	time_str = get_time(findData.time_write);

	return time_str;

}

//C:\\Users\\Administrator\\Desktop\\fixbuild\\hcx_ll   C:\\Users\\Administrator\\Desktop\\fixbuild  
houchenxi::Base* houchenxi::Command_copy::creat_relative_path_file(std::string& ab_disk_path_1, std::string& src_disk_path_1, std::string& dst_vir_path)
{
	std::string ab_disk_path = str_pre_deal(ab_disk_path_1);
	std::string src_disk_path = str_pre_deal(src_disk_path_1);
	Base* base = Path_to_Node(dst_vir_path);
	std::string time = get_DiskFile_time(ab_disk_path);
	if (base == NULL)
	{
		Command_md* md = new Command_md(m_cur_node);
		std::vector<std::string> args;
		args.push_back(dst_vir_path);
		int ret=md->exec_command(args);
		if (ret != SUCESS)
		{
			error_deal(MK_DIR_ERROR);
			return NULL;
		}
		base = Path_to_Node(dst_vir_path);
	}
	
	if (ab_disk_path == src_disk_path)
	{
		std::string file = getFile(ab_disk_path);
		

		if (get_DiskFile_mode(ab_disk_path) != DIROCTRY)
		{

			base->m_children[file] = new File(base, file, time, 0);
			return base->m_children[file];
		}
		else
		{
			std::map<std::string, Base*>::iterator iter = base->m_children.find(file);
			if (iter == base->m_children.end())
			{
				base->m_children[file] = new Dir(base, file, time);
			}
		}

	}
	else
	{
		std::string fileff = getFile(src_disk_path);
		int i = src_disk_path.size();
		std::string relative_path = ab_disk_path.substr(i+1);
		relative_path = fileff + "\\" + relative_path;
		std::string file = getFile(relative_path);
		std::string dir = getDir(relative_path);
		Base* tmp = Path_to_Node(dir);
		if (dir == "")
		{
			tmp = base;
		}
		if (tmp == NULL)
		{
			Command_md* md = new Command_md(base);
			std::vector<std::string> args;
			args.push_back(dir);
			int ret = md->exec_command(args);
			if (ret != SUCESS)
			{
				error_deal(MK_DIR_ERROR);
				return NULL;
			}
			tmp = Path_to_Node(dir);
			if (tmp == NULL)
			{
				error_deal(NULL_PTR_ERROR);
				return NULL;
			}
		}
		if (get_DiskFile_mode(ab_disk_path) != DIROCTRY)
		{
			tmp->m_children[file] = new File(tmp, file, time, 0);
			return tmp->m_children[file];
		}
		else
		{
			std::map<std::string, Base*>::iterator iter = tmp->m_children.find(file);
			if (iter == tmp->m_children.end())
			{
				tmp->m_children[file] = new Dir(tmp, file, time);
			}
		}
	}
	return NULL;

}
/*------------------------Command_copy-----------------------------*/

/*------------------------Command_ren-----------------------------*/
int houchenxi::Command_ren::exec_command(std::vector<std::string>& args)
{
	int ret = args_check(args);
	if (ret != SUCESS)
		return ret;
	Base* base = Path_to_Node(args[0]);
	std::string new_name = delete_space(args[1]);
	//
	Base* parent = base->getParent();
	std::map<std::string, Base*>::iterator iter = parent->m_children.find(new_name);
	if (iter != parent->m_children.end())
		return error_deal(NAME_CONFLICT_ERROR);
	parent->m_children[new_name] = parent->m_children[base->getName()];
	iter = parent->m_children.find(base->getName());
	parent->m_children.erase(iter);
	//
	base->setName(new_name);
	
	if (base->getFile_Mode() != DIROCTRY)
	{
		int ret = base->getAbsolute_Path();
		{
			if (ret != SUCESS)
				return error_deal(ret);
		}
	}
	else
	{
		int ret = ren_dir_ab_name(base);
		{
			if (ret != SUCESS)
				return ret;
		}
	}
	return SUCESS;
}

int houchenxi::Command_ren::ren_dir_ab_name(Base* base)
{
	int ret=base->getAbsolute_Path();
	{
		if (ret != SUCESS)
			return error_deal(ret);
	}
	std::map<std::string, Base*>::iterator iter = base->m_children.begin();
	while (iter!= base->m_children.end())
	{
		
		if (iter->second)
		{
			if (iter->second->getFile_Mode()!= DIROCTRY)
			{
				ret = iter->second->getAbsolute_Path();
				if (ret != SUCESS)
					return error_deal(ret);
			}
			else
			{
				//iter->second->getAbsolute_Path();
				ret = ren_dir_ab_name(iter->second);
				if (ret != SUCESS)
					return error_deal(ret);
			}
		}
		else
		{
			return error_deal(NULL_PTR_ERROR);
		}
		iter++;
	}
	return SUCESS;
}

int houchenxi::Command_ren::args_check(std::vector<std::string>& args)
{
	if (args.size() != 2)
		return error_deal(INPUT_ERROR);
	
	std::string new_name = delete_space(args[1]);
	std::string::size_type idx;
	idx = new_name.find('\\');
	if (idx != std::string::npos)
		return error_deal(ILLEGAL_NAME_ERROR);
	idx = new_name.find('/');
	if (idx != std::string::npos)
		return error_deal(ILLEGAL_NAME_ERROR);
	if (new_name == "." || new_name == "..")
		return error_deal(ILLEGAL_NAME_ERROR);

	Base* base = Path_to_Node(args[0]);
	if (base == NULL)
		return error_deal(NO_DST_FILE_ERROR);

	Base* parent = base->m_parent;
	if (parent == NULL)
		return error_deal(NULL_PTR_ERROR);
	std::map<std::string, Base*>::iterator iter = parent->m_children.find(new_name);
	if (iter != parent->m_children.end())
		return error_deal(DST_NAME_EXIST_ERROR);

	return SUCESS;
}
/*------------------------Command_ren-----------------------------*/

/*------------------------Command_move-----------------------------*/
int houchenxi::Command_move::exec_command(std::vector<std::string>& args)
{
	Base* dst = Path_to_Node(args[args.size()-1]);
	//如果是转移多个文件，那么目标必须是文件夹
	deal_args_op(args);
	unsigned int i = m_option.size();
	if ((args.size() - m_option.size()) > 2)
	{
		if (dst->getFile_Mode() != DIROCTRY)
		{
			return error_deal(MOVE_MUST_DIR_ERROR);
		}
		for (; i < args.size()-1; i++)
		{
			int ret=move(args[i], args[args.size() - 1]);
			if (ret != SUCESS)
				return ret;
		}
	}
	else if ((args.size() - m_option.size()) == 2)
	{
		//目标文件即可以是文件夹也可以是文件
		int ret = move(args[i],args[i+1]);
		if (ret != SUCESS)
			return ret;
	}
	return SUCESS;
}

int houchenxi::Command_move::deal_op()
{
	for (unsigned int i = 0; i < m_option.size(); i++)
	{
		if (m_option[i] == "/y")
		{
			m_op_y = true;
		}
		else
		{
			return error_deal(INVALID_OP_ERROR);
		}
	}
	return  SUCESS;
}

int houchenxi::Command_move::ren_dir_ab_name(Base* base)
{
	int ret = base->getAbsolute_Path();
	{
		if (ret != SUCESS)
			return error_deal(ret);
	}
	std::map<std::string, Base*>::iterator iter = base->m_children.begin();
	while (iter != base->m_children.end())
	{

		if (iter->second)
		{
			if (iter->second->getFile_Mode()!= DIROCTRY)
			{
				ret = iter->second->getAbsolute_Path();
				if (ret != SUCESS)
					return error_deal(ret);
			}
			else
			{
				ret = ren_dir_ab_name(iter->second);
				if (ret != SUCESS)
					return error_deal(ret);
			}
		}
		else
		{
			return error_deal(NULL_PTR_ERROR);
		}
		iter++;
	}
	return SUCESS;
}

int houchenxi::Command_move::chg_parent(Base* src, Base* dst)
{
	if (src == NULL || dst == NULL)
		return error_deal(NULL_PTR_ERROR);
	if (dst->getFile_Mode() != DIROCTRY)
		return error_deal(MUST_DIR_ERROR);

	//找到旧父类
	Base* parent = src->m_parent;
	if(parent==NULL)
		return error_deal(NULL_PTR_ERROR);
	if (parent == dst)
		return SUCESS;

	

	//在新的父类中注册
	std::map<std::string, Base*>::iterator iter1 = dst->m_children.find(src->getName());
	if (iter1 != dst->m_children.end())//有同名冲突
	{
		if (m_op_y)
		{
			delete iter1->second;
			dst->m_children[src->getName()] = src;
		}
		else
		{
			return error_deal(NAME_CONFLICT_ERROR);
		}
	}
	else
	{
		dst->m_children[src->getName()] = src;
	}
	//指向新的父类
	src->m_parent = dst;
	//注销旧父类的注册
	std::map<std::string, Base*>::iterator iter = parent->m_children.find(src->getName());
	if (iter != parent->m_children.end())
	{
		parent->m_children.erase(iter);
	}
	else
	{
		return error_deal(NULL_PTR_ERROR);
	}

	ren_dir_ab_name(src);
	return SUCESS;
}

int houchenxi::Command_move::move(std::string& src, std::string& dst)
{
	Base* src_base = Path_to_Node(src);
	if (src_base == NULL)
		return error_deal(INVALID_PATH_ERROR);

	Base* dst_base = Path_to_Node(dst);

	if (dst_base != NULL)
	{
		if (dst_base->getFile_Mode() == SYMLINKD)
		{
			Slink* slink = (Slink*)dst_base;
			Base* tmp = Path_to_Node(slink->m_link_path);
			if (tmp == NULL)
			{
				return error_deal(LINK_NULL_ERROR);
			}
			dst_base = tmp;
		}

		if (src_base->m_absolute_path == dst_base->m_absolute_path)
		{
			std::cout << "移动的目标位置和当前位置一致" << std::endl;
			return INPUT_ERROR;
		}
	}
	//移动或者覆盖
	if (dst_base)
	{
		if (dst_base->getFile_Mode() == DIROCTRY)
		{
			int ret = chg_parent(src_base, dst_base);
			if (ret != SUCESS)
			{
				return ret;
			}
		}
		else
		{
			if (m_op_y)
			{
				Base* parent = dst_base->getParent();
				src_base->setName(dst_base->getName());
				int ret = chg_parent(src_base, parent);
				if (ret != SUCESS)
				{
					return ret;
				}
			}
			else
			{
				return error_deal(NAME_CONFLICT_ERROR);
			}
		}
	}//移动或者覆盖
	else//改名
	{
		std::string tmp = str_pre_deal(dst);
		std::string dir = getDir(tmp);
		std::string name = getFile(tmp);
		if (dir == "")
		{
			Command_ren* ren = new Command_ren(m_cur_node);
			std::vector<std::string> args;
			args.push_back(src);
			args.push_back(dst);
			ren->exec_command(args);
		}
		else
		{
			Base* parent = Path_to_Node(dir);
			
			if (parent == NULL)
				return error_deal(INVALID_PATH_ERROR);

			
			int ret = chg_parent(src_base, parent);
			if (ret != SUCESS)
			{
				return ret;
			}
			src_base->setName(name);
			ren_dir_ab_name(src_base);
		}

	}
	return SUCESS;
}
/*------------------------Command_move-----------------------------*/

/*------------------------Command_mklink-----------------------------*/
int houchenxi::Command_mklink::exec_command(std::vector<std::string>& args)
{
	if (args.size() != 2)
		return error_deal(INPUT_ERROR);

	std::string src_path = args[0];
	std::string link_path = str_pre_deal(args[1]);

	Base* src_base = Path_to_Node(src_path);
	if (src_base == NULL)
	{
		return error_deal(LINK_NULL_ERROR);
	}

	std::string dir = getDir(link_path);
	std::string name = getFile(link_path);
	std::string time = getCur_time();
	Base* link_dir = NULL;
	if (dir == "")
	{
		link_dir = m_cur_node;
	}
	else
	{
		link_dir = Path_to_Node(dir);
		if (link_dir == NULL)
			return error_deal(INVALID_PATH_ERROR);
	}
	std::map<std::string, Base*>::iterator iter = link_dir->m_children.find(name);
	if (iter != link_dir->m_children.end())
	{
		return error_deal(NAME_CONFLICT_ERROR);
	}

	if (src_base->getFile_Mode() == DIROCTRY)
	{
		link_dir->m_children[name] = new Slink(link_dir,name,time,SYMLINKD);
		
	}
	else
	{
		link_dir->m_children[name] = new Slink(link_dir, name, time, SYMLINK);
		
	}
	Slink* slink = (Slink*)link_dir->m_children[name];
	slink->m_link_path = src_base->m_absolute_path;
	if (slink->m_link_path == "")
		return error_deal(NULL_PTR_ERROR);
	return SUCESS;

}
/*------------------------Command_mklink-----------------------------*/

/*------------------------Command_load-----------------------------*/
int houchenxi::Command_load::exec_command(std::vector<std::string>& args)
{
	int ret = 0;
	ret = args_check(args);
	if (ret != SUCESS)
		return ret;

	ret = clear_root();
	if (ret != SUCESS)
		return ret;

	Base* root = get_root();
	if (root == NULL)
	{
		return error_deal(NULL_PTR_ERROR);
	}

	std::string disk_path = str_pre_deal(args[0].substr(1));

	std::fstream in;

	in.open(disk_path, std::ios::binary | std::ios::in);//打开源文件
	if (in.fail())//打开源文件失败
	{
		in.close();
		return error_deal(OPEN_DISK_FILE_ERROR);
	}

	Dir* dir_root = (Dir*)root;
	dir_root->Dir_DeSerialize(in);
	dir_root->m_parent = dir_root;

	find_parent(dir_root);
	in.close();
	return SUCESS;
}

int houchenxi::Command_load::find_parent(Base* base)
{
	if (base == NULL)
		return error_deal(NULL_PTR_ERROR);
	
	std::map<std::string, Base*>::iterator iter = base->m_children.begin();

	for (; iter != base->m_children.end(); iter++)
	{
		if (iter->second)
		{
			iter->second->m_parent = base;
			if (iter->second->getFile_Mode() == DIROCTRY)
			{
				find_parent(iter->second);
			}
		}
		else
			return error_deal(NULL_PTR_ERROR);
	}
	return SUCESS;
}

int houchenxi::Command_load::args_check(std::vector<std::string>& args)
{
	if (args.size() != 1)
		return error_deal(INPUT_ERROR);

	std::string tmp = delete_space(args[0]);

	if (tmp[0] != '@')
		return error_deal(MUST_DISK_PATH_ERROR);

	return SUCESS;
}

int houchenxi::Command_load::clear_root()
{
	
	Base* root = get_root();
	if (root == NULL)
		return error_deal(NULL_PTR_ERROR);

	std::map<std::string, Base*>::iterator iter = root->m_children.begin();
	while (iter != root->m_children.end())
	{
		std::map<std::string, Base*>::iterator tmp_iter = iter;
		iter++;

		if (tmp_iter->second)
		{
			delete tmp_iter->second;
		}
		root->m_children.erase(tmp_iter);
	}
	return SUCESS;
}
/*------------------------Command_load-----------------------------*/

/*------------------------Command_save-----------------------------*/
int houchenxi::Command_save::exec_command(std::vector<std::string>& args)
{
	int ret = args_check(args);
	if (ret != SUCESS)
		return ret;
	
	Base* root = get_root();
	if (root == NULL)
		return error_deal(NULL_PTR_ERROR);
	args[0] = args[0].substr(1);
	if (get_DiskFile_mode(args[0]) == DIROCTRY)
	{
		args[0] = args[0] + "\\virtual_disk.vir";
	}

	std::fstream in;
	in.open(args[0], std::ios::binary|std::ios::out);

	if (in.fail())//打开源文件失败
	{
		in.close();
		return error_deal(OPEN_DISK_FILE_ERROR);
	}

	Dir* root_dir = (Dir*)root;
	root_dir->Dir_Serialize(in);

	in.close();
	return SUCESS;

}

int houchenxi::Command_save::get_DiskFile_mode(std::string& src_path)
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(src_path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
		return INVALID_FILE_MODE;

	if (findData.attrib & _A_SUBDIR)
		return DIROCTRY;

	return FILE;
}

int houchenxi::Command_save::args_check(std::vector<std::string>& args)
{
	if (args.size() != 1)
		return error_deal(INPUT_ERROR);

	std::string tmp = delete_space(args[0]);

	if (tmp[0] != '@')
		return error_deal(MUST_DISK_PATH_ERROR);

	return SUCESS;
}
/*------------------------Command_save-----------------------------*/

