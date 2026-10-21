
/************************************************************************
*                                                                       *
*       Project: Stealth Fighter(TM)                                    *
*                                                                       *
*<t>    Flight Equations                                                *
*                                                                       *
*       Author: Jim Synoski                                             *
*       Written: Jan 1988                                               *
*       Last Editted: Jan 22,1988                                       *
*                                                                       *
*       Copyright (C) 1988 by MicroProse Software, All Rights Reserved. *
*                                                                       *
************************************************************************/

#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include "library.h"
#include "world.h"
#include "sdata.h"
#include "cockpit.h"
#include "sounds.h"
#include "keycodes.h"
#include "f15defs.h"
#include "awg9.h"
#include "speech.h"
#include "setup.h"
#include "planeai.h"
#include "setup.h"

extern  SWORD   Isin(),Icos();
extern  UBYTE   Rate;
extern  UBYTE   BSPEED;
extern  SBYTE   TRIMAOACHG;
extern  UWORD     Rtime;
extern  int     atarg,gtarg,vScale;
extern  char    Txt[];
extern  int     PovHead,PovPitch,PovRoll,RPov,PovMisl,NextWayPoint;
extern  int     MinAlt,CloseBase;
extern  int     AutoGearOk,AutoLand;
extern  int     Speedy;
extern  UWORD   QTimer;
extern	int	    Fticks;
extern  int     Joystick;
extern  int     KbSense;
extern  UWORD   RealLandings;
extern  int     Firing;
extern  UBYTE   mS1n,mS2n;
extern  int     LastRate;
extern  int		LLowFuelWarn;
extern  int		RLowFuelWarn;
extern  int		BingoFuelWarn;
extern  int MikeHeight;

extern union REGS D;


    extern UBYTE ReqdGees[128];


    extern long PxLng;

    extern long PyLng;

    extern COORD Px;
extern COORD Py;

    extern UWORD AirSpeed;

    extern SWORD Weight;

    extern SWORD FM[3][3];

    extern SWORD HC[3][3];

    extern SWORD PC[3][3];

    extern SWORD RC[3][3];

    extern SWORD TM[3][3];

    extern UWORD OurHead;

    extern SWORD OurPitch;
extern SWORD OurRoll;

    extern SWORD Gees;

    extern UWORD Alt;
extern UWORD OurAlt;
extern UWORD OldAlt;

    extern long RadarAlt;

    extern UBYTE TrimAOA;

    extern ULONG FrameCnt;

    SWORD   TrgMul(),IntDiv(),Itan();
    UWORD   Iasin(),Iacos();
    extern SWORD Accel;
extern SWORD VVI;

    extern SBYTE PrevRoll;
extern SBYTE PrevAOA;
extern SBYTE AOARepCnt;

    extern SBYTE PrevROLLRATE;
extern SBYTE RollRepCnt;

    extern SWORD ThrustI;
    extern int OldThrust;
extern SWORD PitchAngle;
extern SWORD MinSpd;
extern SWORD MinKnots;
extern SWORD ROLLRATE;
extern SWORD AOARATE;
extern SWORD Knots;

    extern UWORD AOAttack;

    int     vHead,vPitch,vRoll, VM[3][3];
    extern char DoPitchLines;

    extern int ejectX;
extern int ejectY;
extern int ejectZ;

    extern int REALFLIGHT;

    extern char BEND;

    extern int THRUST;

    extern int sightX;
extern int sightY;

    extern int BingoFuel;

    extern long FuelFlow;

    extern int GAMNT;


struct  chase {
    long x,y;
    int  z;
    int head,pitch,roll;
};

#define NCHASE 16
extern struct chase chs[NCHASE];


#define THRUSTtoWEIGHT 1
#define MaxSPEED 800    /* at sea level */
#define MinSPEED 100    /* at sea level */

extern int OurLoadOut;
extern confData ConfigData;
extern int MasterArmSW;
extern int SNDDETAIL;

long PlaneWeight();



//***************************************************************************
InitFlightModel()
{
    int f, d;
	extern char gstr224[];


	InitPlaneOnCarrier(2);

	if (Alt > 20) {			// Air Start
		MasterArmSW=1;
	} else {
		Message(gstr224,GAME_MSG);
		Speech(L_CLRD_TAKEOFF,2);
	}

	/* Convert from 20-bit 3D-World coordinates to 15-bit game coordinates */
    Px = (PxLng+16)>>HTOV;
    Py = 0x8000 - ((PyLng+16)>>HTOV);

    MakeRotationMatrix(FM, OurHead,OurPitch,OurRoll);
	InitScottFlight();
    KbSense=REALFLIGHT+1;

	/* calculate bingo fuel */

    f = (85*85/750)*60/4;   // units of fuel per minute
    d = max (Dist2D(wps[1].x-Px, wps[1].y-Py), Dist2D(wps[2].x-Px, wps[2].y-Py));
    BingoFuel = (PlaneWeight()/10)+1000;
//    BingoFuel = (long)f*d*2/(95*10) + 1000;
    InitOurCoolPlane(OurAlt!=0);
}


