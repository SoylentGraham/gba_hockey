#include "gba.h"
#include "types.h"
#include "dma.h"


void DMA_Copy(u8 channel, void* source, void* dest, u32 WordCount, u32 mode)
{
	switch (channel)
	{
	case 0:{
		REG_DMA0SAD = (u32)source;
		REG_DMA0DAD = (u32)dest; 
		REG_DMA0CNT = WordCount | mode;
		break;
		   }
	case 1:{
		REG_DMA1SAD = (u32)source;
		REG_DMA1DAD = (u32)dest;
		REG_DMA1CNT = WordCount | mode;
		break;
		   }
	case 2:{
		REG_DMA2SAD = (u32)source;
		REG_DMA2DAD = (u32)dest;
		REG_DMA2CNT = WordCount | mode;
		break;
		   }
	case 3:{
		REG_DMA3SAD = (u32)source;
		REG_DMA3DAD = (u32)dest;
		REG_DMA3CNT = WordCount | mode;
		break;
		   }
	}
}

