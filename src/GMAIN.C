
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


extern int WeatherType;

extern int RIOEmsOk;


struct _mrec_ {
	int		z;
	long	x;
	long	y;
	int		pad[15];
};
extern struct _mrec_ aipaths[];


extern RPS     * Rp;

extern UBYTE ExitCode;

extern int ExitErrorCode;

extern int DisplayFrame;

extern int F14SNUM[2];

extern int F14SCNUM[2];

extern char    * F14CNUM[5];

extern char    * F14SNAME[30];

extern char    * F14SPER[30];

extern char	* F14VNUM[5];


extern int _training;

extern int DifficultyLevel;


extern confData ConfigData;


extern SndDataInfo SndData;



//***************************************************************************
main(argc, argv, envp)
int     argc;
char    **argv;
char    **envp;
{
	extern char gstr252[];
	extern char gstr253[];
	save_video_state();
    InitOverlay(LoadOverlay(gstr252,gstr253));
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
	extern char gstr254[];
	LoadPicture(1,gstr254);
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
	extern char gstr255[];
	extern char gstr256[];
	extern char gstr257[];
	extern char gstr258[];
	extern char gstr259[];
	extern char gstr260[];
	extern char gstr261[];
	extern char gstr262[];
	extern char gstr263[];
	extern char gstr264[];
	if (ExitErrorCode != 0) {
		printf(gstr255);
		switch(ExitErrorCode) {
			case -60:
				printf(gstr256);
			break;
			case -46:
				printf(gstr257);
			break;
			case -44:
				printf(gstr258);
			break;
			case -42:
				printf(gstr259);
			break;
			case -40:
				printf(gstr260);
			break;
			case -16:
				printf(gstr261);
			break;
			case -14:
				printf(gstr262);
			break;
			case -12:
				printf(gstr263);
			break;
			case -10:
				printf(gstr264);
			break;
		}
	}
}

//***************************************************************************
LoadFiles()
{
	extern char gstr265[];
	extern char gstr266[];
	extern char gstr267[];
	extern char gstr268[];
	extern char gstr269[];
	extern char gstr270[];
	extern char gstr271[];
	extern char gstr272[];
	extern char gstr273[];
	extern char gstr274[];
	extern char gstr275[];
	extern char gstr276[];
	if (LoadEMSPicFile(gstr265,SPRTXPLSEG,0,20)) {
    	LoadEMSPicFile(gstr266,SPRTXPLSEG,1,0);
    	LoadEMSPicFile(gstr267,SPRTXPLSEG,2,0);
    	LoadEMSPicFile(gstr268,SPRTXPLSEG,3,0);
    	LoadEMSPicFile(gstr269,SPRTXPLSEG,4,0);
	}

	if (LoadEMSPicFile(gstr270,MISCSPRTSEG,0,8)) {
		LoadEMSPicFile(gstr271,MISCSPRTSEG,1,0);
	}

	NewEmsFunction();

	switch(ConfigData.World) {
		case 0:
			LoadTheWorld(gstr272);
		break;
		case 1:
			LoadTheWorld(gstr273);
		break;
		case 2:
			LoadTheWorld(gstr274);
		break;
		case 3:
			LoadTheWorld(gstr275);
		break;
		case 4:
			LoadTheWorld(gstr276);
		break;
		default:
			LoadTheWorld(gstr274);
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
	extern char gstr277[];
	extern char gstr278[];

	left = EMSFileUsage();
	left -= EMSPagesGrabbed;
	left = -left;
	if (left<20) LEV=1;
	LoadGroundObjects();
	LEV=0;
	LoadPlanesEms();
	EMSpages(&free,&total);
	if (free>8) {
		if (LoadEMSPicFile(gstr277,HEAD1SEG,0,8)) {
			LoadEMSPicFile(gstr278,HEAD1SEG,1,0);
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
	extern char gstr279[];
	extern char gstr2[];
	FILE *f ;

	f = fopen (gstr279, gstr2) ;
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
	extern char gstr280[];
	extern char gstr2[];
	FILE *f ;

	f = fopen(gstr280, gstr2) ;
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
