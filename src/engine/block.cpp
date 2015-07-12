
/*
 * block.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: nbingham
 */

#include "block.h"

array<int> blockhdl::cubes[256];

blockhdl::blockhdl()
{
	size = 0.0;
	density = NULL;
	init();
}

blockhdl::blockhdl(vec3F origin, double size, float (*density)(vec3f location, void *data))
{
	this->origin = origin;
	this->size = size;
	this->density = density;
	init();
}

blockhdl::~blockhdl()
{

}

void blockhdl::init()
{
	if (cubes[2].size() == 0)
	{		cubes[0] = array<int>::join<0>();
		cubes[1] = array<int>::join<3>(0, 8, 3);
		cubes[2] = array<int>::join<3>(0, 1, 9);
		cubes[3] = array<int>::join<6>(1, 8, 3, 9, 8, 1);
		cubes[4] = array<int>::join<3>(1, 2, 10);
		cubes[5] = array<int>::join<6>(0, 8, 3, 1, 2, 10);
		cubes[6] = array<int>::join<6>(9, 2, 10, 0, 2, 9);
		cubes[7] = array<int>::join<9>(2, 8, 3, 2, 10, 8, 10, 9, 8);
		cubes[8] = array<int>::join<3>(3, 11, 2);
		cubes[9] = array<int>::join<6>(0, 11, 2, 8, 11, 0);
		cubes[10] = array<int>::join<6>(1, 9, 0, 2, 3, 11);
		cubes[11] = array<int>::join<9>(1, 11, 2, 1, 9, 11, 9, 8, 11);
		cubes[12] = array<int>::join<6>(3, 10, 1, 11, 10, 3);
		cubes[13] = array<int>::join<9>(0, 10, 1, 0, 8, 10, 8, 11, 10);
		cubes[14] = array<int>::join<9>(3, 9, 0, 3, 11, 9, 11, 10, 9);
		cubes[15] = array<int>::join<6>(9, 8, 10, 10, 8, 11);
		cubes[16] = array<int>::join<3>(4, 7, 8);
		cubes[17] = array<int>::join<6>(4, 3, 0, 7, 3, 4);
		cubes[18] = array<int>::join<6>(0, 1, 9, 8, 4, 7);
		cubes[19] = array<int>::join<9>(4, 1, 9, 4, 7, 1, 7, 3, 1);
		cubes[20] = array<int>::join<6>(1, 2, 10, 8, 4, 7);
		cubes[21] = array<int>::join<9>(3, 4, 7, 3, 0, 4, 1, 2, 10);
		cubes[22] = array<int>::join<9>(9, 2, 10, 9, 0, 2, 8, 4, 7);
		cubes[23] = array<int>::join<12>(2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4);
		cubes[24] = array<int>::join<6>(8, 4, 7, 3, 11, 2);
		cubes[25] = array<int>::join<9>(11, 4, 7, 11, 2, 4, 2, 0, 4);
		cubes[26] = array<int>::join<9>(9, 0, 1, 8, 4, 7, 2, 3, 11);
		cubes[27] = array<int>::join<12>(4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1);
		cubes[28] = array<int>::join<9>(3, 10, 1, 3, 11, 10, 7, 8, 4);
		cubes[29] = array<int>::join<12>(1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4);
		cubes[30] = array<int>::join<12>(4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3);
		cubes[31] = array<int>::join<9>(4, 7, 11, 4, 11, 9, 9, 11, 10);
		cubes[32] = array<int>::join<3>(9, 5, 4);
		cubes[33] = array<int>::join<6>(9, 5, 4, 0, 8, 3);
		cubes[34] = array<int>::join<6>(0, 5, 4, 1, 5, 0);
		cubes[35] = array<int>::join<9>(8, 5, 4, 8, 3, 5, 3, 1, 5);
		cubes[36] = array<int>::join<6>(1, 2, 10, 9, 5, 4);
		cubes[37] = array<int>::join<9>(3, 0, 8, 1, 2, 10, 4, 9, 5);
		cubes[38] = array<int>::join<9>(5, 2, 10, 5, 4, 2, 4, 0, 2);
		cubes[39] = array<int>::join<12>(2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8);
		cubes[40] = array<int>::join<6>(9, 5, 4, 2, 3, 11);
		cubes[41] = array<int>::join<9>(0, 11, 2, 0, 8, 11, 4, 9, 5);
		cubes[42] = array<int>::join<9>(0, 5, 4, 0, 1, 5, 2, 3, 11);
		cubes[43] = array<int>::join<12>(2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5);
		cubes[44] = array<int>::join<9>(10, 3, 11, 10, 1, 3, 9, 5, 4);
		cubes[45] = array<int>::join<12>(4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10);
		cubes[46] = array<int>::join<12>(5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3);
		cubes[47] = array<int>::join<9>(5, 4, 8, 5, 8, 10, 10, 8, 11);
		cubes[48] = array<int>::join<6>(9, 7, 8, 5, 7, 9);
		cubes[49] = array<int>::join<9>(9, 3, 0, 9, 5, 3, 5, 7, 3);
		cubes[50] = array<int>::join<9>(0, 7, 8, 0, 1, 7, 1, 5, 7);
		cubes[51] = array<int>::join<6>(1, 5, 3, 3, 5, 7);
		cubes[52] = array<int>::join<9>(9, 7, 8, 9, 5, 7, 10, 1, 2);
		cubes[53] = array<int>::join<12>(10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3);
		cubes[54] = array<int>::join<12>(8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2);
		cubes[55] = array<int>::join<9>(2, 10, 5, 2, 5, 3, 3, 5, 7);
		cubes[56] = array<int>::join<9>(7, 9, 5, 7, 8, 9, 3, 11, 2);
		cubes[57] = array<int>::join<12>(9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11);
		cubes[58] = array<int>::join<12>(2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7);
		cubes[59] = array<int>::join<9>(11, 2, 1, 11, 1, 7, 7, 1, 5);
		cubes[60] = array<int>::join<12>(9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11);
		cubes[61] = array<int>::join<15>(5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0);
		cubes[62] = array<int>::join<15>(11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0);
		cubes[63] = array<int>::join<6>(11, 10, 5, 7, 11, 5);
		cubes[64] = array<int>::join<3>(10, 6, 5);
		cubes[65] = array<int>::join<6>(0, 8, 3, 5, 10, 6);
		cubes[66] = array<int>::join<6>(9, 0, 1, 5, 10, 6);
		cubes[67] = array<int>::join<9>(1, 8, 3, 1, 9, 8, 5, 10, 6);
		cubes[68] = array<int>::join<6>(1, 6, 5, 2, 6, 1);
		cubes[69] = array<int>::join<9>(1, 6, 5, 1, 2, 6, 3, 0, 8);
		cubes[70] = array<int>::join<9>(9, 6, 5, 9, 0, 6, 0, 2, 6);
		cubes[71] = array<int>::join<12>(5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8);
		cubes[72] = array<int>::join<6>(2, 3, 11, 10, 6, 5);
		cubes[73] = array<int>::join<9>(11, 0, 8, 11, 2, 0, 10, 6, 5);
		cubes[74] = array<int>::join<9>(0, 1, 9, 2, 3, 11, 5, 10, 6);
		cubes[75] = array<int>::join<12>(5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11);
		cubes[76] = array<int>::join<9>(6, 3, 11, 6, 5, 3, 5, 1, 3);
		cubes[77] = array<int>::join<12>(0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6);
		cubes[78] = array<int>::join<12>(3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9);
		cubes[79] = array<int>::join<9>(6, 5, 9, 6, 9, 11, 11, 9, 8);
		cubes[80] = array<int>::join<6>(5, 10, 6, 4, 7, 8);
		cubes[81] = array<int>::join<9>(4, 3, 0, 4, 7, 3, 6, 5, 10);
		cubes[82] = array<int>::join<9>(1, 9, 0, 5, 10, 6, 8, 4, 7);
		cubes[83] = array<int>::join<12>(10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4);
		cubes[84] = array<int>::join<9>(6, 1, 2, 6, 5, 1, 4, 7, 8);
		cubes[85] = array<int>::join<12>(1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7);
		cubes[86] = array<int>::join<12>(8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6);
		cubes[87] = array<int>::join<15>(7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9);
		cubes[88] = array<int>::join<9>(3, 11, 2, 7, 8, 4, 10, 6, 5);
		cubes[89] = array<int>::join<12>(5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11);
		cubes[90] = array<int>::join<12>(0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6);
		cubes[91] = array<int>::join<15>(9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6);
		cubes[92] = array<int>::join<12>(8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6);
		cubes[93] = array<int>::join<15>(5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11);
		cubes[94] = array<int>::join<15>(0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7);
		cubes[95] = array<int>::join<12>(6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9);
		cubes[96] = array<int>::join<6>(10, 4, 9, 6, 4, 10);
		cubes[97] = array<int>::join<9>(4, 10, 6, 4, 9, 10, 0, 8, 3);
		cubes[98] = array<int>::join<9>(10, 0, 1, 10, 6, 0, 6, 4, 0);
		cubes[99] = array<int>::join<12>(8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10);
		cubes[100] = array<int>::join<9>(1, 4, 9, 1, 2, 4, 2, 6, 4);
		cubes[101] = array<int>::join<12>(3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4);
		cubes[102] = array<int>::join<6>(0, 2, 4, 4, 2, 6);
		cubes[103] = array<int>::join<9>(8, 3, 2, 8, 2, 4, 4, 2, 6);
		cubes[104] = array<int>::join<9>(10, 4, 9, 10, 6, 4, 11, 2, 3);
		cubes[105] = array<int>::join<12>(0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6);
		cubes[106] = array<int>::join<12>(3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10);
		cubes[107] = array<int>::join<15>(6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1);
		cubes[108] = array<int>::join<12>(9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3);
		cubes[109] = array<int>::join<15>(8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1);
		cubes[110] = array<int>::join<9>(3, 11, 6, 3, 6, 0, 0, 6, 4);
		cubes[111] = array<int>::join<6>(6, 4, 8, 11, 6, 8);
		cubes[112] = array<int>::join<9>(7, 10, 6, 7, 8, 10, 8, 9, 10);
		cubes[113] = array<int>::join<12>(0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10);
		cubes[114] = array<int>::join<12>(10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0);
		cubes[115] = array<int>::join<9>(10, 6, 7, 10, 7, 1, 1, 7, 3);
		cubes[116] = array<int>::join<12>(1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7);
		cubes[117] = array<int>::join<15>(2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9);
		cubes[118] = array<int>::join<9>(7, 8, 0, 7, 0, 6, 6, 0, 2);
		cubes[119] = array<int>::join<6>(7, 3, 2, 6, 7, 2);
		cubes[120] = array<int>::join<12>(2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7);
		cubes[121] = array<int>::join<15>(2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7);
		cubes[122] = array<int>::join<15>(1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11);
		cubes[123] = array<int>::join<12>(11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1);
		cubes[124] = array<int>::join<15>(8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6);
		cubes[125] = array<int>::join<6>(0, 9, 1, 11, 6, 7);
		cubes[126] = array<int>::join<12>(7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0);
		cubes[127] = array<int>::join<3>(7, 11, 6);
		cubes[128] = array<int>::join<3>(7, 6, 11);
		cubes[129] = array<int>::join<6>(3, 0, 8, 11, 7, 6);
		cubes[130] = array<int>::join<6>(0, 1, 9, 11, 7, 6);
		cubes[131] = array<int>::join<9>(8, 1, 9, 8, 3, 1, 11, 7, 6);
		cubes[132] = array<int>::join<6>(10, 1, 2, 6, 11, 7);
		cubes[133] = array<int>::join<9>(1, 2, 10, 3, 0, 8, 6, 11, 7);
		cubes[134] = array<int>::join<9>(2, 9, 0, 2, 10, 9, 6, 11, 7);
		cubes[135] = array<int>::join<12>(6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8);
		cubes[136] = array<int>::join<6>(7, 2, 3, 6, 2, 7);
		cubes[137] = array<int>::join<9>(7, 0, 8, 7, 6, 0, 6, 2, 0);
		cubes[138] = array<int>::join<9>(2, 7, 6, 2, 3, 7, 0, 1, 9);
		cubes[139] = array<int>::join<12>(1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6);
		cubes[140] = array<int>::join<9>(10, 7, 6, 10, 1, 7, 1, 3, 7);
		cubes[141] = array<int>::join<12>(10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8);
		cubes[142] = array<int>::join<12>(0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7);
		cubes[143] = array<int>::join<9>(7, 6, 10, 7, 10, 8, 8, 10, 9);
		cubes[144] = array<int>::join<6>(6, 8, 4, 11, 8, 6);
		cubes[145] = array<int>::join<9>(3, 6, 11, 3, 0, 6, 0, 4, 6);
		cubes[146] = array<int>::join<9>(8, 6, 11, 8, 4, 6, 9, 0, 1);
		cubes[147] = array<int>::join<12>(9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6);
		cubes[148] = array<int>::join<9>(6, 8, 4, 6, 11, 8, 2, 10, 1);
		cubes[149] = array<int>::join<12>(1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6);
		cubes[150] = array<int>::join<12>(4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9);
		cubes[151] = array<int>::join<15>(10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3);
		cubes[152] = array<int>::join<9>(8, 2, 3, 8, 4, 2, 4, 6, 2);
		cubes[153] = array<int>::join<6>(0, 4, 2, 4, 6, 2);
		cubes[154] = array<int>::join<12>(1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8);
		cubes[155] = array<int>::join<9>(1, 9, 4, 1, 4, 2, 2, 4, 6);
		cubes[156] = array<int>::join<12>(8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1);
		cubes[157] = array<int>::join<9>(10, 1, 0, 10, 0, 6, 6, 0, 4);
		cubes[158] = array<int>::join<15>(4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3);
		cubes[159] = array<int>::join<6>(10, 9, 4, 6, 10, 4);
		cubes[160] = array<int>::join<6>(4, 9, 5, 7, 6, 11);
		cubes[161] = array<int>::join<9>(0, 8, 3, 4, 9, 5, 11, 7, 6);
		cubes[162] = array<int>::join<9>(5, 0, 1, 5, 4, 0, 7, 6, 11);
		cubes[163] = array<int>::join<12>(11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5);
		cubes[164] = array<int>::join<9>(9, 5, 4, 10, 1, 2, 7, 6, 11);
		cubes[165] = array<int>::join<12>(6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5);
		cubes[166] = array<int>::join<12>(7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2);
		cubes[167] = array<int>::join<15>(3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6);
		cubes[168] = array<int>::join<9>(7, 2, 3, 7, 6, 2, 5, 4, 9);
		cubes[169] = array<int>::join<12>(9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7);
		cubes[170] = array<int>::join<12>(3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0);
		cubes[171] = array<int>::join<15>(6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8);
		cubes[172] = array<int>::join<12>(9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7);
		cubes[173] = array<int>::join<15>(1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4);
		cubes[174] = array<int>::join<15>(4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10);
		cubes[175] = array<int>::join<12>(7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10);
		cubes[176] = array<int>::join<9>(6, 9, 5, 6, 11, 9, 11, 8, 9);
		cubes[177] = array<int>::join<12>(3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5);
		cubes[178] = array<int>::join<12>(0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11);
		cubes[179] = array<int>::join<9>(6, 11, 3, 6, 3, 5, 5, 3, 1);
		cubes[180] = array<int>::join<12>(1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6);
		cubes[181] = array<int>::join<15>(0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10);
		cubes[182] = array<int>::join<15>(11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5);
		cubes[183] = array<int>::join<12>(6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3);
		cubes[184] = array<int>::join<12>(5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2);
		cubes[185] = array<int>::join<9>(9, 5, 6, 9, 6, 0, 0, 6, 2);
		cubes[186] = array<int>::join<15>(1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8);
		cubes[187] = array<int>::join<6>(1, 5, 6, 2, 1, 6);
		cubes[188] = array<int>::join<15>(1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6);
		cubes[189] = array<int>::join<12>(10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0);
		cubes[190] = array<int>::join<6>(0, 3, 8, 5, 6, 10);
		cubes[191] = array<int>::join<3>(10, 5, 6);
		cubes[192] = array<int>::join<6>(11, 5, 10, 7, 5, 11);
		cubes[193] = array<int>::join<9>(11, 5, 10, 11, 7, 5, 8, 3, 0);
		cubes[194] = array<int>::join<9>(5, 11, 7, 5, 10, 11, 1, 9, 0);
		cubes[195] = array<int>::join<12>(10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1);
		cubes[196] = array<int>::join<9>(11, 1, 2, 11, 7, 1, 7, 5, 1);
		cubes[197] = array<int>::join<12>(0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11);
		cubes[198] = array<int>::join<12>(9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7);
		cubes[199] = array<int>::join<15>(7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2);
		cubes[200] = array<int>::join<9>(2, 5, 10, 2, 3, 5, 3, 7, 5);
		cubes[201] = array<int>::join<12>(8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5);
		cubes[202] = array<int>::join<12>(9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2);
		cubes[203] = array<int>::join<15>(9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2);
		cubes[204] = array<int>::join<6>(1, 3, 5, 3, 7, 5);
		cubes[205] = array<int>::join<9>(0, 8, 7, 0, 7, 1, 1, 7, 5);
		cubes[206] = array<int>::join<9>(9, 0, 3, 9, 3, 5, 5, 3, 7);
		cubes[207] = array<int>::join<6>(9, 8, 7, 5, 9, 7);
		cubes[208] = array<int>::join<9>(5, 8, 4, 5, 10, 8, 10, 11, 8);
		cubes[209] = array<int>::join<12>(5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0);
		cubes[210] = array<int>::join<12>(0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5);
		cubes[211] = array<int>::join<15>(10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4);
		cubes[212] = array<int>::join<12>(2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8);
		cubes[213] = array<int>::join<15>(0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11);
		cubes[214] = array<int>::join<15>(0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5);
		cubes[215] = array<int>::join<6>(9, 4, 5, 2, 11, 3);
		cubes[216] = array<int>::join<12>(2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4);
		cubes[217] = array<int>::join<9>(5, 10, 2, 5, 2, 4, 4, 2, 0);
		cubes[218] = array<int>::join<15>(3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9);
		cubes[219] = array<int>::join<12>(5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2);
		cubes[220] = array<int>::join<9>(8, 4, 5, 8, 5, 3, 3, 5, 1);
		cubes[221] = array<int>::join<6>(0, 4, 5, 1, 0, 5);
		cubes[222] = array<int>::join<12>(8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5);
		cubes[223] = array<int>::join<3>(9, 4, 5);
		cubes[224] = array<int>::join<9>(4, 11, 7, 4, 9, 11, 9, 10, 11);
		cubes[225] = array<int>::join<12>(0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11);
		cubes[226] = array<int>::join<12>(1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11);
		cubes[227] = array<int>::join<15>(3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4);
		cubes[228] = array<int>::join<12>(4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2);
		cubes[229] = array<int>::join<15>(9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3);
		cubes[230] = array<int>::join<9>(11, 7, 4, 11, 4, 2, 2, 4, 0);
		cubes[231] = array<int>::join<12>(11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4);
		cubes[232] = array<int>::join<12>(2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9);
		cubes[233] = array<int>::join<15>(9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7);
		cubes[234] = array<int>::join<15>(3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10);
		cubes[235] = array<int>::join<6>(1, 10, 2, 8, 7, 4);
		cubes[236] = array<int>::join<9>(4, 9, 1, 4, 1, 7, 7, 1, 3);
		cubes[237] = array<int>::join<12>(4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1);
		cubes[238] = array<int>::join<6>(4, 0, 3, 7, 4, 3);
		cubes[239] = array<int>::join<3>(4, 8, 7);
		cubes[240] = array<int>::join<6>(9, 10, 8, 10, 11, 8);
		cubes[241] = array<int>::join<9>(3, 0, 9, 3, 9, 11, 11, 9, 10);
		cubes[242] = array<int>::join<9>(0, 1, 10, 0, 10, 8, 8, 10, 11);
		cubes[243] = array<int>::join<6>(3, 1, 10, 11, 3, 10);
		cubes[244] = array<int>::join<9>(1, 2, 11, 1, 11, 9, 9, 11, 8);
		cubes[245] = array<int>::join<12>(3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9);
		cubes[246] = array<int>::join<6>(0, 2, 11, 8, 0, 11);
		cubes[247] = array<int>::join<3>(3, 2, 11);
		cubes[248] = array<int>::join<9>(2, 3, 8, 2, 8, 10, 10, 8, 9);
		cubes[249] = array<int>::join<6>(9, 10, 2, 0, 9, 2);
		cubes[250] = array<int>::join<12>(2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8);
		cubes[251] = array<int>::join<3>(1, 10, 2);
		cubes[252] = array<int>::join<6>(1, 3, 8, 9, 1, 8);
		cubes[253] = array<int>::join<3>(0, 9, 1);
		cubes[254] = array<int>::join<3>(0, 3, 8);
		cubes[255] = array<int>::join<0>();	
	}
}

