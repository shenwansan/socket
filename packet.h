#ifndef __PACKET_H
#define __PACKET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PAC_HEAD 0x1a1a
#define PAC_END 0x2a2a

#define PACKET_ERROR -1
#define PACKET_0 0
#define PACKET_1 1
#define PACKET_2 2
#define PACKET_3 3
#define PACKET_8 8
#define PACKET_10 10
#define PACKET_20 20
#define PCAKET_16 16
#define PACKET_128 128
#define PCAKET_256 256
#define PACKET_1024 1024
#define TRUE 1
#define FALSE 0


typedef struct pac_head
{
	uint16_t head;
	uint8_t type;
	uint32_t len;
	uint8_t  * data;
	uint8_t crc;
	uint16_t end;
}__attribute__((packed))PACKET_HEAD;





#endif