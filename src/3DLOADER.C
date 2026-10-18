extern char gstr0[];
extern char gstr1[];
extern char gstr10[];
extern char gstr100[];
extern char gstr101[];
extern char gstr102[];
extern char gstr103[];
extern char gstr104[];
extern char gstr105[];
extern char gstr106[];
extern char gstr107[];
extern char gstr108[];
extern char gstr11[];
extern char gstr12[];
extern char gstr13[];
extern char gstr14[];
extern char gstr15[];
extern char gstr16[];
extern char gstr17[];
extern char gstr18[];
extern char gstr19[];
extern char gstr2[];
extern char gstr20[];
extern char gstr21[];
extern char gstr22[];
extern char gstr23[];
extern char gstr24[];
extern char gstr25[];
extern char gstr26[];
extern char gstr27[];
extern char gstr28[];
extern char gstr29[];
extern char gstr3[];
extern char gstr30[];
extern char gstr31[];
extern char gstr32[];
extern char gstr33[];
extern char gstr34[];
extern char gstr35[];
extern char gstr36[];
extern char gstr37[];
extern char gstr38[];
extern char gstr39[];
extern char gstr4[];
extern char gstr40[];
extern char gstr41[];
extern char gstr42[];
extern char gstr43[];
extern char gstr44[];
extern char gstr45[];
extern char gstr46[];
extern char gstr47[];
extern char gstr48[];
extern char gstr49[];
extern char gstr5[];
extern char gstr50[];
extern char gstr51[];
extern char gstr52[];
extern char gstr53[];
extern char gstr54[];
extern char gstr55[];
extern char gstr56[];
extern char gstr57[];
extern char gstr58[];
extern char gstr59[];
extern char gstr6[];
extern char gstr60[];
extern char gstr61[];
extern char gstr62[];
extern char gstr63[];
extern char gstr64[];
extern char gstr65[];
extern char gstr66[];
extern char gstr67[];
extern char gstr68[];
extern char gstr69[];
extern char gstr7[];
extern char gstr70[];
extern char gstr71[];
extern char gstr72[];
extern char gstr73[];
extern char gstr74[];
extern char gstr75[];
extern char gstr76[];
extern char gstr77[];
extern char gstr78[];
extern char gstr79[];
extern char gstr8[];
extern char gstr80[];
extern char gstr81[];
extern char gstr82[];
extern char gstr83[];
extern char gstr84[];
extern char gstr85[];
extern char gstr86[];
extern char gstr87[];
extern char gstr88[];
extern char gstr89[];
extern char gstr9[];
extern char gstr90[];
extern char gstr91[];
extern char gstr92[];
extern char gstr93[];
extern char gstr94[];
extern char gstr95[];
extern char gstr96[];
extern char gstr97[];
extern char gstr98[];
extern char gstr99[];

//***************************************************************************
//	FILE: 3DLOADER.C
//
//	Fleet Defender - F14 Tomcat
//	Author: Mike McDonald
//
//	Date: 8/23/93
//
//  Files Used:
//				F14PD01.DAT   Planes/Boats/Objects characteristics file
//              F14MD01.DAT   Missiles Characteristics file
//              F14RD01.DAT   Radar Characteristics file
//
//***************************************************************************
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include "types.h"
#include "io.h"
#include "world.h"
#include "sdata.h"
#include "f15defs.h"

extern  char Txt [];
extern int EMSHANDLE;
extern int NEMSSEG;

struct rrec {
	int RadarEnumId;
	char name[10];
	int range;
	int detectiontype;
	int missiletype;
	int missilecount;
	int jam;
	int tewsnum;
};

struct mrec {
	int MissileEnumId;
	char name[12];
	int range;
	int minrange;
	int maxalt;
	int speed;
	int homing;
	int maneuver;
	int cmeffect;
	int damage;
	char filename[12];
	int type;
};

struct prec {
	int PlaneEnumId;
	char name[7];
	int speed;
	int maneuver;
	int rclass;
	char filename[13];
	int radarsig;
	int gun;
	int alignment;
	int ecm;
	int misl[3];
	int mcnt[3];
	int bclass;
};


#define MAX_PLANE_LIST_CNT		40
#define MAX_RADAR_LIST_CNT		30
#define MAX_MISSILE_LIST_CNT	30

extern int PlaneDataSize;

extern int MissileDataSize;

extern int RadarDataSize;


extern int planerecsize;

extern int missilerecsize;

extern int radarrecsize;


extern int EMS3DObjectSlot;

extern unsigned int PlaneListSeg;

extern unsigned int RadarListSeg;

extern unsigned int MissileListSeg;


