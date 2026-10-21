
//***************************************************************************
// File:	Cockpit.c
//
// Author: Mike McDonald
//
//
// Fleet Defender: F-14 Tomcat
// MicroProse, Inc.
// 180 LakeFront Drive
// HuntValley, MD 21030
//***************************************************************************
#include "library.h"

#define _include_code
#include <stdlib.h>
#include "world.h"
#include "sdata.h"
#include "Cockpit.h"
#include "f15defs.h"
#include "armt.h"
#include "awg9.h"
#include "string.h"
#include "sounds.h"

extern  RPS     *Rp1, RpX1, *Rp2, *Rp3, *Rp3D, TDBRpX;

extern  int 	WingRot;
extern  int     THRUST;
extern  int     ThrustI;
extern  int     Knots;
extern  UWORD     Rtime;
extern  int     Px, Py;
extern  long	PxLng;
extern  long    PyLng;
extern  int     atarg,gtarg;
extern  int     ShowMouse;
extern  char    Txt[];
extern  int     Mscale;
extern  int     Tscale;
extern  UWORD   Alt;
extern  UWORD   QTimer;
extern  int     JAMMER;
extern  int     JAMMER_ACTIVE;
extern  int     MLAUNCH;
extern  int     AIDETECT;
extern  int     WING_HAVETARG;
extern	UWORD	OurAlt;
extern  int     OurRoll;
extern  long	RadarAlt;
extern	int	    OurHead;
extern	int	    AirSpeed;
extern  int     LIGHTS;
extern  int     REALFLIGHT;
extern  long    FuelFlow;
extern  int		BingoFuel;
extern  int		OurPitch;
extern	long	NEWAOA;
extern  int 	mapmode;
extern  SWORD	VVI;
extern  int		LightsOn;
extern	int		BEAM_PIT;
extern  int     MsgOnScreen;
extern  int     HookDown;
extern  int		NextWayPoint;
extern  int		Gees;

extern   char  Coursestr[];
extern   char  Slopestr[];
extern   char  Rangestr[];
extern   char  Decentstr[];
extern   int   MYCLCD;
extern   int   MYCP;
extern   int   LOADDRAG;
extern   int   MYLOAD;
extern   char  *loadstrs[];
extern   int   OurLoadOut;
extern   int PadLockee;
extern   int DifficultyLevel;
extern int Speedy;
extern int SNDDETAIL;

extern char    * CockpitFiles[2][6];


#ifdef YEP
extern char    * CockpitFiles[2][6];

#endif

extern int Lengths[2][6];


typedef struct {
	int x;
	int y;
	int l;
	int w;
	int fromcolor;
	int tocolor;
} clights;
extern clights far CockpitLights[40];





extern UWORD   far VdiMask[250];

extern UWORD   far AdiMask[170];

extern UWORD	far LookDownLMask[130];

extern UWORD	far LookDownRMask[105];

extern UWORD	far RawMask[185];

extern UWORD	far TopCockpitMask[2400];

extern UWORD	far HsdMask[300];

extern UWORD	far DDDMask[888];

extern UWORD	far TIDMask[480];

extern UWORD	far ElevMask[600];


extern SWORD PitchAngle;
extern UWORD DialSprite[];

extern int MASTER_CAUTION;

extern int DAMAGELITE[16];

extern int RadarRepeat;

extern int BackSeat;

extern int RIOEmsOk;

extern int Panning;

// int     LookingDown = 0;
extern int DeClutter;

extern int LLowFuelWarn;

extern int RLowFuelWarn;

extern int BingoFuelWarn;


extern UWORD OutsideWidth;
     /* size of outside view window */
extern UWORD OutsideLength;

// UWORD   OutsideLength = 199;

extern RPS RpCX1;

extern RPS     * RpCRT1;


extern RPS RpCX2;

extern RPS     * RpCRT2;


extern RPS RpCX3;

extern RPS     * RpCRT3;


extern RPS RpCX4;

extern RPS     * RpCRT4;


extern RPS RpCX5;

extern RPS     * RpCRT5;


extern RPS RpCX6;

extern RPS     * RpCRT6;


extern RPS RpCX7;

extern RPS     * RpCRT7;


extern RPS     * RpCRT;


extern RPS     * CrtRps[];

extern int CrtDisplays[8];

extern int CrtUpdate[8];


extern int CrtConfig[8];


extern int CurCRT;


extern int MasterArmSW;

extern int JetisonSW;

extern int Lightemup;

extern int lighton;

extern int VdiMode;

extern int msiz;


extern  int OLDMM;
extern  int OldFuel;
extern  int OldThrust;
extern  int OldLights[40];
extern  int OldEMIS;
extern  int OldMC;

extern int EgtL;

extern int EgtR;


// scaley(y) { return(y*14/16); }

extern struct FPARAM{
	long	X;
	long	Y;
	long	Z;
	long	VEL[3];
	int	MAT[9];
} FPARAMS;

extern int MeatBall;
extern int NumBars;
extern int AZSweep;
extern int     LastSpike;
extern int     LastMud;
extern confData ConfigData;
extern int DataLink;
extern int ClrTarg;
extern int DesignateMode;
extern int HighlightTarg;

extern struct PlaneData far *GetPlanePtr(int PlaneNo);
static int RadarMode=0;
static int PrevRadar=0;
extern int RadarRange;


//***************************************************************************
LoadNewCockpit()
{
    UBYTE   PalPkt[256*3 + 6];
    extern char gstr777[];
    extern char gstr778[];

    if (RIOEmsOk==0 && !BackSeat && (View==REARLEFT || View==REARRIGHT)) {
		if (View==REARLEFT)
			LoadPicToPal(1, gstr777, PalPkt);
		if (View==REARRIGHT)
			LoadPicToPal(1, gstr778, PalPkt);
	} else {
		LoadPicToPal(1, CockpitFiles[BackSeat][View&VNUM], PalPkt);
	}
	InitSpecialPal(View);

   	ScreenOff();
    ShowPage(1);

    ResetCockpitLights();

    switch (View) {
		case REARLEFT:
			msiz = BuildMask(TopCockpitMask,0,0,319,163,1);
			break;

		case REARRIGHT:
			msiz = BuildMask(TopCockpitMask,0,0,319,163,1);
			break;

		case PILOT:
        case PADLOCK:
			break;
		case RIGHT:
		case LEFT:
			msiz = BuildMask(TopCockpitMask,0,0,319,110,1);
			break;
		case DOWN:
			if (!BackSeat) {
				msiz = BuildMask(HsdMask,117,100,86,70,1);
			} else {
  				msiz = BuildMask(TIDMask,91,36,140,114,1);
			}
            break;

        default:
			BuildCockpitSprites();
            break;
    }
	if ((View!=REARLEFT) && (View!=REARRIGHT))
    	ChangeHUDColor(0);
}


//***************************************************************************
BuildCockpitSprites()
{
	int x;
	char ch;
	char temp[5];

	if (!BackSeat) {
		if (View==FRONT) {
			x = BuildMask(TopCockpitMask,0,0,319,150,1);
			x = BuildMask(VdiMask, 119,138,82,57, 1);
			x = BuildMask(AdiMask, 226,136,30,34,1);
			x = BuildMask(LookDownLMask, 0,110,57,44,1);
			x = BuildMask(LookDownRMask,261,110,59,39,1);
			x = BuildMask(RawMask, 260,150,42,41,1);
			LookDownRMask[MASKY]=0;
			LookDownLMask[MASKY]=0;
			UpdateLights();
		}
	} else {
		if (View==FRONT) {
			x = BuildMask(TopCockpitMask,0,0,319,150,1);
			x = BuildMask(DDDMask,116,106,88,60,1);
			x = BuildMask(ElevMask,83,108,20,61,1);
			DDDSwitchRefresh();
		}
	}
}