//***************************************************************************
#pragma optimize("egl", off)
FLIGHT ()
{
	extern char gstr156[];
	extern char gstr225[];
	extern char gstr183[];

    UWORD   FrameAccel,GndSpeed;
    SWORD   TurnRate,EqV,EqSpeed;
    SWORD   HeadChg,PitchChg,RollChg,sRoll;
    SWORD   z1,z2,z3,z4,z,n,dH,dP,b,dx,dy,angl,wobble,KEY,maxroll;
    char    txt[10];
	int GroundH;


    EngineMonitor ();

    OldAlt = OurAlt;

    ROLLRATE = ((mS1n >>4)-8); if (ROLLRATE<0) ++ROLLRATE;
    AOARATE =  ((mS2n >>4)-8); if (AOARATE <0) ++AOARATE;

    ROLLRATE = - 2 * ROLLRATE * (abs(ROLLRATE)+2);
    AOARATE  *=  6;

    if (Speedy!=1) {
        ROLLRATE /= Speedy;
        AOARATE /= Speedy;
    }

    if (Alt==MinAlt && AOARATE<0 && OurPitch<=0) AOARATE = 0;



	/* AutoPilot logic */
    dx = wps[NextWayPoint].x - Px;
    dy = wps[NextWayPoint].y - Py;
    AutoHead = angle (dx,-dy);

	/* Turbulence */
    wobble = 0;

    if (Damaged&D_HYDRAULICS) wobble = 64;

    if (wobble>0 && Alt>MinAlt) {
		ROLLRATE +=  rnd(wobble)-(wobble>>1);
		AOARATE  += (rnd(wobble)-(wobble>>1)) >>1;
    }

	/* Terrain-Following Radar flight */
    if (Status&WHEELSUP && AOARATE==0 && AirSpeed>MinSpd && abs(OurRoll)<0x3000
    && (!Firing)) {
        int     x, y;
        int     TFAlt;

        Rotate2D (0, 1024, OurHead, &x, &y);
        TFAlt = Alt-GetQHeight(PxLng+x, (0x100000-PyLng)-y);
        if ((RadarAlt>>2)<TFAlt) TFAlt = (RadarAlt>>2);
        TFAlt -= 250/4;
        dP = ((PitchAngle-OurPitch) >>5) - TFAlt;
        if (dP>0) {
            AOARATE = rng (dP*2, 0,32);
        }
    }

	/* AfterBurner Damage */
    if (Damaged&D_AFTERBURNER && THRUST>100) {
        THRUST=100;
    }

	/* Engine Damage */
    z=Damaged&(D_LEFTENGINE|D_RIGHTENGINE);
    if (z) {
        if (z==(D_LEFTENGINE|D_RIGHTENGINE)) {
            if (Damaged&D_HYDRAULICS) {
                ROLLRATE = 64;
                AOARATE = abs(OurRoll)>0x4000 ?16:-8;
            }
            THRUST=0;
        } else {
            if (THRUST>50) THRUST=50;
        }
    }

	// Ejection logic
    if (ejected) {
        GAMNT = 0;      // no red-black out

        ROLLRATE = 64;
        AOARATE = abs(OurRoll)>0x4000 ?16:-8;

        if ((ejected&0xfffc)==16 && (Rtime&3)==1) {
            OurPitch = -0x4000;
        }

//        ejectZ += rng (32-(++ejected), -256/FrameRate, 128/FrameRate);
        ejectZ += rng (32-(++ejected), -64/FrameRate, 64/FrameRate);
        if ((ejectZ<0) || (ejected > (20 * FrameRate))) {
            ejectZ = 0;
            if (!(Gtime&3)) EndGame (EJECTEND);
        }

        if (Alt==0) {
            if (AirSpeed) {
        		StartASmoke( PxLng, Alt, (0x100000-PyLng), GRND2XPLO, 17 );
                Sound (N_LoudExplo, 2,0);
            }
            AirSpeed = THRUST = 0;
        }
        else THRUST = 100;
    }

	/* Instantaneous thrust */
    ThrustI += ((int) THRUST - ThrustI)/4/FrameRate;
    if (THRUST>ThrustI) ++ThrustI;
    if (THRUST<ThrustI) ThrustI=THRUST;

/*
    FuelFlow =  THRUST*THRUST;
    FuelFlow += FuelFlow/5;

    if (REALFLIGHT) {
        if (THRUST==144) FuelFlow=45000;
        FuelFlow -= ( FuelFlow*(long)(OurAlt>45000?45000:OurAlt) ) >>16;
    }

    if (((QTimer&15)==0) && THRUST) {   // every 4 seconds
//        z =  (THRUST*THRUST/750 +2);

        z = (FuelFlow+450)/900; //convert hourly to per 4 seconds

        if (Damaged&D_LFUELSYSTEM) z<<=1;
		if (Damaged&D_RFUELSYSTEM) z<<=1;

        if (THRUST==144) z <<=(1+REALFLIGHT);
        if (Fuel>1500/2 && Fuel-z<=1500/2) {
            Message("Warning, Low Fuel",RIO_MSG);
			LLowFuelWarn=1;
			RLowFuelWarn=1;
        }
        if (Fuel>BingoFuel && Fuel-z<=BingoFuel) {
            Message("Bingo Fuel",RIO_MSG);
			BingoFuelWarn=1;
        }
//        Fuel -= z;
    }

*/

    if (Fuel<=0) { THRUST = ThrustI = 0; Fuel=0; }

	/*********************************************************
	*                                                        *
	*   Determine Acceleration and new airspeed of Aircraft  *
	*                                                        *
	*       AirSpeed = fps x 16                              *
	*       GndSpeed = fps x 16                              *
	*       Accel    = feet per second per second x 16       *
	*		                                                 *
	*********************************************************/

	/* Calculate Gees  x16 */

    Gees =  ReqdGees[abs(OurRoll)>>8 &127];
    if (Alt>MinAlt) Gees += AOARATE/2;  /*** /2 ****/
    if (Gees>128) {
        Gees=128;
        AOARATE = rng ( 128 - ReqdGees[abs(OurRoll)>>8 &127], 0,AOARATE);
    }

	/* Equilibrium Speed and Stall Speed (Level flight) */
    EqV = (long) MaxSPEED * (ThrustI - SinX(OurPitch , 80))/100; /*** 120 */
    MinKnots = MinSPEED;

	/* Adjust for Altitude */
    EqV = ((long) EqV * ((Alt>>7) + 1024))>>10;
    MinKnots = ((long) MinKnots * ((OurAlt>>6) + 1024))>>10;

	/* Adjust for Gees */
    EqV = ( (long) EqV * (128 - Gees))>>7;
    MinKnots = ( (long) MinKnots * Sqrt(Gees<<2) >>3);
    MinKnots = abs(MinKnots);

	/* Adjust for FLAPS and WHEELS */
    if (!(Status&WHEELSUP)) {
        EqV -= EqV>>3;
    }

    MinSpd = MinKnots * FPStoKNTS;

    EqSpeed = rng (EqV, 0,899)*FPStoKNTS;

	/* Update AirSpeed */
    AirSpeed += ((long) EqSpeed - (int) AirSpeed)/16/FrameRate;     /* Now apply acceleration to airspeed */

    AOAttack = (12<<8) * (long) MinSpd/(abs(AirSpeed)+1);
    if (AOAttack>(32<<8)) AOAttack=32<<8;
    PitchAngle = CosX (OurRoll, AOAttack-(3<<8));

    if (Status&BRAKESON) {
        if (Alt==MinAlt) {
            AirSpeed -= (RealLandings?8:12)*FPStoKNTS/FrameRate;
            if (AirSpeed<32) AirSpeed = 0;
        } else
			AirSpeed -= (AirSpeed>>4)/FrameRate;
    }

    if (AirSpeed > 45000) AirSpeed = 0;  /* Keeps us from going backwards */

    GndSpeed = CosX (OurPitch,AirSpeed);

    Knots = AirSpeed/FPStoKNTS  ; /* Convert to knots */

	if ((SNDDETAIL!=1) && (SNDDETAIL!=3)) {
    	SndSounds(N_KnotsAre, Knots, ThrustI);
        if (ThrustI==0 && OldThrust!=0)
            SndSounds(N_SpoolDown);
        OldThrust = 0;
	}
    OldThrust = ThrustI;

	/****************************************
	*                                       *
	*   Calculate turning rate of aircraft  *
	*   TurnRate = WdDeg per second         *
	*                                       *
	****************************************/

    TurnRate = ((long) (sinX(OurRoll,Gees<<4))<<7) / ((int) (AirSpeed>>9)+32);
    TurnRate = CosX(OurPitch,TurnRate);                         //AWH ^^ 8

	/****************************************
	*                                       *
	*   Apply Roll, Pitch, and Heading      *
	*   changes to the flight matrix.       *
	*                                       *
	*   Roll, pitch, and heading changes    *
	*   are on a per second basis.          *
	*                                       *
	****************************************/

    if (Alt==MinAlt) {
        TurnRate=-ROLLRATE<<6;
        ROLLRATE=0;
        if (Knots<MinKnots) {
            AOARATE=0;
        }
    }

	if (ejected || (!REALFLIGHT)){
    	RollChg = ((long) ROLLRATE << 7) / FrameRate ; /*  Do Roll change  */

    	if (RollChg != 0) {
        	RC[0][0] = RC[1][1] = Icos(RollChg);     /* Create change matrix */
        	RC[0][1] = Isin(RollChg);
        	RC[1][0] = -RC[0][1];
        	MatrixMul (FM,RC);      /* Now roll aircraft */
    	}

    	PitchChg = (AOARATE << 7)/ FrameRate;             /*  Do Pitch change  */

    	if (PitchChg != 0) {
        	PC[1][1] = PC[2][2] = Icos(PitchChg);    /* Create change matrix */
        	PC[2][1] = Isin(PitchChg);
        	PC[1][2] = -PC[2][1];
        	MatrixMul (FM,PC);         /* Now pitch aircraft  */
    	}


    	HeadChg = TurnRate / FrameRate;
    	if (HeadChg != 0) {
        	HC[0][0] = HC[2][2] = Icos(HeadChg);
        	HC[0][2] = Isin(HeadChg);
        	HC[2][0] = -HC[0][2];
        	MatrixMul (HC,FM);
    	}

    	Decompose();

    	if (AirSpeed<MinSpd && Alt>MinAlt) {
        	OurPitch -= (MinSpd-AirSpeed)>>(RealLandings+1);
        	if (OurPitch<0 || Alt<200) Sound (N_StallHorn, 1,0);
    	}

    	if (Alt==MinAlt) {
        	if (OurRoll) {
            	OurRoll=0;
        	}
        	if (OurPitch<0 || (OurPitch>0 && Knots<MinKnots)) {
            	OurPitch=0;
        	}
    	}


    	DoPitchLines = (abs(OurPitch) - abs(OurRoll)/2 > 0x1000) ? 1: 0;

    	MakeRotationMatrix (FM, OurHead,OurPitch,OurRoll);


		/********************************
		*                               *
		*   Fill Variables for 3D code  *
		*                               *
		********************************/


    	OldAlt = OurAlt;
    	VVI =  (TrgMul(AirSpeed/(64>>0),Isin(OurPitch-PitchAngle)));

    	if (!AutoLand) {          /* AWH */
        	OurAlt += VVI / FrameRate;

        	PxLng += (TrgMul(GndSpeed,Isin(OurHead))) /(64>>0)/ FrameRate;
        	PyLng += (TrgMul(GndSpeed,Icos(OurHead))) /(64>>0)/ FrameRate;

    	}

	} else {
		ScottFlight();
	}


	/* Convert from 20-bit 3D-World coordinates to 15-bit game coordinates */
    Px = (PxLng+16)>>HTOV;
    Py = ((0x100000-PyLng)+16)>>HTOV;

	/* Make sure we stay on the map */
    z = rng (Px, 0x0100,0x7e00);
    if ( z != Px) {
        Px = z;
        PxLng = (long) Px<<HTOV;
    }
    z = rng (Py, 0x200,0x7d00);
    if ( z != Py) {
        Py = z;
        PyLng = (long) (0x8000-Py)<<HTOV;
    }

    if (OurAlt >62000 || OurAlt<MinAlt) OurAlt = MinAlt;
    if (OurAlt >60000) OurAlt = 60000;

    Alt = OurAlt >>2;


    GroundH = GetQHeight(PxLng,(0x100000-PyLng));

    RadarAlt = (long)OurAlt-(long)(GetQHeight (PxLng, (0x100000-PyLng)) );
//    RadarAlt = (long)OurAlt-(long)(GetQHeight (PxLng, (0x100000-PyLng)) <<2);

	/* Historical gunsight */

    n = Rtime&15;
    chs[n].head = OurHead;
    chs[n].pitch = OurPitch;
    chs[n].roll = OurRoll;
    chs[n].x = PxLng;
    chs[n].y = PyLng;
    chs[n].z = Alt;

    if (DESIGNATED != -1) {
      n = rng ( (LastRate*Dist2D(Px-ps[DESIGNATED].x,Py-ps[DESIGNATED].y)>>8), 0,12);
    } else {
      n = LastRate-1;
    }
    n  = (Rtime-n) &15;
    dH = (int) OurHead - chs[n].head;
    dP = OurPitch - chs[n].pitch;
    sightX = sinX (OurRoll, dP>>2) + cosX (OurRoll, -dH>>2);
    {register int t; t = cosX (OurRoll, dP>>1);
    t += sinX (OurRoll,  dH>>2);
    sightY = t;}
}
#pragma optimize("egl", on)


