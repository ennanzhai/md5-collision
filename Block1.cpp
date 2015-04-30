#include <iostream>
#include <vector>
#include "tools.h"
//#include "Md5Compress.h"

void Orignal_find_block1_fun(uint32 block[], const uint32 IV[]);

void using_s11_find_block1(uint32 block[], const uint32 IV[]);

void using_s10_find_block1(uint32 block[], const uint32 IV[]);

void using_s01_find_block1(uint32 block[], const uint32 IV[]);

void using_s00_find_block1(uint32 block[], const uint32 IV[]);

void find_block1(uint32 block[], const uint32 IV[])
{
	if (((IV[1] ^ IV[2]) & (1 << 31)) == 0 && ((IV[1] ^ IV[3]) & (1 << 31)) == 0 && (IV[3] & (1 << 25)) == 0 && (IV[2] & (1 << 25)) == 0 && (IV[1] & (1 << 25)) == 0 && ((IV[2] ^ IV[1])&1) == 0)
	{
		uint32 IV2[4] = { IV[0] + (1 << 31), IV[1] + (1 << 31) + (1 << 25), IV[2] + (1 << 31) + (1 << 25), IV[3] + (1 << 31) + (1 << 25) };
		//std::cout << "kf" << std::endl;
		if ((IV[1] & (1 << 6)) != 0 && (IV[1] & 1) != 0) 
		{
			std::cout << "Using S11" << std::flush;
			using_s11_find_block1(block, IV2);
		}
		else if ((IV[1] & (1 << 6)) != 0 && (IV[1] & 1) == 0) 
		{
			std::cout << "Using S10" << std::flush;
			using_s10_find_block1(block, IV2);
		}
		else if ((IV[1] & (1 << 6)) == 0 && (IV[1] & 1) != 0)
		{
			std::cout << "Using S01" << std::flush;
			using_s01_find_block1(block, IV2);
		} 
		else 
		{
			std::cout << "Using S00" << std::flush;
			using_s00_find_block1(block, IV2);
		}
		block[4] += 1 << 31;
		block[11] += 1 << 15;
		block[14] += 1 << 31;
	}
	else 
	{
		std::cout << "Using Normal Method" << std::flush;
		Orignal_find_block1_fun(block, IV);
	}
}