//***************************************************************************
LookDown()
{
	if (!BackSeat) {
		Rp3D->Length1 = 50;
   		RpCRT1->YBgn = 27;
   		RpCRT2->YBgn = 25;
		RpCRT4->YBgn = 39;

		VdiMask[MASKY] = 27;
		VdiMask[3] = 57;
		AdiMask[MASKY] = 26;
		RawMask[MASKY] = 39;
	}
}

//***************************************************************************
LookUp ()
{
	if (!BackSeat) {
		Rp3D->Length1  = 150;
		RpCRT1->YBgn   = 138;
		RpCRT2->YBgn   = 136;
		RpCRT4->YBgn   = 150;

		VdiMask[MASKY] = 138;
		AdiMask[MASKY] = 136;
		RawMask[MASKY] = 150;
	}
}

//***************************************************************************
Flip ()
{
	char tmp[80];
	char s[10];
	static int lastview=-1;
	extern int FlipCnt;

//    FireColors ();
    MouseHIDE ();

	switch(View) {
		case FRONT:
			if (!BackSeat) {
				if (Status&0x10) RpPrint (Rp3D, 0,2, "TRNG" );
				if (Speedy>1) {
					SetFont (Rp3D, F_3x4);
					RpPrint (Rp3D, 5,8, "x");
					RpPrint (Rp3D, 0,8, itoa(Speedy,s,10));
				}
				MaskCopy(TopCockpitMask);
				if (!MsgOnScreen)
					VdiMask[3] = 57;
				else
					VdiMask[3] = 54;
				MaskCopy(VdiMask);
				MaskCopy(AdiMask);
				MaskCopy(RawMask);
			} else {
				if (Status&0x10) RpPrint (Rp3D, 0,2, "TRNG" );
				if (Speedy>1) {
					SetFont (Rp3D, F_3x4);
					RpPrint (Rp3D, 5,8, "x");
					RpPrint (Rp3D, 0,8, itoa(Speedy,s,10));
				}
				MaskCopy(TopCockpitMask);
				MaskCopy(DDDMask);
				MaskCopy(ElevMask);
			}
		break;
		case DOWN:
			if (!BackSeat) {
				if (Status&0x10) RpPrint (Rp3D, 0,2, "TRNG" );
				if (Speedy>1) {
					SetFont (Rp3D, F_3x4);
					RpPrint (Rp3D, 5,8, "x");
					RpPrint (Rp3D, 0,8, itoa(Speedy,s,10));
				}
				MaskCopy(LookDownLMask);
				MaskCopy(LookDownRMask);
				MaskCopy(AdiMask);
				MaskCopy(RawMask);
				MaskCopy(HsdMask);
				MaskCopy(VdiMask);
			} else {
				MaskCopy(TIDMask);
			}
		break;
		case LEFT:
		case RIGHT:
		case REARLEFT:
		case REARRIGHT:
			MaskCopy(TopCockpitMask);
		break;
		case MAGICCAM2:
		case 17+VFULL+VOFFSET:
			if ((DifficultyLevel==0) && (PadLockee > -1)) {
				_fstrcpy ((char *)tmp, (char far *)GetPlanePtr(ps[PadLockee].type)->name);
				SetFont(Rp3D,F_3x5B);
				RpPrint(Rp3D,0,10,tmp);
			}
		case FRONTAL:
			RectCopy(Rp3D, 0,0, OutsideWidth, OutsideLength, Rp1, (320-OutsideWidth) >>1,0);
		break;
		case PADLOCK:
		case PILOT:
		case STEADY:
		case WINGMAN:
		case MAGICCAM1:
		case POVVIEW:
		case EJECTVIEW:
		case TOWERVIEW:
		case LSOVIEW:
			RectCopy(Rp3D, 0,0, OutsideWidth, OutsideLength, Rp1, (320-OutsideWidth) >>1,0);
		break;
    }

    if (ShowMouse) MouseSHOW ();
	if ((lastview != View) || (FlipCnt)) {
    	ScreenOn();
		lastview = View;
		if (FlipCnt > 0) FlipCnt--;
	}
	if ((SNDDETAIL!=1) || (SNDDETAIL!=3))
    	SndSounds(View>FRONTAL ? N_JetFromOutside:N_JetFromInside);
}


//***************************************************************************
ConfigCrts ()
{
    int i;

    for (i=0; i<8; i++) {
        CrtDisplays[i] = CrtConfig[i];
    }
}


//***************************************************************************
DoCockpitDisplays ()
{
    static int LastCur = 2;
	char s[10];
	int alt,x,y;
	int althund;
	int x2,y2;
	static int FirstArm=0;
	int i;

	if (!BackSeat) {
		if (View==DOWN) {
			DoCRT(0);
			DoCRT(1);
 			DoCRT(2);
			DoCRT(3);
			DrawVVGauge();
			DrawAirSpeedGauge();
			DrawAltGauge();
			DrawRadarAlt();
			DrawCompass();
			DrawStores();
			ShowFuel();
			DrawEngineStat();
			GDisplay();
   		}

	    if (View==FRONT) {
			HUD();
			DrawVVGauge();
		  	DrawAirSpeedGauge();
			DrawAltGauge();
			DrawRadarAlt();
			DrawAOADisplay();
			DrawSweepDisplay();
			DrawStores();
			DrawCompass();
			DrawIndexer();
			DoCRT(0);
			DoCRT(1);
			DoCRT(3);
			UpdateCockpitLites();
			DisplayMaster();
			GDisplay();
		}
		if ((View==REARLEFT || View==REARRIGHT) && RIOEmsOk) {
			DisplayRioHead();
		}
		if (View==LEFT) {
			DisplayLeftStuff();
		}
		if (View==RIGHT) {
			DisplayRightStuff(1);
		}
	} else {
		if (View==FRONT) {
			DoCRT(4);
			DDDSwitches();
			DrawRadarElev();
			DrawRadarStatus();
			DrawRadarRange();
		}
		if (View==DOWN) {
			DoCRT(5);
			DrawRadarStatus();
			TIDShowRange();
			DrawWayPoint();
//			DisplayBackLights();
			DisplayTIDSwitches();
		}
		if (View==LEFT) {
			DrawRadarDials();
		}
		if (View==RIGHT) {
			DisplayRightStuff(0);
		}

	}
	if (View==PADLOCK || View==PILOT) {
		DoPadLockView();
	}
}

//***************************************************************************
DisplayMaster()
{
	static int prevcaut=-1;

	if (MASTER_CAUTION != prevcaut) {
		if (MASTER_CAUTION) {
			RectFill(Rp1,152,119,16,2,16);
		} else {
			RectFill(Rp1,152,119,16,2,18);
		}
	}
	prevcaut = MASTER_CAUTION;
}