#define	DEG 182

extern int DNSTSQR[];



TAStoIAS(int TAS )
{
	int	i,j,D;

	i= OurAlt/5000;
	j= OurAlt-i*5000;
	j/=10;

	D= DNSTSQR[i]+ MDiv(DNSTSQR[i+1]-DNSTSQR[i],j,500);
	return(MDiv(TAS,1000,D));
}


TblTRPL(int *TBL,unsigned int val, unsigned int interval)
{
	unsigned	i,j;
	int	D;

	i= val/interval;
	j= val-i*interval;
   D= *(TBL+i)+MDiv(*(TBL+i+1)-*(TBL+i),(j/2),(interval/2));
	return(D);

}


#include "armt.h"

extern int W_WEIGHTS[];



// phoenix = 985
// sparrow 510
// sidewinder 180

struct load{
   int   WT;
   int   DRG;
   };
extern struct load loadout[7];


extern char  * loadstrs[];


extern int MYLOAD;


extern int LOADDRAG;


extern   int F14_STATION[12];

extern int loadoutweight;


int   PlaneDrag()
{
   int   i,p,t;

   for (i=p=0;i<12;i++)
      {
      switch (F14_STATION[i])
         {
         case W_AIM_54:
            if (i<4)
               p+=20;
			p+=3;
			break;
         case W_AIM_9:
         case W_AIM_7:
            p++;
            break;
         case W_FUEL_TANK:
//            p++;
         default:
            break;
         }
      }
   return(26+p/2);
}