extern int EMSHANDLE;
extern int NEMSSEG;
extern int ExitErrorCode;

extern confData ConfigData;


char far *fstrcpy(char far *D, char far *S);


//***************************************************************************
int Load3DPlaneInfo(int PlaneEnumId)
{
	int i;
	int PlaneId=-1;
	FILE *PlaneDataFile;
	long recseek;
	struct prec planerec;
	int missile0, missile1;
	int returnval;


	//************************************************************
	// CHANGE THIS YOU LOOSER!!!

	if ((PlaneDataListCnt+1)>MAX_PLANE_LIST_CNT) {
		sprintf(Txt,gstr0,PlaneDataListCnt,
				MAX_PLANE_LIST_CNT);
		Message(Txt,DEBUG_MSG);
		return(-1);
	}

	PlaneId = PlaneSeek(PlaneEnumId);

	if (PlaneId != -1) return PlaneId;

	PlaneDataFile = fopen(gstr1,gstr2);

	// Make Sure this NEVER happens - This should be a FATAL_ERROR
	if (PlaneDataFile==NULL) {
		Message(gstr3,DEBUG_MSG);
		exit(-1);
	}

	recseek = (long)(PlaneEnumId * planerecsize);
	fseek(PlaneDataFile,recseek,SEEK_SET);
	if (feof(PlaneDataFile))
		return -1;
	fread(&planerec,planerecsize,1,PlaneDataFile);

	// Make Sure this NEVER happens - This should be a FATAL_ERROR
	if (!LoadEMSObject(planerec.filename,EMS3DObjectSlot)) {
		sprintf(Txt,gstr4,planerec.PlaneEnumId,planerec.filename);
		Message(Txt,DEBUG_MSG);
	}
	PlaneDataList[PlaneDataListCnt].objslot = EMS3DObjectSlot;
	EMS3DObjectSlot++;


	PlaneDataList[PlaneDataListCnt].PlaneEnumId = planerec.PlaneEnumId;
	fstrcpy(PlaneDataList[PlaneDataListCnt].name,planerec.name);
	PlaneDataList[PlaneDataListCnt].speed = planerec.speed;
	PlaneDataList[PlaneDataListCnt].maneuver = planerec.maneuver;

	// Get Radar class and store radar - look up missile info also
	// when storing radar - store missile

	RadarAdd(planerec.rclass);

	PlaneDataList[PlaneDataListCnt].rclass = planerec.rclass;

	PlaneDataList[PlaneDataListCnt].radarsig = planerec.radarsig;
	PlaneDataList[PlaneDataListCnt].gun = planerec.gun;
	PlaneDataList[PlaneDataListCnt].alignment = planerec.alignment;
	PlaneDataList[PlaneDataListCnt].ecm = planerec.ecm;


	missile0 = MissileAdd(planerec.misl[0]);
	PlaneDataList[PlaneDataListCnt].misl[0] = planerec.misl[0];
	missile1 = MissileAdd(planerec.misl[1]);
	PlaneDataList[PlaneDataListCnt].misl[1] = planerec.misl[1];
	missile1 = MissileAdd(planerec.misl[2]);
	PlaneDataList[PlaneDataListCnt].misl[2] = planerec.misl[2];


	// Get Missile class and store missile(s)
	PlaneDataList[PlaneDataListCnt].mcnt[0] = planerec.mcnt[0];
	PlaneDataList[PlaneDataListCnt].mcnt[1] = planerec.mcnt[1];
	PlaneDataList[PlaneDataListCnt].mcnt[2] = planerec.mcnt[2];

	PlaneDataList[PlaneDataListCnt].bclass = planerec.bclass;

	PlaneDataListCnt++;

	fclose(PlaneDataFile);
	returnval = PlaneDataListCnt -1;

	if(missile0 < 0)   /*  Negative Value indicates Cruise Missile, number is index into planes array   SRE  */
		Load3DPlaneInfo(-missile0);
	if(missile1 < 0)
		Load3DPlaneInfo(-missile1);

	return (returnval);
}


//***************************************************************************
int PlaneSeek(int PlaneEnumId)
{
	int i;
	int cnt=-1;

	for (i=0; i<PlaneDataListCnt; i++) {
		if (PlaneDataList[i].PlaneEnumId == PlaneEnumId) {
			cnt=i;
			break;
		}
	}

	return (cnt);
}

