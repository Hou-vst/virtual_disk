#pragma once
#include<vector>
#include<map>
#include"virtual_disk_node.h"
#include <time.h> 
/*
���� . .. c: C: ��ͷ��һ��תΪ����·�����д���
�����ڵ�ǰĿ¼��Ѱ����Ӧ���ļ�
*/

namespace houchenxi
{

	std::string getCur_time();

	std::string get_time(time_t tt);

	int error_deal(int ERROR_ID);

	//ȥ���ַ���ǰ����Ч�Ŀո�
	std::string delete_space(std::string& str);

	class CommandBase
	{
	public:
		CommandBase(Base* base)
		{
			m_cur_node = base;
		}
		virtual int exec_command(std::vector<std::string>& args) = 0;
		virtual int deal_op() = 0;//ÿ�����������ķ�ʽ����һ�����ṩ�ӿ��������Լ�ר��ʵ��

		std::string getFile(std::string& path);
		
		bool is_Disk_Path(std::string& path);

		std::string getDir(std::string& path);
		
		int deal_args_op(std::vector<std::string>& args);

		std::string str_pre_deal(std::string str);
		
		std::string delete_backslash(std::string& str);

		Base*  get_root();

		Base* Path_to_Node(std::string& path);

		//��һ������������ͨ������ڶ�����������ͨ���
		bool isMatch(std::string s, std::string p);

		//�����Ҫ����ð�����֮ǰҪԤ�����ַ���
		bool isAbsolutePath(std::string& tmp);
	
		bool need_wildcard_match(std::string& path);

		std::vector<std::string> wildcard_match(std::string& path);

		Base* m_cur_node;
		
		std::vector<std::string> m_option;
	private:

		Base * Relative_path_to_Node(std::string& tmp);
	};

	//·�����ܴ��ڶ������Ҫȫ����ʾ����
	//���û��·��������ʾ��ǰĿ¼���ļ�
	class Command_dir : public CommandBase
	{
	public:
		int m_file_num;
		int m_total_file_size;
		int m_dir_num;
		int m_total_dir_size;
		bool m_op_ad;
		bool m_op_s;
		

		Command_dir(Base* base):CommandBase(base)
		{
			m_op_ad = false;
			m_op_s = false;
		}

		virtual int exec_command(std::vector<std::string>& args);

		//�����������Ŀ¼��չʾ
		int show_diroctry_with_op(Base* base);

		//չʾĿ¼�µ����ļ�
		int show_diroctry(Base* base, bool ad);

		int show(Base* base);

		int show_default(Base* base);

		int show_file(Base* base);

		int show_symlink(Base* base);

		int show_symlinkd(Base* base);

		int show_dir(Base* base);

		virtual int deal_op();
		
	};

	class Command_md : public CommandBase
	{
	public:
		Command_md(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op();
		int Relative_path_md(std::string& tmp);
	};

	class Command_rd : public CommandBase
	{
	public:
		Command_rd(Base* base) :CommandBase(base)
		{
			m_op_s = false;
		}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op();
		int rd(Base* base);
		int del_single_file(Base* base);
		bool m_op_s;
	};

	class Command_cd : public CommandBase
	{
	public:
		Command_cd(Base* base) :CommandBase(base) {}

		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op() { return SUCESS; }
	};
	
	class Command_del : public CommandBase
	{
	public:
		Command_del(Base* base) :CommandBase(base) 
		{
			m_op_s = false;
		}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op();
		int del(Base* base);
		int del_single_file(Base* base);
		bool m_op_s;
	};
	
