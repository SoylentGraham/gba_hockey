
#ifndef CORE_GBA_H
#define CORE_GBA_H


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned long word;

//typedef int:1 flag;
//typedef unsigned char:1 bool;




#define BG_WRAP 0x2000

#define BG_MODE(n) (n)

#define BG_MODE_0 0
#define BG_MODE_1 1
#define BG_MODE_2 2
#define BG_MODE_3 3
#define BG_MODE_4 4
#define BG_MODE_5 5

#define OBJ_1D_MAP 0x40
#define OBJ_2D_MAP 0

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000



//grem
#define TARGET_TOP_BG0 0x1 //"first target"
#define TARGET_TOP_BG1 0x2
#define TARGET_TOP_BG2 0x4
#define TARGET_TOP_BG3 0x8
#define TARGET_TOP_OBJ 0x10
#define TARGET_TOP_BD 0x20

#define TARGET_LOW_BG0 0x0100 //2nd target
#define TARGET_LOW_BG1 0x0200
#define TARGET_LOW_BG2 0x0400
#define TARGET_LOW_BG3 0x0800
#define TARGET_LOW_OBJ 0x1000
#define TARGET_LOW_BD 0x2000

#define REG_COLEV      *(volatile u16*)0x4000052
#define REG_DISPCNT    *(volatile u32*)0x4000000
#define REG_DISPCNT_L  *(volatile u16*)0x4000000
#define REG_DISPCNT_H  *(volatile u16*)0x4000002
#define REG_DISPSTAT   *(volatile u16*)0x4000004
#define REG_VCOUNT     *(volatile u16*)0x4000006
#define REG_BG0CNT     *(volatile u16*)0x4000008
#define REG_BG1CNT     *(volatile u16*)0x400000A
#define REG_BG2CNT     *(volatile u16*)0x400000C
#define REG_BG3CNT     *(volatile u16*)0x400000E
#define REG_BG0HOFS    *(volatile u16*)0x4000010
#define REG_BG0VOFS    *(volatile u16*)0x4000012
#define REG_BG1HOFS    *(volatile u16*)0x4000014
#define REG_BG1VOFS    *(volatile u16*)0x4000016
#define REG_BG2HOFS    *(volatile u16*)0x4000018
#define REG_BG2VOFS    *(volatile u16*)0x400001A
#define REG_BG3HOFS    *(volatile u16*)0x400001C
#define REG_BG3VOFS    *(volatile u16*)0x400001E
#define REG_BG2PA      *(volatile u16*)0x4000020
#define REG_BG2PB      *(volatile u16*)0x4000022
#define REG_BG2PC      *(volatile u16*)0x4000024
#define REG_BG2PD      *(volatile u16*)0x4000026
#define REG_BG2X       *(volatile u32*)0x4000028
#define REG_BG2X_L     *(volatile u16*)0x4000028
#define REG_BG2X_H     *(volatile u16*)0x400002A
#define REG_BG2Y       *(volatile u32*)0x400002C
#define REG_BG2Y_L     *(volatile u16*)0x400002C
#define REG_BG2Y_H     *(volatile u16*)0x400002E
#define REG_BG3PA      *(volatile u16*)0x4000030
#define REG_BG3PB      *(volatile u16*)0x4000032
#define REG_BG3PC      *(volatile u16*)0x4000034
#define REG_BG3PD      *(volatile u16*)0x4000036
#define REG_BG3X       *(volatile u32*)0x4000038
#define REG_BG3X_L     *(volatile u16*)0x4000038
#define REG_BG3X_H     *(volatile u16*)0x400003A
#define REG_BG3Y       *(volatile u32*)0x400003C
#define REG_BG3Y_L     *(volatile u16*)0x400003C
#define REG_BG3Y_H     *(volatile u16*)0x400003E
#define REG_WIN0H      *(volatile u16*)0x4000040
#define REG_WIN1H      *(volatile u16*)0x4000042
#define REG_WIN0V      *(volatile u16*)0x4000044
#define REG_WIN1V      *(volatile u16*)0x4000046
#define REG_WININ      *(volatile u16*)0x4000048
#define REG_WINOUT     *(volatile u16*)0x400004A
#define REG_MOSAIC     *(volatile u32*)0x400004C
#define REG_MOSAIC_L   *(volatile u32*)0x400004C
#define REG_MOSAIC_H   *(volatile u32*)0x400004E
#define REG_BLDMOD     *(volatile u16*)0x4000050
#define REG_COLEY1     *(volatile u16*)0x4000052
#define REG_COLEY2     *(volatile u16*)0x4000054
#define REG_SG10       *(volatile u32*)0x4000060
#define REG_SG10_L     *(volatile u16*)0x4000060
#define REG_SG10_H     *(volatile u16*)0x4000062
#define REG_SG11       *(volatile u16*)0x4000064
#define REG_SG20       *(volatile u16*)0x4000068
#define REG_SG21       *(volatile u16*)0x400006C
#define REG_SG30       *(volatile u32*)0x4000070
#define REG_SG30_L     *(volatile u16*)0x4000070
#define REG_SG30_H     *(volatile u16*)0x4000072
#define REG_SG31       *(volatile u16*)0x4000074
#define REG_SG40       *(volatile u16*)0x4000078
#define REG_SG41       *(volatile u16*)0x400007C
#define REG_SGCNT0     *(volatile u32*)0x4000080
#define REG_SGCNT0_L   *(volatile u16*)0x4000080
#define REG_SGCNT0_H   *(volatile u16*)0x4000082
#define REG_SGCNT1     *(volatile u16*)0x4000084
#define REG_SGBIAS     *(volatile u16*)0x4000088
#define REG_SGWR0      *(volatile u32*)0x4000090
#define REG_SGWR0_L    *(volatile u16*)0x4000090
#define REG_SGWR0_H    *(volatile u16*)0x4000092
#define REG_SGWR1      *(volatile u32*)0x4000094
#define REG_SGWR1_L    *(volatile u16*)0x4000094
#define REG_SGWR1_H    *(volatile u16*)0x4000096
#define REG_SGWR2      *(volatile u32*)0x4000098
#define REG_SGWR2_L    *(volatile u16*)0x4000098
#define REG_SGWR2_H    *(volatile u16*)0x400009A
#define REG_SGWR3      *(volatile u32*)0x400009C
#define REG_SGWR3_L    *(volatile u16*)0x400009C
#define REG_SGWR3_H    *(volatile u16*)0x400009E
#define REG_SGFIF0A    *(volatile u32*)0x40000A0
#define REG_SGFIFOA_L  *(volatile u16*)0x40000A0
#define REG_SGFIFOA_H  *(volatile u16*)0x40000A2
#define REG_SGFIFOB    *(volatile u32*)0x40000A4
#define REG_SGFIFOB_L  *(volatile u16*)0x40000A4
#define REG_SGFIFOB_H  *(volatile u16*)0x40000A6