long PlaneWeight()
{
	int	i,j,c;
	long	W;

	W=40000L;
	for (i=0;i<F14_STATION_NUM;i++) {
		if (F14_STATION[i] > 0)
			W+= W_WEIGHTS[F14_STATION[i]];
	}

//	W+= loadout[MYLOAD].WT;
	W+= Fuel;

	return(W);
}



extern int DEBG1;

extern int DEBG2;

extern int AUTOCARLAND;


extern	int	ARCTAN( int Y, int X);

struct	FPARAM{
	long	X;
	long	Y;
	long	Z;
	long	VEL[3];
	int	MAT[9];
};
extern struct FPARAM FPARAMS;

extern int ONTHECAT;

extern int ONTHEHOOK;




extern int ScottFlightMode;

extern int ScottLandingMode;

extern int LSORating;

extern int tchdwnrating;

extern int TRIM;



InitScottFlight()
{
	long	T;

	int	V[3];
   int   M[9];

	FPARAMS.X=PxLng*100;
	T=((unsigned long)(OurAlt))&0xffff;
	FPARAMS.Y=T*100;
	FPARAMS.Z=(0x100000L-PyLng)*100;
		MakeMatHPR( OurHead,-OurPitch,OurRoll, FPARAMS.MAT);
//	MakeMatHPR(-OurHead,OurPitch,0, FPARAMS.MAT);
	V[0]=V[1]=0;
	V[2]=-(AirSpeed>>4);

#ifdef YEP
	if ((T>MinAlt) && abs(V[2])<300)
		V[2]=300;
#endif

   Trans3X3(FPARAMS.MAT,M);
	Rotate(V,M);
//	Rotate(V,FPARAMS.MAT);
	FPARAMS.VEL[0]= V[0];
	FPARAMS.VEL[1]= V[1];
	FPARAMS.VEL[2]= V[2];
	FPARAMS.VEL[0]*=100;
	FPARAMS.VEL[1]*=100;
	FPARAMS.VEL[2]*=100;

   TRIM=Get1GTrim(AirSpeed);


}

extern int GTABLE[];

//int	GTABLE[]={
//	0, 0, 0, 10, 75, 100, 162, 171, 200,
//	215, 262, 309, 450, 537, 800, 825, 900, 1000, 1200 };

long labs(long X);


extern int DNSTBL[];


int	Density( UWORD ALT )
{
	unsigned	i,j;
	int	D;

	i= ALT/5000;
	j= ALT-i*5000;
	j/=10;

	D= DNSTBL[i]+ MDiv(DNSTBL[i+1]-DNSTBL[i],j,500);
	return(D);
}


extern int MAXENGAIR[];

extern int MILENGAIR[];

int	EngineBreathe( UWORD ALT )
{

   if (THRUST>100)
      return(TblTRPL( MAXENGAIR,ALT,10000));

   return(TblTRPL( MILENGAIR,ALT,10000));

}

extern int STTBL[33];


extern  UWORD   RudderStick;
extern	int	RUDDERVALS;
extern	int	RudderTrim;

extern int GEFFECT;

extern int PRESSUREBLACK;


extern int FASTROLL;


extern UWORD OldAutoAlt;

extern int OLDAOA;

extern long NEWAOA;

extern int OLDSLEW;

extern int SLEW;

extern UWORD TFAS[32];

extern int TFAPTR;

extern int TFAPTRCNT;


extern long ROLLINERTIA;

extern int DirectLiftControl;

extern int DIRECTLIFTSPEED;


extern int DEBUG1;

extern int DEBUG2;




Get1GTrim(int AirSpeed)
{
	int	RELWND,i;
	long	STICKGS,WEIGHT,MAXSTICKGS;

	RELWND=TAStoIAS( (AirSpeed>>4) );
   WEIGHT= PlaneWeight();
	if (RELWND>=680)
		MAXSTICKGS= GTABLE[17];
	else
		{
		i=RELWND-40*(RELWND/40);
		MAXSTICKGS= GTABLE[RELWND/40]
						+ ((GTABLE[1+RELWND/40]-GTABLE[RELWND/40])*i)/40;
		}
	MAXSTICKGS*=32;
	MAXSTICKGS= MAXSTICKGS*400/(WEIGHT/100);
	if (MAXSTICKGS>28800L) MAXSTICKGS=28800L;

   STICKGS=(3200L*128L)/(MAXSTICKGS+1);
   if (STICKGS>128)
      STICKGS=128;
   return((int)(STICKGS));
}

extern int AutoTrim;

ToggleAutoTrim()
{
	extern char gstr226[];
	extern char gstr227[];
	extern char gstr228[];
   AutoTrim^=1;
   if (ConfigData.FightModel==2)
      {
      if (AutoTrim)
         {
         Message(gstr226,RIO_NOW_MSG);
         }
      else
         {
         Message(gstr227,RIO_NOW_MSG);
         }
      }
   else
      {
      Message(gstr228,RIO_NOW_MSG);
      }
}