	class Command_copy : public CommandBase
	{
	public:
		Command_copy(Base* base) :CommandBase(base)
		{
			m_op_r = false;
		}
		//args����Ҫ��������·�����ɰ������·���������һ��һ���ǿ���Ŀ��·��
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op();
		int copy(std::string src_path,std::string dst_path);
		int copy_vir2vir(std::string& src_path, std::string dst_path);
		int copy_dis2vir(std::string& src_disk_path, std::string dst_vir_path);
		int list_Disk_Files(const char * dir, std::vector<std::string>& ab_disk_path);
		int get_DiskFile_mode(std::string& src_path);
		std::string get_DiskFile_time(std::string& src_path);
		Base* vir_r_copy(Base* base,Base* parent);
		//ֻ���𴴽��ļ�������ļ��Ĵ�С��Ϊ0
		Base* creat_relative_path_file(std::string& ab_disk_path, std::string& src_disk_path, std::string& dst_vir_path);
		std::vector<std::string> read_disk_file(std::string& path);

		bool m_op_r;
	};

	class Command_ren : public CommandBase
	{
	public:
		Command_ren(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op() { return SUCESS; }
		int args_check(std::vector<std::string>& args);
		int ren_dir_ab_name(Base* base);
	};

	class Command_move : public CommandBase
	{
	public:
		Command_move(Base* base) :CommandBase(base)
		{
			m_op_y = false;
		}
		//·����������������ʱ�����Ǹ���
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op();
		int ren_dir_ab_name(Base* base);
		int move(std::string& src, std::string& dst);
		//src����Ϊ�ļ���Ҳ����ΪĿ¼������dst�������ļ�
		//���dst�����ڣ�����dst�����λ��Ҳ�����ڣ���ɿ����Ǹ���
		int chg_parent(Base* src,Base* dst);
		bool m_op_y;
	};

	class Command_mklink : public CommandBase
	{
	public:
		Command_mklink(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op() { return SUCESS; }
	};

	class Command_save : public CommandBase
	{
	public:
		Command_save(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op() { return SUCESS; }
		int args_check(std::vector<std::string>& args);
		int get_DiskFile_mode(std::string& src_path);
	};

	class Command_load : public CommandBase
	{
	public:
		Command_load(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args);
		virtual int deal_op() { return SUCESS; }
		int args_check(std::vector<std::string>& args);
		int find_parent(Base* base);
	private:
		int clear_root();
	};

	class Command_cls : public CommandBase
	{
	public:
		Command_cls(Base* base) :CommandBase(base) {}
		virtual int exec_command(std::vector<std::string>& args)
		{
			system("cls");
			return SUCESS;
		}
		virtual int deal_op() { return SUCESS; }
	};

	class CommandContext
	{
	public:
		

		CommandContext(Base* node,std::string& comand,std::map<std::string,int>& g_command_map)
		{
			base = NULL;
			createCmdInstance(node,comand, g_command_map);
		}

		int createCmdInstance(Base* node,std::string& comand, std::map<std::string, int>& g_command_map)
		{
			if (base)
			{
				delete base;
				base = NULL;
			}

			std::map<std::string, int>::iterator iter = g_command_map.find(comand);
			if (iter!= g_command_map.end())
			{
				switch (iter->second)
				{
				case DIR:
					base = new Command_dir(node);
					break;
				case MD:
					base = new Command_md(node);
					break;
				case RD:
					base = new Command_rd(node);
					break;
				case CD:
					base = new Command_cd(node);
					break;
				case DEL:
					base = new Command_del(node);
					break;
				case COPY:
					base = new Command_copy(node);
					break;
				case REN:
					base = new Command_ren(node);
					break;
				case MKLINK:
					base = new Command_mklink(node);
					break;
				case SAVE:
					base = new Command_save(node);
					break;
				case LOAD:
					base = new Command_load(node);
					break;
				case CLS:
					base = new Command_cls(node);
					break;
				case MOVE:
					base = new Command_move(node);
					break;
				
				default:
					break;
				}
			}
			else
			{
				return NULL_PTR_ERROR;
			}
			base->m_cur_node = node;
			return SUCESS;
		}

		int getResult(std::vector<std::string>& args)
		{
			if (!base)
			{
				return NULL_PTR_ERROR;
			}

			int ret=base->exec_command(args);
			
			return ret;
		}

		~CommandContext()
		{
			if (base)
			{
				delete base;
			}
		}
		CommandBase * base;
		
	private:
		
	};
}   