inline int blockhdl::index(int i, int j, int k, int width)
{
	return (i*width + j)*width + k;
}

void blockhdl::generate(void *data)
{
	if (density != NULL)
	{
		values.resize((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1));
		float step = size/(float)cubes_per_side;
		for (int i = 0; i < cubes_per_side+1; i++)
			for (int j = 0; j < cubes_per_side+1; j++)
				for (int k = 0; k < cubes_per_side+1; k++)
					values[index(i, j, k)] = density(vec3f(origin) + step*vec3f(i, j, k), data);
	}
}

void blockhdl::generate(array<float> svalues, vec3i offset, void *data)
{
	if (density != NULL && svalues.size() == (cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1))
	{
		values.resize((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1));
		float step = size/(float)cubes_per_side;
		for (int i = 0; i < cubes_per_side+1; i++)
			for (int j = 0; j < cubes_per_side+1; j++)
				for (int k = 0; k < cubes_per_side+1; k++)
				{
					//if (!(i&1) && !(j&1) && !(k&1))
					//	values[index(i, j, k)] = svalues[index(i/2 + offset[0], j/2 + offset[1], k/2 + offset[2])];
					//else
						values[index(i, j, k)] = density(vec3f(origin) + step*vec3f(i, j, k), data);
				}
	}
	else if (density != NULL)
		generate(data);
}

