/*
 * main.cpp
 *
 *  Created on: 2016年5月10日
 *  Author: hhq163
 */

#include "ClientConn.h"
#include "netlib.h"
#include "Thread.h"
#include "IM.BaseDefine.pb.h"
#include "Client.h"
using namespace std;

#define MAX_LINE_LEN	1024
string connect_router = "http://192.168.200.170";

CClient* g_pClient = NULL;
class CmdThread : public CThread
{
public:
	void OnThreadRun()
	{
		while (true)
		{
			fprintf(stderr, "%s", PROMPT);	// print to error will not buffer the printed message

			if (fgets(m_buf, MAX_LINE_LEN - 1, stdin) == NULL)
			{
				fprintf(stderr, "fgets failed: %d\n", errno);
				continue;
			}

			m_buf[strlen(m_buf) - 1] = '\0';	// remove newline character

			split_cmd(m_buf);

			exec_cmd();
		}
	}
private:
	char	m_buf[MAX_LINE_LEN];
};


CmdThread g_cmd_thread;

int main(int argc, char* argv[]){

}