#define REG_DMA0SAD     *(volatile u32*)0x40000B0
#define REG_DMA0SAD_L   *(volatile u16*)0x40000B0
#define REG_DMA0SAD_H   *(volatile u16*)0x40000B2
#define REG_DMA0DAD     *(volatile u32*)0x40000B4
#define REG_DMA0DAD_L   *(volatile u16*)0x40000B4
#define REG_DMA0DAD_H   *(volatile u16*)0x40000B6
#define REG_DMA0CNT     *(volatile u32*)0x40000B8
#define REG_DMA0CNT_L   *(volatile u16*)0x40000B8
#define REG_DMA0CNT_H   *(volatile u16*)0x40000BA
#define REG_DMA1SAD     *(volatile u32*)0x40000BC
#define REG_DMA1SAD_L   *(volatile u16*)0x40000BC
#define REG_DMA1SAD_H   *(volatile u16*)0x40000BE
#define REG_DMA1DAD     *(volatile u32*)0x40000C0
#define REG_DMA1DAD_L   *(volatile u16*)0x40000C0
#define REG_DMA1DAD_H   *(volatile u16*)0x40000C2
#define REG_DMA1CNT     *(volatile u32*)0x40000C4
#define REG_DMA1CNT_L   *(volatile u16*)0x40000C4
#define REG_DMA1CNT_H   *(volatile u16*)0x40000C6
#define REG_DMA2SAD     *(volatile u32*)0x40000C8
#define REG_DMA2SAD_L   *(volatile u16*)0x40000C8
#define REG_DMA2SAD_H   *(volatile u16*)0x40000CA
#define REG_DMA2DAD     *(volatile u32*)0x40000CC
#define REG_DMA2DAD_L   *(volatile u16*)0x40000CC
#define REG_DMA2DAD_H   *(volatile u16*)0x40000CE
#define REG_DMA2CNT     *(volatile u32*)0x40000D0
#define REG_DMA2CNT_L   *(volatile u16*)0x40000D0
#define REG_DMA2CNT_H   *(volatile u16*)0x40000D2
#define REG_DMA3SAD     *(volatile u32*)0x40000D4
#define REG_DMA3SAD_L   *(volatile u16*)0x40000D4
#define REG_DMA3SAD_H   *(volatile u16*)0x40000D6
#define REG_DMA3DAD     *(volatile u32*)0x40000D8
#define REG_DMA3DAD_L   *(volatile u16*)0x40000D8
#define REG_DMA3DAD_H   *(volatile u16*)0x40000DA
#define REG_DMA3CNT     *(volatile u32*)0x40000DC
#define REG_DMA3CNT_L   *(volatile u16*)0x40000DC
#define REG_DMA3CNT_H   *(volatile u16*)0x40000DE

