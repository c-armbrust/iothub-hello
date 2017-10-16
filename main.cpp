// Simple example using the _LL_ calls of Devices SDK and threads.

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "iothub_client.h"
#include "iothub_message.h"
#include "iothubtransportamqp.h"
#include <thread>
#include <iostream>

void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
	std::cout << "Send confirmation received with result: " << ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result) << std::endl;
	//IoTHubMessage_Destroy(messageHandle);
}

void run_demo()
{
	const char* deviceConnectionString = "[device connection string]";

	IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
	char msgText[1024];
	IOTHUB_MESSAGE_HANDLE  messageHandle;

	std::cout << "Simulated device demo\n";
	
	// init platform
	if(platform_init() != 0){
		std::cout << "Failed to initialize the platform" << std::endl;
	}
	else
	{
		// init iothub client handle
		if((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(deviceConnectionString, AMQP_Protocol)) == NULL){
			std::cout << "ERROR: iotHubClientHandle is NULL!" << std::endl;
		}
		else
		{
			// init iothub  message handle
			sprintf_s(msgText, sizeof(msgText), "hello world from the simulated device!");
			if((messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText))) == NULL){
				std::cout << "iotHubMessageHandle is NULL!" << std::endl;
			}
			else
			{
				// add message properties
				MAP_HANDLE propMap = IoTHubMessage_Properties(messageHandle);
				if(Map_AddOrUpdate(propMap, "Group", "group1") != MAP_OK){
					std::cout << "ERROR: Map_AddOrUpdate Failed!" << std::endl;
				}

				// send message
				if(IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, SendConfirmationCallback, NULL) != IOTHUB_CLIENT_OK) {
					std::cout << "ERROR: IoTHubClient_LL_SendEventAsync Failed!" << std::endl;
				}
				else
				{
					std::cout << "Message sent success!" << std::endl;
				}
				
				//IoTHubClient_LL_DoWork(iotHubClientHandle);
				//ThreadAPI_Sleep(1);

				while(1){
					IoTHubClient_LL_DoWork(iotHubClientHandle);
					ThreadAPI_Sleep(1);
				}
			}

			
			// destroy iothub client handle
			IoTHubClient_LL_Destroy(iotHubClientHandle);
		}
		// deinit platform
		platform_deinit();	
	}
}


int main()
{
	std::thread t([&]{
		run_demo();
	});

	t.detach();

	while(1){
		ThreadAPI_Sleep(1000);
	}
	
	return 0;
}

