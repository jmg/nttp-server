#ifndef MsQeue_CPP
#define MsQeue_CPP

#define DllExport __declspec(dllexport)
#include "Console.cpp"
#include <windows.h> 
#include <string>
#include <iostream>
#import "mqoa.dll" no_namespace
#include "Utils/Logs.h"

using namespace std;

class MsQeue
{
	private:	
		IMSMQQueuePtr qQueue;  
		IMSMQQueueInfoPtr qInfo;
		IMSMQMessagePtr qMsg; 
		
	public:
		MsQeue()
		{
			CoInitialize(NULL);
			OleInitialize(NULL);
			try
			{
				qInfo.CreateInstance("MSMQ.MSMQQueueInfo");
				qMsg.CreateInstance("MSMQ.MSMQMessage");				
				
				qInfo->PathName = ".\\Private$\\myPrivateQueue";
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se creo la cola en: ", qInfo->PathName );
				qInfo->Journal = 1;				
				qInfo->Create();
				qInfo->Refresh();
			}
			catch (_com_error com)
			{
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "", (char*)com.Description());
				CoUninitialize();
			}
		}
		void Insert(BSTR XmlNew)
		{			
			qQueue = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);          
			qMsg->Body = XmlNew;
			qMsg->Send(qQueue); 				
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inserto en la cola", (char*)XmlNew);						
		}
		BSTR Get()
		{			
			_variant_t timeOut((long)1000);
			_variant_t wantBody((bool)true);			

			qQueue = qInfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);  

			qMsg = qQueue->Receive(&vtMissing, &vtMissing, &wantBody, &timeOut);

			if (qMsg == NULL) 
			{
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "No hay mas mensajes en la cola", NOPARMS);
				return NULL;
			}
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se saco de la cola ", (char*)qMsg->Body.bstrVal);
			return qMsg->Body.bstrVal;						
		}
		~MsQeue()
		{
			try
			{
				CoUninitialize();
			}
			catch(exception)
			{}
		}
};

#endif