#define REG_TM0CNT     *(volatile u16*)0x4000102
#define REG_TM1D       *(volatile u16*)0x4000104
#define REG_TM1CNT     *(volatile u16*)0x4000106
#define REG_TM2D       *(volatile u16*)0x4000108
#define REG_TM2CNT     *(volatile u16*)0x400010A
#define REG_TM3D       *(volatile u16*)0x400010C
#define REG_TM3CNT     *(volatile u16*)0x400010E
#define REG_SCD0       *(volatile u16*)0x4000120
#define REG_SCD1       *(volatile u16*)0x4000122
#define REG_SCD2       *(volatile u16*)0x4000124
#define REG_SCD3       *(volatile u16*)0x4000126
#define REG_SCCNT      *(volatile u32*)0x4000128
#define REG_SCCNT_L    *(volatile u16*)0x4000128
#define REG_SCCNT_H    *(volatile u16*)0x400012A
#define REG_P1         *(volatile u16*)0x4000130
#define REG_P1CNT      *(volatile u16*)0x4000132
#define REG_R          *(volatile u16*)0x4000134
#define REG_HS_CTRL    *(volatile u16*)0x4000140
#define REG_JOYRE      *(volatile u32*)0x4000150
#define REG_JOYRE_L    *(volatile u16*)0x4000150
#define REG_JOYRE_H    *(volatile u16*)0x4000152
#define REG_JOYTR      *(volatile u32*)0x4000154
#define REG_JOYTR_L    *(volatile u16*)0x4000154
#define REG_JOYTR_H    *(volatile u16*)0x4000156
#define REG_JSTAT      *(volatile u32*)0x4000158
#define REG_JSTAT_L    *(volatile u16*)0x4000158
#define REG_JSTAT_H    *(volatile u16*)0x400015A
#define REG_IE         *(volatile u16*)0x4000200
#define REG_IF         *(volatile u16*)0x4000202
#define REG_WSCNT      *(volatile u16*)0x4000204
#define REG_IME        *(volatile u16*)0x4000208
#define REG_PAUSE      *(volatile u16*)0x4000300

#define REG_PALETTE_BASE      (u16 *)0x5000000
#define REG_TILE_BASE         (u8 *)0x6000000
#define REG_TILEMAP_BASE      (u16 *)0x6000000