TrimIt(int amnt)
{
   char str[20];
	extern char gstr228[];
	extern char gstr229[];

   if (ConfigData.FightModel!=2)
      {
      Message(gstr228,RIO_NOW_MSG);
      return;
      }


   TRIM+= amnt;

   if (TRIM>126) TRIM=126;
   if (TRIM<-126) TRIM=-126;
   sprintf(str,gstr229,TRIM);
   Message(str,RIO_NOW_MSG);
}


extern int fueltime;

extern long MilFuelTBLperhour[];

extern long MaxFuelTBLperhour[];

// next tables are 1 minute intervals per engine !!!!!!!!
extern int MilFuelTBL[];

extern int MaxFuelTBL[];

DoFuel()
{
   int   T;
   int *TBL;
   long  D;
	extern char gstr230[];
	extern char gstr231[];

   // every 3 seconds

   if ((fueltime+=Fticks)>180)
      {
      TBL= (THRUST>100)? MaxFuelTBL:MilFuelTBL;

      T= (int)(FPARAMS.Y/100L);
      T= TblTRPL(TBL,T,10000);  // 2 engines !!!

      T= MDiv(fueltime,T,3600);
      if (THRUST<100) T= MDiv(T,THRUST,100);
      if (T<1) T=1;

      if ((Fuel>=1500)&&((Fuel-T)<1500))
         {
         Message(gstr230,RIO_MSG);
			LLowFuelWarn=1;
			RLowFuelWarn=1;
         }
      if ((Fuel>=BingoFuel)&&((Fuel-T)<BingoFuel))
         {
         Message(gstr231,RIO_MSG);
			BingoFuelWarn=1;
         }

      Fuel-=T;
      fueltime=0;
      }
}

extern int MYCLCD;

extern int MYCP;

extern int MYTFACT;