//***************************************************************************
DrawRadarStatus()
{
	char s[25];
	int l;
	int offset=0;
	extern char gstr194[];
	extern char gstr195[];
	extern char gstr196[];
	extern char gstr197[];
	extern char gstr198[];
	extern char gstr199[];
	extern char gstr200[];
	extern char gstr201[];
	extern char gstr202[];
	extern char gstr203[];
	extern char gstr204[];
	extern char gstr205[];
	extern char gstr206[];
	extern char gstr207[];
	extern char gstr208[];
	extern char gstr209[];
	extern char gstr210[];
	extern char gstr211[];
	extern char gstr212[];
	extern char gstr213[];

	SetFont(Rp2,F_3x5B);
	Rp2->APen=View?202:54;

	switch(AWG9_MODE) {
		// PD Search

		case AWG9_SRCH_WIDE:
			strcpy(s,gstr194);
		break;
		case AWG9_SRCH_MEDIUM:
			strcpy(s,gstr195);
		break;
		case AWG9_SRCH_NARROW:
			strcpy(s,gstr196);
		break;
		case AWG9_SRCH_AUTHENTIC:
			strcpy(s,gstr197);
		break;


		// RWS
		case AWG9_RWS_WIDE:
			strcpy(s,gstr198);
		break;
		case AWG9_RWS_MEDIUM:
			strcpy(s,gstr199);
		break;
		case AWG9_RWS_NARROW:
			strcpy(s,gstr200);
		break;
		case AWG9_RWS_AUTHENTIC:
			strcpy(s,gstr201);
		break;

		// TWSA
		case AWG9_TWSA_MEDIUM:
			strcpy(s,gstr202);
		break;
		case AWG9_TWSA_NARROW:
			strcpy(s,gstr203);
		break;
		case AWG9_TWSA_AUTHENTIC:
			strcpy(s,gstr204);
		break;

		// TWSM
		case AWG9_TWSM_MEDIUM:
			strcpy(s,gstr205);
		break;
		case AWG9_TWSM_NARROW:
			strcpy(s,gstr206);
		break;
		case AWG9_TWSM_AUTHENTIC:
			strcpy(s,gstr207);
		break;

		// Easy Mode
		case AWG9_STANDARD_SRCH:
			strcpy(s,gstr208);
		break;
		case AWG9_STANDARD_TRACK:
			strcpy(s,gstr209);
		break;

		// PD Single Target Track
		case AWG9_PDSTT:
			strcpy(s,gstr210);
		break;

		// Special Modes
		case AWG9_BORESIGHT:
			strcpy(s,gstr211);
		break;
		case AWG9_VSL:
			strcpy(s,gstr212);
		break;
		default:
			strcpy(s,gstr213);
		break;
	}

	l = pstrlen(Rp2,s);

	if (View==FRONT)
		offset=171;
	else
		offset=0;

	RectFill(Rp2,131,(5+offset),57,6,37);
	RpPrint(Rp2,(159-(l/2)),(5+offset),s);
	RectCopy(Rp2,131,(5+offset),57,6,Rp1,131,(5+offset));
}

//***************************************************************************
int GDisplay()
{
	char txt[10], s[10];
	int	N;
	int	m;
	extern char gstr214[];
	extern char gstr215[];

	m = View ? 0x70 : 0;
	SetFont(Rp2,F_3x5B);
	Rp2->APen=(LightsOn?54:2);

	RectFill(Rp2,267,140-m,20,5,63);
	N=abs(Gees+1);
	txt[0]=0;
	if (Gees<-1) strcpy(txt,gstr214);
	strcat(txt, itoa(N/16,s,10));
	strcat (txt, gstr215);
	strcat(txt, itoa((10*(N&15))/16,s,10));
	RpPrint(Rp2,269,140-m,txt);
	RectCopy(Rp2,267,140-m,20,5,Rp1,267,140-m);
}



//***************************************************************************
int DisplayTIDSwitches()
{
	if (DesignateMode) {
		ChangeColor(Rp1,134,168,12,7,36,17);
	} else  {
		ChangeColor(Rp1,134,168,12,7,17,36);
	}

	if (ClrTarg) {
		ChangeColor(Rp1,148,168,12,7,36,17);
		ClrTarg--;
		if (ClrTarg<0) ClrTarg=0;
	} else {
		ChangeColor(Rp1,148,168,12,7,17,36);
	}

	if (HighlightTarg) {
		ChangeColor(Rp1,162,168,12,7,36,17);
	} else {
		ChangeColor(Rp1,162,168,12,7,17,36);
	}

	if (DataLink) {
		ChangeColor(Rp1,176,168,12,7,36,17);
	} else {
		ChangeColor(Rp1,176,168,12,7,17,36);
	}

}

//***************************************************************************
DisplayLeftStuff()
{
	int mseg;
	static int prevstat=-1;
	int stat;

	MouseHIDE();

	stat = !(Status & WHEELSUP);

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		TransRectCopy(mseg,155,137,161,57,Rp2,79,105,0,0);

		if (stat)
			TransRectCopy(mseg,206,89,53,44,Rp2,245,92,0,0);
		else
			TransRectCopy(mseg,263,89,53,44,Rp2,245,92,0,0);

		RectCopy(Rp2,245,92,53,44,Rp1,245,92);

		if (THRUST != -1) {
   			if ((THRUST <= 25))
				TransRectCopy(mseg,117,37,40,41,Rp2,82,118,0,0);
			if (((THRUST > 25) && (THRUST < 50)) || (THRUST == 50))
				TransRectCopy(mseg,159,37,34,46,Rp2,118,108,0,0);
			if (((THRUST > 50) && (THRUST < 75)) || (THRUST == 75))
				TransRectCopy(mseg,195,37,37,45,Rp2,150,107,0,0);
			if ((THRUST > 75) && (THRUST < 100))
				TransRectCopy(mseg,234,37,40,46,Rp2,177,108,0,0);
	   		if (THRUST >= 100)
				TransRectCopy(mseg,276,37,41,40,Rp2,197,112,0,0);
		}
		RectCopy(Rp2,79,105,161,57,Rp1,79,105);
	}

	prevstat = stat;

	if (ShowMouse) MouseShow();
}


//***************************************************************************
DisplayRightStuff(int arg)
{
	int mseg;
	int xoff, yoff;

	MouseHIDE();

	if (arg) {
		xoff = yoff = 0;
	}
	else {
		yoff = -10;
		xoff = 1;
	}

	if (arg) {
		if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
			if (HookDown)
				TransRectCopy(mseg,140,1,38,28,Rp2,1,79,0,0);
			else
				TransRectCopy(mseg,99,1,38,28,Rp2,1,79,0,0);
			RectCopy(Rp2,1,79,38,28,Rp1,1,79);
		}
	}

	// Display Damaged Stuff

	if (Damaged&D_GEAR) {
		ChangeColor(Rp1,84+yoff,172+xoff,28,72,2,16);
		ChangeColor(Rp1,84+yoff,172+xoff,28,72,4,17);
	}

	if (Damaged&D_HOOK) {
		ChangeColor(Rp1,99+yoff,172+xoff,25,20,3,62);
		ChangeColor(Rp1,99+yoff,172+xoff,25,20,5,63);
	}

	if (BingoFuelWarn) {
		ChangeColor(Rp1,113+yoff,168+xoff,26,24,2,16);
		ChangeColor(Rp1,113+yoff,168+xoff,26,24,4,17);
	}

	if (Damaged&D_HYDRAULICS) {
		ChangeColor(Rp1,143+yoff,157+xoff,23,35,2,16);
		ChangeColor(Rp1,143+yoff,157+xoff,23,35,4,17);
	}

	if (Damaged&D_LEFTENGINE) {
		ChangeColor(Rp1,158+yoff,157+xoff,18,35,3,62);
		ChangeColor(Rp1,158+yoff,157+xoff,18,35,5,63);
	}

	if (LLowFuelWarn) {
		ChangeColor(Rp1,173+yoff,156+xoff,13,36,2,16);
		ChangeColor(Rp1,173+yoff,156+xoff,13,36,4,17);
	}

	if (RLowFuelWarn) {
		ChangeColor(Rp1,188+yoff,156+xoff,13,36,3,62);
		ChangeColor(Rp1,188+yoff,156+xoff,13,36,5,63);
	}


	if (Damaged&D_RIGHTENGINE) {
		ChangeColor(Rp1,199+yoff,157+xoff,17,35,2,16);
		ChangeColor(Rp1,199+yoff,157+xoff,17,35,4,17);
	}

