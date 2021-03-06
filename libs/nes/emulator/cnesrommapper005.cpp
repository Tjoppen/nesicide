//    NESICIDE - an IDE for the 8-bit NES.
//    Copyright (C) 2009  Christopher S. Pow

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "cnesrommapper005.h"

#include "cnes6502.h"
#include "cnesppu.h"

#include "cregisterdata.h"

// Mapper 005 Registers
static CBitfieldData* tbl5000Bitfields [] =
{
   new CBitfieldData("Duty Cycle", 6, 2, "%X", 4, "25%", "50%", "75%", "12.5%"),
   new CBitfieldData("Channel State", 5, 1, "%X", 2, "Running", "Halted"),
   new CBitfieldData("Envelope Disabled", 4, 1, "%X", 2, "No", "Yes"),
   new CBitfieldData("Volume/Envelope", 0, 4, "%X", 0)
};

static CBitfieldData* tbl5001Bitfields [] =
{
   new CBitfieldData("Sweep Enabled", 7, 1, "%X", 2, "No", "Yes"),
   new CBitfieldData("Sweep Divider", 4, 3, "%X", 0),
   new CBitfieldData("Sweep Direction", 3, 1, "%X", 2, "Down", "Up"),
   new CBitfieldData("Sweep Shift", 0, 3, "%X", 0)
};