/*
#define REG_COLEV      *(u16*)0x4000052
#define REG_DISPCNT    *(u32*)0x4000000
#define REG_DISPCNT_L  *(u16*)0x4000000
#define REG_DISPCNT_H  *(u16*)0x4000002
#define REG_DISPSTAT   *(u16*)0x4000004
#define REG_VCOUNT     *(u16*)0x4000006
#define REG_BG0CNT     *(u16*)0x4000008
#define REG_BG1CNT     *(u16*)0x400000A
#define REG_BG2CNT     *(u16*)0x400000C
#define REG_BG3CNT     *(u16*)0x400000E
#define REG_BG0HOFS    *(u16*)0x4000010
#define REG_BG0VOFS    *(u16*)0x4000012
#define REG_BG1HOFS    *(u16*)0x4000014
#define REG_BG1VOFS    *(u16*)0x4000016
#define REG_BG2HOFS    *(u16*)0x4000018
#define REG_BG2VOFS    *(u16*)0x400001A
#define REG_BG3HOFS    *(u16*)0x400001C
#define REG_BG3VOFS    *(u16*)0x400001E
#define REG_BG2PA      *(u16*)0x4000020
#define REG_BG2PB      *(u16*)0x4000022
#define REG_BG2PC      *(u16*)0x4000024
#define REG_BG2PD      *(u16*)0x4000026
#define REG_BG2X       *(u32*)0x4000028
#define REG_BG2X_L     *(u16*)0x4000028
#define REG_BG2X_H     *(u16*)0x400002A
#define REG_BG2Y       *(u32*)0x400002C
#define REG_BG2Y_L     *(u16*)0x400002C
#define REG_BG2Y_H     *(u16*)0x400002E
#define REG_BG3PA      *(u16*)0x4000030
#define REG_BG3PB      *(u16*)0x4000032
#define REG_BG3PC      *(u16*)0x4000034
#define REG_BG3PD      *(u16*)0x4000036
#define REG_BG3X       *(u32*)0x4000038
#define REG_BG3X_L     *(u16*)0x4000038
#define REG_BG3X_H     *(u16*)0x400003A
#define REG_BG3Y       *(u32*)0x400003C
#define REG_BG3Y_L     *(u16*)0x400003C
#define REG_BG3Y_H     *(u16*)0x400003E
#define REG_WIN0H      *(u16*)0x4000040
#define REG_WIN1H      *(u16*)0x4000042
#define REG_WIN0V      *(u16*)0x4000044
#define REG_WIN1V      *(u16*)0x4000046
#define REG_WININ      *(u16*)0x4000048
#define REG_WINOUT     *(u16*)0x400004A
#define REG_MOSAIC     *(u32*)0x400004C
#define REG_MOSAIC_L   *(u32*)0x400004C
#define REG_MOSAIC_H   *(u32*)0x400004E
#define REG_BLDMOD     *(u16*)0x4000050
#define REG_COLEY1     *(u16*)0x4000052
#define REG_COLEY2     *(u16*)0x4000054
#define REG_SG10       *(u32*)0x4000060
#define REG_SG10_L     *(u16*)0x4000060
#define REG_SG10_H     *(u16*)0x4000062
#define REG_SG11       *(u16*)0x4000064
#define REG_SG20       *(u16*)0x4000068
#define REG_SG21       *(u16*)0x400006C
#define REG_SG30       *(u32*)0x4000070
#define REG_SG30_L     *(u16*)0x4000070
#define REG_SG30_H     *(u16*)0x4000072
#define REG_SG31       *(u16*)0x4000074
#define REG_SG40       *(u16*)0x4000078
#define REG_SG41       *(u16*)0x400007C
#define REG_SGCNT0     *(u32*)0x4000080
#define REG_SGCNT0_L   *(u16*)0x4000080
#define REG_SGCNT0_H   *(u16*)0x4000082
#define REG_SGCNT1     *(u16*)0x4000084
#define REG_SGBIAS     *(u16*)0x4000088
#define REG_SGWR0      *(u32*)0x4000090
#define REG_SGWR0_L    *(u16*)0x4000090
#define REG_SGWR0_H    *(u16*)0x4000092
#define REG_SGWR1      *(u32*)0x4000094
#define REG_SGWR1_L    *(u16*)0x4000094
#define REG_SGWR1_H    *(u16*)0x4000096
#define REG_SGWR2      *(u32*)0x4000098
#define REG_SGWR2_L    *(u16*)0x4000098
#define REG_SGWR2_H    *(u16*)0x400009A
#define REG_SGWR3      *(u32*)0x400009C
#define REG_SGWR3_L    *(u16*)0x400009C
#define REG_SGWR3_H    *(u16*)0x400009E
#define REG_SGFIF0A    *(u32*)0x40000A0
#define REG_SGFIFOA_L  *(u16*)0x40000A0
#define REG_SGFIFOA_H  *(u16*)0x40000A2
#define REG_SGFIFOB    *(u32*)0x40000A4
#define REG_SGFIFOB_L  *(u16*)0x40000A4
#define REG_SGFIFOB_H  *(u16*)0x40000A6

#define REG_DMA0SAD     *(u32*)0x40000B0
#define REG_DMA0SAD_L   *(u16*)0x40000B0
#define REG_DMA0SAD_H   *(u16*)0x40000B2
#define REG_DMA0DAD     *(u32*)0x40000B4
#define REG_DMA0DAD_L   *(u16*)0x40000B4
#define REG_DMA0DAD_H   *(u16*)0x40000B6
#define REG_DMA0CNT     *(u32*)0x40000B8
#define REG_DMA0CNT_L   *(u16*)0x40000B8
#define REG_DMA0CNT_H   *(u16*)0x40000BA
#define REG_DMA1SAD     *(u32*)0x40000BC
#define REG_DMA1SAD_L   *(u16*)0x40000BC
#define REG_DMA1SAD_H   *(u16*)0x40000BE
#define REG_DMA1DAD     *(u32*)0x40000C0
#define REG_DMA1DAD_L   *(u16*)0x40000C0
#define REG_DMA1DAD_H   *(u16*)0x40000C2
#define REG_DMA1CNT     *(u32*)0x40000C4
#define REG_DMA1CNT_L   *(u16*)0x40000C4
#define REG_DMA1CNT_H   *(u16*)0x40000C6
#define REG_DMA2SAD     *(u32*)0x40000C8
#define REG_DMA2SAD_L   *(u16*)0x40000C8
#define REG_DMA2SAD_H   *(u16*)0x40000CA
#define REG_DMA2DAD     *(u32*)0x40000CC
#define REG_DMA2DAD_L   *(u16*)0x40000CC
#define REG_DMA2DAD_H   *(u16*)0x40000CE
#define REG_DMA2CNT     *(u32*)0x40000D0
#define REG_DMA2CNT_L   *(u16*)0x40000D0
#define REG_DMA2CNT_H   *(u16*)0x40000D2
#define REG_DMA3SAD     *(volatile u32*)0x40000D4
#define REG_DMA3SAD_L   *(volatile u16*)0x40000D4
#define REG_DMA3SAD_H   *(volatile u16*)0x40000D6
#define REG_DMA3DAD     *(volatile u32*)0x40000D8
#define REG_DMA3DAD_L   *(volatile u16*)0x40000D8
#define REG_DMA3DAD_H   *(volatile u16*)0x40000DA
#define REG_DMA3CNT     *(volatile u32*)0x40000DC
#define REG_DMA3CNT_L   *(volatile u16*)0x40000DC
#define REG_DMA3CNT_H   *(volatile u16*)0x40000DE

#define REG_TM0CNT     *(u16*)0x4000102
#define REG_TM1D       *(u16*)0x4000104
#define REG_TM1CNT     *(u16*)0x4000106
#define REG_TM2D       *(u16*)0x4000108
#define REG_TM2CNT     *(u16*)0x400010A
#define REG_TM3D       *(u16*)0x400010C
#define REG_TM3CNT     *(u16*)0x400010E
#define REG_SCD0       *(u16*)0x4000120
#define REG_SCD1       *(u16*)0x4000122
#define REG_SCD2       *(u16*)0x4000124
#define REG_SCD3       *(u16*)0x4000126
#define REG_SCCNT      *(u32*)0x4000128
#define REG_SCCNT_L    *(u16*)0x4000128
#define REG_SCCNT_H    *(u16*)0x400012A
#define REG_P1         *(u16*)0x4000130
#define REG_P1CNT      *(u16*)0x4000132
#define REG_R          *(u16*)0x4000134
#define REG_HS_CTRL    *(u16*)0x4000140
#define REG_JOYRE      *(u32*)0x4000150
#define REG_JOYRE_L    *(u16*)0x4000150
#define REG_JOYRE_H    *(u16*)0x4000152
#define REG_JOYTR      *(u32*)0x4000154
#define REG_JOYTR_L    *(u16*)0x4000154
#define REG_JOYTR_H    *(u16*)0x4000156
#define REG_JSTAT      *(u32*)0x4000158
#define REG_JSTAT_L    *(u16*)0x4000158
#define REG_JSTAT_H    *(u16*)0x400015A
#define REG_IE         *(u16*)0x4000200
#define REG_IF         *(u16*)0x4000202
#define REG_WSCNT      *(u16*)0x4000204
#define REG_IME        *(u16*)0x4000208
#define REG_PAUSE      *(u16*)0x4000300

#define REG_PALETTE_BASE      (u16 *)0x5000000
#define REG_TILE_BASE         (u8 *)0x6000000
#define REG_TILEMAP_BASE      (u16 *)0x6000000

*/










