#include "VMCommand.h"

//-------------------------------------------------------------------------------------
Bool VMCommandCenter::RegisterCommand(StringPtr _strCmdName, VMCommand::VMCommandHandler _pHandler, s32 _iMinParamCount, const VMCommand::ParamList* _defaultParamList)
{
	if(_strCmdName == NULL || !strcmp(_strCmdName, ""))
	{
		LOG_INFO("[VMCommandCenter::RegisterCommand] invalid command name\n");
		return false;
	}
	VMCommandMapIterator it = m_CommandMap.find(_strCmdName);
	if(it != m_CommandMap.end())
	{
		LOG_INFO("[VMCommandCenter::RegisterCommand] duplicate command name\n");
		return false;
	}
	VMCommand* newCommand = new VMCommand(_pHandler, _iMinParamCount, _defaultParamList);
	m_CommandMap.insert(std::pair<std::string, VMCommand*>(_strCmdName, newCommand));
	return true;
}
//#pragma optimize("", off)
Bool VMCommandCenter::ExecuteFromString(StringPtr _cmd)
{
	Char* token = NULL;
	Char tmp[VMCommand::kMaxCommandLength];
	strncpy(tmp, _cmd, VMCommand::kMaxCommandLength);
	token = strtok(tmp, " ");
	token = _strlwr(token);
	VMCommandMapIterator it = m_CommandMap.find(token);
	if(it == m_CommandMap.end())
	{
		LOG_INFO("[VMCommandCenter::ExecuteFromString] unknown command\n");
		return false;
	}
	VMCommand& rc = *((*it).second);
	VMCommand::ParamList paramList;
	while(1)
	{
		token = strtok(NULL, " ");
		if(!token)
			break;

		VMCommandParamHolder vmParam;
		vmParam.Set(token);
		paramList.PushBack(vmParam);
	}
	s32 iRet = rc.Execute(paramList);
	if(iRet)
	{
		LOG_INFO("[CMD] executing command failed: %s\n", _cmd);
		return false;
	}
	else
	{
		//LOG_INFO("[CMD] execute command successfully: %s\n", _cmd);
	}
	return true;
}