//	if (Damaged&D_WINGSWEEP) {
//		ChangeColor(Rp1,211+xoff,166+yoff,35,26,3,62);
//		ChangeColor(Rp1,211+xoff,166+yoff,35,26,5,63);
//	}

	if (Damaged&D_AWG9) {
		ChangeColor(Rp1,247+yoff,166+xoff,29,26,3,62);
		ChangeColor(Rp1,247+yoff,166+xoff,29,26,5,63);
	}

	if (Damaged&D_FIRECONTROL) {
		ChangeColor(Rp1,262+yoff,166+xoff,30,26,2,16);
		ChangeColor(Rp1,262+yoff,166+xoff,30,26,4,17);
		ChangeColor(Rp1,256+yoff,166+xoff,6,7,2,16);
		ChangeColor(Rp1,256+yoff,166+xoff,6,7,4,17);
	}

	if (Damaged&D_NAV) {
		ChangeColor(Rp1,277+yoff,174+xoff,29,18,3,62);
		ChangeColor(Rp1,277+yoff,174+xoff,29,18,5,63);
	}

	if (ShowMouse) MouseShow();
}

//***************************************************************************
int DrawRadarElev()
{
	int beampit;
	RectFill(Rp2,86,106,16,58,33);

	beampit=BEAM_PIT;
	if (beampit>9100) beampit=9100;
	if (beampit<-8736) beampit=-8736;

	if (LightsOn)
		DrawLine(Rp2,88,133-((beampit/182)/2),100,133-((beampit/182)/2),54);
	else
		DrawLine(Rp2,88,133-((beampit/182)/2),100,133-((beampit/182)/2),16);
}


//***************************************************************************
int DrawRadarAlt()
{
	int newalt;
	int soffset;
	int x2,y2;
	int mseg;
	unsigned t,b;

	if (View==FRONT)
		soffset=0;
	else
		soffset=112;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		TransRectCopy(mseg,254,2,35,29,Rp2,9,(162-soffset),0,0);
	}
   newalt= (RadarAlt>3000)? 3000:RadarAlt;
   if (newalt<500)
      {
      t=MDiv(newalt,8192,500);
      }
   else
      {
      if (newalt<1000)
         {
         t= MDiv((newalt-500),4096,500)+8192;
         }
      else
         {
         t= MDiv((newalt-1000),4096,4000)+8192+4096;
         }
      }

   t=(t<<2);
	Rotate2D(0,-10,t,&x2,&y2);

	DrawLine(Rp2,26,(176-soffset),26-x2,(176+scaley(y2)-soffset),(LightsOn?54:2));
	RectCopy(Rp2,10,(163-soffset),32,26,Rp1,10,(163-soffset));
}

//***************************************************************************
int DrawAirSpeedGauge()
{
	char s[10];
	int soffset;

	if (View==FRONT)
		soffset=0;
	else
		soffset=110;

	SetFont(Rp2,F_3x5B);
	Rp2->APen=(LightsOn?54:2);
	RectFill(Rp2,70,144-soffset,11,5,(LightsOn?22:0));
	itoa((TAStoIAS(Knots)),s,10);
	RpPrint(Rp2,70,144-soffset,s);
	RectCopy(Rp2,70,144-soffset,11,5,Rp1,70,144-soffset);
}

//***************************************************************************
int DrawIndexer()
{
	unsigned int pos;
	int c;

	pos = -1;
	if (!(Status & WHEELSUP)) {
		switch(MeatBall) {
			case 0:
				pos=2;
			break;
			case 1:
				pos=2;
			break;
			case 2:
				pos=1;
			break;
			case 3:
				pos=0;
			break;
			case 4:
				pos=0;
			break;
		}
	}
	c = (pos==2) ? 75 : 42;
	RectFill(Rp1,77,72,81,72,c);
	RectFill(Rp1,78,73,80,73,c);
	DrawDot(Rp1,79,74,c);
	c = (pos==1) ? 84 : 42;
	RectFill(Rp1,80,78,82,78,c);
	RectFill(Rp1,80,79,82,79,c);
	c = pos ? 42 : 17;
	DrawDot(Rp1,83,83,c);
	RectFill(Rp1,82,84,84,84,c);
	RectFill(Rp1,81,85,85,85,c);
}


//***************************************************************************
int DrawWayPoint()
{
	int mseg;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		switch(NextWayPoint) {
			case 0:
				TransRectCopy(mseg,68,136,20,18,Rp2,221,46,0,0);
			break;
			case 1:
				TransRectCopy(mseg,1,136,20,18,Rp2,222,46,0,0);
			break;
			case 2:
				TransRectCopy(mseg,23,136,20,18,Rp2,221,46,0,0);
			break;
			case 3:
				TransRectCopy(mseg,45,136,21,18,Rp2,221,46,0,0);
			break;
		}
		RectCopy(Rp2,222,46,20,18,Rp1,222,46);
	}
}
//***************************************************************************
int DrawRadarDials()
{
	static int NumBars=0;
	static int SweepSize=0;
	int mseg;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {

	switch(SELECTED_BAR_NUM) {
		case BAR_1:
			TransRectCopy(mseg,64,175,20,14,Rp2,113,169,0,0);
			NumBars=0;
		break;
		case BAR_2:
			TransRectCopy(mseg,43,175,20,14,Rp2,113,169,0,0);
			NumBars=1;
		break;
		case BAR_4:
			TransRectCopy(mseg,22,175,20,14,Rp2,113,169,0,0);
			NumBars=2;
		break;
		case BAR_8:
			TransRectCopy(mseg,1,175,20,14,Rp2,113,169,0,0);
			NumBars=3;
		break;

	}

	RectCopy(Rp2,113,169,20,14,Rp1,113,169);

	switch(SELECTED_AZ) {
		case DEG65:
			TransRectCopy(mseg,1,158,22,16,Rp2,95,142,0,0);
			SweepSize=3;
		break;
		case DEG40:
			TransRectCopy(mseg,24,158,22,16,Rp2,95,142,0,0);
			SweepSize=2;
		break;
		case DEG20:
			TransRectCopy(mseg,47,158,22,16,Rp2,95,142,0,0);
			SweepSize=1;
		break;
		case DEG10:
			TransRectCopy(mseg,70,158,22,16,Rp2,95,142,0,0);
			SweepSize=0;
		break;
	}
	RectCopy(Rp2,95,142,22,16,Rp1,95,142);
	}
}

//***************************************************************************
TIDShowRange()
{
	static RANGES[] = { 200,100,50,25,10 };
	char   s[10];
	int    l;
	int mseg;
	int d;

	d = MsgOnScreen ? 7 : 0;
	RectFill(Rp2,146,21,29,9,BLACK);
	l = pstrlen(Rp2,itoa(RANGES[Tscale],s,10));
	RpPrint(Rp2,160-(l/2),23,s);
	RectCopy(Rp2,146,21,29,9,Rp1,146,21);

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		switch(Tscale) {
			case 0:
				TransRectCopy(mseg,104,90,24,17,Rp2,128,182,0,0);
			break;
			case 1:
				TransRectCopy(mseg,78,90,24,17,Rp2,128,182,0,0);
			break;
			case 2:
				TransRectCopy(mseg,52,90,24,17,Rp2,128,182,0,0);
			break;
			case 3:
				TransRectCopy(mseg,26,90,24,17,Rp2,128,182,0,0);
			break;
			case 4:
				TransRectCopy(mseg,1,90,23,17,Rp2,129,182,0,0);
			break;
		}
		RectCopy(Rp2,128,182,24,17-d,Rp1,128,182);
	}
}