#endif

/*
#ifndef GBA_HEADER
#define GBA_HEADER


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned long word;

//typedef int:1 flag;
//typedef unsigned char:1 bool;



#define BG_WRAP 0x2000

#define BG_MODE(n) (n)

#define BG_MODE_0 0
#define BG_MODE_1 1
#define BG_MODE_2 2
#define BG_MODE_3 3
#define BG_MODE_4 4
#define BG_MODE_5 5

#define OBJ_1D_MAP 0x40
#define OBJ_2D_MAP 0

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000



//grem
#define TARGET_TOP_BG0 0x1 //"first target"
#define TARGET_TOP_BG1 0x2
#define TARGET_TOP_BG2 0x4
#define TARGET_TOP_BG3 0x8
#define TARGET_TOP_OBJ 0x10
#define TARGET_TOP_BD 0x20

#define TARGET_LOW_BG0 0x0100 //2nd target
#define TARGET_LOW_BG1 0x0200
#define TARGET_LOW_BG2 0x0400
#define TARGET_LOW_BG3 0x0800
#define TARGET_LOW_OBJ 0x1000
#define TARGET_LOW_BD 0x2000



#define REG_COLEV      *(u16*)0x4000052


#define REG_DISPCNT    *(u32*)0x4000000
#define REG_DISPCNT_L  *(u16*)0x4000000
#define REG_DISPCNT_H  *(u16*)0x4000002
#define REG_DISPSTAT   *(u16*)0x4000004
#define REG_VCOUNT     *(u16*)0x4000006
#define REG_BG0CNT     *(u16*)0x4000008
#define REG_BG1CNT     *(u16*)0x400000A
#define REG_BG2CNT     *(u16*)0x400000C
#define REG_BG3CNT     *(u16*)0x400000E
#define REG_BG0HOFS    *(u16*)0x4000010
#define REG_BG0VOFS    *(u16*)0x4000012
#define REG_BG1HOFS    *(u16*)0x4000014
#define REG_BG1VOFS    *(u16*)0x4000016
#define REG_BG2HOFS    *(u16*)0x4000018
#define REG_BG2VOFS    *(u16*)0x400001A
#define REG_BG3HOFS    *(u16*)0x400001C
#define REG_BG3VOFS    *(u16*)0x400001E
#define REG_BG2PA      *(u16*)0x4000020
#define REG_BG2PB      *(u16*)0x4000022
#define REG_BG2PC      *(u16*)0x4000024
#define REG_BG2PD      *(u16*)0x4000026
#define REG_BG2X       *(u32*)0x4000028
#define REG_BG2X_L     *(u16*)0x4000028
#define REG_BG2X_H     *(u16*)0x400002A
#define REG_BG2Y       *(u32*)0x400002C
#define REG_BG2Y_L     *(u16*)0x400002C
#define REG_BG2Y_H     *(u16*)0x400002E
#define REG_BG3PA      *(u16*)0x4000030
#define REG_BG3PB      *(u16*)0x4000032
#define REG_BG3PC      *(u16*)0x4000034
#define REG_BG3PD      *(u16*)0x4000036
#define REG_BG3X       *(u32*)0x4000038
#define REG_BG3X_L     *(u16*)0x4000038
#define REG_BG3X_H     *(u16*)0x400003A
#define REG_BG3Y       *(u32*)0x400003C
#define REG_BG3Y_L     *(u16*)0x400003C
#define REG_BG3Y_H     *(u16*)0x400003E
#define REG_WIN0H      *(u16*)0x4000040
#define REG_WIN1H      *(u16*)0x4000042
#define REG_WIN0V      *(u16*)0x4000044
#define REG_WIN1V      *(u16*)0x4000046
#define REG_WININ      *(u16*)0x4000048
#define REG_WINOUT     *(u16*)0x400004A
#define REG_MOSAIC     *(u32*)0x400004C
#define REG_MOSAIC_L   *(u32*)0x400004C
#define REG_MOSAIC_H   *(u32*)0x400004E
#define REG_BLDMOD     *(u16*)0x4000050
#define REG_COLEY1     *(u16*)0x4000052
#define REG_COLEY2     *(u16*)0x4000054
#define REG_SG10       *(u32*)0x4000060
#define REG_SG10_L     *(u16*)0x4000060
#define REG_SG10_H     *(u16*)0x4000062
#define REG_SG11       *(u16*)0x4000064
#define REG_SG20       *(u16*)0x4000068
#define REG_SG21       *(u16*)0x400006C
#define REG_SG30       *(u32*)0x4000070
#define REG_SG30_L     *(u16*)0x4000070
#define REG_SG30_H     *(u16*)0x4000072
#define REG_SG31       *(u16*)0x4000074
#define REG_SG40       *(u16*)0x4000078
#define REG_SG41       *(u16*)0x400007C
#define REG_SGCNT0     *(u32*)0x4000080
#define REG_SGCNT0_L   *(u16*)0x4000080
#define REG_SGCNT0_H   *(u16*)0x4000082
#define REG_SGCNT1     *(u16*)0x4000084
#define REG_SGBIAS     *(u16*)0x4000088
#define REG_SGWR0      *(u32*)0x4000090
#define REG_SGWR0_L    *(u16*)0x4000090
#define REG_SGWR0_H    *(u16*)0x4000092
#define REG_SGWR1      *(u32*)0x4000094
#define REG_SGWR1_L    *(u16*)0x4000094
#define REG_SGWR1_H    *(u16*)0x4000096
#define REG_SGWR2      *(u32*)0x4000098
#define REG_SGWR2_L    *(u16*)0x4000098
#define REG_SGWR2_H    *(u16*)0x400009A
#define REG_SGWR3      *(u32*)0x400009C
#define REG_SGWR3_L    *(u16*)0x400009C
#define REG_SGWR3_H    *(u16*)0x400009E
#define REG_SGFIF0A    *(u32*)0x40000A0
#define REG_SGFIFOA_L  *(u16*)0x40000A0
#define REG_SGFIFOA_H  *(u16*)0x40000A2
#define REG_SGFIFOB    *(u32*)0x40000A4
#define REG_SGFIFOB_L  *(u16*)0x40000A4
#define REG_SGFIFOB_H  *(u16*)0x40000A6
#define REG_DM0SAD     *(u32*)0x40000B0
#define REG_DM0SAD_L   *(u16*)0x40000B0
#define REG_DM0SAD_H   *(u16*)0x40000B2
#define REG_DM0DAD     *(u32*)0x40000B4
#define REG_DM0DAD_L   *(u16*)0x40000B4
#define REG_DM0DAD_H   *(u16*)0x40000B6
#define REG_DM0CNT     *(u32*)0x40000B8
#define REG_DM0CNT_L   *(u16*)0x40000B8
#define REG_DM0CNT_H   *(u16*)0x40000BA
#define REG_DM1SAD     *(u32*)0x40000BC
#define REG_DM1SAD_L   *(u16*)0x40000BC
#define REG_DM1SAD_H   *(u16*)0x40000BE
#define REG_DM1DAD     *(u32*)0x40000C0
#define REG_DM1DAD_L   *(u16*)0x40000C0
#define REG_DM1DAD_H   *(u16*)0x40000C2
#define REG_DM1CNT     *(u32*)0x40000C4
#define REG_DM1CNT_L   *(u16*)0x40000C4
#define REG_DM1CNT_H   *(u16*)0x40000C6
#define REG_DM2SAD     *(u32*)0x40000C8
#define REG_DM2SAD_L   *(u16*)0x40000C8
#define REG_DM2SAD_H   *(u16*)0x40000CA
#define REG_DM2DAD     *(u32*)0x40000CC
#define REG_DM2DAD_L   *(u16*)0x40000CC
#define REG_DM2DAD_H   *(u16*)0x40000CE
#define REG_DM2CNT     *(u32*)0x40000D0
#define REG_DM2CNT_L   *(u16*)0x40000D0
#define REG_DM2CNT_H   *(u16*)0x40000D2
#define REG_DM3SAD     *(u32*)0x40000D4
#define REG_DM3SAD_L   *(u16*)0x40000D4
#define REG_DM3SAD_H   *(u16*)0x40000D6
#define REG_DM3DAD     *(u32*)0x40000D8
#define REG_DM3DAD_L   *(u16*)0x40000D8
#define REG_DM3DAD_H   *(u16*)0x40000DA
#define REG_DM3CNT     *(u32*)0x40000DC
#define REG_DM3CNT_L   *(u16*)0x40000DC
#define REG_DM3CNT_H   *(u16*)0x40000DE
#define REG_TM0D       *(u16*)0x4000100
#define REG_TM0CNT     *(u16*)0x4000102
#define REG_TM1D       *(u16*)0x4000104
#define REG_TM1CNT     *(u16*)0x4000106
#define REG_TM2D       *(u16*)0x4000108
#define REG_TM2CNT     *(u16*)0x400010A
#define REG_TM3D       *(u16*)0x400010C
#define REG_TM3CNT     *(u16*)0x400010E
#define REG_SCD0       *(u16*)0x4000120
#define REG_SCD1       *(u16*)0x4000122
#define REG_SCD2       *(u16*)0x4000124
#define REG_SCD3       *(u16*)0x4000126
#define REG_SCCNT      *(u32*)0x4000128
#define REG_SCCNT_L    *(u16*)0x4000128
#define REG_SCCNT_H    *(u16*)0x400012A
#define REG_P1         *(u16*)0x4000130
#define REG_P1CNT      *(u16*)0x4000132
#define REG_R          *(u16*)0x4000134
#define REG_HS_CTRL    *(u16*)0x4000140
#define REG_JOYRE      *(u32*)0x4000150
#define REG_JOYRE_L    *(u16*)0x4000150
#define REG_JOYRE_H    *(u16*)0x4000152
#define REG_JOYTR      *(u32*)0x4000154
#define REG_JOYTR_L    *(u16*)0x4000154
#define REG_JOYTR_H    *(u16*)0x4000156
#define REG_JSTAT      *(u32*)0x4000158
#define REG_JSTAT_L    *(u16*)0x4000158
#define REG_JSTAT_H    *(u16*)0x400015A
#define REG_IE         *(u16*)0x4000200
#define REG_IF         *(u16*)0x4000202
#define REG_WSCNT      *(u16*)0x4000204
#define REG_IME        *(u16*)0x4000208
#define REG_PAUSE      *(u16*)0x4000300

#define REG_PALETTE_BASE      (u16 *)0x5000000
#define REG_TILE_BASE         (u8 *)0x6000000
#define REG_TILEMAP_BASE      (u16 *)0x6000000












#endif

*/
