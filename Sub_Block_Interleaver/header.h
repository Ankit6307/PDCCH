#ifndef HEADER_H
#define HEADER_H
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <stdio.h>
#include <ap_fixed.h>
#include <math.h>
#include <hls_math.h>
#include <fstream>
#include <iostream>
struct data_axi
{
	ap_uint<128> data;
	ap_uint<1> last;


};
struct data_axi1
{
	ap_uint<512> data;
	ap_uint<1> last;
};
//typedef hls::stream<data_axi1> STREAM_U;
typedef hls::stream<data_axi> STREAM_T;

typedef ap_uint<128> datau128b;
typedef ap_uint<16> datau16b;
typedef ap_uint<64> datau64b;
typedef ap_uint<32> datau32b;
typedef ap_uint<8> datau8b;
typedef ap_uint<512> datau512b;
//const  ap_uint<32> P[32]={0,1,2,4,3,5,6,7,8,16,9,17,10,18,11,19,12,20,13,21,14,22,15,23,24,25,26,28,27,29,30,31};//LookupTable
const  ap_uint<32> P[32]={0,1,2,4,3,5,6,7,8,10,12,14,16,18,20,22,9,11,13,15,17,19,21,23,24,25,26,28,27,29,30,31};
//const  ap_uint<32> P[32]={31,30,29,27,28,26,25,24,23,15,22,14,21,13,20,12,19,11,18,10,17,9,16,8,7,6,5,3,4,2,1,0};
void SUB_BLOCK_INT(STREAM_T &inData, hls::stream<datau64b> &cnData,STREAM_T &outData);

#endif