//***************************************************************************
int DrawRadarRange()
{
	char s[10];
	int temprange=0;
	int l=0;

	RectFill(Rp2,150,92,23,9,BLACK);
	switch(RadarRange) {
		case 0:
			temprange=5;
		break;
		case 1:
			temprange=10;
		break;
		case 2:
			temprange=20;
		break;
		case 3:
			temprange=50;
		break;
		case 4:
			temprange=100;
		break;
		case 5:
			temprange=200;
		break;
	}
	itoa(temprange,s,10);
	l = pstrlen(Rp2,s);
	RpPrint(Rp2,(162-(l/2)),94,s);
	RectCopy(Rp2,150,92,23,9,Rp1,150,92);
}

//***************************************************************************
int DDDSwitchRefresh()
{
	extern char gstr216[];
	extern char gstr217[];
	extern char gstr218[];
	extern char gstr219[];
	extern char gstr220[];
	extern char gstr221[];
	MouseHIDE();

	if (AWG9_SNIFF) {
		ChangeColor(Rp1,227,112,14,5,16,34);
	}
	if (!(AWG9_SNIFF)) {
		ChangeColor(Rp1,227,112,14,5,34,16);
	}

	switch(RadarRange) {
		case 0:
			ChangeColor(Rp1,113,77,11,10,34,16);
			RpPrint(Rp2,150,92,gstr216);
		break;
		case 1:
			ChangeColor(Rp1,130,77,11,10,34,16);
			RpPrint(Rp1,150,92,gstr217);
		break;
		case 2:
			ChangeColor(Rp1,147,77,11,10,34,16);
			RpPrint(Rp1,150,92,gstr218);
		break;
		case 3:
			ChangeColor(Rp1,164,77,11,10,34,16);
			RpPrint(Rp1,150,92,gstr219);
		break;
		case 4:
			ChangeColor(Rp1,181,77,11,10,34,16);
			RpPrint(Rp1,150,92,gstr220);
		break;
		case 5:
			ChangeColor(Rp1,198,77,12,10,34,16);
			RpPrint(Rp1,150,92,gstr221);
		break;
	}

	switch(AWG9_MODE) {
		case AWG9_TWSA:
		case AWG9_TWSA_MEDIUM:
		case AWG9_TWSA_NARROW:
		case AWG9_TWSA_AUTHENTIC:
			ChangeColor(Rp1,225,147,17,6,34,16);
		break;
		case AWG9_TWSM:
		case AWG9_TWSM_MEDIUM:
		case AWG9_TWSM_NARROW:
		case AWG9_TWSM_AUTHENTIC:
			ChangeColor(Rp1,243,147,17,6,34,16);
		break;
		case AWG9_SRCH_WIDE:
		case AWG9_SRCH_MEDIUM:
		case AWG9_SRCH_NARROW:
		case AWG9_SRCH_AUTHENTIC:
			ChangeColor(Rp1,243,133,17,6,34,16);
		break;
		case AWG9_RWS_WIDE:
		case AWG9_RWS_MEDIUM:
		case AWG9_RWS_NARROW:
		case AWG9_RWS_AUTHENTIC:
			ChangeColor(Rp1,243,157,16,6,34,16);
		break;
		case AWG9_PDSTT:
			ChangeColor(Rp1,225,133,16,6,34,16);
		break;
	}
	if (ShowMouse) MouseShow();
}

//***************************************************************************
int DDDSwitches()
{
	extern char gstr216[];
	extern char gstr217[];
	extern char gstr218[];
	extern char gstr219[];
	extern char gstr220[];
	extern char gstr221[];
	SetFont(Rp2,F_3x5B);
	Rp2->APen=1;
	Rp2->BPen=1;


	MouseHIDE();
	if (AWG9_SNIFF) {
		ChangeColor(Rp1,227,122,14,5,16,34);
	}
	if (!(AWG9_SNIFF)) {
		ChangeColor(Rp1,227,122,14,5,34,16);
	}


	if (PrevRadar!=RadarRange) {
		switch(RadarRange) {
			case 0:
				ChangeColor(Rp1,113,77,11,10,34,16);
				RpPrint(Rp2,150,92,gstr216);
			break;
			case 1:
				ChangeColor(Rp1,130,77,11,10,34,16);
				RpPrint(Rp1,150,92,gstr217);
			break;
			case 2:
				ChangeColor(Rp1,147,77,11,10,34,16);
				RpPrint(Rp1,150,92,gstr218);
			break;
			case 3:
				ChangeColor(Rp1,164,77,11,10,34,16);
				RpPrint(Rp1,150,92,gstr219);
			break;
			case 4:
				ChangeColor(Rp1,181,77,11,10,34,16);
				RpPrint(Rp1,150,92,gstr220);
			break;
			case 5:
				ChangeColor(Rp1,198,77,12,10,34,16);
				RpPrint(Rp1,150,92,gstr221);
			break;
			RectCopy(Rp2,150,92,23,9,Rp1,160,92);
		}
		switch(PrevRadar) {
			case 0:
				ChangeColor(Rp1,113,77,11,10,16,34);
			break;
			case 1:
				ChangeColor(Rp1,130,77,11,10,16,34);
			break;
			case 2:
				ChangeColor(Rp1,147,77,11,10,16,34);
			break;
			case 3:
				ChangeColor(Rp1,164,77,11,10,16,34);
			break;
			case 4:
				ChangeColor(Rp1,181,77,11,10,16,34);
			break;
			case 5:
				ChangeColor(Rp1,198,77,12,10,16,34);
			break;
		}
		PrevRadar=RadarRange;
	}

	if (RadarMode!=AWG9_MODE) {
		switch(AWG9_MODE) {
			case AWG9_TWSA:
			case AWG9_TWSA_MEDIUM:
			case AWG9_TWSA_NARROW:
			case AWG9_TWSA_AUTHENTIC:
				if ((RadarMode == AWG9_TWSA_NARROW)	|| (RadarMode == AWG9_TWSA_MEDIUM)) break;
				ChangeColor(Rp1,225,147,17,6,34,16);
			break;
			case AWG9_TWSM:
			case AWG9_TWSM_MEDIUM:
			case AWG9_TWSM_NARROW:
			case AWG9_TWSM_AUTHENTIC:
				if ((RadarMode == AWG9_TWSM_NARROW)	|| (RadarMode == AWG9_TWSM_MEDIUM)) break;
				ChangeColor(Rp1,243,147,17,6,34,16);
			break;
			case AWG9_SRCH_WIDE:
			case AWG9_SRCH_MEDIUM:
			case AWG9_SRCH_NARROW:
			case AWG9_SRCH_AUTHENTIC:
				if ((RadarMode == AWG9_SRCH_WIDE) || (RadarMode == AWG9_SRCH_NARROW)
					|| (RadarMode == AWG9_SRCH_MEDIUM)) break;
				ChangeColor(Rp1,243,133,17,6,34,16);
			break;
			case AWG9_RWS_WIDE:
			case AWG9_RWS_MEDIUM:
			case AWG9_RWS_NARROW:
			case AWG9_RWS_AUTHENTIC:
				if ((RadarMode == AWG9_RWS_WIDE) || (RadarMode == AWG9_RWS_NARROW)
					|| (RadarMode == AWG9_RWS_MEDIUM)) break;
				ChangeColor(Rp1,243,157,16,6,34,16);
			break;
			case AWG9_PDSTT:
				ChangeColor(Rp1,225,133,16,6,34,16);
			break;
		}

		switch(RadarMode) {
			case AWG9_TWSA:
			case AWG9_TWSA_MEDIUM:
			case AWG9_TWSA_NARROW:
			case AWG9_TWSA_AUTHENTIC:
				if ((AWG9_MODE == AWG9_TWSA_NARROW)	|| (AWG9_MODE == AWG9_TWSA_MEDIUM)) break;
				ChangeColor(Rp1,225,147,17,6,16,34);
			break;
			case AWG9_TWSM:
			case AWG9_TWSM_MEDIUM:
			case AWG9_TWSM_NARROW:
			case AWG9_TWSM_AUTHENTIC:
				if ((AWG9_MODE == AWG9_TWSM_NARROW)	|| (AWG9_MODE == AWG9_TWSM_MEDIUM)) break;
				ChangeColor(Rp1,243,147,17,6,16,34);
			break;
			case AWG9_SRCH_WIDE:
			case AWG9_SRCH_MEDIUM:
			case AWG9_SRCH_NARROW:
			case AWG9_SRCH_AUTHENTIC:
				if ((AWG9_MODE == AWG9_SRCH_WIDE) || (AWG9_MODE == AWG9_SRCH_NARROW)
					|| (AWG9_MODE == AWG9_SRCH_MEDIUM)) break;
				ChangeColor(Rp1,243,133,17,6,16,34);
			break;
			case AWG9_RWS_WIDE:
			case AWG9_RWS_MEDIUM:
			case AWG9_RWS_NARROW:
			case AWG9_RWS_AUTHENTIC:
				if ((AWG9_MODE == AWG9_RWS_WIDE) || (AWG9_MODE == AWG9_RWS_NARROW)
					|| (AWG9_MODE == AWG9_RWS_MEDIUM)) break;
				ChangeColor(Rp1,243,157,16,6,16,34);
			break;
			case AWG9_PDSTT:
				ChangeColor(Rp1,225,133,16,6,16,34);
			break;
		}
		RadarMode=AWG9_MODE;
	}

	if (ShowMouse) MouseShow();
}


