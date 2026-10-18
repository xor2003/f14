//***************************************************************************
// FILE: GMAIN.C
//
// Fleet Defender - F14 Tomcat
// Project Manager: Scott Spanburg
// Revised by: Mike McDonald
//
// Adapted from F-15 Strike Eagle code by Sid Mieyer, Andy Hollis
//
//***************************************************************************
#include <stdio.h>
#include <fcntl.h>
#include "types.h"
#include "io.h"
#include "library.h"
#include "f15defs.h"
#include "world.h"
#include "sdata.h"
#include "cockpit.h"
#include "common.h"
#include "snddrv.h"
#include "awg9.h"

extern  RPS     *Rp1, *Rp2, *Rp3D;
extern  char    Txt [];
extern  UBYTE   BEND;
extern  UWORD   GRPAGE0[];
extern  UWORD   Joystick;
extern  UWORD   Speedy;
extern  UWORD   CenterPosition[];
extern  char  *CockpitFiles[2][6];

extern int EMSHANDLE;
extern int NEMSSEG;
extern int AWG9_REALITY_LEVEL;
extern int RIO_ASSIST;
extern int World;

extern  int     StickType;

extern int MissionWeather;
extern int CLOUDTHICK;
extern UWORD CLOUDHEIGHT;
extern int LEV;
extern int EMSPagesGrabbed;
extern int EMSPagesFree;
extern int MsgEMS;
extern int whichourcarr;

extern int far EMSpages(int *a, int *b);


int WeatherType=0;
int RIOEmsOk=0;

struct _mrec_ {
	int		z;
	long	x;
	long	y;
	int		pad[15];
};
extern struct _mrec_ aipaths[];


RPS     *Rp;
extern UBYTE ExitCode;

extern int ExitErrorCode;

extern int DisplayFrame;

int     F14SNUM[2] = { 5252,18 };
int     F14SCNUM[2] = { 2525,18 };
char    *F14CNUM[5] = { "F14R" };
char    *F14SNAME[30] = { "Quality Assurance" };
char    *F14SPER[30] = { "Vaughn Thomas" };
char	*F14VNUM[5]  = { "1.18" };

int _training;
extern int DifficultyLevel;


confData ConfigData;

SndDataInfo SndData;


//***************************************************************************
main(argc, argv, envp)
int     argc;
char    **argv;
char    **envp;
{
	save_video_state();
    InitOverlay(LoadOverlay("Mgraphic.exe","Fonts.F14"));
    TurnOnGraphicsMode(0);

	InitOptions();

	LoadSoundConfig();
    InitSound();
    InitGraphicPages();
    InitGraph('M');

	WaitScreen();

	set_ctrl_b();
    MouseON();

	GetJoyConfig();
	LoadConfig();
    LoadFiles();
    InitLogFile();
	ConfigCrts();

	InitCarrier();
	LoadScottMission();
	LEV=0;
    InitEmsFiles();
	InitStealth();
	InitWeather();

	ScreenOff();
	ClearPage(0,0);

	LoadSprites();
	Init3DPal();
	InitDamnPalettes();
	InitFlir();


	ARMT_INIT();
    AWG9_INIT();
	InitThreatTbl();

    MainGameLoop();

	PutNumLockBack();

    MouseOFF();
    reset_ctrl_b();
    FreeEMS();
    restore_video_state();
	DisplayErrorMessages();
    Exit(ExitCode);
}

//***************************************************************************
int WaitScreen()
{
	LoadPicture(1,"gm-load.PIC");
	ShowPage(1);
	ScreenOn();
}

//***************************************************************************
int InitAbort()
{
	Release3DMemory();
    MouseOFF();
    reset_ctrl_b();
    FreeEMS();
    restore_video_state();

	SndShutdown();

	DisplayErrorMessages();
    Exit(ExitCode);
}

//***************************************************************************
int InitWeather()
{
	if ((MissionWeather<0) || (MissionWeather>2))
		MissionWeather=1;
	if ((MissionWeather!=1) && (MissionWeather!=2)) {
		WeatherType=rnd(3);
		switch (WeatherType) {
			case 0:
				CLOUDHEIGHT=9150;
				CLOUDTHICK=500;
			break;
			default:
				CLOUDHEIGHT=9150;
				CLOUDTHICK=500;
			break;
			case 1:
				CLOUDHEIGHT=6150;
				CLOUDTHICK=2500;
			break;
			case 2:
				CLOUDHEIGHT=2240;
				CLOUDTHICK=4240;
			break;
		}
	} else {
		WeatherType=-1;
		CLOUDHEIGHT=8150;
		CLOUDTHICK=600;
	}
}

//***************************************************************************
int DisplayErrorMessages()
{
	if (ExitErrorCode != 0) {
		printf("\nProgram Termination:  ");
		switch(ExitErrorCode) {
			case -60:
				printf("3D Object Not Found.\n");
			break;
			case -46:
				printf("MISSION.F14 File Not Found.\n");
			break;
			case -44:
				printf("F14MD01.DAT File Not Found.\n");
			break;
			case -42:
				printf("F14RD01.DAT File Not Found.\n");
			break;
			case -40:
				printf("F14PD01.DAT File Not Found.\n");
			break;
			case -16:
				printf("Not Enough Memory to Load Radar Objects.\n");
			break;
			case -14:
				printf("Not Enough Memory to Load Missile Objects.\n");
			break;
			case -12:
				printf("Not Enough Memory to Load Planes Objects.\n");
			break;
			case -10:
				printf("Not Enough Memory to load objects.\n");
			break;
		}
	}
}

