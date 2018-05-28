#include"virtual_disk_manager.h"


int houchenxi::Manager::getInput()
{
	char s[255];
	memset(s, 256, 0);
	std::cout << m_cur_node->m_absolute_path << "\\"<<">";
	std::cin.getline(s, 255);
	
	std::string tmp = s;
	std::string input = delete_space(tmp);
	input = input + " ";

	//找到命令
	int i = input.find(" ");
	m_cmd = input.substr(0,i);

	if (m_cmd == "")
		return SUCESS;

	std::map<std::string, int>::iterator iter = g_command_map.find(m_cmd);
	if (iter == g_command_map.end())
		return error_deal(INVALID_CMD_ERROR);

	//找到参数
	 tmp = input.substr(i);
	 std::string args_str = delete_space(tmp);

	 std::vector<int> index;
	 for (unsigned int i = 0; i < args_str.size(); i++)
	 {
		 if (args_str[i] == '"')
		 {
			 index.push_back(i);
		 }
	 }

	 if (index.size() % 2 != 0)
	 {
		 return error_deal(INPUT_ERROR);
	 }
	 
	 int inx = 0;
	 for (unsigned int i = 0; i < index.size(); i=i+2)
	 {
		 std::string tt = args_str.substr(inx,index[i]-inx);
		 if (tt != "")
		 {
			 bulid_args_vector(m_args,tt);
		 }
		 
		 tt = args_str.substr(index[i]+1,index[i+1]-index[i]-1);
		 if (tt != "")
		 {
			 m_args.push_back(tt);
		 }
		 inx = index[i + 1] + 1;
	 }
	 std::string tt = args_str.substr(inx);
	 if (tt != "")
	 {
		 bulid_args_vector(m_args, tt);
	 }


	return SUCESS;
}

int houchenxi::Manager::bulid_args_vector(std::vector<std::string>& args,std::string& str)
{
	std::string tmp = delete_space(str);
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;

		int start = i;
		while (tmp[i] != ' '&&i < tmp.size())
		{
			i++;
		}
		args.push_back(tmp.substr(start, i - start));
	}
	return SUCESS;
}

int houchenxi::Manager::exec()
{
	while (1)
	{
		int ret =getInput();
		if (ret != SUCESS)
		{
			continue;
		}
		
		if (m_cmd == "exit")
			return 0;
		CommandContext* context = new CommandContext(m_cur_node,m_cmd,g_command_map);
		ret=context->getResult(m_args);

		
		if (m_cmd == "cd"&&ret==SUCESS)
		{
			m_cur_node = g_cur_node;
		}
		m_cmd = "";
		m_args.clear();
	}
	return 0;
}