static CBitfieldData* tbl5002Bitfields [] =
{
   new CBitfieldData("Period Low-bits", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5003Bitfields [] =
{
   new CBitfieldData("Length", 3, 5, "%X", 32, "0A","FE","14","02","28","04","50","06","A0","08","3C","0A","0E","0C","1A","0E","0C","10","18","12","30","14","60","16","C0","18","48","1A","10","1C","20","1E"),
   new CBitfieldData("Period High-bits", 0, 3, "%X", 0)
};

static CBitfieldData* tbl5010Bitfields [] =
{
   new CBitfieldData("IRQ Enabled", 7, 1, "%X", 2, "No", "Yes"),
   new CBitfieldData("Loop", 6, 1, "%X", 2, "No", "Yes"),
   new CBitfieldData("Period", 0, 4, "%X", 16, "1AC","17C","154","140","11E","0FE","0E2","0D6","0BE","0A0","08E","080","06A","054","048","036")
};

static CBitfieldData* tbl5011Bitfields [] =
{
   new CBitfieldData("Volume", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5015Bitfields [] =
{
   new CBitfieldData("Square2 Channel", 1, 1, "%X", 2, "Disabled", "Enabled"),
   new CBitfieldData("Square1 Channel", 0, 1, "%X", 2, "Disabled", "Enabled"),
};

static CBitfieldData* tbl5100Bitfields [] =
{
   new CBitfieldData("PRG Mode", 0, 2, "%X", 4, "32KB", "16KB", "16KB+8KB", "8KB")
};

static CBitfieldData* tbl5101Bitfields [] =
{
   new CBitfieldData("CHR Mode", 0, 2, "%X", 4, "8KB", "4KB", "2KB", "1KB")
};

static CBitfieldData* tbl5102Bitfields [] =
{
   new CBitfieldData("PRG-RAM Protect A", 0, 2, "%X", 0)
};

static CBitfieldData* tbl5103Bitfields [] =
{
   new CBitfieldData("PRG-RAM Protect B", 0, 2, "%X", 0)
};

static CBitfieldData* tbl5104Bitfields [] =
{
   new CBitfieldData("EXRAM Mode", 0, 2, "%X", 4, "Extra Nametable mode", "Extended Attribute mode", "CPU access mode", "CPU read-only mode")
};

static CBitfieldData* tbl5105Bitfields [] =
{
   new CBitfieldData("$2000 mapping", 0, 2, "%X", 4, "$2000", "$2400", "EXRAM", "Fill mode"),
   new CBitfieldData("$2400 mapping", 2, 2, "%X", 4, "$2000", "$2400", "EXRAM", "Fill mode"),
   new CBitfieldData("$2800 mapping", 4, 2, "%X", 4, "$2000", "$2400", "EXRAM", "Fill mode"),
   new CBitfieldData("$2C00 mapping", 6, 2, "%X", 4, "$2000", "$2400", "EXRAM", "Fill mode")
};

static CBitfieldData* tbl5106Bitfields [] =
{
   new CBitfieldData("Fill Tile", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5107Bitfields [] =
{
   new CBitfieldData("Fill Attribute bits", 0, 2, "%X", 0)
};

static CBitfieldData* tbl5113Bitfields [] =
{
   new CBitfieldData("$6000 mapping", 0, 3, "%X", 0)
};

static CBitfieldData* tbl5114Bitfields [] =
{
   new CBitfieldData("PRG mapping", 0, 7, "%02X", 0),
   new CBitfieldData("ROM select", 7, 1, "%X", 2, "RAM", "ROM")
};

static CBitfieldData* tbl5115Bitfields [] =
{
   new CBitfieldData("PRG mapping", 0, 7, "%02X", 0),
   new CBitfieldData("ROM select", 7, 1, "%X", 2, "RAM", "ROM")
};

static CBitfieldData* tbl5116Bitfields [] =
{
   new CBitfieldData("PRG mapping", 0, 7, "%02X", 0),
   new CBitfieldData("ROM select", 7, 1, "%X", 2, "RAM", "ROM")
};

static CBitfieldData* tbl5117Bitfields [] =
{
   new CBitfieldData("PRG mapping", 0, 7, "%02X", 0)
};

static CBitfieldData* tbl5120Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5121Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5122Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5123Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5124Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5125Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5126Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5127Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5128Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5129Bitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl512ABitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl512BBitfields [] =
{
   new CBitfieldData("CHR mapping", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5130Bitfields [] =
{
   new CBitfieldData("CHR high bits", 0, 2, "%X", 0)
};

static CBitfieldData* tbl5200Bitfields [] =
{
   new CBitfieldData("Tile", 0, 5, "%X", 0),
   new CBitfieldData("Side", 6, 1, "%X", 2, "Left", "Right"),
   new CBitfieldData("Enabled", 7, 1, "%X", 2, "No", "Yes")
};

static CBitfieldData* tbl5201Bitfields [] =
{
   new CBitfieldData("Split Y scroll", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5202Bitfields [] =
{
   new CBitfieldData("Split 4KB CHR page", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5203Bitfields [] =
{
   new CBitfieldData("IRQ Target", 0, 8, "%02X", 0),
};

static CBitfieldData* tbl5204Bitfields [] =
{
   new CBitfieldData("In Frame", 6, 1, "%X", 2, "No", "Yes"),
   new CBitfieldData("Enabled/Pending", 7, 1, "%X", 2, "No", "Yes")
};

static CBitfieldData* tbl5205Bitfields [] =
{
   new CBitfieldData("Multiplicand & Result LSB", 0, 8, "%02X", 0)
};

static CBitfieldData* tbl5206Bitfields [] =
{
   new CBitfieldData("Multiplier & Result MSB", 0, 8, "%02X", 0)
};

static CRegisterData* tblRegisters [] =
{
   new CRegisterData(0x5000, "Square1 Control", nesMapperLowRead, nesMapperLowWrite, 4, tbl5000Bitfields),
   new CRegisterData(0x5001, "Square1 Sweep", nesMapperLowRead, nesMapperLowWrite, 4, tbl5001Bitfields),
   new CRegisterData(0x5002, "Square1 Period LSB", nesMapperLowRead, nesMapperLowWrite, 1, tbl5002Bitfields),
   new CRegisterData(0x5003, "Square1 Length", nesMapperLowRead, nesMapperLowWrite, 2, tbl5003Bitfields),
   new CRegisterData(0x5004, "Square2 Control", nesMapperLowRead, nesMapperLowWrite, 4, tbl5000Bitfields),
   new CRegisterData(0x5005, "Square2 Sweep", nesMapperLowRead, nesMapperLowWrite, 4, tbl5001Bitfields),
   new CRegisterData(0x5006, "Square2 Period LSB", nesMapperLowRead, nesMapperLowWrite, 1, tbl5002Bitfields),
   new CRegisterData(0x5007, "Square2 Length", nesMapperLowRead, nesMapperLowWrite, 2, tbl5003Bitfields),
   new CRegisterData(0x5010, "DMC Control", nesMapperLowRead, nesMapperLowWrite, 3, tbl5010Bitfields),
   new CRegisterData(0x5011, "DMC DAC", nesMapperLowRead, nesMapperLowWrite, 1, tbl5011Bitfields),
   new CRegisterData(0x5015, "APU Control", nesMapperLowRead, nesMapperLowWrite, 2, tbl5015Bitfields),
   new CRegisterData(0x5100, "PRG Mode Select", nesMapperLowRead, nesMapperLowWrite, 1, tbl5100Bitfields),
   new CRegisterData(0x5101, "CHR Mode Select", nesMapperLowRead, nesMapperLowWrite, 1, tbl5101Bitfields),
   new CRegisterData(0x5102, "PRG-RAM Write Protect A", nesMapperLowRead, nesMapperLowWrite, 1, tbl5102Bitfields),
   new CRegisterData(0x5103, "PRG-RAM Write Protect B", nesMapperLowRead, nesMapperLowWrite, 1, tbl5103Bitfields),
   new CRegisterData(0x5104, "EXRAM Mode Select", nesMapperLowRead, nesMapperLowWrite, 1, tbl5104Bitfields),
   new CRegisterData(0x5105, "Mirroring Mode Select", nesMapperLowRead, nesMapperLowWrite, 4, tbl5105Bitfields),
   new CRegisterData(0x5106, "Fill Tile", nesMapperLowRead, nesMapperLowWrite, 1, tbl5106Bitfields),
   new CRegisterData(0x5107, "Fill Attribute", nesMapperLowRead, nesMapperLowWrite, 1, tbl5107Bitfields),
   new CRegisterData(0x5113, "$6000 Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5113Bitfields),
   new CRegisterData(0x5114, "PRG Control", nesMapperLowRead, nesMapperLowWrite, 2, tbl5114Bitfields),
   new CRegisterData(0x5115, "PRG Control", nesMapperLowRead, nesMapperLowWrite, 2, tbl5115Bitfields),
   new CRegisterData(0x5116, "PRG Control", nesMapperLowRead, nesMapperLowWrite, 2, tbl5116Bitfields),
   new CRegisterData(0x5117, "PRG Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5117Bitfields),
   new CRegisterData(0x5120, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5120Bitfields),
   new CRegisterData(0x5121, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5121Bitfields),
   new CRegisterData(0x5122, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5122Bitfields),
   new CRegisterData(0x5123, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5123Bitfields),
   new CRegisterData(0x5124, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5124Bitfields),
   new CRegisterData(0x5125, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5125Bitfields),
   new CRegisterData(0x5126, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5126Bitfields),
   new CRegisterData(0x5127, "CHR A Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5127Bitfields),
   new CRegisterData(0x5128, "CHR B Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5128Bitfields),
   new CRegisterData(0x5129, "CHR B Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl5129Bitfields),
   new CRegisterData(0x512A, "CHR B Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl512ABitfields),
   new CRegisterData(0x512B, "CHR B Control", nesMapperLowRead, nesMapperLowWrite, 1, tbl512BBitfields),
   new CRegisterData(0x5130, "CHR High bits", nesMapperLowRead, nesMapperLowWrite, 1, tbl5130Bitfields),
   new CRegisterData(0x5200, "Split Screen Control", nesMapperLowRead, nesMapperLowWrite, 3, tbl5200Bitfields),
   new CRegisterData(0x5201, "Split Screen Vert Scroll", nesMapperLowRead, nesMapperLowWrite, 1, tbl5201Bitfields),
   new CRegisterData(0x5202, "Split Screen CHR Page", nesMapperLowRead, nesMapperLowWrite, 1, tbl5202Bitfields),
   new CRegisterData(0x5203, "IRQ Trigger", nesMapperLowRead, nesMapperLowWrite, 1, tbl5203Bitfields),
   new CRegisterData(0x5204, "IRQ Control", nesMapperLowRead, nesMapperLowWrite, 2, tbl5204Bitfields),
   new CRegisterData(0x5205, "8*8 Multiplier", nesMapperLowRead, nesMapperLowWrite, 1, tbl5205Bitfields),
   new CRegisterData(0x5206, "8*8 Multiplier", nesMapperLowRead, nesMapperLowWrite, 1, tbl5206Bitfields),
};

static const char* rowHeadings [] =
{
   ""
};

static const char* columnHeadings [] =
{
   "5000", "5001", "5002", "5003", "5004", "5005", "5006", "5007", "5010", "5011", "5015",
   "5100", "5101", "5102", "5103", "5104", "5105", "5106", "5107",
   "5113", "5114", "5115", "5116", "5117",
   "5120", "5121", "5122", "5123", "5124", "5125", "5126", "5127", "5128", "5129", "512A", "512B",
   "5130",
   "5200", "5201", "5202", "5203", "5204", "5205", "5206"
};

static CRegisterDatabase* dbRegisters = new CRegisterDatabase(eMemory_cartMapper,1,44,44,tblRegisters,rowHeadings,columnHeadings);

uint8_t  CROMMapper005::m_prgMode = 0;
uint8_t  CROMMapper005::m_chrMode = 0;
uint8_t  CROMMapper005::m_chrHigh = 0;
uint8_t  CROMMapper005::m_irqScanline = 0;
uint8_t  CROMMapper005::m_irqEnabled = 0;
uint8_t  CROMMapper005::m_irqStatus = 0;
bool           CROMMapper005::m_prgRAM [] = { false, false, false };
bool           CROMMapper005::m_wp = false;
uint8_t  CROMMapper005::m_wp1 = 0;
uint8_t  CROMMapper005::m_wp2 = 0;
uint8_t  CROMMapper005::m_mult1 = 0;
uint8_t  CROMMapper005::m_mult2 = 0;
uint16_t CROMMapper005::m_prod = 0;
uint8_t  CROMMapper005::m_fillTile = 0;
uint8_t  CROMMapper005::m_fillAttr = 0;
uint32_t CROMMapper005::m_ppuCycle = 0;
uint8_t  CROMMapper005::m_chr[] = { 0, };
uint8_t  CROMMapper005::m_reg[] = { 0, };
CAPUSquare CROMMapper005::m_square[2];
CAPUDMC    CROMMapper005::m_dmc;

CROMMapper005::CROMMapper005()
{
}

CROMMapper005::~CROMMapper005()
{
}

void CROMMapper005::RESET ( bool soft )
{
   m_mapper = 5;

   m_dbRegisters = dbRegisters;

   m_square[0].RESET();
   m_square[1].RESET();
   m_dmc.RESET();

   CROM::RESET ( m_mapper, soft );

   m_prgMode = 0;
   m_chrMode = 0;
   m_chrHigh = 0;
   m_irqScanline = 0;
   m_irqStatus = 0;

   m_pPRGROMmemory [ 0 ] = m_PRGROMmemory [ 0 ];
   m_prgRAM [ 0 ] = false;
   m_pPRGROMmemory [ 1 ] = m_PRGROMmemory [ 1 ];
   m_prgRAM [ 1 ] = false;
   if ( m_numPrgBanks == 2 )
   {
      m_pPRGROMmemory [ 2 ] = m_PRGROMmemory [ 0 ];
      m_prgRAM [ 2 ] = false;
   }
   else
   {
      m_pPRGROMmemory [ 2 ] = m_PRGROMmemory [ 2 ];
      m_prgRAM [ 2 ] = false;
   }
   m_pPRGROMmemory [ 3 ] = m_PRGROMmemory [ m_numPrgBanks-1 ];

   // CHR ROM/RAM already set up in CROM::RESET()...
}

void CROMMapper005::SYNCCPU ( void )
{
   m_square[0].TIMERTICK();
   m_square[1].TIMERTICK();
   m_dmc.TIMERTICK();
}

void CROMMapper005::SYNCPPU ( uint32_t ppuCycle, uint32_t ppuAddr )
{
   int32_t scanline = CYCLE_TO_VISY(ppuCycle);

   m_ppuCycle = ppuCycle;

   SETPPU();

   if ( scanline < 239 )
   {
      m_irqStatus = 0x40;
      C6502::RELEASEIRQ(eNESSource_Mapper);
   }

   if ( scanline == 239 )
   {
      m_irqStatus &= ~(0x40);
   }

   if ( (m_irqScanline > 0) && (scanline == m_irqScanline) )
   {
      m_irqStatus |= 0x80;
   }

   if ( m_irqEnabled && (m_irqStatus&0x80) )
   {
      C6502::ASSERTIRQ ( eNESSource_Mapper );

      if ( nesIsDebuggable() )
      {
         // Check for IRQ breakpoint...
         CNES::CHECKBREAKPOINT(eBreakInMapper,eBreakOnMapperEvent,0,MAPPER_EVENT_IRQ);
      }
   }
}

void CROMMapper005::SETCPU ( void )
{
}

void CROMMapper005::SETPPU ( void )
{
   int32_t rasterX = CYCLE_TO_VISX(m_ppuCycle);

   if ( (rasterX >= 256) &&
        (rasterX < 320) )
   {
      // Sprite fetches
      m_pCHRmemory [ 0 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[0] ];
      m_pCHRmemory [ 1 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[1] ];
      m_pCHRmemory [ 2 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[2] ];
      m_pCHRmemory [ 3 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[3] ];
      m_pCHRmemory [ 4 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[4] ];
      m_pCHRmemory [ 5 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[5] ];
      m_pCHRmemory [ 6 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[6] ];
      m_pCHRmemory [ 7 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[7] ];
   }
   else
   {
      // Background fetches
      m_pCHRmemory [ 0 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[8] ];
      m_pCHRmemory [ 1 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[9] ];
      m_pCHRmemory [ 2 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[10] ];
      m_pCHRmemory [ 3 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[11] ];
      m_pCHRmemory [ 4 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[8] ];
      m_pCHRmemory [ 5 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[9] ];
      m_pCHRmemory [ 6 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[10] ];
      m_pCHRmemory [ 7 ] = m_CHRmemory [ (m_chrHigh<<8)|m_chr[11] ];
   }
}

uint32_t CROMMapper005::HMAPPER ( uint32_t addr )
{
   if ( (m_prgRAM[0]) && (addr < 0xA000) )
   {
      return CROM::SRAMVIRT ( addr );
   }
   else if ( (m_prgRAM[1]) && (addr >= 0xA000) && (addr < 0xC000) )
   {
      return CROM::SRAMVIRT ( addr );
   }
   else if ( (m_prgRAM[2]) && (addr >= 0xC000) && (addr < 0xE000) )
   {
      return CROM::SRAMVIRT ( addr );
   }
   else
   {
      return CROM::PRGROM(addr);
   }
}

void CROMMapper005::HMAPPER ( uint32_t addr, uint8_t data )
{
   // if PRG RAM has been mapped in it will already be there...
   // first check to ensure writes allowed...
   if ( !m_wp )
   {
      CROM::SRAMVIRT ( addr, data );
   }
}

uint32_t CROMMapper005::DEBUGINFO ( uint32_t addr )
{
   switch ( addr )
   {
   case 0x5000:
      return m_reg[0];
      break;
   case 0x5001:
      return m_reg[1];
      break;
   case 0x5002:
      return m_reg[2];
      break;
   case 0x5003:
      return m_reg[3];
      break;
   case 0x5004:
      return m_reg[4];
      break;
   case 0x5005:
      return m_reg[5];
      break;
   case 0x5006:
      return m_reg[6];
      break;
   case 0x5007:
      return m_reg[7];
      break;
   case 0x5010:
      return m_reg[8];
      break;
   case 0x5011:
      return m_reg[9];
      break;
   case 0x5015:
      return m_reg[10];
      break;
   case 0x5100:
      return m_reg[11];
      break;
   case 0x5101:
      return m_reg[12];
      break;
   case 0x5102:
      return m_reg[13];
      break;
   case 0x5103:
      return m_reg[14];
      break;
   case 0x5104:
      return m_reg[15];
      break;
   case 0x5105:
      return m_reg[16];
      break;
   case 0x5106:
      return m_reg[17];
      break;
   case 0x5107:
      return m_reg[18];
      break;
   case 0x5113:
      return m_reg[19];
      break;
   case 0x5114:
      return m_reg[20];
      break;
   case 0x5115:
      return m_reg[21];
      break;
   case 0x5116:
      return m_reg[22];
      break;
   case 0x5117:
      return m_reg[23];
      break;
   case 0x5120:
      return m_reg[24];
      break;
   case 0x5121:
      return m_reg[25];
      break;
   case 0x5122:
      return m_reg[26];
      break;
   case 0x5123:
      return m_reg[27];
      break;
   case 0x5124:
      return m_reg[28];
      break;
   case 0x5125:
      return m_reg[29];
      break;
   case 0x5126:
      return m_reg[30];
      break;
   case 0x5127:
      return m_reg[31];
      break;
   case 0x5128:
      return m_reg[32];
      break;
   case 0x5129:
      return m_reg[33];
      break;
   case 0x512A:
      return m_reg[34];
      break;
   case 0x512B:
      return m_reg[35];
      break;
   case 0x5130:
      return m_reg[36];
      break;
   case 0x5200:
      return m_reg[37];
      break;
   case 0x5201:
      return m_reg[38];
      break;
   case 0x5202:
      return m_reg[39];
      break;
   case 0x5203:
      return m_reg[40];
      break;
   case 0x5204:
      return m_reg[41];
      break;
   case 0x5205:
      return m_reg[42];
      break;
   case 0x5206:
      return m_reg[43];
      break;
   }
   return 0;
}

uint32_t CROMMapper005::LMAPPER ( uint32_t addr )
{
   uint8_t data = 0xff;

   if ( addr >= 0x6000 )
   {
      data = CROM::SRAMVIRT(addr);
   }
   else if ( addr >= 0x5C00 )
   {
      data = EXRAM ( addr );
   }
   else
   {
      switch ( addr )
      {
         case 0x5204:
            data = m_irqStatus;
            m_irqStatus &= ~(0x80);
            C6502::RELEASEIRQ ( eNESSource_Mapper );
            break;
         case 0x5205:
            data = m_prod&0xFF;
            break;
         case 0x5206:
            data = (m_prod>>8)&0xFF;
            break;
      }
   }

   return data;
}

void CROMMapper005::LMAPPER ( uint32_t addr, uint8_t data )
{
   uint8_t prgRAM;

   if ( addr >= 0x6000 )
   {
      CROM::SRAMVIRT(addr,data);
   }
   else if ( addr >= 0x5C00 )
   {
      EXRAM ( addr, data );
   }
   else
   {
      switch ( addr )
      {
      case 0x5000:
         m_reg[0] = data;
         m_square[0].APU(0,data);
         break;
      case 0x5001:
         m_reg[1] = data;
         m_square[0].APU(1,data);
         break;
      case 0x5002:
         m_reg[2] = data;
         m_square[0].APU(2,data);
         break;
      case 0x5003:
         m_reg[3] = data;
         m_square[0].APU(3,data);
         break;
      case 0x5004:
         m_reg[4] = data;
         m_square[1].APU(0,data);
         break;
      case 0x5005:
         m_reg[5] = data;
         m_square[1].APU(1,data);
         break;
      case 0x5006:
         m_reg[6] = data;
         m_square[1].APU(2,data);
         break;
      case 0x5007:
         m_reg[7] = data;
         m_square[1].APU(3,data);
         break;
      case 0x5010:
         m_reg[8] = data;
         m_dmc.APU(0,data);
         break;
      case 0x5011:
         m_reg[9] = data;
         m_dmc.APU(1,data);
         break;
      case 0x5015:
         m_reg[10] = data;
         m_square[0].ENABLE(!!(data&0x01));
         m_square[1].ENABLE(!!(data&0x02));
         break;
      case 0x5100:
         m_reg[11] = data;
         m_prgMode = data&0x3;
         break;
      case 0x5101:
         m_reg[12] = data;
         m_chrMode = data&0x3;
         break;
      case 0x5102:
         m_reg[13] = data;
         m_wp1 = data&0x3;

         if ( (m_wp1 == 0x2) && (m_wp2 == 0x1) )
         {
            m_wp = false;
         }
         else
         {
            m_wp = true;
         }

         break;
      case 0x5103:
         m_reg[14] = data;
         m_wp2 = data&0x3;

         if ( (m_wp1 == 0x2) && (m_wp2 == 0x1) )
         {
            m_wp = false;
         }
         else
         {
            m_wp = true;
         }

         break;
      case 0x5104:
         m_reg[15] = data;
         break;
      case 0x5105:
         m_reg[16] = data;
         CPPU::MIRROR ( data&0x03, (data&0x0C)>>2, (data&0x30)>>4, (data&0xC0)>>6 );
         break;
      case 0x5106:
         m_reg[17] = data;
         m_fillTile = data;
         break;
      case 0x5107:
         m_reg[18] = data;
         m_fillAttr = data&0x3;
         break;
      case 0x5113:
         m_reg[19] = data;
         data &= 0x7;
         CROM::REMAPSRAM ( 0x6000, data );
         break;
      case 0x5114:
         m_reg[20] = data;
         prgRAM = !(data&0x80);
         data &= 0x7F;

         if ( prgRAM )
         {
            data &= 0x7;
         }

         if ( m_prgMode == 3 )
         {
            m_prgRAM[0] = prgRAM;
            if ( m_prgRAM[0] )
            {
               CROM::REMAPSRAM ( 0x8000, data );
            }
            else
            {
               m_pPRGROMmemory [ 0 ] = m_PRGROMmemory [ data ];
            }
         }

         break;
      case 0x5115:
         m_reg[21] = data;
         prgRAM = !(data&0x80);
         data &= 0x7F;

         if ( prgRAM )
         {
            data &= 0x7;
         }

         if ( (m_prgMode == 1) || (m_prgMode == 2) )
         {
            m_prgRAM[0] = prgRAM;
            m_prgRAM[1] = prgRAM;
            if ( prgRAM )
            {
               CROM::REMAPSRAM ( 0x8000, (data&0xFE)+0 );
               CROM::REMAPSRAM ( 0xA000, (data&0xFE)+1 );
            }
            else
            {
               m_pPRGROMmemory [ 0 ] = m_PRGROMmemory [ (data&0xFE)+0 ];
               m_pPRGROMmemory [ 1 ] = m_PRGROMmemory [ (data&0xFE)+1 ];
            }
         }
         else if ( m_prgMode == 3 )
         {
            m_prgRAM[1] = prgRAM;
            if ( prgRAM )
            {
               CROM::REMAPSRAM ( 0x8000, data );
            }
            else
            {
               m_pPRGROMmemory [ 1 ] = m_PRGROMmemory [ data ];
            }
         }

         break;
      case 0x5116:
         m_reg[22] = data;
         prgRAM = !(data&0x80);
         data &= 0x7F;

         if ( prgRAM )
         {
            data &= 0x7;
         }

         if ( (m_prgMode == 2) || (m_prgMode == 3) )
         {
            m_prgRAM[2] = prgRAM;
            if ( prgRAM )
            {
               CROM::REMAPSRAM ( 0xC000, data );
            }
            else
            {
               m_pPRGROMmemory [ 2 ] = m_PRGROMmemory [ data ];
            }
         }

         break;
      case 0x5117:
         m_reg[23] = data;
         if ( m_prgMode == 0 )
         {
            m_pPRGROMmemory [ 0 ] = m_PRGROMmemory [ (data&0xFC)+0 ];
            m_pPRGROMmemory [ 1 ] = m_PRGROMmemory [ (data&0xFC)+1 ];
            m_pPRGROMmemory [ 2 ] = m_PRGROMmemory [ (data&0xFC)+2 ];
            m_pPRGROMmemory [ 3 ] = m_PRGROMmemory [ (data&0xFC)+3 ];
         }
         else if ( m_prgMode == 1 )
         {
            m_pPRGROMmemory [ 2 ] = m_PRGROMmemory [ (data&0xFE)+0 ];
            m_pPRGROMmemory [ 3 ] = m_PRGROMmemory [ (data&0xFE)+1 ];
         }
         else if ( (m_prgMode == 2) || (m_prgMode == 3) )
         {
            m_pPRGROMmemory [ 3 ] = m_PRGROMmemory [ data ];
         }

         break;
      case 0x5120:
         m_reg[24] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[0] = data;
         }
         break;
      case 0x5121:
         m_reg[25] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[1] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[0] = (data<<1)+0;
            m_chr[1] = (data<<1)+1;
         }
         break;
      case 0x5122:
         m_reg[26] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[2] = data;
         }
         break;
      case 0x5123:
         m_reg[27] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[3] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[2] = (data<<1)+0;
            m_chr[3] = (data<<1)+1;
         }
         else if ( m_chrMode == 1 )
         {
            m_chr[0] = (data<<2)+0;
            m_chr[1] = (data<<2)+1;
            m_chr[2] = (data<<2)+2;
            m_chr[3] = (data<<2)+3;
         }
         break;
      case 0x5124:
         m_reg[28] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[4] = data;
         }
         break;
      case 0x5125:
         m_reg[29] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[5] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[4] = (data<<1)+0;
            m_chr[5] = (data<<1)+1;
         }
         break;
      case 0x5126:
         m_reg[30] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[6] = data;
         }
         break;
      case 0x5127:
         m_reg[31] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[7] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[6] = (data<<1)+0;
            m_chr[7] = (data<<1)+1;
         }
         else if ( m_chrMode == 1 )
         {
            m_chr[4] = (data<<2)+0;
            m_chr[5] = (data<<2)+1;
            m_chr[6] = (data<<2)+2;
            m_chr[7] = (data<<2)+3;
         }
         else if ( m_chrMode == 0 )
         {
            m_chr[0] = (data<<3)+0;
            m_chr[1] = (data<<3)+1;
            m_chr[2] = (data<<3)+2;
            m_chr[3] = (data<<3)+3;
            m_chr[4] = (data<<3)+4;
            m_chr[5] = (data<<3)+5;
            m_chr[6] = (data<<3)+6;
            m_chr[7] = (data<<3)+7;
         }
         break;
      case 0x5128:
         m_reg[32] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[8] = data;
         }
         break;
      case 0x5129:
         m_reg[33] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[9] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[8] = (data<<1)+0;
            m_chr[9] = (data<<1)+1;
         }
         break;
      case 0x512A:
         m_reg[34] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[10] = data;
         }
         break;
      case 0x512B:
         m_reg[35] = data;
         if ( m_chrMode == 3 )
         {
            m_chr[11] = data;
         }
         else if ( m_chrMode == 2 )
         {
            m_chr[10] = (data<<1)+0;
            m_chr[11] = (data<<1)+1;
         }
         else if ( m_chrMode == 1 || m_chrMode == 0 )
         {
            m_chr[8] = (data<<2)+0;
            m_chr[9] = (data<<2)+1;
            m_chr[10] = (data<<2)+2;
            m_chr[11] = (data<<2)+3;
         }
         break;
      case 0x5130:
         m_reg[36] = data;
         m_chrHigh = data&0x3;
         break;
      case 0x5200:
         m_reg[37] = data;
         break;
      case 0x5201:
         m_reg[38] = data;
         break;
      case 0x5202:
         m_reg[39] = data;
         break;
      case 0x5203:
         m_reg[40] = data;
         m_irqScanline = data;
         break;
      case 0x5204:
         m_reg[41] = data;
         m_irqEnabled = data&0x80;
         break;
      case 0x5205:
         m_reg[42] = data;
         m_mult1 = data;
         m_prod = m_mult1*m_mult2;
         break;
      case 0x5206:
         m_reg[43] = data;
         m_mult2 = data;
         m_prod = m_mult1*m_mult2;
         break;
      }
   }
   SETPPU();
}

uint16_t CROMMapper005::AMPLITUDE ( void )
{
   float famp;
   int16_t amp;
   int16_t delta;
   int16_t out[100] = { 0, };
   static int16_t outLast = 0;
   uint8_t sample;
   uint8_t* sq1dacSamples = m_square[0].GETDACSAMPLES();
   uint8_t* sq2dacSamples = m_square[1].GETDACSAMPLES();
   uint8_t* dmcDacSamples = m_dmc.GETDACSAMPLES();
   static int32_t outDownsampled = 0;

   for ( sample = 0; sample < m_square[0].GETDACSAMPLECOUNT(); sample++ )
   {
//      output = square_out + tnd_out
//
//
//                            95.88
//      square_out = -----------------------
//                          8128
//                   ----------------- + 100
//                   square1 + square2
//
//
//                            159.79
//      tnd_out = ------------------------------
//                            1
//                ------------------------ + 100
//                triangle   noise    dmc
//                -------- + ----- + -----
//                  8227     12241   22638
      famp = 0.0;
      if ( (*(sq1dacSamples+sample))+(*(sq2dacSamples+sample)) )
      {
         famp = (95.88/((8128.0/((*(sq1dacSamples+sample))+(*(sq2dacSamples+sample))))+100.0));
      }
      if ( (*(dmcDacSamples+sample)) )
      {
         famp += (159.79/((1.0/((((*(dmcDacSamples+sample))/22638.0))))+100.0));
      }
      amp = (int16_t)(float)(65535.0*famp*0.50);

      (*(out+sample)) = amp;

      outDownsampled += (*(out+sample));
   }

   outDownsampled = (int32_t)((float)outDownsampled/((float)m_square[0].GETDACSAMPLECOUNT()));

   delta = outDownsampled - outLast;
   outDownsampled = outLast+((delta*65371)/65536); // 65371/65536 is 0.9975 adjusted to 16-bit fixed point.

   outLast = outDownsampled;

   // Reset DAC averaging...
   m_square[0].CLEARDACAVG();
   m_square[1].CLEARDACAVG();
   m_dmc.CLEARDACAVG();

   return outDownsampled;
}

void CROMMapper005::SOUNDENABLE(uint32_t mask)
{
   m_square[0].MUTE(0);//!(mask&0x01));
   m_square[1].MUTE(0);//!(mask&0x02));
   m_dmc.MUTE(0);//!(mask&0x04));
}