//***************************************************************************
LoadFiles()
{
	if (LoadEMSPicFile("sp1ex2.pic",SPRTXPLSEG,0,20)) {
    	LoadEMSPicFile("sp2ex2.pic",SPRTXPLSEG,1,0);
    	LoadEMSPicFile("sp3ex2.pic",SPRTXPLSEG,2,0);
    	LoadEMSPicFile("sp4ex2.pic",SPRTXPLSEG,3,0);
    	LoadEMSPicFile("sp5ex2.pic",SPRTXPLSEG,4,0);
	}

	if (LoadEMSPicFile("cpsp1.PIC",MISCSPRTSEG,0,8)) {
		LoadEMSPicFile("cpsp2.PIC",MISCSPRTSEG,1,0);
	}

	NewEmsFunction();

	switch(ConfigData.World) {
		case 0:
			LoadTheWorld("NCN01.WLD");
		break;
		case 1:
			LoadTheWorld("MEDN01.WLD");
		break;
		case 2:
			LoadTheWorld("USAN01.WLD");
		break;
		case 3:
			LoadTheWorld("KOR04.WLD");
		break;
		case 4:
			LoadTheWorld("INDIA09.WLD");
		break;
		default:
			LoadTheWorld("USAN01.WLD");
		break;
	}

	LoadMsgEMS();
	Init3DPlanes();
	LoadSpecialObjects();
	LoadClouds();
}

//***************************************************************************
InitEmsFiles()
{
	unsigned int	free;
	unsigned int	total;
	int		left;

	left = EMSFileUsage();
	left -= EMSPagesGrabbed;
	left = -left;
	if (left<20) LEV=1;
	LoadGroundObjects();
	LEV=0;
	LoadPlanesEms();
	EMSpages(&free,&total);
	if (free>8) {
		if (LoadEMSPicFile("RIO-1.PIC",HEAD1SEG,0,8)) {
			LoadEMSPicFile("RIO-2.PIC",HEAD1SEG,1,0);
		}
		RIOEmsOk=1;
	} else {
		RIOEmsOk=0;
	}
	InitSpeech();
}

//***************************************************************************
int InitDamnPalettes()
{
    UBYTE   PalPkt[256*3 + 6];

	View=FRONT;
    InitSpecialPal(-1);
    LoadPicToPal(1, CockpitFiles[0][FRONT], PalPkt);
	UpdatePalette();
    ShowPage(1);

    BuildCockpitSprites();
}

//***************************************************************************
int LoadConfig()
{
	FILE *f ;

	f = fopen ("config.f14", "rb") ;
	fread(&_training,   sizeof (_training),   1, f) ;
	fread(&ConfigData, sizeof (ConfigData), 1, f) ;
	fclose(f);

	AWG9_REALITY_LEVEL = ConfigData.Radar;
	RIO_ASSIST = ConfigData.RioAssist;
	if (_training) Status ^= TRAINING;
}

//***************************************************************************
int LoadSoundConfig()
{
	FILE *f ;

	f = fopen("config.snd", "rb") ;
	fread(&SndData, sizeof(SndData), 1, f) ;
	fclose(f);
}

//***************************************************************************
MainGameLoop()
{

	SetJoysticks(StickType);
    KBInit();

    do {
        UpdateTime1();
		UpdateTime2();

        TakeInputs();
        LocalCmds();

        if ((--DisplayFrame)==0) {
            GenDsp();
            DoCockpitDisplays();
			Messages();
        }

		ProcessInputs();
		FLIGHT();

		Stealth();

		DoPlayerOnTheCat();  // keeps plane in sink with boat

        AWG9();
        TEWS_SYS();
        UpdatePalette();

        if (DisplayFrame==0) {
            Flip();
            DisplayFrame=Speedy;
        }
    } while (!BEND);

	if (whichourcarr != -999) {
		LogIt(45,whichourcarr,
			aipaths[whichourcarr].x,
			aipaths[whichourcarr].y,
			aipaths[whichourcarr].z,
			-999,-999,0L,1);
	}

	Release3DMemory();

    MouseHIDE();
    ClearPage(0, BLACK);
    ClearPage(1, BLACK);

    SndShutdown();
    UnInitSpeech();
    DumpLogFile();
	GetRidOfKeyJoy();
}

//***************************************************************************
InitGraphicPages()
{
    SetGraphicPage(0, GRPAGE0[0] = AllocGraphicPage(0)/*+320/16*/);
    SetGraphicPage(1, GRPAGE0[1] = AllocGraphicPage(1));
    SetGraphicPage(2, GRPAGE0[2] = AllocGraphicPage(2));
    Rp = Rp2;
}

//***************************************************************************
int NoOp1()
{
}

//***************************************************************************
int NoOp2()
{
}