void blockhdl::split(void *data)
{
	if (children.size() == 0)
	{
		children.resize(8);
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
				{
					int c = index(i, j, k, 2);
					children[c].points = points;
					children[c].density = density;
					children[c].size = size/2.0;
					children[c].origin = origin + vec3F(vec3f(i, j, k)*size/2.0);
					children[c].generate(values, vec3i(i, j, k)*cubes_per_side/2, data);
				}

		unload();
		for (int i = 0; i < children.size(); i++)
			children[i].load();

		values.clear();
	}
	else
		for (int i = 0; i < children.size(); i++)
			if (children[i].points.size() > 0 || children[i].children.size() > 0)
				children[i].split(data);
}

void blockhdl::merge()
{
	if (children.size() > 0)
	{
		values.resize((cubes_per_side+1)*(cubes_per_side+1)*(cubes_per_side+1));
		for (int i = 0; i < cubes_per_side+1; i++)
			for (int j = 0; j < cubes_per_side+1; j++)
				for (int k = 0; k < cubes_per_side+1; k++)
					values[index(i, j, k)] = children[index((2*i)/cubes_per_side, (2*j)/cubes_per_side, (2*k)/cubes_per_side, 2)].values[index(2*(i%(cubes_per_side/2+1)), 2*(j%(cubes_per_side/2+1)), 2*(k%(cubes_per_side/2+1)))];

		for (int i = 0; i < children.size(); i++)
			children[i].unload();

		children.clear();

		load();
	}
}

