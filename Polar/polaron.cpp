#include"header.h"
void polar_config(stream<datau64b> &config, stream<axi_data_32bit> &BA_IN, stream<axi_data_128bit> &payload_in,
		        stream<datau32b> &ctrl_out, stream<axi_data_32bit> &Din, stream<axi_data_32bit> &Dout, stream<datau64b> &BA_OUT, stream<axi_data_128bit> &payload_out ){

#pragma HLS interface ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=config
#pragma HLS INTERFACE axis port=BA_IN
#pragma HLS INTERFACE axis port=payload_in
#pragma HLS INTERFACE axis port= ctrl_out
#pragma HLS INTERFACE axis port=Din
#pragma HLS INTERFACE axis port=Dout
#pragma HLS INTERFACE axis port=BA_OUT
#pragma HLS INTERFACE axis port=payload_out
datau64b data_addr_words[10] = {0x000000, 0x000004, 0x000010, 0x00000C, 0x3202008, 0x00200C, 0x02000, 0x000004, 0x3F00010, 0x0100000};


	static int state, c1=0, c2=1;
    static datau32b value = 0x10000;
	static datau64b conf,temp,temp_in, a;
	static datau8b K;
	static datau11b N;
	static datau11b E;
	static datau16b RNTI;
	static datau32b ctrl;
	static axi_data_32bit in_val, out_val;
	static datau64b BA_OUT_temp;
	static axi_data_128bit in_data;
	static datau1b flag=0;

   // while(state < 8){
	switch(state){
	case 0:
		{
		    //state 0
			cout<<"state "<<state<<" "<<endl;
			conf   = config.read();
			K      = conf.range(7,0);
			N      = conf.range(18,8);
			temp.range(15,0) = N;
	        temp.range(31,16) = K;
			state = 1;
			break;
		}


	case 1:
		{
		//state 1
			cout<<"state "<<state<<" "<<endl;
			for(c1 = 0;c1<=6;c1++){
#pragma HLS LOOP_TRIPCOUNT min=7 max=7
#pragma HLS PIPELINE
				 if(c1!=6){
				 BA_OUT.write(data_addr_words[c1]);
				 state = 1;
				 }
				 if(c1==6){
		         a.range(19,0) = data_addr_words[c1];
		         //cout << K << "  " << N << endl;
		         a.range(51,20) = temp.range(31,0);
		         //cout << a << endl;
				 BA_OUT.write(a);
				 state = 2;
				 }
			}

			break;
		}
	case 2:
		{
		//state = 2
			cout<<"state "<<state<<" "<<endl;
			for(c2=1;c2<=N/16;c2++){  /// Another way is to write a while loop and stop it when tlast of BA_IN is 1
#pragma HLS LOOP_TRIPCOUNT min=8 max=32
#pragma HLS PIPELINE
				temp_in = BA_IN.read().data;
				cout<<"BA_table "<<temp_in<<" "<<endl;

				BA_OUT_temp.range(19,0) = value.range(19,0);
				BA_OUT_temp.range(51,20) = temp_in.range(31,0);
				BA_OUT.write(BA_OUT_temp);
				value+=4;
				state = 2;
			}
			state = 3;
			break;
		}
	case 3:
		{
		//state 3
			cout<<"state "<<state<<" "<<endl;
			for(c1=7;c1<=9;c1++){
#pragma HLS LOOP_TRIPCOUNT min=3 max=3
#pragma HLS PIPELINE
				BA_OUT.write(data_addr_words[c1]);
			}

			state = 4;
			break;
		}

	case 4:
		{
			cout<<"state "<<state<<" "<<endl;
			RNTI = conf.range(63,48);
			ctrl.range(6,0) = 0;
			ctrl.range(7,7) = 0;
			ctrl.range(23,8) =RNTI;
			ctrl.range(31,24) = 0;
			ctrl_out.write(ctrl);
			state=5;
			break;
		}
	case 5:
		{   cout<<"state "<<state<<" "<<endl;
		//in_val.data = ceil((float)K/8);in_val.last =1;
		//in_val.last =0;
		cout<<"K "<< K << " "<<endl;
		if(K<=128){
			in_val.data =(K+7)/8;in_val.last =1;
		}
		else if(flag==1)
		{
			in_val.data = (K-128+7)/8;in_val.last =1;
		}
		else
		{
			flag=1;
			in_val.data= 16; in_val.last =0;
		}
			cout<<in_val.data<<endl ;
			Din.write(in_val);
			if(in_val.last)
      			 state = 6;
      		 else
      			 state = 5;
      		 break;

		}
	case 6:
		{
			cout<<"state "<<state<<" "<<endl;
                 //payload_out.write(payload_in.read());
			payload_in.read(in_data);
			payload_out.write(in_data);
    	    if(in_data.last)
 	          state = 7;
 	        else
 	     	  state = 6;
 	        break;

			cout<<"state "<<state<<" "<<endl;
			out_val.data = 0x10;
			out_val.last =1;
			Dout.write(out_val);
         	     if(out_val.last)
      	     	 state = 7;
      	     else
      	     	 state = 6;
      	     break;
      	}
	case 7:
		{
			cout<<"state "<<state<<" "<<endl;
			out_val.data = 0x10;
			out_val.last =1;
			Dout.write(out_val);

	       	if(out_val.last)
      		  {
      			c1=0;
      			c2=1;
      			value = 0x10000;
      			state = 0;
      		  }
			break;
		}

   }
  }
