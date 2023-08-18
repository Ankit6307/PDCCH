#ifndef _HEADER_H_
#define _HEADER_H_

#include <ap_int.h>
#include <hls_stream.h>
#include <cmath>
#include<iostream>
#include<bitset>
using namespace std;
using namespace hls;
typedef ap_uint<128> datau128b;
typedef ap_uint<32> datau32b;
typedef ap_uint<64> datau64b;
typedef ap_uint<8> datau8b;
typedef ap_uint<11> datau11b;
typedef ap_uint<16> datau16b;
typedef ap_uint<1> datau1b;


struct axi_data_32bit{
	datau32b data;
	bool last;
};
struct axi_data_128bit{
	datau128b data;
	bool last;
};

void polar_config(stream<datau64b> &config, stream<axi_data_32bit> &BA_IN, stream<axi_data_128bit> &payload_in,
		        stream<datau32b> &ctrl_out, stream<axi_data_32bit> &Din, stream<axi_data_32bit> &Dout, stream<datau64b> &BA_OUT, stream<axi_data_128bit> &payload_out );
#endif