//***************************************************************************
int RadarAdd(int RadarEnumId)
{
	int i;
	int cnt=-1;
	FILE *RadarDataFile;
	long radarrecseek;
	struct rrec radarrec;

	// RadarAdd will search the loaded radar characteristics for the RadarEnumId
	// If the radar is found, true is returned
	// If the radar is not found, the characteristics is loaded and true returned
	// If a radar is not found in the file, then return false

	if ((RadarDataListCnt+1) > MAX_RADAR_LIST_CNT) {
		Message(gstr5,DEBUG_MSG);
		return(-1);
	}

	for (i=0; i<RadarDataListCnt; i++) {
		if (RadarDataList[i].RadarEnumId == RadarEnumId) {
			cnt=i;
			break;
		}
	}

	if (cnt != -1) return(1);

	// Search file for Radar Entry
	RadarDataFile = fopen(gstr6,gstr2);

	if (RadarDataFile==NULL) {
		Message(gstr7,DEBUG_MSG);
		exit(-1);
	}

	radarrecseek = (long)(RadarEnumId * radarrecsize);
	fseek(RadarDataFile,radarrecseek,SEEK_SET);
	// If EOF, then we have a problem, invalid ID, return error
	if (feof(RadarDataFile)) {
		Message(gstr8,DEBUG_MSG);
		fclose(RadarDataFile);
		return (0);
	}

	fread(&radarrec,radarrecsize,1,RadarDataFile);

	RadarDataList[RadarDataListCnt].RadarEnumId = radarrec.RadarEnumId;
	fstrcpy(RadarDataList[RadarDataListCnt].name,radarrec.name);
	RadarDataList[RadarDataListCnt].range = ((radarrec.range * 16)/10);
	RadarDataList[RadarDataListCnt].detectiontype = radarrec.detectiontype;

	// Check if missile is there, if not add type
	MissileAdd(radarrec.missiletype);
	RadarDataList[RadarDataListCnt].missiletype = radarrec.missiletype;

	RadarDataList[RadarDataListCnt].missilecount = radarrec.missilecount;
	RadarDataList[RadarDataListCnt].jam = radarrec.jam;
	RadarDataList[RadarDataListCnt].tewsnum = radarrec.tewsnum;

	RadarDataListCnt++;
	fclose(RadarDataFile);
	return (1);
}

//***************************************************************************
int MissileAdd(int MissileEnumId)
{
	int i;
	int cnt=-1;
	FILE *MissileDataFile;
	long missilerecseek;
	struct mrec missilerec;

	if ((MissileDataListCnt+1) > MAX_MISSILE_LIST_CNT) {
		Message(gstr9,DEBUG_MSG);
		return(-1);
	}


	for (i=0; i<MissileDataListCnt; i++) {
		if (MissileDataList[i].MissileEnumId == MissileEnumId) {
			cnt=i;
			break;
		}
	}

	if (cnt != -1) return(1);

	// Search file for Missile Entry
	MissileDataFile = fopen(gstr10,gstr2);

	if (MissileDataFile==NULL) {
		Message(gstr11,DEBUG_MSG);
		exit(-1);
	}

	missilerecseek = (long)(MissileEnumId * missilerecsize);
	fseek(MissileDataFile,missilerecseek,SEEK_SET);
	// If EOF, then we have a problem, invalid ID, return error
	if (feof(MissileDataFile)) {
		fclose(MissileDataFile);
		return (0);
	}
	fread(&missilerec,missilerecsize,1,MissileDataFile);

	MissileDataList[MissileDataListCnt].MissileEnumId = missilerec.MissileEnumId;
	fstrcpy(MissileDataList[MissileDataListCnt].name,missilerec.name);
	MissileDataList[MissileDataListCnt].range = ((missilerec.range*16)/10);  // KLUDE - nmi-to-km
	MissileDataList[MissileDataListCnt].minrange = ((missilerec.minrange*16)/10);
	MissileDataList[MissileDataListCnt].maxalt = missilerec.maxalt;
	MissileDataList[MissileDataListCnt].speed = missilerec.speed;
	MissileDataList[MissileDataListCnt].homing = missilerec.homing;
	MissileDataList[MissileDataListCnt].maneuver = missilerec.maneuver;
	MissileDataList[MissileDataListCnt].cmeffect = missilerec.cmeffect;
	MissileDataList[MissileDataListCnt].damage = missilerec.damage;
	MissileDataList[MissileDataListCnt].objslot = EMS3DObjectSlot;
	MissileDataList[MissileDataListCnt].type = missilerec.type;

	// Make Sure this NEVER happens - This should be a FATAL_ERROR
	if (!LoadEMSObject(missilerec.filename,EMS3DObjectSlot)) {
		sprintf(Txt,gstr12,missilerec.MissileEnumId,missilerec.filename);
		Message(Txt,DEBUG_MSG);
	}

	EMS3DObjectSlot++;
	MissileDataListCnt++;
	fclose(MissileDataFile);

	if(missilerec.type < 0)
		return(missilerec.type);

	return(1);
}