ScottFlight()
{
	int	TM[9],PM[9],RM[9];
	int	DELROLL,DELPITCH,DELHEAD;
	int	TMP[3];
	long	MASS,STICKGS,WEIGHT,MAXSTICKGS;
	long	G[3],LIFTGS,DRAGS;
	long newaoa;
	int	V[3];
	long FPV[3];
	long	VSQ[3];
	int   XR[3];
	int	RELWND;
	long	LTHRUST;
	int	stx,sty;
	long	DVX,DVY,DVZ;
	long	SFticks;
	long	CLCD,CP,DNSTY;
	int	i;
	int	OLDH,OLDP,OLDR;
	extern char gstr232[];
	extern char gstr233[];
	extern char gstr156[];
	extern char gstr183[];
	extern char gstr234[];
	extern char gstr235[];
	extern char gstr236[];
	extern int SLEW;
	extern int SELECTED_BAR_NUM;
	extern int SavStx;
	extern int SavSty;
	extern int BackSeat;
	SWORD	n,dH,dP;
	UWORD ALT;
	long	D;
	UWORD UTMP;
	UWORD UTMP1;
	int	CLEAN;
	char str[40];

	DoFuel();               //always reduce fuel




//   ConfigData.FightModel=2;   // REMOVE !!!!!!!!!!!!!!!!!!!!!!!!!!



	if (ONTHECAT||ONTHEHOOK) {
		PitchAngle = VVI = NEWAOA = 0;
		return(1);
	}


	if ((i=OnTheDeck(FPARAMS.X, FPARAMS.Y, FPARAMS.Z))) {
		if (Splatted(&FPARAMS,i)) {
			// make a big fireball here
			return(1);
		}
		if (FPARAMS.Y>=6600) {
			if (i>=2) {
				ONTHEHOOK=(i-1);
				LSORating=tchdwnrating;
				if ((i==3)||(i==4)) LSORating+=10;
				if (LSORating<0) LSORating=0;
				sprintf(str,gstr232,ONTHEHOOK);
				Message(str,RIO_MSG);
				sprintf(str,gstr233,LSORating);
				Message(str,RIO_MSG);
				LogIt (SLG_AIR_LAND, -1, PxLng, (0x100000 - PyLng), Alt, -LSORating, ONTHEHOOK, 0L, LOG_AIR);
				Speech(L_CUT_POWER);
			}
		}
	} else {
		tchdwnrating=0;
	}

	FASTROLL=1;
   if ((ConfigData.FightModel==2)&&((AutoTrim)||(Speedy>1)))
      {
      TRIM=Get1GTrim(AirSpeed);
      }


	// check for slewing
	if (PxLng!=(FPARAMS.X/100)) {
		FPARAMS.X=100*PxLng;
		FPARAMS.Y=100L*(long)OurAlt;
	}
	if (FPARAMS.Z/-100 - PyLng != -0x100000L) {
		FPARAMS.Z=100*(0x100000L-PyLng);
		FPARAMS.Y=100L*(long)OurAlt;
	}

//	CLCD=53;
//	CP=36;

	CLCD=MYCLCD=35;
//	CP=MYCP;

//   CP=MYCP=loadout[MYLOAD].DRG;

	CP=MYCP=PlaneDrag();

	SFticks= Fticks;
	if (SFticks<1) SFticks=1;

	ALT= (FPARAMS.Y/100);

	CLEAN=0;
	D=WEIGHT= PlaneWeight();

/*
	if ( D>80000L && !(Rdrs[ms[0].base].status&TANKER))
		{
		D= D-80000L;
		Fuel-= D/2;
		}

	if ( (WEIGHT-(unsigned long)Fuel)<40100L )
		CLEAN=1;
*/

	DNSTY= Density(ALT);
	if (THRUST>=100) DirectLiftControl=0;

	MASS=WEIGHT/32;

	LTHRUST= (THRUST>100)? 160:THRUST;

	LTHRUST*= 388;
	if (LTHRUST>56000L) LTHRUST= 56000L;

   if (ConfigData.FightModel==0 && ALT<50000)
      D=100;
   else
   	D=EngineBreathe(ALT);



	LTHRUST*= D;

	LTHRUST/= MASS;


	Copy3X3(FPARAMS.MAT,PM);

	V[0]= (FPARAMS.VEL[0]/10);
	V[1]= (FPARAMS.VEL[1]/10);
	V[2]= (FPARAMS.VEL[2]/10);
	Rotate(V,PM);
	FPV[0]=V[0];
	FPV[0]*=10;
	FPV[1]=V[1];
	FPV[1]*=10;
	FPV[2]=V[2];
	FPV[2]*=10;;
	RELWND= abs(V[2]/10);

	RELWND=TAStoIAS(RELWND);


	VSQ[2]= FPV[2]/100;
	VSQ[2]*=VSQ[2];


	TMP[0]=0;
	TMP[1]=-32*100;
	TMP[2]=0;
	Rotate(TMP,PM);
	G[0]=TMP[0];
	G[1]=TMP[1];
	G[2]=TMP[2];



	stx=mS1n&0xff;
	sty=mS2n&0xff;
	stx-=128;
	sty-=128;

   if (ConfigData.BackSeat==2 && BackSeat) {
      stx=0;
      sty=0;
   }
   if (ConfigData.BackSeat==1 && BackSeat) {
      stx=SavStx;
      sty=SavSty;
   }
   if (!BackSeat) {
      SavStx=stx;
      SavSty=sty;
   }

   if (sty<-128) sty=-128;
   if (sty>128) sty=128;


	stx= (stx<0)? -STTBL[abs(stx)>>2]:STTBL[abs(stx)>>2];
	sty= (sty<0)? -STTBL[abs(sty)>>2]:STTBL[abs(sty)>>2];



   if ( (Damaged&D_HYDRAULICS)&&(ConfigData.FightModel==2) )
      {
      sty/=2;
      stx= stx/2+16+(rnd2()&0x1f);
      }


	// make new bogus relwind from if flaps add 20 ???

	if (RELWND>=680)
		MAXSTICKGS= GTABLE[17];
	else
		{
		i=RELWND-40*(RELWND/40);
		MAXSTICKGS= GTABLE[RELWND/40]
						+ ((GTABLE[1+RELWND/40]-GTABLE[RELWND/40])*i)/40;
		}


	MAXSTICKGS*=32;



	MAXSTICKGS= MAXSTICKGS*400/(WEIGHT/100);

	if (MAXSTICKGS>28800L) MAXSTICKGS=28800L;

	STICKGS= (long)(sty)*MAXSTICKGS/31;

	STICKGS/= Speedy;






	if ( AutoAlt)
		{
		if ( (abs((int)(STICKGS))>3200) ||
			  ( (OurRoll<(-45*DEG)) || (OurRoll>(45*DEG)) )  )
			{
			AutoAlt=0;
			Message (gstr183,RIO_MSG);
			}
		}
   if (ConfigData.FightModel==2)
      {
// for TRIM here
      STICKGS+= ((long)(TRIM)*MAXSTICKGS)/128L;
      }


	if (AutoAlt)
		{
      // add teraign folowing here for easy,moderate flight
      if (ConfigData.FightModel==2)
         {
		   i= AutoAlt-(ALT>>2);
		   if (i<-50) i=-50;
		   if (i>400) i=400;
         TRIM=Get1GTrim(AirSpeed);
		   STICKGS+= 64L*(long)(i);
		   STICKGS-= FPARAMS.VEL[1];
		   STICKGS+= (32*(10000/ACosB(100,abs(OurRoll))-100 ));

         if (STICKGS<-4800) STICKGS=-4800;
         if (STICKGS>9600) STICKGS=9600;

         }
      else
         {
	      if (AutoAlt&& (AutoAlt!=OldAutoAlt) )
		      {
		      for (i=TFAPTR=0;i<4;i++)
			      TFAS[i]=0;
		      AutoAlt= (FPARAMS.Y/100);
		      }
	      OldAutoAlt=AutoAlt;

		   UTMP1= AutoAlt;

		   UTMP= (GetQHeight ( (long)(FPARAMS.X/100+FPARAMS.VEL[0]/100),
								   (long)(FPARAMS.Z/100+FPARAMS.VEL[2]/100))<<2)+500;
		   if (UTMP>TFAS[TFAPTR]) TFAS[TFAPTR]=UTMP;
		   UTMP= (GetQHeight ( (long)(FPARAMS.X/100+FPARAMS.VEL[0]/25),
								   (long)(FPARAMS.Z/100+FPARAMS.VEL[2]/25))<<2)+500;
		   if (UTMP>TFAS[TFAPTR]) TFAS[TFAPTR]=UTMP;

		   UTMP= (GetQHeight ( (long)(FPARAMS.X/100+FPARAMS.VEL[0]/400),
								   (long)(FPARAMS.Z/100+FPARAMS.VEL[2]/400))<<2)+500;
		   if (UTMP>TFAS[TFAPTR]) TFAS[TFAPTR]=UTMP;


		   if ( (TFAPTRCNT+=SFticks)>60 )
			   {
			   TFAPTRCNT-=60;
			   TFAPTR++;
			   TFAPTR&=3;
			   TFAS[TFAPTR]=0;
			   }
		   for (i=0;i<4;i++)
			   if (TFAS[i]>UTMP1) UTMP1=TFAS[i];

		   if ( AutoAlt> UTMP1 ) UTMP1= AutoAlt;

		   i= UTMP1-ALT;
		   if (i<-50) i=-50;
		   if (i>400) i=400;
		   STICKGS+= 64l*(long)(i);
		   STICKGS-= FPARAMS.VEL[1];
		   STICKGS+= (32*(10000/ACosB(100,abs(OurRoll))-100 ));
		   if (STICKGS<-4800) STICKGS=-4800;

         }
		}

// no 1 g level flight computer help here except in easy flight
	if ( (((FPARAMS.Y/100)>MinAlt) || (RELWND>280 )) &&(ConfigData.FightModel<2)  )
		STICKGS+= 3200;


	if ( STICKGS >MAXSTICKGS)
		STICKGS= MAXSTICKGS;
	if (STICKGS<(-300*32) ) STICKGS=(-300*32);

	if ( (Damaged&D_LEFTENGINE) && (Damaged&D_RIGHTENGINE) )
		STICKGS=0;



//   DEBUG1=(int) (STICKGS/320L);
//   DEBUG2=(int) (MAXSTICKGS/320L);


// fix wing swept CLCD here: not so much g bleed, better lift to drag
// check to see if TAS or IAS based !!!!!!!

   if (RELWND<400) CLCD=45;



	// figure new cp from gear,flaps

	DRAGS= (labs(STICKGS)*10)/CLCD;
	LIFTGS= STICKGS+G[1];
	D= CP;
	if (!(Status&WHEELSUP))
		D+= 200;
//		D+= 250;
//	if (Status&FLAPSDOWN)
//		D+= 250;

	UTMP= ALT/125;
	if (ALT>25000)
		UTMP+= (ALT-25000)/25;


	DRAGS= DRAGS+ (((D*(VSQ[2]/10)*DNSTY)/MASS))/100 -LTHRUST+ G[2];


	DELPITCH=DELHEAD=DELROLL= 0;

	if ( (FPARAMS.Y/100)>MinAlt)
		{
		if ( (Status&BRAKESON)&& !(Damaged&D_HYDRAULICS) )
			{
			DRAGS+= -FPV[2]/40;
			}
//		if ( Status&BRAKESON && !(Damaged&D_HYDRAULICS) )
//			DRAGS+= -FPV[2]/10*(40000/WEIGHT);






		// a = F/m  m= W/32.2
		// new V+= a*t
		// new POS+= Vold*t+ 1/2at^2
		// get new rel x , rel v

		Trans3X3(PM,TM);


		D=RELWND;
		if (RELWND<200)
			{
			D= ((RudderTrim+RUDDERVALS)*15*D)/200;
			}
		else
			{
			if (RELWND>800)
				D=800;
			D= ((RudderTrim+RUDDERVALS)*15*(1000-D))/800;
			}
		D= (long)(OLDSLEW)+(((D-OLDSLEW)*SFticks/(60))*(long)(RELWND)/200);
		SLEW= D;
		OLDSLEW=SLEW;


		G[0]+= ASinB((int)LTHRUST,(int)(3*SLEW));
		G[0]+= (3*SLEW)/4;


		D= FPV[0]*SFticks/60+ G[0]*SFticks*SFticks/(2*60*60);
		XR[0]= (D/10);
		D= FPV[1]*SFticks/60+ LIFTGS*SFticks*SFticks/(2*60*60);
		XR[1]=(D/10);
		D= FPV[2]*SFticks/60+ DRAGS*SFticks*SFticks/(2*60*60);
		XR[2]=(D/10);
		Rotate(XR,TM);

		DVX= G[0]*SFticks/60;
		DVY= LIFTGS*SFticks/60;
		DVZ= DRAGS*SFticks/60;

		V[0]= (DVX/10);
		V[1]= (DVY/10);
		V[2]= (DVZ/10);
		Rotate(V,TM);
		D= XR[0];
		FPARAMS.X+= (10*D)/4;
		D= XR[1];
		FPARAMS.Y+= 10*D;
		D= XR[2];
		FPARAMS.Z+= (10*D)/4;
		D= V[0];
		FPARAMS.VEL[0]+= 10*D;
		D= V[1];
		FPARAMS.VEL[1]+= 10*D;
		D= V[2];
		FPARAMS.VEL[2]+= 10*D;
		if ((FPARAMS.Y/100)>60000) FPARAMS.Y=100*60000;

// dont forget to put in cross control autocoordinated flight later
// because ITS NOT IN YET !!!

		if ( labs(FPV[2]+DVZ)>400 ) {
			D= ARCTAN( -(int)((FPV[0]+DVX)/10),  -(int)((FPV[2]+DVZ)/10) );
			DELHEAD= D;
			D=ARCTAN( (int)((FPV[1]+DVY)/10),  -(int)((FPV[2]+DVZ)/10) );
			DELPITCH= (3*D)/4;
		}



		D= -stx;


		if (FASTROLL)
			D*=2;

		if ( (Damaged&D_LEFTENGINE) && (Damaged&D_RIGHTENGINE) )
			D=-16;

		if (RELWND>700)
			D= (1024*D)*SFticks/60;
		else
			D= (D*1024/70)*RELWND*SFticks/600;

//////   rudder overide to ailerons
//		D+= (RUDDERVALS>>7);
		D-= SLEW/5;




		D/= Speedy;

		D= D*100/(100+(WEIGHT-40000)/200);


		if (AutoAlt)
			D+= (OurRoll*SFticks/(120));

      if (ConfigData.FightModel==0)
         {
         ROLLINERTIA=D;
         }
      else
         {
         ROLLINERTIA+= ((D-ROLLINERTIA)*Fticks/30);
         }

      DELROLL= ROLLINERTIA;


		MakeMatHPR (-DELHEAD,-DELPITCH,-DELROLL, TM);
		Mult3X3(PM,TM,RM);
		GetHPR(RM,&OurHead,&OurPitch,&OurRoll );
		OurPitch= -OurPitch;
		MakeMatHPR( OurHead,-OurPitch,OurRoll, FPARAMS.MAT);
		}
	else
		{
		SLEW=0;

		if ((FPARAMS.Y/100)<MinAlt)
			FPARAMS.Y=100*MinAlt;
		if (FPARAMS.VEL[1]<0) FPARAMS.VEL[1]=0;
		OurRoll=OurPitch=0;
		DELHEAD=DELPITCH=0;
		if (STICKGS<0) STICKGS=0;
		if (LIFTGS>0)
			{
			FPARAMS.VEL[1]= LIFTGS*SFticks/60;
			// brakes off
			// maybe add del pitch here ??

			}
		else
			{
			// weight on wheels effect
			DRAGS+= -LIFTGS/8;
			}

		if ( Status&BRAKESON && !(Damaged&D_HYDRAULICS) )
			{
			DRAGS+= -FPV[2]/10*(40000/WEIGHT);
			// add ground brakes

			DRAGS+= 10*100;

			}

		if (RELWND>1)
			{
			if (RudderStick || RUDDERVALS)
				{
				DELHEAD= (RUDDERVALS*(140/5)*SFticks)/60;
				}
			else
				{
				if (RELWND<80)
					DELHEAD= stx*140*SFticks/60;
				}
			}

		DELHEAD/=Speedy;

		DVZ=DRAGS*SFticks/60;

		//cheat the takeoff acceleration like crazy
		//if (DRAGS<0) DVZ/=2;

		FPV[2]+= DVZ;
		if (FPV[2]>0) FPV[2]=0;
		if ( (FPV[2]>-300)&&(DVZ>0) ) FPV[2]=0;

		OurHead+= DELHEAD;
		MakeMatHPR( OurHead,0,0,FPARAMS.MAT);
		Trans3X3(FPARAMS.MAT,TM);
		V[0]=0;
		V[1]=0;
		V[2]=(FPV[2]/10);
		Rotate(V,TM);


		FPARAMS.X+= FPARAMS.VEL[0]*SFticks/60;
		FPARAMS.Z+= FPARAMS.VEL[2]*SFticks/60;
		FPARAMS.Y+= FPARAMS.VEL[1]*SFticks/60;
		FPARAMS.VEL[0]= V[0];
		FPARAMS.VEL[2]= V[2];
		FPARAMS.VEL[0]*=10;
		FPARAMS.VEL[2]*=10;
		}

	if ( (FPARAMS.Y/100)<MinAlt)
		FPARAMS.Y=100*MinAlt;




// add bogus angle of attack

	newaoa= 130*STICKGS/(VSQ[2]/180+1);


// lower angles of attack with swept wings
   newaoa/=2;

	newaoa = ((WEIGHT*newaoa)/40000L);

	if (newaoa<0) newaoa=0;
	if (newaoa>(12*182)) newaoa=12*182;
	newaoa= OLDAOA+(newaoa-OLDAOA)*SFticks/(60);
	OLDAOA=newaoa;

//		sprintf(Txt,"NEWAOA: %i",NEWAOA);
//		Message(Txt);

   if (newaoa>1270)   //5.5*182
      {
		Sound(N_StallHorn, 1,0);
		Message(gstr234,RIO_MSG);
      }


	MakeMatRPH(0,-(int)newaoa,SLEW,TM);

	Mult3X3(FPARAMS.MAT,TM,RM);
	GetHPR(RM,&OurHead,&OurPitch,&OurRoll );
	OurPitch= -OurPitch;


	DoPitchLines = (abs(OurPitch) - abs(OurRoll)/2 > 0x1000) ? 1: 0;
	MakeRotationMatrix (FM, OurHead,OurPitch,OurRoll);

	AirSpeed= abs( (int)(FPV[2]/100) )<<4;

	VVI= (FPARAMS.VEL[1]/100);


	Knots = abs((int)(FPV[2]/100));
	Knots= (Knots*10)/16;


	PitchAngle= (128*newaoa)/100;

	Gees= (STICKGS/200);

	PxLng=(FPARAMS.X/100);
	PyLng=(0x100000L-(FPARAMS.Z/100));

	OurAlt=(FPARAMS.Y/100);
	Alt = OurAlt >>2;

	if (Gees>0)
		{
		if (Gees<(3*16))
			GEFFECT-= MDiv(GEFFECT,Fticks,360);
		else
			GEFFECT+= MDiv(Gees,Fticks,96);
		}
	else
		{
		if (Gees>(-2*16))
			GEFFECT-= MDiv(GEFFECT,Fticks,600);
		else
			GEFFECT+= MDiv(Gees,Fticks,6);
		}

	if (GEFFECT>7200) GEFFECT=7200;
	if (GEFFECT<-7200) GEFFECT=-7200;


   if ((Gees>(6*16))&&!(Damaged&D_HYDRAULICS)&&(ConfigData.FightModel==2) )
      {
      if (Gees>(8*16))
         {
         Damaged|=D_HYDRAULICS;
         Message(gstr235,RIO_MSG);
         }
      else
         {
         Message(gstr236,RIO_MSG);
         }
      }



	if ( Damaged&D_COCKPIT&& (ALT>18000) )
		{
		if ( (PRESSUREBLACK+=(SFticks/2))>1000 ) PRESSUREBLACK=1000;
		}
	else
		{
		if ( (PRESSUREBLACK-=2*SFticks)<0 ) PRESSUREBLACK=0;
		}

	GAMNT=0;
	if (GEFFECT>600) GAMNT=(GEFFECT-600)/4;
	if (GEFFECT<-600) GAMNT=(GEFFECT+600)/4;
	if (abs(GAMNT)>100) GAMNT= (GAMNT>0)? 100:-100;
	if (PRESSUREBLACK>600)
		{
		if ( ((PRESSUREBLACK-600)/4)>GAMNT )
			GAMNT=( (PRESSUREBLACK-600)/4);
		}

	NEWAOA=newaoa;

}


