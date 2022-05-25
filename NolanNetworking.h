/* NolanNetworking.h
* 
* Includes mandatory headers as well as any little utilities 
* I feel are useful for networking or for any network
* related projects
* 
*/

#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <chrono>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

/* Name: hasSpace
*
*  Input: string
*  Return Type: Bool
* 
*  Description: This will check each character to determine
*				if the stringcontains a space.
*/
bool hasSpace(std::string s) 
{
	for (int i = 0; i < s.length(); i++) 
	{
		if (std::isspace(s.at(i)))
			return true;
	}

	return false;
}