//***************************************************************************
int GetPicType(int PlaneNo)
{
	if (PlaneNo <0) {
		TXT(gstr13);
		TXN(PlaneNo);
		Message(Txt,DEBUG_MSG);
		return -1;
	}

	if (PlaneNo > PlaneDataListCnt) {
		Message(gstr14,DEBUG_MSG);
		return -1;
	}

	return(PlaneDataList[PlaneNo].objslot);
}

//***************************************************************************
struct PlaneData far *GetPlanePtr(int PlaneNo)
{

	if (PlaneNo <0) {
		TXT(gstr15);
		TXN(PlaneNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}

	if (PlaneNo > PlaneDataListCnt) {
		TXT(gstr16);
		TXN(PlaneNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}
	return &(PlaneDataList[PlaneNo]);
}

//***************************************************************************
struct MissileData far *GetMissilePtr(int MissileNo)
{
	int i;
	int FoundNo=-1;

	if (MissileNo <0) {
		TXT(gstr17);
		TXN(MissileNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}

	for (i=0; i<MissileDataListCnt; i++) {
		if (MissileDataList[i].MissileEnumId == MissileNo) {
			FoundNo = i;
			break;
		}
	}

	if (FoundNo == -1) {
		TXT(gstr18);
		TXN(MissileNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}
	return &(MissileDataList[FoundNo]);
}

//***************************************************************************
struct RadarData far *GetRadarPtr(int RadarNo)
{
	int i;
	int FoundNo=-1;

	if (RadarNo <0) {
		TXT(gstr19);
		TXN(RadarNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}

	for (i=0; i<RadarDataListCnt; i++) {
		if (RadarDataList[i].RadarEnumId == RadarNo) {
			FoundNo = i;
			break;
		}
	}

	if (FoundNo == -1) {
		TXT(gstr20);
		TXN(RadarNo);
		Message(Txt,DEBUG_MSG);
		return (NULL);
	}
	return &(RadarDataList[FoundNo]);
}


//***************************************************************************
int Release3DMemory()
{
	if (EMSHANDLE)
		EMSReleasePages(EMSHANDLE);
}

//***************************************************************************
int Init3DPlanes()
{
	// FATAL_ERROR
	if (_dos_allocmem((PlaneDataSize*MAX_PLANE_LIST_CNT),&PlaneListSeg)) {
		ExitErrorCode=-12;
		InitAbort();
	}
	FP_SEG(PlaneDataList) = PlaneListSeg;
	FP_OFF(PlaneDataList) = 0;

	if (_dos_allocmem((RadarDataSize*MAX_RADAR_LIST_CNT),&RadarListSeg)) {
		ExitErrorCode=-16;
		InitAbort();
	}
	FP_SEG(RadarDataList) = RadarListSeg;
	FP_OFF(RadarDataList) = 0;

	if (_dos_allocmem((MissileDataSize*MAX_MISSILE_LIST_CNT),&MissileListSeg)) {
		ExitErrorCode=-14;
		InitAbort();
	}
	FP_SEG(MissileDataList) = MissileListSeg;
	FP_OFF(MissileDataList) = 0;
}

//***************************************************************************
int LoadSpecialObjects()
{
	int i,j,squad;

	char *CarrierAmerica[14] = {
		gstr21,		// Base - Level 1
		gstr22,		// Tower - Level 1 & 2
		gstr23,		// Deck - Level 1 & 2
		gstr24,		// Wires
		gstr25,		// Ball
		gstr26,	  		// Blast Door 1
		gstr27,	   		// Blast Door 2
		gstr28,	   		// Blast Door 3
		gstr29,			// Blast Door 4
		gstr30,		// Base - Level 2
		gstr31,		// Base - Level 3
		gstr32,		// Tower - Level 3
		gstr33,		// Carrier - Level 4
		gstr34		// Wake
	};

	char *CarrierNimitz[14] = {
		gstr35,		// Base - Level 1
		gstr36,		// Tower - Level 1 & 2
		gstr37,		// Deck - Level 1 & 2
		gstr38,		// Wires
		gstr39,		// Ball
		gstr40,			// Blast Door 1
		gstr41,			// Blast Door 2
		gstr42,			// Blast Door 3
		gstr43,	 		// Blast Door 4
		gstr44,		// Base - Level 2
		gstr45,		// Base - Level 3
		gstr46,		// Tower - Level 3
		gstr47,		// Carrier - Level 4
		gstr48		// Wake
	};

	char *CarrierSaratoga[14] = {
		gstr49,		// Base - Level 1
		gstr50,		// Tower - Level 1 & 2
		gstr51,		// Deck - Level 1 & 2
		gstr52,		// Wires
		gstr53,		// Ball
		gstr54,			// Blast Door 1
		gstr55,			// Blast Door 2
		gstr56,			// Blast Door 3
		gstr57,			// Blast Door 4
		gstr58,		// Base - Level 2
		gstr59,		// Base - Level 3
		gstr60,		// Tower - Level 3
		gstr61,		// Carrier - Level 4
		gstr62		// Wake
	};

	char *CoolF14Objects[9] = {
		gstr63,
		gstr64,
		gstr65,
		gstr66,
		gstr67,
		gstr68,
		gstr69,
		gstr70,
		gstr71,
	};

	char *CoolF14Tails[22] = {
//		"F14LTSAN.3DZ",
//		"F14RTSAN.3DZ",
		gstr72,	// Tophatters VF-14
		gstr73,
		gstr74, // Swordsman  VF-32
		gstr75,
		gstr76,	// Starfighters VF-33
		gstr77,
		gstr78,	// Black Aces	VF-41
		gstr79,
		gstr80,	// Be-Devilers	VF-74
		gstr81,
		gstr82,	// Jolly Rogers VF-84
		gstr83,
		gstr84,	// Diamondbacks	VF-102
		gstr85,
		gstr86,	// Sluggers VF-103
		gstr87,
		gstr88,	// Ghostriders VF-142
		gstr89,
		gstr90,	// Pukin' Dogs VF-143
		gstr91,
		gstr92,
		gstr93
	};

	char *Missiles[7] = {
		gstr94,
		gstr95,
		gstr96,
		gstr97,
		gstr98,
		gstr99,
		gstr100
	};

	char *MiscSpecial[9] = {
		gstr101,
		gstr102,
		gstr103,
		gstr104,
		gstr105,
		gstr106,
		gstr107
	};

	// Load Carrier
	for (i=0; i<14; i++) {
		switch(ConfigData.Carrier) {
			case 0:
				if (!LoadEMSObject(CarrierAmerica[i],i)) {
					sprintf(Txt,gstr108,CarrierAmerica[i]);
					Message(Txt,DEBUG_MSG);
				}
			break;
			case 1:
		   		if (!LoadEMSObject(CarrierNimitz[i],i)) {
					sprintf(Txt,gstr108,CarrierNimitz[i]);
					Message(Txt,DEBUG_MSG);
				}
			break;
			case 2:
				if (!LoadEMSObject(CarrierSaratoga[i],i)) {
					sprintf(Txt,gstr108,CarrierSaratoga[i]);
					Message(Txt,DEBUG_MSG);
				}
			break;
			default:
   				if (!LoadEMSObject(CarrierAmerica[i],i)) {
					sprintf(Txt,gstr108,CarrierAmerica[i]);
					Message(Txt,DEBUG_MSG);
				}
			break;
		}
	}

	// Load F-14

	for (j=0,i=14; i<23; j++,i++) {
   		if (!LoadEMSObject(CoolF14Objects[j],i)) {
			sprintf(Txt,gstr108,CoolF14Objects[j]);
			Message(Txt,DEBUG_MSG);
		}
	}

	// Load F-14 Squadron

	squad = ConfigData.Squadron;
	if (squad<0) squad = 20;
	if (squad>9) squad = 20;
	squad = squad * 2;

   	if (!LoadEMSObject(CoolF14Tails[squad],23)) {
		sprintf(Txt,gstr108,CoolF14Tails[squad]);
		Message(Txt,DEBUG_MSG);
	}


   	if (!LoadEMSObject(CoolF14Tails[squad+1],24)) {
		sprintf(Txt,gstr108,CoolF14Tails[squad+1]);
		Message(Txt,DEBUG_MSG);
	}

	// Load Missiles

	for (j=0,i=25; i<32; j++,i++) {
   		if (!LoadEMSObject(Missiles[j],i)) {
			sprintf(Txt,gstr108,Missiles[j]);
			Message(Txt,DEBUG_MSG);
		}
	}

	// Misc.

	for (j=0,i=32; i<39; j++,i++) {
   		if (!LoadEMSObject(MiscSpecial[j],i)) {
			sprintf(Txt,gstr108,MiscSpecial[j]);
			Message(Txt,DEBUG_MSG);
		}
	}

	EMS3DObjectSlot=39;
	MissileAdd(ML_AIM54C);
	MissileAdd(ML_AIM7M);
	MissileAdd(ML_AIM9M);
}


