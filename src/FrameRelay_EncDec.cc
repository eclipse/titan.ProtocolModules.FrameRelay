/******************************************************************************
* Copyright (c) 2005, 2015  Ericsson AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
* Gabor Szalai
//******************************************************************************/
//  File:     FrameRelay_EncDec.cc
//  Rev:      R2A
//  Prodnr:   CNL 113 790
///////////////////////////////////////////////////////////////////////////////

#include "FrameRelay_Types.hh"
#include "TTCN3.hh"

namespace FrameRelay__Types {


const unsigned char swap6 [64] = {
0x00, 0x20, 0x10, 0x30, 0x08, 0x28, 0x18, 0x38,
0x04, 0x24, 0x14, 0x34, 0x0C, 0x2C, 0x1C, 0x3C,
0x02, 0x22, 0x12, 0x32, 0x0A, 0x2A, 0x1A, 0x3A,
0x06, 0x26, 0x16, 0x36, 0x0E, 0x2E, 0x1E, 0x3E,
0x01, 0x21, 0x11, 0x31, 0x09, 0x29, 0x19, 0x39,
0x05, 0x25, 0x15, 0x35, 0x0D, 0x2D, 0x1D, 0x3D,
0x03, 0x23, 0x13, 0x33, 0x0B, 0x2B, 0x1B, 0x3B,
0x07, 0x27, 0x17, 0x37, 0x0F, 0x2F, 0x1F, 0x3F };
const unsigned char swap4 [16] = {
0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f
};

OCTETSTRING f__PDU__Frame__Relay__Enc(const PDU__Frame__Relay& pdu){

  TTCN_Buffer buff;
  
  unsigned char ch=0;
  
  const unsigned char* dlci_bits=(const unsigned char*)pdu.dlci();
  
  
  ch= ( swap6[dlci_bits[0]& 0x3F] << 2 ) | (((((const unsigned char*)pdu.c__r())[0]) & 0x01)<<1) | ((((const unsigned char*)pdu.ea1())[0]) & 0x01);  
  buff.put_c(ch);
  
  ch=0;
  ch= (swap4[( ((dlci_bits[1]&0x03)<<2) |((dlci_bits[0]&0xC0) >> 6) )]<<4)| (((((const unsigned char*)pdu.fecn())[0]) & 0x01)<<3)| (((((const unsigned char*)pdu.becn())[0]) & 0x01)<<2)| (((((const unsigned char*)pdu.de())[0]) & 0x01)<<1)| ((((const unsigned char*)pdu.ea2())[0]) & 0x01); 
  buff.put_c(ch);
  
  buff.put_string(pdu.information().data());
  
  OCTETSTRING ret_val;
  buff.get_string(ret_val);
  return ret_val;

}

PDU__Frame__Relay f__PDU__Frame__Relay__Dec(const OCTETSTRING& stream){
  PDU__Frame__Relay ret_val;
  
  const unsigned char* data=(const unsigned char*)stream;
  
  unsigned char dlci[2]={0,0};
  unsigned char ch=0;
  dlci[1]= ((data[1]>>3)& 0x02) | ((data[1]>>5)& 0x01);
  
  dlci[0]= ((data[1]>>1)& 0x40) | ((data[1]<<1)& 0x80)| swap6[ (data[0]>>2) & 0x3F];
  
  ch=data[0];
  ret_val.ea1()=BITSTRING(1,&ch);
  ch=data[0]>>1;
  ret_val.c__r()=BITSTRING(1,&ch);
  ret_val.dlci()=BITSTRING(10,&(dlci[0]));

  ch=data[1];
  ret_val.ea2()=BITSTRING(1,&ch);
  
  ch=data[1]>>1;
  ret_val.de()=BITSTRING(1,&ch);
  ch=data[1]>>2;
  ret_val.becn()=BITSTRING(1,&ch);
  ch=data[1]>>3;
  ret_val.fecn()=BITSTRING(1,&ch);
  
  ret_val.information().data()=OCTETSTRING(stream.lengthof()-2,data+2);
  
  return ret_val;

}

}
