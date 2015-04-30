#include <iostream>
#include <vector>

//#include "Md5Compress.h"
#include "tools.h"

uint32 init_1, init_2;

void find_block0(uint32 block[], const uint32 IV[])
{
	uint32 Q[68] = { IV[0], 
					 IV[3], 
					 IV[2], 
					 IV[1] 
				   };

	std::vector<uint32> q_vec_mac_4(16);
	for (unsigned k = 0; k < q_vec_mac_4.size(); ++k)
		q_vec_mac_4[k] = ((k << 2) ^ (k << 26)) & 0x38000004;

	std::vector<uint32> q_vec_mac(8);

	for (unsigned k = 0; k < q_vec_mac.size(); ++k)
		q_vec_mac[k] = ((k << 13) ^ (k << 4)) & 0x2060;
		
	std::vector<uint32> q_vec_mac_9(1 << 16);
	for (unsigned k = 0; k < q_vec_mac_9.size(); ++k)
		q_vec_mac_9[k] = ((k << 1) ^ (k << 2) ^ (k << 5) ^ (k << 7) ^ (k << 8) ^ (k << 10) ^ (k << 11) ^ (k << 13)) & 0x0eb94f16;

	while (true)
	{
		Q[FLAG + 1] = changing();
		Q[FLAG + 3] = (changing() & 0xfe87bc3f) | 0x017841c0;
		Q[FLAG + 4] = (changing() & 0x44000033) | 0x000002c0 | (Q[FLAG + 3] & 0x0287bc00);
		Q[FLAG + 5] = 0x41ffffc8 | (Q[FLAG + 4] & 0x04000033);
		Q[FLAG + 6] = 0xb84b82d6;
		Q[FLAG + 7] = (changing() & 0x68000084) | 0x02401b43;
		Q[FLAG + 8] = (changing() & 0x2b8f6e04) | 0x005090d3 | (~Q[FLAG + 7] & 0x40000000);
		Q[FLAG + 9] = 0x20040068 | (Q[FLAG + 8] & 0x00020000) | (~Q[FLAG + 8] & 0x40000000);
		Q[FLAG + 10] = (changing() & 0x40000000) | 0x1040b089;
		Q[FLAG + 11] = (changing() & 0x10408008) | 0x0fbb7f16 | (~Q[FLAG + 10] & 0x40000000);
		Q[FLAG + 12] = (changing() & 0x1ed9df7f) | 0x00022080 | (~Q[FLAG + 11] & 0x40200000);
		Q[FLAG + 13] = (changing() & 0x5efb4f77) | 0x20049008;
		Q[FLAG + 14] = (changing() & 0x1fff5f77) | 0x0000a088 | (~Q[FLAG + 13] & 0x40000000);
		Q[FLAG + 15] = (changing() & 0x5efe7ff7) | 0x80008000 | (~Q[FLAG + 14] & 0x00010000);
		Q[FLAG + 16] = (changing() & 0x1ffdffff) | 0xa0000000 | (~Q[FLAG + 15] & 0x40020000);

		MD5_REVERSE_STEP(0, 0xd76aa478, 7);
		MD5_REVERSE_STEP(6, 0xa8304613, 17);
		MD5_REVERSE_STEP(7, 0xfd469501, 22);
		MD5_REVERSE_STEP(11, 0x895cd7be, 22);
		MD5_REVERSE_STEP(14, 0xa679438e, 17);
		MD5_REVERSE_STEP(15, 0x49b40821, 22);

		const uint32 tt1 = FF(Q[FLAG + 1], Q[FLAG + 0], Q[FLAG - 1]) + Q[FLAG - 2] + 0xe8c7b756;		
		const uint32 tt17 = GG(Q[FLAG + 16], Q[FLAG + 15], Q[FLAG + 14]) + Q[FLAG + 13] + 0xf61e2562;
		const uint32 tt18 = Q[FLAG + 14] + 0xc040b340 + block[6];
		const uint32 tt19 = Q[FLAG + 15] + 0x265e5a51 + block[11];
		const uint32 tt20 = Q[FLAG + 16] + 0xe9b6c7aa + block[0];
		const uint32 tt5 = RR(Q[FLAG + 6] - Q[FLAG + 5], 12) - FF(Q[FLAG + 5], Q[FLAG + 4], Q[FLAG + 3]) - 0x4787c62a;


		unsigned counter = 0;

		while (counter < (1 << 7))
		{
			const uint32 q16 = Q[FLAG + 16];
			uint32 q17 = ((changing() & 0x3ffd7ff7) | (q16&0xc0008008)) ^ 0x40000000;
			++counter;

			uint32 q18 = GG(q17, q16, Q[FLAG + 15]) + tt18;
			q18 = RL(q18, 9); 
			q18 += q17;
			if (0x00020000 != ((q18 ^ q17) & 0xa0020000))
				continue;

			uint32 q19 = GG(q18, q17, q16) + tt19;
			q19 = RL(q19, 14); 
			q19 += q18;
			if (0x80000000 != (q19 & 0x80020000))
				continue;
			
			uint32 q20 = GG(q19, q18, q17) + tt20;
			q20 = RL(q20, 20); 
			q20 += q19;
			if (0x00040000 != ((q20 ^ q19) & 0x80040000))
				continue;

			block[1] = q17-q16; 
			block[1] = RR(block[1], 5); 
			block[1] -= tt17;
			uint32 q2 = block[1] + tt1; 
			q2 = RL(q2, 12); 
			q2 += Q[FLAG + 1];
			block[5] = tt5 - q2;

			Q[FLAG + 2] = q2;
			Q[FLAG + 17] = q17;
			Q[FLAG + 18] = q18;
			Q[FLAG + 19] = q19;
			Q[FLAG + 20] = q20;
			MD5_REVERSE_STEP(2, 0x242070db, 17);

			counter = 0;
			break;
		}
		if (counter != 0)
			continue;

		const uint32 q4 = Q[FLAG + 4];
		const uint32 q9backup = Q[FLAG + 9];
		const uint32 tt21 = GG(Q[FLAG+20], Q[FLAG+19], Q[FLAG+18]) + Q[FLAG+17] + 0xd62f105d;

		unsigned counter2 = 0;
		while (counter2 < 16)
		{
			Q[FLAG+4] = q4 ^ q_vec_mac_4[counter2];

			++counter2;
			//std::cout << counter2 << std::endl;
			
			MD5_REVERSE_STEP(5, 0x4787c62a, 12);
			uint32 q21 = tt21 + block[5];
			q21 = RL(q21,5); q21 += Q[FLAG+20];
			if (((q21^Q[FLAG+20]) & 0x80020000) != 0)
				continue;

			Q[FLAG + 21] = q21;
			MD5_REVERSE_STEP(3, 0xc1bdceee, 22);
			MD5_REVERSE_STEP(4, 0xf57c0faf, 7);
			MD5_REVERSE_STEP(7, 0xfd469501, 22);
			
			const uint32 tt22 = GG(Q[FLAG + 21], Q[FLAG + 20], Q[FLAG + 19]) + Q[FLAG + 18] + 0x02441453;
			const uint32 tt23 = Q[FLAG + 19] + 0xd8a1e681 + block[15];
			const uint32 tt24 = Q[FLAG + 20] + 0xe7d3fbc8 + block[4];

			const uint32 tt9 = Q[FLAG + 6] + 0x8b44f7af;
			const uint32 tt10 = Q[FLAG + 7] + 0xffff5bb1;
			const uint32 tt8 = FF(Q[FLAG + 8], Q[FLAG + 7], Q[FLAG + 6]) + Q[FLAG + 5] + 0x698098d8;		
			const uint32 tt12 = RR(Q[FLAG+13]-Q[FLAG+12],7) - 0x6b901122;
			const uint32 tt13 = RR(Q[FLAG+14]-Q[FLAG+13],12) - FF(Q[FLAG+13],Q[FLAG+12],Q[FLAG+11]) - 0xfd987193;
			
			unsigned counter3 = 0;

			while (counter3 < 8)
			{
				uint32 q10 = Q[FLAG+10] ^ (q_vec_mac[counter3] & 0x60);
				Q[FLAG + 9] = q9backup ^ (q_vec_mac[counter3] & 0x2000);

				++counter3;
				//std::cout << counter3 << std::endl;

				uint32 m10 = RR(Q[FLAG+11] - q10,17);
				m10 -= FF(q10, Q[FLAG+9], Q[FLAG+8]) + tt10;

				uint32 aa = Q[FLAG + 21];
				uint32 dd = tt22+m10; 
				dd = RL(dd, 9) + aa;
				
				if (0x80000000 != (dd & 0x80000000)) 
					continue;

				uint32 bb = Q[FLAG + 20];
				uint32 cc = tt23 + GG(dd, aa, bb); 
				if ((cc & 0x20000) != 0) 
					continue;

				cc = RL(cc, 14) + dd;
				if ((cc & 0x80000000) != 0) 
					continue;

				bb = tt24 + GG(cc, dd, aa); 
				bb = RL(bb, 20) + cc;
				if ((bb & 0x80000000) == 0) 
					continue;

				block[10] = m10;
				block[13] = tt13 - q10;

				for (unsigned counter4 = 0; counter4 < (1 << 16); ++counter4)
				{
					uint32 q9 = Q[FLAG + 9] ^ q_vec_mac_9[counter4];
					block[12] = tt12 - FF(Q[FLAG + 12], Q[FLAG + 11], q10) - q9;
					uint32 m8 = q9 - Q[FLAG + 8];
					block[8] = RR(m8, 7) - tt8; 
					uint32 m9 = q10 - q9;
					block[9] = RR(m9, 12) - FF(q9, Q[FLAG + 8], Q[FLAG + 7]) - tt9; 

					uint32 a = aa;
					uint32 b = bb; 
					uint32 c = cc; 
					uint32 d = dd;
					//uint32
					
					MD5_STEP(GG, a, b, c, d, block[9], 0x21e1cde6, 5);
					MD5_STEP(GG, d, a, b, c, block[14], 0xc33707d6, 9);
					MD5_STEP(GG, c, d, a, b, block[3], 0xf4d50d87, 14);
					MD5_STEP(GG, b, c, d, a, block[8], 0x455a14ed, 20);
					MD5_STEP(GG, a, b, c, d, block[13], 0xa9e3e905, 5);
					MD5_STEP(GG, d, a, b, c, block[2], 0xfcefa3f8, 9);
					MD5_STEP(GG, c, d, a, b, block[7], 0x676f02d9, 14);
					MD5_STEP(GG, b, c, d, a, block[12], 0x8d2a4c8a, 20);
					MD5_STEP(HH, a, b, c, d, block[5], 0xfffa3942, 4);
					MD5_STEP(HH, d, a, b, c, block[8], 0x8771f681, 11);

					c += HH(d, a, b) + block[11] + 0x6d9d6122;
					if ((c & (1 << 15)) != 0) 
						continue;

					c = (c << 16 | c >> 16) + d;
					
					MD5_STEP(HH, b, c, d, a, block[14], 0xfde5380c, 23);
					MD5_STEP(HH, a, b, c, d, block[1], 0xa4beea44, 4);
					MD5_STEP(HH, d, a, b, c, block[4], 0x4bdecfa9, 11);
					MD5_STEP(HH, c, d, a, b, block[7], 0xf6bb4b60, 16);
					MD5_STEP(HH, b, c, d, a, block[10], 0xbebfbc70, 23);
					MD5_STEP(HH, a, b, c, d, block[13], 0x289b7ec6, 4);
					MD5_STEP(HH, d, a, b, c, block[0], 0xeaa127fa, 11);
					MD5_STEP(HH, c, d, a, b, block[3], 0xd4ef3085, 16);
					MD5_STEP(HH, b, c, d, a, block[6], 0x04881d05, 23);
					MD5_STEP(HH, a, b, c, d, block[9], 0xd9d4d039, 4);
					MD5_STEP(HH, d, a, b, c, block[12], 0xe6db99e5, 11);
					MD5_STEP(HH, c, d, a, b, block[15], 0x1fa27cf8, 16);
					MD5_STEP(HH, b, c, d, a, block[2], 0xc4ac5665, 23);
					if (((b^d) & 0x80000000) != 0)
						continue;

					MD5_STEP(II, a, b, c, d, block[0], 0xf4292244, 6);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, d, a, b, c, block[7], 0x432aff97, 10);
					if (((b ^ d) >> 31) == 0) 
						continue;
					MD5_STEP(II, c, d, a, b, block[14], 0xab9423a7, 15);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, b, c, d, a, block[5], 0xfc93a039, 21);
					if (((b ^ d) >> 31) != 0) 
						continue;
					MD5_STEP(II, a, b, c, d, block[12], 0x655b59c3, 6);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, d, a, b, c, block[3], 0x8f0ccc92, 10);
					if (((b ^ d) >> 31) != 0)
						continue;
					MD5_STEP(II, c, d, a, b, block[10], 0xffeff47d, 15);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, b, c, d, a, block[1], 0x85845dd1, 21);
					if (((b ^ d) >> 31) != 0) 
						continue;
					MD5_STEP(II, a, b, c, d, block[8], 0x6fa87e4f, 6);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, d, a, b, c, block[15], 0xfe2ce6e0, 10);
					if (((b ^ d) >> 31) != 0) 
						continue;
					MD5_STEP(II, c, d, a, b, block[6], 0xa3014314, 15);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, b, c, d, a, block[13], 0x4e0811a1, 21);
					if (((b ^ d) >> 31) == 0) 
						continue;
					MD5_STEP(II, a, b, c, d, block[4], 0xf7537e82, 6);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, d, a, b, c, block[11], 0xbd3af235, 10);
					if (((b ^ d) >> 31) != 0) 
						continue;
					MD5_STEP(II, c, d, a, b, block[2], 0x2ad7d2bb, 15);
					if (((a ^ c) >> 31) != 0) 
						continue;
					MD5_STEP(II, b, c, d, a, block[9], 0xeb86d391, 21);

					uint32 IHV1 = b + IV[1];
					uint32 IHV2 = c + IV[2];
					uint32 IHV3 = d + IV[3];

					bool normal_metric = true;
					if (0x02000000 != ((IHV2 ^ IHV1) & 0x86000000)) 
						normal_metric = false;
					if (((IHV1^IHV3) & 0x82000000) != 0) 
						normal_metric = false;
					if ((IHV1 & 0x06000020) != 0) 
						normal_metric = false;
					
					bool form_metrics = true;

					if ( ((IHV1 ^ IHV2) >> 31) != 0 || ((IHV1 ^ IHV3) >> 31) != 0 ) 
						form_metrics = false;
					if ( (IHV3 & (1 << 25)) != 0 || (IHV2 & (1 << 25)) != 0 || (IHV1 & (1 << 25)) != 0 || ((IHV2 ^ IHV1) & 1) != 0) 
						form_metrics = false;
										
					if (!(normal_metric || form_metrics)) 
						continue;

					std::cout << "...." << std::flush;

					uint32 IV1[4], IV2[4];
					for (int t = 0; t < 4; ++t)
						IV2[t] = IV1[t] = IV[t];

					uint32 block2[16];
					for (int t = 0; t < 16; ++t)
						block2[t] = block[t];
					block2[4] += (1 << 31);
					block2[11] += (1 << 15);
					block2[14] += (1 << 31);

					md5_compress(IV1, block);
					md5_compress(IV2, block2);

					if ((IV2[0] == IV1[0] + (1 << 31))
						    && (IV2[1] == IV1[1] + (1 << 31) + (1 << 25))
							&& (IV2[2] == IV1[2] + (1 << 31) + (1 << 25))
							&& (IV2[3] == IV1[3] + (1 << 31) + (1 << 25)))
						return;

					if (IV2[0] != IV1[0] + (1 << 31))
						std::cout << "OK!" << std::flush;
				}
			}
		}
	}
}