//***************************************************************************
int DrawCompass()
{
	int x,y;
	int soffset;
	int mseg;

	if (View==FRONT)
		soffset=0;
	else
	  	soffset=112;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		TransRectCopy(mseg,218,2,32,35,Rp2,219,(166-soffset),0,0);
	}

	Rotate2D(0,10,-OurHead,&x,&y);
	DrawLine(Rp2,235,178-soffset,235-x,178-scaley(y)-soffset,(LightsOn?54:2));

	RectCopy(Rp2,219,166-soffset,32,27,Rp1,219,166-soffset);
}

//***************************************************************************
int DrawVVGauge()
{
	int vv;
	int x,y;
	int soffset;
	int mseg;
	int tv;

	if (View==FRONT)
		soffset=0;
	else
		soffset=112;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		TransRectCopy(mseg,189,2,25,22,Rp2,26,(136-soffset),0,0);
	}

	tv=VVI*6;              // = ft/min /10
	if (tv>400) tv=400;
	if (tv<-400) tv=-400;
	tv= tv*65;             // 2500 is about 90 degrees on dial
	x=-ACosB(8,tv);
	y=ASinB(8,tv);

	DrawLine(Rp2,38,147-soffset,38+x,(147-scaley(y)-soffset),(LightsOn?54:2));
	RectCopy(Rp2,26,137-soffset,23,19,Rp1,26,137-soffset);
}

//***************************************************************************
int DrawAltGauge()
{
	int font=0;
	unsigned int alt;
	int x,y;
	unsigned int althund;
	int newalt;
	char s[10];
	int soffset;
	int mseg;


	if (View==FRONT)
		soffset=0;
	else
		soffset=110;

	GetFont(Rp2,font);
	SetFont(Rp2,F_3x5B);
	Rp2->APen=(LightsOn?54:2);
	alt = OurAlt;
	althund = OurAlt%1000;

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
		TransRectCopy(mseg,1,35,33,29,Rp2,55,(164-soffset),0,0);
	}

	itoa((alt/1000),s,10);
	RectFill(Rp2,59,(175-soffset),10,6,22);
	RpPrint(Rp2,61,(175-soffset),s);
	Rotate2D(0,-10,althund*65,&x,&y);
	DrawLine(Rp2,71,(177-soffset),70-x,(177+scaley(y)-soffset),(LightsOn?54:2));

	if (!MsgOnScreen)
		RectCopy(Rp2,52,(162-soffset),39,28,Rp1,52,(162-soffset));
	else
		RectCopy(Rp2,52,(162-soffset),39,28,Rp1,52,(162-soffset));

	SetFont(Rp2,font);
}

//***************************************************************************
int DrawStores()
{
	int i,j;
	int status;
	int ypos;
	int prior;
	int storelight[8][2] = {
		96, 127,	/* 1 */
		107,127,	/* 3 */
		118,127,	/* 7 */
		129,127,	/*  9 - 11 */
		185,127,    /* 10 - 12 */
		196,127,	/* 8 */
		207,127,	/* 4 */
		218,127,	/* 2 */
	};

	prior = AAGetStation();

	if (View==FRONT)
		ypos=127;
	else
		ypos=16;

	for (i=0; i<12; i++) {
		if (i==4) continue;
		if (i==5) continue;

		if (F14_STATION[i]==-1) continue;

		/* Currently the only stores status is filled/empty - need */
		/* currently designated */
		if (F14_STATION[i]!=0) {
			if (LightsOn)
				status=2;
			else
				status=2;
		} else
			status=5;

		if (i==prior) status=16;

		if (i==0) j=0;
		if (i==1) j=7;

		if (i==2) j=1;
		if (i==3) j=6;

		if (i==6) j=2;
		if (i==7) j=5;


		if (i==8) j=4;
		if (i==9) j=3;

		if (i==10) j=4;
		if (i==11) j=3;

		if (status==16) {
			RectFill(Rp2,storelight[j][0],ypos,6,3,BLACK);
			DrawDot(Rp2,storelight[j][0]+1,ypos,WHITE);
			DrawDot(Rp2,storelight[j][0]+3,ypos,WHITE);
			DrawDot(Rp2,storelight[j][0]+5,ypos,WHITE);
			DrawDot(Rp2,storelight[j][0]+0,ypos+1,WHITE);
			DrawDot(Rp2,storelight[j][0]+2,ypos+1,WHITE);
			DrawDot(Rp2,storelight[j][0]+4,ypos+1,WHITE);
			DrawDot(Rp2,storelight[j][0]+1,ypos+2,WHITE);
			DrawDot(Rp2,storelight[j][0]+3,ypos+2,WHITE);
			DrawDot(Rp2,storelight[j][0]+5,ypos+2,WHITE);
		} else {
			RectFill(Rp2,storelight[j][0],ypos,6,3,status);
		}
		RectCopy(Rp2,storelight[j][0],ypos,6,2,Rp1,storelight[j][0],ypos);
	}
}


//***************************************************************************
DrawAOADisplay()
{
	int inewaoa;

	RectFill(Rp2,83,109,3,17,29);
	inewaoa = NEWAOA/91;
	if (inewaoa > 30) inewaoa=30;
	if (inewaoa < 2) inewaoa=2;
	RectFill(Rp2,83,125 - inewaoa/2,3,inewaoa/2,2);
	DrawLine(Rp2,82,110,84,110,34);
	DrawLine(Rp2,82,115,84,115,34);
	DrawLine(Rp2,82,120,84,120,34);
 	DrawLine(Rp2,82,125,84,125,34);
	RectCopy(Rp2,83,109,3,17,Rp1,83,109);
}


//***************************************************************************
DrawSweepDisplay()
{
	extern int swprevrot;
	extern int swtimer;
#define prevrot swprevrot
#define timer swtimer
	int pos;

	if (--timer > 0) return 1;
	timer = 20;

	if (WingRot<=(30*DEG))
   		pos=0;
	if ((WingRot>(30*DEG)) && (WingRot<(40*DEG)))
		pos=1;
	if ((WingRot>=(40*DEG)) && (WingRot<=(50*DEG)))
		pos=2;
	if ((WingRot>(50*DEG)) && (WingRot<(60*DEG)))
		pos=3;
	if (WingRot>=(60*DEG))
		pos=4;

	prevrot=WingRot;
	RectFill(Rp2,236,106,1,21,23);
	DrawDot(Rp2,236,109,57);
	DrawDot(Rp2,236,114,57);
	DrawDot(Rp2,236,119,57);
	DrawDot(Rp2,236,124,57);
	RectCopy(Rp2,236,106,1,21,Rp1,236,106);

	switch(pos) {
		case 0:
			RectFill(Rp2,236,106,236,108,54);
		break;
		case 1:
			RectFill(Rp2,236,110,236,113,54);
		break;
		case 2:
			RectFill(Rp2,236,115,236,118,54);
		break;
		case 3:
			RectFill(Rp2,236,120,236,123,54);
		break;
		case 4:
			RectFill(Rp2,236,125,236,127,54);
		break;
	}
	RectCopy(Rp2,236,106,1,21,Rp1,236,106);
}