Splatted(struct FPARAM *F, int spltcode)
{
   int   ouch;
   long  VVILIMIT;
   int   ROLLLIMIT;
   int   SPDLIMIT;
	extern char gstr241[];
	extern char gstr242[];
	extern char gstr243[];
	extern char gstr244[];
	extern char gstr245[];
	extern char gstr246[];
	extern char gstr247[];

// check for blowed up by vvi or OurRoll based on landing level
// check for gear down while on the deck

   ouch=0;

   if (tchdwnrating==0)
      {
      switch (ConfigData.CarrierLanding)
         {
         case 0:
            VVILIMIT=-2000L;
            ROLLLIMIT=(10*182);
            SPDLIMIT=20;
            break;
         case 1:
            VVILIMIT=-1500L;
            ROLLLIMIT=(5*182);
            SPDLIMIT=10;
            break;
         case 2:
            VVILIMIT=-1200L;
            ROLLLIMIT=(3*182);
            SPDLIMIT=6;
            break;
         }
      tchdwnrating=10;
      if (F->VEL[1]> VVILIMIT )
         tchdwnrating+=10;
      if ( abs(OurRoll) < ROLLLIMIT  )
         tchdwnrating+=5;
      if (abs(Knots-135)<SPDLIMIT)
         tchdwnrating+=5;
	  Sound(N_CarrierLanding, 1,0);
      }

   if (spltcode==-1)
      {
      F->Y= 12000;
      ouch=7;
      }


//   if (F->VEL[1]< -2100L ) ouch=1;
   if (F->Y <(5500)) ouch=2;
   if ( (F->Y-F->VEL[1])<6300 ) ouch=3;

//   if ( abs(OurRoll) > (10*182) ) ouch=4;
   if (OurPitch< -900 )
      {
      OurPitch=-900;
      ouch=5;
      }
   if (OurPitch > (20*182) )
      {
      OurPitch= (20*182);
      ouch=6;
      }

   if (F->Y<6600) F->Y= 6600;
   if (F->VEL[1]<0) F->VEL[1]=0;

   OurRoll= 0;                   // fix the mat here too !!

   if (ouch)
      {
      if ((ConfigData.CarrierLanding==2)&&((Status & TRAINING)==0))
         {
         StartASmoke( PxLng, Alt, (0x100000-PyLng), GRND2XPLO, 17 );
         Sound (N_FarExplo, 2,0);
         EndGame (CRASHLAND);
         LogIt (46, -1, PxLng, (0x100000-PyLng), Alt, -999, -999, 0L, LOG_AIR);
         LSORating=0;
         }
      }
   if (ouch)
      {
      tchdwnrating=-40;
      }


   return(0);

}


