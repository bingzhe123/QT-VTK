#include "IPSThreadHelper.h"

extern TCPClient instruct;

void IPSThreadHelper::run()
{
	int iResult;
	std::vector<char> InputBuff;
	const int recvbuflen = 2048;
	std::string session_id;
	instruct.Connect("127.0.0.1", 9100);
	instruct.SetReceiveTimeout(5000);     // unit: ms
	instruct.SetSendTimeout(1000);
	while (!stop_thread)
	{
		char buf[100] = { 0 };

		instruct.ReceiveBytes(buf, 100);

		/*if (buf[0] == 0 )
		{
			qmessagebox::information(null, "message", qstring::fromlocal8bit("服务器无响应"),
				qmessagebox::yes, qmessagebox::yes);
			return;
		}
		else if (connected == false)
		{
			qmessagebox::information(null, "message", qstring::fromlocal8bit("跳过登录"),
				qmessagebox::yes, qmessagebox::yes);
			loginsuccess();
			return;
		}

		jsonparser parser;
		parser.reset();
		std::string recivebuf = buf;

		jsonparser::object *obj = parser.parse(recivebuf);
		std::string commandindex = obj->getstring("command_index");
		if (commandindex == "ips_login")
		{
			emit()
		}*/
	//	char recvbuf[recvbuflen] = { 0 };
	//	iResult = recv(socket_client_, recvbuf, recvbuflen, 0);
	//	if (iResult <= 0) {
	//		if (iResult == 0) {
	//			printf("Connection closing...\n");
	//		}
	//		else {
	//			printf("recv failed:%d\n", WSAGetLastError());
	//			closesocket(socket_client_);
	//			break;
	//		}
	//		break;
	//	}
	//	else {
	//		//const char* packetEndTag = "\0";
	//		const char packetEndTag = '\0';
	//		int vectorSize = InputBuff.size();//计算之前的size
	//		InputBuff.resize(iResult + vectorSize);//当前总的size
	//		memcpy(InputBuff.data() + vectorSize, recvbuf, iResult);//新接收到的数据加载到之前的后面
	//		std::string tmpStr(InputBuff.begin(), InputBuff.end());
	//		auto index = tmpStr.find(packetEndTag);
	//		std::string packetData;
	//		if (index != std::string::npos)
	//		{
	//			packetData = tmpStr.substr(0, index);
	//			Json::Reader reader;
	//			Json::Value root;
	//			if (reader.parse(packetData, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	//			{
	//				std::string command = root["command_index"].asString();
	//				const char* command_index = command.c_str();
	//				hash_t test = hash_(command_index);
	//				const std::lock_guard<std::mutex> lock(send_flag_mutex);
	//				switch (hash_(command_index))
	//				{
	//				case hash_compile_time("CT_Reconstruction_Complete"):
	//				{
	//					send_flag_ = 0;
	//					session_id = root["session_ID"].asString();
	//					session_id_array0_.push(session_id);
	//					break;
	//				}
	//				case hash_compile_time("DR_Reconstruction_Complete"):
	//				{
	//					send_flag_ = 1;
	//					session_id = root["session_ID"].asString();
	//					session_id_array1_.push(session_id);
	//					CString* session_id_CStr = new CString(session_id.c_str());
	//					PostMessage(WM_GET_DRDATA_MSG, NULL, (LPARAM)session_id_CStr);
	//					break;
	//				}
	//				case hash_compile_time("IPS_Login"):
	//				{
	//					int returnValue = root["return"].asInt();
	//					if (0 == returnValue)//回调给界面
	//					{
	//						std::cout << "IPS login succeed!" << std::endl;
	//					}
	//					else if (1 == returnValue)
	//					{
	//						std::cout << "IPS login failed!" << std::endl;
	//					}
	//					else
	//					{
	//						std::cout << "IPS login failed!" << std::endl;
	//					}
	//					break;
	//				}
	//				case hash_compile_time("Modify_Password"):
	//				{
	//					int returnValue = root["return"].asInt();
	//					if (0 == returnValue)//回调给界面
	//					{
	//						std::cout << "Modify password OK!" << std::endl;
	//					}
	//					else if (1 == returnValue)
	//					{
	//						std::cout << "Modify password error!" << std::endl;
	//					}
	//					else
	//					{
	//						std::cout << "Modify password error!" << std::endl;
	//					}
	//					break;
	//				}
	//				case hash_compile_time("Manual_Judgment"):
	//				{
	//					send_flag_ = 4;
	//					session_id = root["session_ID"].asString();
	//					session_id_array4_.push(session_id);
	//					session_id_array5_.push(session_id);
	//					std::cout << "Recieve manual judgment command OK!" << std::endl;
	//					break;
	//				}
	//				case hash_compile_time("Manual_Judgment_Result"):
	//				{
	//					std::cout << "Manual judgment result OK!" << std::endl;
	//					break;
	//				}
	//				case hash_compile_time("Modify_Model"):
	//				{
	//					std::cout << "Modify model OK!" << std::endl;
	//					break;
	//				}
	//				case hash_compile_time("Get_Review_List"):
	//				{
	//					std::cout << "Get review list OK!" << std::endl;
	//					break;
	//				}
	//				case hash_compile_time("Get_Session_Result"):
	//				{
	//					std::cout << "Get session result OK!" << std::endl;
	//					break;
	//				}
	//				}
	//			}
	//			else
	//			{
	//				std::cout << "JSON formate is error!" << std::endl;
	//			}
	//			InputBuff.erase(InputBuff.begin(), InputBuff.begin() + packetData.length() + 1);
	//			InputBuff.shrink_to_fit();
	//		}
	//	}

	//	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	instruct.Close();
}

void IPSThreadHelper::stop()
{
	stop_thread = true;
}

IPSThreadHelper::IPSThreadHelper()
{
	stop_thread = false;
}
