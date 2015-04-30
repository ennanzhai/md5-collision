#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <windows.h>

#include "tools.h"

using namespace std;

const uint32 MD5IV[] = { 
	
						0x67452301, 
	
						0xefcdab89, 
	
						0x98badcfe, 
	
						0x10325476 
					  };

unsigned load_block(istream& in, uint32 block[])
{
	unsigned len = 0;
	unsigned re = 0;
	char c;
	for (unsigned k = 0; k < 16; ++k)
	{
		block[k] = 0;
		for (unsigned i = 0; i < 4; ++i)
		{
			in.get(c);
			if (in) 
				++len;
			else
				c = 0;
			block[k] += uint32((unsigned char)(c)) << (i * 8);
		}
	}
	re = len;
	return re;
}

void save_block(ostream& out, const uint32 block[])
{
	unsigned i;
	for (unsigned k = 0; k < 16; ++k)
	{
		for (i = 0; i < 4; ++i)
			out << (unsigned char)((block[k] >> (i * 8)) & 0xFF);
	}
}

void find_collision(const uint32 IV[], uint32 msg1block0[], uint32 msg1block1[], uint32 msg2block0[], uint32 msg2block1[], bool isorno)
{
	if (isorno)
	{
		cout << "Let us generating the 1st block: " << flush;
		//cout << endl;	
	}

	find_block0(msg1block0, IV);               // using find block 0

	uint32 IHV[4] = { IV[0], IV[1], IV[2], IV[3] };
	
	md5_compress(IHV, msg1block0);             // MD5 is compressed

	if (isorno)
	{
		cout << endl;
		cout << "Let us generating the 2nd block: " << flush;
		cout << endl;
	}

	find_block1(msg1block1, IHV);              // using find block 1

	for (int i = 0; i < 16; ++i)
	{
		msg2block0[i] = msg1block0[i];
		msg2block1[i] = msg1block1[i];
	}

	msg2block0[4] += 1 << 31; 
	msg2block0[11] += 1 << 15; 
	msg2block0[14] += 1 << 31;

	msg2block1[4] += 1 << 31; 
	msg2block1[11] -= 1 << 15; 
	msg2block1[14] += 1 << 31;
	
	if (isorno)
		cout << endl;
}

int main()
{
	uint32 IV[4] = { MD5IV[0], MD5IV[1], MD5IV[2], MD5IV[3] };
	uint32 msg1block0[16];
	uint32 msg1block1[16];
	uint32 msg2block0[16];
	uint32 msg2block1[16];

	srand(static_cast<unsigned>(time(static_cast<time_t*>(NULL))));

	init_1 = uint32(time(NULL));
	init_2 = 0x12345678;
	//cout << (1<<9) << endl;
	

	cout << "大家好 ! MD5的碰撞查找开始了............" << endl;
	cout << endl;
	cout << "Now Ennan Zhai Presentation Md5 Collision for You!" << endl;
	cout << endl;
	cout << "Loading please wait ... ..." << endl;
	Sleep(2000);

	cout << endl;
	
	cout << "Initial Compress Blocak: " << endl;
	
	Sleep(1500);

	int r_time = rand() % 20;
	for (int i = 0; i != r_time; ++i)
	{
		cout << ".";
		Sleep(500);
	}
	cout << endl;
	
	cout << "Initial Block Changement" << endl;
	r_time = rand() % 30;
	for (int i = 0; i != r_time; ++i)
	{
		cout << ".";
		Sleep(500);
	}

	cout << endl;
	
	find_collision(IV, msg1block0, msg1block1, msg2block0, msg2block1, true);

	ofstream ofs1("OurMsg1", ios::binary);
	save_block(ofs1, msg1block0);
	save_block(ofs1, msg1block1);
	ofstream ofs2("OurMsg2", ios::binary);
	save_block(ofs2, msg2block0);
	save_block(ofs2, msg2block1);

	cout << "OK ! 搞定了 ! " << endl;
    cout << endl;
	system("pause");

	return 0;
}





