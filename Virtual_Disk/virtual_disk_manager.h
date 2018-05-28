#pragma once
#include<map>
#include"global.h"
#include<vector>
#include"virtual_disk_node.h"
#include"virtual_disk_command.h"
#include"global.h"

namespace houchenxi
{
	

	class Manager
	{
	public:
		Manager()
		{
			Generate_CMD_Map();
			std::string time = getCur_time();
			m_root = new Dir(NULL,"C:",time);
			m_root->m_parent = m_root;
			m_root->getAbsolute_Path();
			m_cur_node = m_root;
		}

		int getInput();
		int bulid_args_vector(std::vector<std::string>& args, std::string& str);
		int exec();

		Base* m_cur_node;
		Base* m_root;
		std::map<std::string, int> g_command_map;
		
	private:
		void Generate_CMD_Map()
		{
			g_command_map["dir"] = DIR;
			g_command_map["mkdir"] = MD;
			g_command_map["rd"] = RD;
			g_command_map["cd"] = CD;
			g_command_map["del"] = DEL;
			g_command_map["copy"] = COPY;
			g_command_map["ren"] = REN;
			g_command_map["move"] = MOVE;
			g_command_map["mklink"] = MKLINK;
			g_command_map["save"] = SAVE;
			g_command_map["load"] = LOAD;
			g_command_map["cls"] = CLS;
		}

		std::string m_cmd;
		std::vector<std::string> m_args;
	};
}