//***************************************************************************
int DrawEngineStat()
{
	int Lrpm,Rrpm;
	int Lff,Rff;

	(int)Lrpm = (ThrustI>144 ? 144: ThrustI);
	(int)Rrpm = (ThrustI>144 ? 144: ThrustI);

	Lff = (THRUST/4)-6;
	Rff = (THRUST/4)-6;

	if (Lrpm<0) Lrpm=0;
	if (Rrpm<0) Rrpm=0;
	if (Lff<0) Lff=0;
	if (Rff<0) Rff=0;

	if ((Lrpm>22) && (EgtL<15)) EgtL++;
	if ((Rrpm>22) && (EgtR<15)) EgtR++;
	if ((Lrpm>102) && (EgtL<22)) EgtL++;
	if ((Rrpm>102) && (EgtR<22)) EgtR++;
	if ((Lff<20) && (EgtL>15)) EgtL--;
	if ((Rff<20) && (EgtR>15)) EgtR--;

	if (Lff<3 && EgtL>5) EgtL--;
	if (Rff<3 && EgtR>5) EgtR--;

	Lrpm=(Lrpm/4)-5;
	Rrpm=(Rrpm/4)-5;


	RectFill(Rp2,51,103,3,31,BLACK);
	RectFill(Rp2,59,103,3,31,BLACK);

	RectFill(Rp2,71,103,3,31,BLACK);
	RectFill(Rp2,79,103,3,31,BLACK);

	RectFill(Rp2,91,103,3,31,BLACK);
	RectFill(Rp2,99,103,3,31,BLACK);

	RectFill(Rp2,51,135-Lrpm,3,Lrpm,(LightsOn?54:2));
	RectFill(Rp2,59,135-Lrpm,3,Lrpm,(LightsOn?54:2));

	RectCopy(Rp2,51,103,3,31,Rp1,51,103);
	RectCopy(Rp2,59,103,3,31,Rp1,59,103);

	RectFill(Rp2,71,135-EgtL,3,EgtL,(LightsOn?54:2));
	RectFill(Rp2,79,135-EgtR,3,EgtR,(LightsOn?54:2));

	RectCopy(Rp2,71,103,3,31,Rp1,71,103);
	RectCopy(Rp2,79,103,3,31,Rp1,79,103);

	RectFill(Rp2,91,135-Lff,3,Lff,(LightsOn?54:2));
	RectFill(Rp2,99,135-Rff,3,Rff,(LightsOn?54:2));

	RectCopy(Rp2,91,103,3,31,Rp1,91,103);
	RectCopy(Rp2,99,103,3,31,Rp1,99,103);
}

//***************************************************************************
DisplayBackLights()
{
	if (CheckLights(SAM_LIGHT))
		ChangeColor(Rp1,290,72,309,77,18,17);
	else
		ChangeColor(Rp1,290,72,309,77,17,18);

	if (CheckLights(AAA_LIGHT))
		ChangeColor(Rp1,290,80,309,85,18,17);
	else
		ChangeColor(Rp1,290,80,309,85,17,18);


	if (CheckLights(AI_LIGHT))
		ChangeColor(Rp1,290,88,309,93,18,17);
	else
		ChangeColor(Rp1,290,88,309,93,17,18);

}


//***************************************************************************
DisplayLights(int lightnumber,int mode)
{
	if (mode) {
		ChangeColor(Rp1,CockpitLights[lightnumber].x,
				CockpitLights[lightnumber].y,
				CockpitLights[lightnumber].l,
				CockpitLights[lightnumber].w,
				CockpitLights[lightnumber].fromcolor,
				CockpitLights[lightnumber].tocolor);
	}
	else {
		ChangeColor(Rp1,CockpitLights[lightnumber].x,
				CockpitLights[lightnumber].y,
				CockpitLights[lightnumber].l,
				CockpitLights[lightnumber].w,
				CockpitLights[lightnumber].tocolor,
				CockpitLights[lightnumber].fromcolor);
	}
}

//***************************************************************************
DoCRT(int i)
{
    int a, d;

    RpCRT = CrtRps[i];
    d = CrtDisplays[i];

    switch (d) {
        case VDI:
			if (VdiMode) {
				a=VdiCrt(i);
			} else {
				a=DrawGunCameraView();
				if (ConfigData.GunCamera==2)
					FlirCopy(VdiMask);
			}
		break;
        case HSD:
			if (RadarRepeat) {
				if (AWG9_REALITY_LEVEL == AWG9_EASY) {
					DoTac(i);
				} else {
					DoTID(i);
				}
			} else {
				a=HsdCrt(i);
				FlirCopy(HsdMask);
			}
		break;
		case DDD:
			a=DoDDD(i);
		break;
		case TID:
			if (AWG9_REALITY_LEVEL==AWG9_EASY) {
				a=DoTAC(i);
			} else {
				a=DoTID(i);
			}
		break;
		case TEWS:
			a=TEWS_GFX();
		break;
		case ADI:
			a=AdiCrt(i);
		break;
        default:
			a=0;
		break;
    }
    CrtUpdate[i] = a;
}


//***************************************************************************
int ZoomDisplay()
{
	if (!BackSeat) {
		if (View==FRONT) {
         	GunCameraZoom(1);
		}
		if (View==DOWN) {
			if (Tscale<4) Tscale++;
		}
	} else {
		if (View==FRONT) {
			AWG9_DEC_RANGE();
		}
		if (View==DOWN) {
			if (Tscale<4) Tscale++;
		}
	}
}

//***************************************************************************
UnZoomDisplay()
{
	if (!BackSeat) {
		if (View==FRONT) {
	        GunCameraZoom(-1);
		}
		if (View==DOWN) {
			if (Tscale>0) --Tscale;
		}
	} else {
		if (View==FRONT) {
			AWG9_INC_RANGE();
		}
		if (View==DOWN) {
			if (Tscale>0) --Tscale;
		}
	}
}

//***************************************************************************
MakeItGreen()
{
	VdiMask[MASKX] = RpCRT->XBgn;
	VdiMask[MASKY] = RpCRT->YBgn;
	FlirCopy(VdiMask);
}

//***************************************************************************
ResetCockpitLights()
{
	int i;

	OLDMM =
	OldFuel =
	OldEMIS =
	OldMC   =
	OldThrust = -1;

	for (i=0; i<40; i++) OldLights[i]=0;
}

//***************************************************************************
UpdateCockpitLites()
{
	UpdateLights();
}


