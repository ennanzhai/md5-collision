#ifndef TOOLS_H
#define TOOLS_H

#define FLAG 3

#define MD5_STEP(f, a, b, c, d, m, ac, rc) (a += f(b, c, d) + m + ac, a = (a << rc | a >> (32 - rc)) + b)

#define MD5_REVERSE_STEP(t,AC,RC) (block[t] = Q[FLAG + t + 1] - Q[FLAG + t], block[t] = RR(block[t], RC) - FF(Q[FLAG + t], Q[FLAG + t - 1], Q[FLAG + t - 2]) - Q[FLAG + t - 3] - AC )

typedef unsigned int uint32;

void find_block0(uint32 block[], const uint32 IV[]);

void find_block1(uint32 block[], const uint32 IV[]);

void md5_compress(uint32 ihv[], const uint32 block[]);

extern uint32 init_1, init_2;

inline uint32 changing()
{
	uint32 t = init_1 ^ (init_1 << 10);
	init_1 = init_2;
	init_2 = (init_2 ^(init_2 >> 10)) ^ (t ^ (t >> 13));
	return init_1;
}

inline uint32 FF(uint32 b, uint32 c, uint32 d) 
{	
	return d ^ (b & (c ^ d)); 
}

inline uint32 GG(uint32 b, uint32 c, uint32 d) 
{	
	return c ^ (d & (b ^ c)); 
}

inline uint32 HH(uint32 b, uint32 c, uint32 d) 
{	
	return b ^ c ^ d; 
}

inline uint32 II(uint32 b, uint32 c, uint32 d) 
{	
	return c ^ (b | ~d); 
}

inline uint32 RL(uint32 x, unsigned int n)
{	
	return (x << n) | (x >> (32 - n)); 
}

inline uint32 RR(uint32 x, unsigned int n)
{	
	return (x >> n) | (x << (32 - n)); 
}


#endif