void blockhdl::load()
{
	float step = size/(float)cubes_per_side;
	points.clear();
	points.reserve(cubes_per_side*cubes_per_side*cubes_per_side*15);
	for (int i = 0; i < cubes_per_side; i++)
		for (int j = 0; j < cubes_per_side; j++)
			for (int k = 0; k < cubes_per_side; k++)
			{
				int cube = 0;
				cube |= (values[index(i,   j,   k  )] > 0) << 0;
				cube |= (values[index(i,   j+1, k  )] > 0) << 1;
				cube |= (values[index(i+1, j+1, k  )] > 0) << 2;
				cube |= (values[index(i+1, j,   k  )] > 0) << 3;
				cube |= (values[index(i,   j,   k+1)] > 0) << 4;
				cube |= (values[index(i,   j+1, k+1)] > 0) << 5;
				cube |= (values[index(i+1, j+1, k+1)] > 0) << 6;
				cube |= (values[index(i+1, j,   k+1)] > 0) << 7;

				for (int l = 0; l < cubes[cube].size(); l++)
				{
					float p = 0.0f;
					switch (cubes[cube][l])
					{
					case 0: // 0, 0, 0 -> 0, 1, 0
						p = values[index(i, j, k)]/(values[index(i, j, k)] - values[index(i, j+1, k)]);
						points.push_back(step*vec3f((float)i, (float)j+p, (float)k));
						break;
					case 1: // 0, 1, 0 -> 1, 1, 0
						p = values[index(i, j+1, k)]/(values[index(i, j+1, k)] - values[index(i+1, j+1, k)]);
						points.push_back(step*vec3f((float)i+p, (float)j+1.0f, (float)k));
						break;
					case 2: // 1, 1, 0 -> 1, 0, 0
						p = values[index(i+1, j, k)]/(values[index(i+1, j, k)] - values[index(i+1, j+1, k)]);
						points.push_back(step*vec3f((float)i+1.0f, (float)j+p, (float)k));
						break;
					case 3: // 1, 0, 0 -> 0, 0, 0
						p = values[index(i, j, k)]/(values[index(i, j, k)] - values[index(i+1, j, k)]);
						points.push_back(step*vec3f((float)i+p, (float)j, (float)k));
						break;
					case 4: // 0, 0, 1 -> 0, 1, 1
						p = values[index(i, j, k+1)]/(values[index(i, j, k+1)] - values[index(i, j+1, k+1)]);
						points.push_back(step*vec3f((float)i, (float)j+p, (float)k+1.0f));
						break;
					case 5: // 0, 1, 1 -> 1, 1, 1
						p = values[index(i, j+1, k+1)]/(values[index(i, j+1, k+1)] - values[index(i+1, j+1, k+1)]);
						points.push_back(step*vec3f((float)i+p, (float)j+1.0f, (float)k+1.0f));
						break;
					case 6: // 1, 1, 1 -> 1, 0, 1
						p = values[index(i+1, j, k+1)]/(values[index(i+1, j, k+1)] - values[index(i+1, j+1, k+1)]);
						points.push_back(step*vec3f((float)i+1.0f, (float)j+p, (float)k+1.0f));
						break;
					case 7: // 1, 0, 1 -> 0, 0, 1
						p = values[index(i, j, k+1)]/(values[index(i, j, k+1)] - values[index(i+1, j, k+1)]);
						points.push_back(step*vec3f((float)i+p, (float)j, (float)k+1.0f));
						break;
					case 8: // 0, 0, 0 -> 0, 0, 1
						p = values[index(i, j, k)]/(values[index(i, j, k)] - values[index(i, j, k+1)]);
						points.push_back(step*vec3f((float)i, (float)j, (float)k+p));
						break;
					case 9: // 0, 1, 0 -> 0, 1, 1
						p = values[index(i, j+1, k)]/(values[index(i, j+1, k)] - values[index(i, j+1, k+1)]);
						points.push_back(step*vec3f((float)i, (float)j+1.0f, (float)k+p));
						break;
					case 10: // 1, 1, 0 -> 1, 1, 1
						p = values[index(i+1, j+1, k)]/(values[index(i+1, j+1, k)] - values[index(i+1, j+1, k+1)]);
						points.push_back(step*vec3f((float)i+1.0f, (float)j+1.0f, (float)k+p));
						break;
					case 11: // 1, 0, 0 -> 1, 0, 1
						p = values[index(i+1, j, k)]/(values[index(i+1, j, k)] - values[index(i+1, j, k+1)]);
						points.push_back(step*vec3f((float)i+1.0f, (float)j, (float)k+p));
						break;
					}
				}
			}
}

void blockhdl::unload()
{
	points.clear();
}

void blockhdl::render(int vertex_location, int origin_location)
{
	if (children.size() > 0)
		for (int i = 0; i < children.size(); i++)
			children[i].render(vertex_location, origin_location);
	else
	{
		glUniform3f(origin_location, (float)origin[0], (float)origin[1], (float)origin[2]);
		glEnableVertexAttribArray(vertex_location);
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, false, 0, points.data);
		glDrawArrays(GL_TRIANGLES, 0, points.size());
		glDisableVertexAttribArray(vertex_location);
	}
}