//***************************************************************************
UpdateLights()
{
	int status;
	UWORD mseg;

	/* Check if status has changed */

	status = CheckLights(GEAR_LIGHT);
	if (!(OldLights[GEAR_LIGHT] == status)) {
		DisplayLights(GEAR_LIGHT,status);
		OldLights[GEAR_LIGHT]=status;
	}

	status = CheckLights(BRAKE_LIGHT);
	if (!(OldLights[BRAKE_LIGHT] == status)) {
		DisplayLights(BRAKE_LIGHT,status);
		OldLights[BRAKE_LIGHT]=status;
	}

	status = CheckLights(HOTTRIG_LIGHT);

	if (!(OldLights[HOTTRIG_LIGHT] == status)) {
		DisplayLights(HOTTRIG_LIGHT,status);
		OldLights[HOTTRIG_LIGHT]=status;
		if (status) {
			MouseHIDE();
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,113,149,18,19,Rp2,213,102,0,0);
			}
			RectCopy(Rp2,213,102,18,19,Rp1,213,102);
			if (ShowMouse) MouseShow();
		} else {
			MouseHIDE();
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,134,149,18,19,Rp2,213,102,0,0);
			}
			RectCopy(Rp2,213,102,18,19,Rp1,213,102);
			if (ShowMouse) MouseShow();
		}
	}

	status = CheckLights(SEAMLOCK_LIGHT);
	if (!(OldLights[SEAMLOCK_LIGHT] == status)) {
		DisplayLights(SEAMLOCK_LIGHT,status);
		OldLights[SEAMLOCK_LIGHT]=status;
	}
	status = CheckLights(SAM_LIGHT);
	if (!(OldLights[SAM_LIGHT] == status)) {
		DisplayLights(SAM_LIGHT,status);
		OldLights[SAM_LIGHT]=status;
	}

	status = CheckLights(AI_LIGHT);
	if (!(OldLights[AI_LIGHT] == status)) {
		DisplayLights(AI_LIGHT,status);
		OldLights[AI_LIGHT]=status;
	}

	status = CheckLights(AAA_LIGHT);
	if (!(OldLights[AAA_LIGHT] == status)) {
		DisplayLights(AAA_LIGHT,status);
		OldLights[AAA_LIGHT]=status;
	}
	status = CheckLights(LEFTFIRE_LIGHT);
	if (!(OldLights[LEFTFIRE_LIGHT] == status)) {
		DisplayLights(LEFTFIRE_LIGHT,status);
		OldLights[LEFTFIRE_LIGHT]=status;
	}
	status = CheckLights(RIGHTFIRE_LIGHT);
	if (!(OldLights[RIGHTFIRE_LIGHT] == status)) {
		DisplayLights(RIGHTFIRE_LIGHT,status);
		OldLights[RIGHTFIRE_LIGHT]=status;
	}

	MouseHIDE();
	if (JetisonSW) {
		if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
			TransRectCopy(mseg,111,125,20,20,Rp2,88,102,0,0);
		}
	} else {
		if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
			TransRectCopy(mseg,134,125,20,20,Rp2,88,102,0,0);
		}
	}
	RectCopy(Rp2,88,102,20,20,Rp1,88,102);
	if (ShowMouse) MouseShow();

}

//***************************************************************************
static CheckLights(int i)
{
	static int tempflash=0;

	switch(i) {
		case 0:
			return(0);
		case 1:
			return !(Status & WHEELSUP);
		case 2:
			return ((Status & BRAKESON) && !(Damaged&D_HYDRAULICS));
		case 3:
			return HookDown;
		case 5:
			return MLAUNCH;
		case 6:
			if (LastMud>1) {
				if (tempflash) {
					tempflash ^= 1;
					return(1);
				} else {
					tempflash ^= 1;
					return(0);
				}
			}
			if (LastSpike>1) {
				return(1);
			} else {
				return(0);
			}
		case 7:
			return AIDETECT;
		case 8:
			return WING_HAVETARG;
		case 10:
			return MasterArmSW;
		case 11:
			return (Damaged&D_LEFTENGINE);
		case 19:
			return (Damaged&D_RIGHTENGINE);
	}
}

//***************************************************************************
ShowFuel()
{
	int Lfuel, f, Lfbar, Rfuel, Rfbar;

	f = Fuel;
	Lfuel = f/2;
	Rfuel = f/2;

	if (Lfuel<1) Lfuel=0;
	if (Rfuel<1) Rfuel=0;
	if (f<0) f=0;

    if (f!=OldFuel) {
        OldFuel = f;

		Lfbar = Lfuel/360;
		Rfbar = Rfuel/360;

		if (Lfbar>28) Lfbar=28;
		if (Rfbar>28) Lfbar=28;

		if (Lfbar<1) Lfbar=1;
		if (Rfbar<1) Rfbar=1;

        MouseHIDE ();
        Rp1->FontNr=F_3x5A;
        Rp1->APen=(LightsOn?54:2);

        RectFill (Rp1, 234,96, 20,5, BLACK);
        RpPrint  (Rp1, 234,96, TXTN(f));
		RectFill(Rp1,234,106,20,5,BLACK);
		RpPrint(Rp1,234,106,TXTN(BingoFuel));

		RectFill(Rp1,220,122,15,5,BLACK);
		RpPrint(Rp1,220,122,TXTN(Lfuel));

		RectFill(Rp1,252,122,15,5,BLACK);
		RpPrint(Rp1,252,122,TXTN(Rfuel));

		RectFill(Rp2,238,114,5,27,BLACK);
		RectFill(Rp2,238,141-Lfbar,5,Lfbar,(LightsOn?54:2));
		RectCopy(Rp2,238,114,5,27,Rp1,238,114);

		RectFill(Rp2,244,114,5,27,BLACK);
		RectFill(Rp2,244,141-Rfbar,5,Rfbar,(LightsOn?54:2));
		RectCopy(Rp2,244,114,5,27,Rp1,244,114);

		MouseSHOW();
    }
}


//***************************************************************************
EngineAlreadyOn()
{
}

//***************************************************************************
EngineMonitor()
{
#ifdef YEP
    if (THRUST!=0 && TempLeft<930) { // wind up the numbers at engine startup
        TempLeft += 9;
        TempRight = TempLeft+3;

        if ( OilLeft< 75)  {
            OilLeft += 12;
            OilRight = OilLeft +1;
        }

        if ((QTimer&31)==0) {        // every 8 seconds after warm-up, let them wander a little

            if (OilLeft>=75) {
                OilLeft   =  75+rnd2(5);
                OilRight  =  75+rnd2(4);
            }

            if (TempLeft>=930) {
                TempLeft  = 930+rnd2(21);
                TempRight = 930+rnd2(23);
            }
        }

        OldThrust = -1;
    }
    if (Fuel==0) OilLeft = OilRight = TempLeft = TempRight = 0;
#endif
}

//***************************************************************************
JetisonSwitch(int mode)
{
	UWORD mseg;

	MouseHide();
	if (mode) {
		JetisonSW++;
		if (View==FRONT && (!BackSeat)) {
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,111,125,20,20,Rp2,88,102,0,0);
			}
			RectCopy(Rp2,88,102,20,20,Rp1,88,102);
		}
	}
	else
	{
		JetisonSW=0;
		if (View==FRONT && (!BackSeat)) {
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,134,125,20,20,Rp2,88,102,0,0);
			}
			RectCopy(Rp2,88,102,20,20,Rp1,88,102);
		}
	}
	if (ShowMouse) MouseSHOW();
}

//***************************************************************************
MasterArmSwitch()
{
	UWORD mseg;
	MouseHIDE();

	if (!MasterArmSW) {
		MasterArmSW++;
		if (View==FRONT) {
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,113,149,18,19,Rp2,213,102,0,0);
			}
			RectCopy(Rp2,213,102,18,19,Rp1,213,102);
			ChangeColor(Rp1,119,120,11,3,18,16);
		}
	}
	else
	{
		MasterArmSW=0;
		if (View==FRONT) {
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,0))) {
				TransRectCopy(mseg,134,149,18,19,Rp2,213,102,0,0);
			}
			RectCopy(Rp2,213,102,18,19,Rp1,213,102);
			ChangeColor(Rp1,119,120,11,3,16,18);
		}
	}
	if (ShowMouse) MouseSHOW();
}


