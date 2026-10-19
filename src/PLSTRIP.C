
/*  File:   Planes.c                              */
/*  Author: Sid Meier                             */
/*                                                */
/*  Game logic for Stealth: enemy planes          */


#define ELFAI 1
#include	<stdio.h>
#include	<stdlib.h>
#include "library.h"
#include "planeai.h"

#include "world.h"
#include "behave.h"

#include "sdata.h"
#include "Cockpit.h"
#include "sounds.h"
#include "f15defs.h"
#include "speech.h"
#include "armt.h"
#include "proto.h"
#include "setup.h"

extern long lfpx;
extern long lfpy;
extern long lox;
extern long loy;


extern int lastcatused;

extern int sotnum;

extern int friendfire;

extern int enemyfire;

extern long oldFPX;

extern long oldFPZ;

extern UWORD oldAltval;

extern char OurWingName[20];

extern  UBYTE   ExitCode;
extern	int		DESIGNATED;
extern	int		LOCKED;
extern  char    Txt[];
extern  COORD   xydist ();
extern  long    LMul   ();

extern  COORD   Px,Py;
extern  long    PxLng, PyLng;
extern  int     OurHead,OurRoll, OurPitch;
extern   UWORD       AirSpeed;
extern  SWORD	Knots;
extern  int     AltDummyUnused;
extern  int     Demo;
extern  int     FIRST_ENEMY_TRACE,DMAX9;
extern  int     TSpeed;

extern  UWORD     Rtime;
extern  int     PovMisl;
extern  int     detected, atarg;
extern  COORD   xlast[],ylast[];
extern  int     zlast[];
extern  int     Rcolor;
extern  int     CloseBase[],CloseDist[];
extern  int     CloseGuy;
extern  int     Iflew;
extern  int     LastRate;
extern  int     REALFLIGHT;
extern  int     RunwayLeftX;
extern  RPS     *Rp3D;
extern	int	    Fticks;
extern  int     Speedy;
extern  int     whichourcarr;     // holds value for which boat our carrier
extern	int		LastCall;
extern int  AvailCat[];
extern int OurLoadOut;
extern int  MyMSGTYP;
extern int msgtime;

extern int olddog;


extern struct Planes * psi;
  // pointer into "ps" structure array for this plane

static    int   i;          // index of this plane in "ps" array

extern int planecnt;
			// used since somebody desided to make i static
							// and after the planes file grew to more than
							// 5000 lines I got sick of it and desided to
							// kludge something so I could split the file.

#if 1
// #ifdef REMOVEDCODE
static    COORD xd,yd;      // Delta coords to plane's destination
static    int   zd;
static    COORD dist;       // distance to plane's destination
static    int   angl;       // angle to plane's destination
#endif

extern int pdest;
      // pitch angle to plane's destination

static    int   dpitch;     // delta pitch this frame
extern int droll;
      // delta roll this frame

extern int PlanesLaunched;

extern int AIWingMan;


extern int tempvar;

extern int tempbehavevar;

extern long AIBOMBX;
     // 20-bit Scott-style X position of predicted Bomb impact
extern long AIBOMBY;
     // 20-bit Scott-style Y position of predicted Bomb impact
extern UWORD AITTGT;
      // 2 * Time to target in seconds
extern UWORD AITREL;
      // 2 * Time to target intercept (bomb release) in seconds
extern UWORD AIRTGT;
      // ground range to target in tenths of nautical miles
extern int MyWingy;

extern int OtherAIDo;

extern int OtherAITime;

extern int WingID[3];

extern int AIWingMenu;

extern int AIWingMenuTime;

extern struct _path_ aipaths[];
extern struct _wayPoint_ aiwaypoints[];
extern struct _action_ aiactions[];


extern struct PlaneData far *GetPlanePtr(int PlaneNo);
extern struct MissileData far *GetMissilePtr(int MissileNo);
extern struct RadarData far *GetRadarPtr(int RadarNo);


extern struct PlaneData far * ptype;

extern int ourplanetype;
	/* enum id of our plane type */


extern struct FPARAM{
	long	X;
	long	Y;
	long	Z;
	long	VEL[3];
	int	MAT[9];
} FPARAMS;

extern int firstbehaveshow;


// The Strategy:
//
// Planes have three overall plans:
//
//      o PATROL means to fly amongst nearby enemy entries in the Rdrs list,
//        quitting when "time" runs out.  When this occurs, the nearest enemy
//        base is chosen as destination and the plane flys home or just
//        disappears.  When near the base, landing is initiated.  When landed,
//        the plane is deactivated.  PATROL is the default plane type.
//
//      o LOITER is similar to PATROL, except that the target is an enemy
//        base and never changes.  This gives infinite touch-n-go's.
//
//
// Overriding factors include:
//
//      o If he "pings" you, he comes after you until he loses you.
//
//      o If "detected" planes will go after your last known position
//
// Slots in the planes array are as follows:
//
//      o Last four are for CloseBase touch-n-gos
//



/* -----------------------------*/
/* Process all planes except us */
/* -----------------------------*/
doplanes ();
extern UWORD Alt;extern char gstr359[];extern char gstr360[];extern char gstr361[];extern char gstr362[];extern char gstr130[];extern char gstr363[];extern char gstr364[];extern char gstr365[];extern char gstr366[];extern char gstr360[];extern UWORD Alt;extern char gstr367[];extern char gstr368[];extern UWORD Alt;extern char gstr369[];extern char gstr370[];extern char gstr371[];extern char gstr372[];extern char gstr373[];extern char gstr374[];extern char gstr375[];extern char gstr376[];extern UWORD Alt;extern int _pd0;extern int _pd1;extern int _pd2;extern int _pd3;extern char gstr377[];extern int _pd4;extern int _pd5;extern int _pd6;extern int _pd7;extern int _pd8;extern int _pd9;extern int _pd10;extern int _pd11;extern int _pd12;extern char gstr378[];extern char gstr379[];extern char gstr380[];extern char gstr381[];extern char gstr130[];extern int _pd13;extern int _pd14;extern int _pd15;extern int _pd16;extern int _pd17;extern int _pd18;extern int _pd19;extern int _pd20;extern char gstr382[];extern char gstr383[];extern char gstr384[];extern char gstr385[];extern char gstr386[];extern UWORD Alt;extern char gstr387[];extern char gstr388[];extern char gstr389[];extern char gstr135[];extern char gstr130[];extern char gstr390[];extern int _pq0;extern int _pq1;extern int _pq2;extern int _pq3;extern int _pq4;extern int _pq5;extern int _pq6;extern int _pq7;extern int _pq8;extern int _pq9;extern int _pq10;extern int _pq11;extern int _pq12;extern int _pq13;extern int _pq14;extern int _pq15;extern int _pq16;extern int _pq17;extern int _pq18;extern int _pq19;extern int _pq20;extern int _pq21;extern int _pq22;extern int _pq23;extern char gstr391[];extern char gstr392[];extern char gstr388[];extern char gstr393[];extern char gstr140[];extern char gstr384[];extern char gstr137[];extern char gstr138[];extern char gstr393[];extern char gstr140[];extern char gstr394[];extern char gstr134[];extern char gstr393[];extern char gstr140[];

#define SMWALLMIDDIST 50
#define SMWALLXDIST 100
#define WALLMIDDIST 1250
#define WALLXDIST 2500
#define WALLHDIST 750
void Wall() {
	int dx, dy, dz;
	int tx, ty, tz, txl, tyl, tzl;
	int relangle;
	int tangle, tpitch, troll, leadhead, isleader, tanglel;
	extern char gstr395[];
	extern char gstr372[];
	extern char gstr140[];
	COORD tdist, distkm, tdistnm;
	int prewing, prespeed, secplace;
	int txi, tyi;
	int wingcnt, prewing2;
	long lx, ly;
	int dpwhead;
	int dangle, tsecplace, tsecp2, dadd;

	isleader = FALSE;
	prespeed = 0;
	secplace = (bv[i].behaveflag & 15);

// **************************************************************************
//  Find out who is the flight leader
// **************************************************************************
//	prewing = GetSquadLeader(i);
	prewing = GetBehaviorLeader(i);

	dpwhead = bv[prewing].finhead - ps[prewing].head;

	if(i != prewing)
		bv[i].target = bv[prewing].target;

	if(((secplace <= 1) || (prewing == i) || (abs(dpwhead) > 0x600)) && (bv[i].behaveflag & ENEMYNOFIRE)) {
		if(prewing != i) {
			bv[i].groundt = bv[prewing].groundt;
			bv[i].xloc = bv[prewing].xloc;
			bv[i].yloc = bv[prewing].yloc;
		}
		if(bv[i].groundt != -1) {
			FlyWayPoint();
			return;
		}
	}

	if((secplace <= 1) || (prewing == i) || (abs(dpwhead) > 0x600)) {
		txi = psi->x + sinX((psi->head - 0x4000), SMWALLMIDDIST);
		tyi = psi->y - cosX((psi->head - 0x4000), SMWALLMIDDIST);
		tz = 3250;
		if(bv[i].target == -1) {
			tx = Px;
			ty = Py;
//			tz = Alt + 500;
		}
		else {
			tx = ps[bv[i].target].x;
			ty = ps[bv[i].target].y;
//			tz = ps[bv[i].target].alt + 500;
		}
	}
	else {
		txi = psi->x;
		tyi = psi->y;
		tz = ps[prewing].alt;
/*		if(bv[i].target == -1) {
			tz = Alt;
		}
		else {
			tz = ps[bv[i].target].alt;
		}  */
		prespeed = bv[prewing].basespeed;
		if(secplace == 2) {
			tx = ps[prewing].x + sinX((ps[prewing].head + 0x4000), WALLXDIST);
			ty = ps[prewing].y - cosX((ps[prewing].head + 0x4000), WALLXDIST);
			lx = ailoc[prewing].xloc + sinXL((ps[prewing].head + 0x4000), ((long)AILOCDIV * WALLXDIST));
			ly = ailoc[prewing].yloc - cosXL((ps[prewing].head + 0x4000), ((long)AILOCDIV * WALLXDIST));
			tz -= WALLHDIST;
		}
		else if(secplace == 3) {
			tx = ps[prewing].x + sinX((ps[prewing].head - 0x4000), WALLXDIST);
			ty = ps[prewing].y - cosX((ps[prewing].head - 0x4000), WALLXDIST);
			lx = ailoc[prewing].xloc + sinXL((ps[prewing].head - 0x4000), ((long)AILOCDIV * WALLXDIST));
			ly = ailoc[prewing].yloc - cosXL((ps[prewing].head - 0x4000), ((long)AILOCDIV * WALLXDIST));
			tz -= WALLHDIST;
		}
		else if(secplace == 4) {
			tx = ps[prewing].x + sinX((ps[prewing].head - 0x4000), (WALLXDIST * 2));
			ty = ps[prewing].y - cosX((ps[prewing].head - 0x4000), (WALLXDIST * 2));
			lx = ailoc[prewing].xloc + sinXL((ps[prewing].head - 0x4000), ((long)AILOCDIV * WALLXDIST * 2));
			ly = ailoc[prewing].yloc - cosXL((ps[prewing].head - 0x4000), ((long)AILOCDIV * WALLXDIST * 2));
			tz += WALLHDIST<<1;
		}
		else if(secplace > 4){
			tsecplace = secplace - 4;
			dadd = 2 + tsecplace >> 1;
			if(tsecplace & 1) {
				dangle = 0x4000;
			}
			else {
				dangle = -0x4000;
			}
			tx = ps[prewing].x + sinX((ps[prewing].head + dangle), (WALLXDIST * dadd));
			ty = ps[prewing].y - cosX((ps[prewing].head + dangle), (WALLXDIST * dadd));
			lx = ailoc[prewing].xloc + sinXL((ps[prewing].head + dangle), ((long)AILOCDIV * WALLXDIST * dadd));
			ly = ailoc[prewing].yloc - cosXL((ps[prewing].head + dangle), ((long)AILOCDIV * WALLXDIST * dadd));
			while(tsecplace > 4)
				tsecplace = tsecplace >> 2;
			if((tsecplace == 2) || (tsecplace == 3)) {
				tz -= WALLHDIST;
			}
			else if(tsecplace == 4)
				tz += WALLHDIST<<1;
		}
		if((GLowDroll) && (!(ps[i].status & TAKEOFF))) {
			ps[i].status |= JUSTPUT;
			if(tz < 50)
				tz = 50;
			JustPutHere(i, lx, ly, tz, prewing);
			return;
		}

		LFlyTo(lx, ly, tz, prewing, prespeed);
		return;
	}

	if(tz < 50) {
		tz = psi->alt;
	}
	dx = tx - txi;
	dy = ty - tyi;
	tdist = Dist2D(dx, dy);
//	tpitch = GetAltPitch(psi->alt, tz, tdist);
	tpitch = GetAltPitch(psi->alt, tz, 50);
	tangle = angle(dx, -dy);
	tpitch = rng2(tpitch, -32<<8, 16<<8);
	distkm = tdist*32/825;
	tdistnm = (tdist * 2)/95;

	if((prewing == i) && (!(psi->status & TAKEOFF))) {
		if((tdistnm < 34) || ((CheckAnyEnemyLock(i)) && (tdistnm < 41))) {
			wingcnt = prewing;
			prewing2 = prewing;
			while (prewing2 >= 0) {
				while (wingcnt >= 0) {
						bv[wingcnt].Behavior = Wall2;
						wingcnt = bv[wingcnt].wingman;
				}
				prewing2 = bv[prewing2].nextsec;
				wingcnt = prewing2;
			}
	 	}
	}

	relangle = tangle - psi->head;

	troll = relangle - psi->roll;
	pdest = tpitch;

	TurnAIPlaneBy(i, relangle);

	bv[i].finhead = relangle + ps[i].head;

	if(prespeed) {
//		FormationSpeedAdjust(prespeed, relangle, tdist, prewing);
		bv[i].basespeed = prespeed - 10;
	}

#if 0
	if(((secplace <= 1) || (prewing == i) || (abs(dpwhead) > 0x600)) && (bv[i].behaveflag & ENEMYNOFIRE)) {
		if(prewing != i) {
			bv[i].groundt = bv[prewing].groundt;
			bv[i].xloc = bv[prewing].xloc;
			bv[i].yloc = bv[prewing].yloc;
		}
		if(bv[i].groundt != -1)
			FlyWayPoint();
	}
#endif

	if(prewing != i) {
		if(ps[i].alt > 75) {
			pdest = ps[prewing].pitch;
		}
	}
#ifdef ELFBMSG
	if(tempvar) {
		TXT(gstr395);
		TXN(i);
		TXA(gstr372);
		TXN(bv[i].basespeed);
		TXA(gstr140);
		TXN(tdist);
		Message(Txt,DEBUG_MSG);
	}
#endif
}

void FreeFighter() {
	int dx, dy, dz;
	int relangle, rrelangle;
	int tangle, tpitch, troll;
	int enemyhead, enemyalt;
	extern char gstr396[];
	extern char gstr372[];
	extern char gstr140[];
	COORD tdist, tdistnm;

	if(!(ps[i].status & (FRIENDLY|NEUTRAL)) && bv[i].winglevel < 4) {
		StraightAttack();
		return;
	}

	bv[i].basespeed = ptype->speed;
	if(bv[i].target == -1) {
		dx = Px - psi->x;
		dy = Py - psi->y;
		enemyhead = OurHead;
	}
	else {
		dx = ps[bv[i].target].x - psi->x;
		dy = ps[bv[i].target].y - psi->y;
		enemyhead = ps[bv[i].target].head;
	}

	GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &tdistnm, &enemyalt, 1250);

	relangle = tangle - psi->head;
	rrelangle = (0x8000 + tangle) - enemyhead;
	pdest = tpitch;

	if(tdistnm < 7) {
		if(abs(rrelangle) < 0x4000) {
			relangle = (enemyhead - 0x8000) - psi->head;
		}
		else {
			if(rrelangle < 0) {
				relangle = relangle + 0x4000;
			}
			else {
				relangle = relangle - 0x4000;
			}
			if(tdistnm < 2) {
				if(rrelangle < 0) {
					relangle = relangle + 0x1000;
				}
				else {
					relangle = relangle - 0x1000;
				}
			}
		}
	}

	TurnAIPlaneBy(i, relangle);

#ifdef ELFBMSG
	if(tempvar) {
		TXT(gstr396);
		TXN(i);
		TXA(gstr372);
		TXN(bv[i].basespeed);
		TXA(gstr140);
		TXN(tdist);
		Message(Txt,DEBUG_MSG);
	}
#endif
}

void Wall2()
{
	int secplace;
	int dx, dy;
	COORD tdist, tdistnm;
	int tpitch, tangle, enemyalt;
	int prewing, wingcnt;
	int winglead;

	secplace = (bv[i].behaveflag & 15);

	if(bv[i].target == -1) {
		dx = Px - psi->x;
		dy = Py - psi->y;
	}
	else {
		dx = ps[bv[i].target].x - psi->x;
		dy = ps[bv[i].target].y - psi->y;
	}
	GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &tdistnm, &enemyalt, 0);

	if((CheckDetection(i) != -999) && ((secplace == 0) || (secplace == 3))) {
		if(tdistnm < EVASIVEDIST) {
			winglead = bv[i].leader;
			if(winglead >= 0){
				winglead = CheckDetection(winglead);
			}
			else
				winglead = -999;
			if(winglead == -999) {
				PostHole();
				return;
			}
		}
	}

	if(secplace > 4) {
		if(secplace & 1)
			BracketRight();
		else
			BracketLeft();
	}
	else if(secplace <= 2) {
		BracketRight();
	}
	else {
		BracketLeft();
	}

	prewing = GetSquadLeader(i);
	if(tdistnm < 21)
	{
		wingcnt = prewing;
		while (prewing >= 0) {
			while (wingcnt >= 0) {
				bv[wingcnt].Behavior = FlyStraight;
				wingcnt = bv[wingcnt].wingman;
			}
			prewing = bv[prewing].nextsec;
			wingcnt = prewing;
		}
	}
}

void Ladder2()
{
	int secplace;
	int dx, dy;
	COORD tdist, tdistnm;
	int tpitch, tangle, enemyalt;
	int prewing, wingcnt, tempalt;
	int angleinc, angleevade;
	int designatedsec, rrangle, enemyhead;
	int rangle;
	int winglead;

	secplace = (bv[i].behaveflag & 15);
	bv[i].basespeed = ptype->speed;

	prewing = GetSquadLeader(i);
	designatedsec = FirstTargInGroup(prewing);
	if(designatedsec != -1)
		designatedsec = bv[designatedsec].behaveflag & 15;

	if(bv[i].target == -1) {
		dx = Px - psi->x;
		dy = Py - psi->y;
		enemyhead = OurHead;
	}
	else {
		dx = ps[bv[i].target].x - psi->x;
		dy = ps[bv[i].target].y - psi->y;
		enemyhead = ps[bv[i].target].head;
	}
	GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &tdistnm, &enemyalt, 0);

	if(designatedsec == i) {
		if(tdistnm < EVASIVEDIST) {
			winglead = bv[i].leader;
			if(winglead >= 0){
				winglead = CheckDetection(winglead);
			}
			else
				winglead = -999;
			if(winglead == -999) {
				PostHole();
				return;
			}
		}
		else {
			StraightAttack();
			return;
		}
	}

	if((secplace <= 1) || ((secplace == 3) && (designatedsec == 1))) {
		rrangle = (0x8000 + tangle) - enemyhead;
//		if(LOCKED != -1) {
		if(LOCKED == i) {
			rangle = (enemyhead + 0x6800) - psi->head;

			TurnAIPlaneBy(i, rangle);

		}
		else {
		 	if(rrangle < -0x2000)
		 		angleinc = -0x1000;
		 	else
		 		angleinc = -0x1800;
		 	angleevade = 0x4000;
			BaseBracket(angleinc, angleevade);
		}
		tempalt = enemyalt - 1000;
		if(tempalt < 50)
			tempalt = 50;
		if(ps[i].alt > tempalt) {
//			pdest = -0x2000 - psi->pitch;
			pdest = -0x2000;
		}
		else
			pdest = 0;
	}
	else if((secplace == 2) || ((secplace == 4) && (designatedsec == 2))) {
		angleinc = 0x3000;
		angleevade = +0x4000;
		BaseBracket(angleinc, angleevade);
		if(ps[i].alt < (enemyalt + 750)) {
//			pdest = 0x2000 - psi->pitch;
			pdest = 0x2000;
		}
		else
			pdest = 0;
	}
//	else if((LOCKED != -1) || (secplace != 4)){
	else if((LOCKED == i) || (secplace != 4)){
		angleinc = -0x3000;
		angleevade = 0x4000;
		BaseBracket(angleinc, angleevade);
	}
	else {
		StraightAttack();
	}

	if(!((secplace <= 1) || ((secplace == 3) && (designatedsec == 1)))) {
		if(tdistnm < 24)
		{
			wingcnt = prewing;
			while (prewing >= 0) {
				while (wingcnt >= 0) {
					secplace = (bv[wingcnt].behaveflag & 15);
					if(!((secplace <= 1) || ((secplace == 3) && (designatedsec == 1))))
					{
						bv[wingcnt].Behavior = FlyStraight;
					}
					wingcnt = bv[wingcnt].wingman;
				}
				prewing = bv[prewing].nextsec;
				wingcnt = prewing;
			}
		}
	}
	else {
		if(abs(tangle) > 0x5000) {
			bv[i].Behavior = FlyStraight;
		}
	}
}

void LadderCircle ()
{
	int txi, tyi;
	int dx, dy, dz;
	int relangle;
	int tangle, tpitch, troll, leadhead, isleader, tanglel, enemyalt;
	extern char gstr397[];
	extern char gstr398[];
	extern char gstr399[];
	extern char gstr140[];
	extern char gstr134[];
	COORD tdist, distkm, tdistnm;
	int prewing, prespeed, secplace;
	int whofollow, tmpcnt, temphead;
	int prewing2, wingcnt;

	if (psi->status & PANIC) {
		bv[i].Behavior = StraightAttack;
		ChangeGroupBehavior(i, 0);
	}
	bv[i].varcnt = 0;
	prewing = GetSquadLeader(i);
	whofollow = i;
	if(bv[i].leader >= 0)
		whofollow = bv[i].leader;
	else if(bv[i].prevsec >= 0) {
		tmpcnt = bv[i].prevsec;
		while(tmpcnt >= 0) {
			whofollow = tmpcnt;
			tmpcnt = bv[tmpcnt].wingman;
		}
	}

	tdist = tangle = 0;
	bv[i].basespeed = ptype->speed / 2;

	if((prewing == i) || (whofollow == i)) {
		droll = 0x3000 - psi->roll;   /*  was 0x2800  */
		pdest = 0;
		if(bv[i].target == -1) {
			dx = Px - psi->x;
			dy = Py - psi->y;
		}
		else {
			dx = ps[bv[i].target].x - psi->x;
			dy = ps[bv[i].target].y - psi->y;
		}

		GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &tdistnm, &enemyalt, 0);

		if((tdistnm > 30) && (abs(tangle - temphead) < 0x4000)) {
			ps[i].status |= BACKTOBASE;
			bv[i].Behavior = AIGoHome;
#ifdef SHOWHOME
			TXT(gstr397);
			TXN(i);
			Message(Txt,DEBUG_MSG);
#endif
			GetAIBase(i);
			prewing2 = prewing;
			wingcnt = prewing2;
			while (prewing2 >= 0) {
				while (wingcnt >= 0) {
					if(wingcnt != prewing2)
						bv[wingcnt].Behavior = Formation;
					ps[wingcnt].status |= BACKTOBASE;
					bv[wingcnt].Behavior = AIGoHome;
					GetAIBase(wingcnt);
					wingcnt = bv[wingcnt].wingman;
				}
				prewing2 = bv[prewing2].nextsec;
				wingcnt = prewing2;
			}
		}
	}
	else {
		temphead = (ps[whofollow].head + 0x8000);
//		txi = ps[whofollow].x + sinX(temphead, 20);
//		tyi = ps[whofollow].y - cosX(temphead, 20);
		txi = ps[whofollow].x;
		tyi = ps[whofollow].y;

		dx = txi - psi->x;
		dy = tyi - psi->y;

		GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &tdistnm, &enemyalt, 0);

		tpitch = GetAltPitch(psi->alt, ps[whofollow].alt, 50);
		tpitch = rng2(tpitch, -32<<8, 16<<8);

		relangle = tangle - psi->head;

		troll = relangle - psi->roll;
		pdest = tpitch;

		if(tdistnm < 1)
			bv[i].basespeed -= 100;

		TurnAIPlaneBy(i, relangle);

/*		if(abs(relangle) > 0x2000) {
			if(relangle < 0)
				droll = 0xB000 - psi->roll;
			else
				droll = 0x5000 - psi->roll;
		}
		else {
			droll = 2 * (relangle - ps[whofollow].roll);
		}    I commented this out because I can't figure out why I
 			 used whofollow at the end of this.   SRE*/
	}
#ifdef ELFBMSG
	if(tempvar) {
		TXT(gstr398);
		TXN(i);
		TXA(gstr399);
		TXN(whofollow);
		TXA(gstr140);
		TXN(tdist);
		TXA(gstr134);
		TXN(relangle);
		Message(Txt,DEBUG_MSG);
	}
#endif
}

extern int _pd21;
extern int _pd22;
extern int _pd23;
int CheckDetection(int planenum)
{
	int cnt;
	COORD tdist, tdist1;
	int returnval = -999;
	int dx, dy, x, y;
	extern int LOCKEDx;

	x = ps[planenum].x;
	y = ps[planenum].y;

	if((LOCKEDx == planenum) && (!FriendMatchType(planenum, FRIENDLY))) {
    	dx = Px-x;
    	dy = Py-y;
    	tdist = xydist(dx,dy);
		returnval = -1;
	}

    for (cnt=0; cnt<NPLANES; cnt++) {
		if((bv[cnt].target == planenum) && (bv[cnt].behaveflag & ENEMYLOCK) && (!FriendMatch(planenum, cnt))) {
    	    if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) && (!(ps[cnt].status&CRASH)) && (ps[cnt].speed)) { /* Planes in the air */
       			tdist1 = xydist(ps[cnt].x-x, ps[cnt].y-y);
				if(returnval == -999) {
					if(tdist1 < tdist) {
						tdist = tdist1;
						returnval = cnt;
					}
				}
				else {
					tdist = tdist1;
					returnval = cnt;
				}
			}
		}
	}
	return(returnval);
}

int CheckAnyEnemyLock(int planenum)
{
	int cnt;

	return(0);

	if(!(ps[planenum].status & FRIENDLY)) {
		if(LOCKED != -1)
			return(1);
	}

    for (cnt=0; cnt<NPLANES; cnt++) {
		if((!FriendMatch(cnt, planenum)) && (bv[cnt].behaveflag & ENEMYLOCK)) {
    	    if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) && (!(ps[cnt].status&CRASH)) && (ps[cnt].speed)) { /* Planes in the air */
				return(1);
			}
		}
	}
	return(0);
}

void ShowAIWeaponStatus(int ainum)
{
    char Str[80];
	int cnt;
    struct sam  *S;
	static struct PlaneData far *aiptype;
	int flag;
	extern char gstr400[];
	extern char gstr401[];
	extern char gstr130[];
	extern char gstr402[];
	extern char gstr403[];
	extern char gstr404[];
	extern char gstr405[];
	extern char gstr406[];
	extern char gstr407[];

	flag = 0;
	aiptype = GetPlanePtr(ps[ainum].type);

	GetWingName(ainum, Str);
	TXT(Str);
	if(OurLoadOut == SL_GUNS) {
		TXA(gstr400);
	}
	else if(CheckAIMissiles(ainum) == -1) {
		TXA(gstr401);
	}
	else {
		TXA(gstr130);
		for(cnt = 0; cnt < 3; cnt++) {
			if(ps[ainum].mcnt[cnt] == 0)
				continue;
			if((cnt > 0) && flag)
				TXA(gstr402);
			flag = 1;
			TXN(ps[ainum].mcnt[cnt]);
			if(aiptype->misl[cnt] == ML_AIM7M)
				TXA(gstr403);
			else if(aiptype->misl[cnt] == ML_AIM9M)
				TXA(gstr404);
			else if(aiptype->misl[cnt] == ML_AIM120)
				TXA(gstr405);
			else if(aiptype->misl[cnt] == ML_AIM54C)
				TXA(gstr406);
			else
				TXA(gstr407);
		}
	}
	Message(Txt,WINGMAN_MSG);
}

void ShowAIStatus(int ainum, int otherai, int effects)
{
	int flag;
	int gotone;
	int eff;
	int rtemp;
	extern char gstr408[];
	extern char gstr409[];
	extern char gstr402[];
	extern char gstr410[];
	extern char gstr411[];
	extern char gstr412[];
	extern char gstr413[];
	extern char gstr414[];
	extern char gstr415[];
	extern char gstr416[];
	extern char gstr417[];
	extern char gstr418[];
	extern char gstr419[];

	flag = 0;
	gotone = 0;
	GetWingName(ainum, Txt);

	if(effects) {
		eff = effects;
		TXA (gstr408);
	}
	else {
		eff = bv[ainum].behaveflag & 0xe0;
	}

	if(eff & DAMAGERADAR) {
		eff &= ~DAMAGERADAR;
    	TXA (gstr409);
		flag = 1;
		gotone = 1;
	}
	if(eff & DAMAGEENGINE) {
		eff &= ~DAMAGEENGINE;
		if(gotone) {
			if(eff)
				TXA(gstr402);
			else
				TXA(gstr410);
		}
		TXA (gstr411);
		flag = 1;
		gotone = 1;
	}
	if(eff & DAMAGECONTROLSURFACE) {
		eff &= ~DAMAGECONTROLSURFACE;
		if(gotone) {
			if(eff)
				TXA(gstr402);
			else
				TXA(gstr410);
		}
		if(effects)
			TXA (gstr412);
		else
			TXA (gstr413);
		gotone = 1;
	}
	if((eff & DAMAGEMUNITIONS) && (effects != 0)) {
		if(gotone)
			TXA(gstr402);
		TXA (gstr414);
		gotone = 1;
	}

	if(gotone) {
		TXA (gstr415);
	}
	else {
		rtemp = (Rtime + otherai) & 3;
		if(rtemp == 0)
			TXA (gstr416);
		else if(rtemp == 1)
			TXA (gstr417);
		else if(rtemp == 2)
			TXA (gstr418);
		else
			TXA (gstr419);
	}
	Message(Txt,WINGMAN_MSG);
	if(effects == 0)
		ShowAIWeaponStatus(ainum);
}

void ShowMenuStuff(int MenuID)
{
	extern char gstr420[];
	extern char gstr421[];
	extern char gstr422[];
	extern char gstr423[];
	extern char gstr424[];
	switch(MenuID) {
		case GETWINGBRACKET:
//		    MsgBox (316, 2, 2);
//    	    RpPrint (Rp3D, 2,     2, "1 Bracket Left, 2 Bracket Right, 3 High, 4 Low, 5 Straight");
    	    Message(gstr420, WING_MENU_MSG);
		break;

		case GETWINGFORMATION:
//		    MsgBox (316, 2, 2);
//    	    RpPrint (Rp3D, 2,     2, "1 Parade, 2 Cruise, 3 Combat Spread");
    	    Message(gstr421, WING_MENU_MSG);
		break;

		case GETWINGSANITIZE:
//		    MsgBox (316, 2, 2);
//    	    RpPrint (Rp3D, 2,     2, "Sanitize 1 Left, 2 Right, 3 Front, 4 Back");
    	    Message(gstr422, WING_MENU_MSG);
		break;

		case GETNEGVSEPERATION:
//		    MsgBox (316, 2, 2);
//    	    RpPrint (Rp3D, 2,     2, "Negative 1 1000, 2 2000, ... , 9 9000, 0 10000 Ft Verticle Seperation");
    	    Message(gstr423, WING_MENU_MSG);
		break;

		case GETPOSVSEPERATION:
//		    MsgBox (316, 2, 2);
//    	    RpPrint (Rp3D, 2,     2, "Positive 1 1000, 2 2000, ... , 9 9000, 0 10000 Ft Verticle Seperation");
    	    Message(gstr424, WING_MENU_MSG);
		break;
	}
}

int ProcessAICommunications(int ainum)
{
	int otherai;
    char Str[80];
    char Str2[80];
	int htemp;
	extern char gstr425[];
	extern char gstr426[];
	extern char gstr427[];
	extern char gstr428[];
	extern char gstr429[];
	extern char gstr430[];
	extern char gstr431[];
	extern char gstr432[];
	extern char gstr433[];
	extern char gstr434[];
	extern char gstr435[];
	extern char gstr436[];
	extern char gstr437[];
	extern char gstr438[];
	extern char gstr439[];
	extern char gstr440[];

    Rp3D->APen   = WHITE;
    Rp3D->FontNr = F_3x5B;

/*	if(behavecommands != 0) {
		TXT("behavecommands ");
		TXN(behavecommands);
		Message(Txt,DEBUG_MSG);
	}  */
	if(bv[ainum].prevsec == -1) {
		otherai = 1;
	}
	else if(bv[ainum].leader >= 0) {
		if(bv[bv[ainum].leader].prevsec == -1) {
			otherai = 2;
		}
		else
			otherai = 0;
	}
	else
		otherai = 0;

	if(((Rtime & 31) == 0) && (GetSquadLeader(ainum) == -1))
		CheckAISix(ainum);

	GetWingName(ainum, Str);

    TXT (Str);


	if((behavecommands == OTHERWINGAI) && (otherai)) {   /*  SHIFT F8  */
		OtherAITime = Gtime + 4;
	}

	if(Gtime > AIWingMenuTime) {
		AIWingMenuTime = 0;
		AIWingMenu = 0;
	}

	if(((bv[ainum].leader == -1) && (Gtime > OtherAITime) && (!OtherAIDo))
			|| ((otherai) && ((OtherAIDo) || (Gtime <= OtherAITime)))){
		if((otherai) && (behavecommands != 0)) {
			if(behavecommands != OTHERWINGAI)   /*  SHIFT F8  */
				OtherAITime = 0;
			OtherAIDo = behavecommands;
		}
		if(Gtime < 20) {
			if((behavecommands == AIRETURNBASE) && (MyWingy < 3)) {
				MyWingy ++;
			}
			else if((behavecommands == OTHERWINGAI) && (MyWingy == 3)) {
				MyWingy ++;
			}
			else if((behavecommands == AISHOWSTATUS) && (MyWingy == 4)) {
				MyWingy = -999;
				InitAddedWingmen(-999);
			}
		}

//		ShowMenuStuff(AIWingMenu);

		if(behavecommands >= AIBRACLEFT) {
			return(ProcessAICommMenu(ainum));
		}

		switch (behavecommands) {

#if 0
			case AIARMWEAPONSRADAR:     /*   ALT F1 KEY  */
				if(bv[ainum].behaveflag & ENEMYNOFIRE) {  /*  RADAR and WEAPONS HOLD/RELEASE  */
					if(!(bv[ainum].behaveflag & DAMAGERADAR))
						bv[ainum].behaveflag |= RADARON;
					bv[ainum].behaveflag &= ~ENEMYNOFIRE;
					if(CheckAIMissiles(ainum) != -1) {
						TXA(gstr425);
						Message(Txt,WINGMAN_MSG);
					}
					else {
						TXA(gstr426,WINGMAN_MSG);
						Message(Txt);
					}
				}
				else {
					bv[ainum].behaveflag &= ~RADARON;
					bv[ainum].behaveflag |= ENEMYNOFIRE;
					TXA(gstr427);
					Message(Txt,WINGMAN_MSG);
				}
			break;
#endif

			case AIATTACKCLOSE:   /*  ALT F1 KEY  */
				bv[ainum].Behavior = StraightAttack;   /*  ATTACK CLOSE  */
				if(!(bv[ainum].behaveflag & DAMAGERADAR))
					bv[ainum].behaveflag |= RADARON;
				bv[ainum].behaveflag &= ~ENEMYNOFIRE;
				bv[ainum].behaveflag &= ~NO_SWITCH_TARGET;
				GetEnemyTarget(ainum);
				TXA(gstr428);
//				TXN(bv[ainum].target);
//				Message(Txt,WINGMAN_MSG);
				Speech(ROGER);
				GetSortInfo(ainum);
				ShowAIWeaponStatus(ainum);
				return(1);
			break;

			case AIATTACKTARGET:    /*  ALT F2 KEY  */
				if(DESIGNATED != -1) {                        /*  ATTACK MY TARGET  */
					if(ps[DESIGNATED].status & FRIENDLY) {
						TXA(gstr429);
						Message(Txt,WINGMAN_MSG);
					}
					else {
						TXA(gstr430);
						Message(Txt,WINGMAN_MSG);
						Speech(ROGER);
						Speech(TARGETING);
						if(bv[ainum].behaveflag & ENEMYNOFIRE) {
							TXT(Str);
							TXA(gstr431);
							Message(Txt,WINGMAN_MSG);
						}
						bv[ainum].behaveflag |= NO_SWITCH_TARGET;
						bv[ainum].target = DESIGNATED;
						ShowAIWeaponStatus(ainum);
						return(1);
					}
				}
			break;

			case AIFORMUP:   /*  ALT F3 KEY  */
				AIWingMenu = GETWINGFORMATION;
				AIWingMenuTime = Gtime + 5;
				if(OtherAIDo)
					OtherAITime = AIWingMenuTime;
				ShowMenuStuff(AIWingMenu);
			break;

			case AIBRACKET: 	 /*  ALT F4  */
				AIWingMenu = GETWINGBRACKET;
				AIWingMenuTime = Gtime + 5;
				if(OtherAIDo)
					OtherAITime = AIWingMenuTime;
				ShowMenuStuff(AIWingMenu);
			break;

			case AIHELPME:  /*  ALT F5  */
				Speech(ROGER);
				if(CheckWingmanSix(-1)) {
					TXA(gstr432);    /*  Wingmen help us  */
					Message(Txt,WINGMAN_MSG);
				}
				else if(NearWingTarget(-1)) {
					TXA(gstr433);    /*  Wingmen help us  */
					Message(Txt,WINGMAN_MSG);
				}
			break;

			case AISANITIZE:  /*  ALT F6  */
				if(bv[ainum].behaveflag & DAMAGERADAR) {
					TXA(gstr434);
					Message(Txt,WINGMAN_MSG);
				}
				else {
					bv[ainum].behavecnt = 0;
					AIWingMenu = GETWINGSANITIZE;
					AIWingMenuTime = Gtime + 5;
					ShowMenuStuff(AIWingMenu);
				}
				if(OtherAIDo)
					OtherAITime = AIWingMenuTime;
			break;

			case AISORTAI:  /*  ALT F7  */
					Speech(ROGER);
					TXA(gstr435);    /*  Wingmen id targets  */
					GetSortInfo(ainum);
			break;

			case AISHOWSTATUS:   /*  ALT F8 KEY  */
				Speech(ROGER);
				ShowAIStatus(ainum, otherai, 0);
			break;

			case AIREJOIN:		 /*  ALT F9 KEY  */
				bv[ainum].Behavior = Formation;
				bv[ainum].behaveflag &= ~RADARON;
				bv[ainum].behaveflag |= ENEMYNOFIRE;
				bv[ainum].verticlesep = 0;
				TXA(gstr436);
				Message(Txt,WINGMAN_MSG);
				Speech(ROGER);
				return(1);
			break;

			case AIRETURNBASE:   /*  ALT F10 KEY  */
				if(ps[ainum].status & BACKTOBASE) {  /*  UNDO RETURN TO BASE  */
					ps[ainum].status &= ~BACKTOBASE;
					bv[ainum].Behavior = Formation;
					TXA(gstr437);
					Message(Txt,WINGMAN_MSG);
					Speech(ROGER);
					return(0);
				}
				ps[ainum].status |= BACKTOBASE;
				bv[ainum].Behavior = AIGoHome;
				TXA(gstr438);
				Message(Txt,WINGMAN_MSG);
				Speech(ROGER);
//				Speech(RTB);
				return(1);
#if 0
				if((bv[ainum].behaveflag & (DAMAGERADAR|DAMAGEENGINE|DAMAGECONTROLSURFACE))  /*  RETURN TO BASE  */
						|| ((ps[ainum].mcnt[0] == 0) && (ps[ainum].mcnt[1] == 0))) {
					ps[ainum].status |= BACKTOBASE;
					bv[ainum].Behavior = AIGoHome;
					TXA(gstr438);
					Message(Txt,WINGMAN_MSG);
					return(1);
				}
				else {
					TXA(gstr439);  /*  Plane Is FINE  */
					Message(Txt,WINGMAN_MSG);
/*					bv[ainum].Behavior = AICarrierLand;
					bv[ainum].threathead = 100;
					ps[ainum].status |= LANDING;  */
					ps[ainum].status |= BACKTOBASE;
					bv[ainum].Behavior = AIGoHome;
				}
#endif
			break;

#ifdef ELFREFUEL
			case AIDOREFUEL:     /*   SHIFT F5 KEY  */
				GetRefuelPlane();
				bv[ainum].fueltank = 10000;
				TXA(gstr440);
				Message(Txt,WINGMAN_MSG);
			break;
#endif

		}
	}
	return(0);
}

int ProcessAICommMenu(int ainum)
{
	int otherai;
    char Str[80];
    char Str2[80];
	int htemp;
	extern char gstr441[];
	extern char gstr442[];
	extern char gstr443[];
	extern char gstr444[];
	extern char gstr445[];
	extern char gstr446[];
	extern char gstr447[];
	extern char gstr448[];
	extern char gstr449[];
	extern char gstr450[];

    Rp3D->APen   = WHITE;
    Rp3D->FontNr = F_3x5B;

	GetWingName(ainum, Str);

    TXT (Str);


	switch (behavecommands) {
		case AIBRACLEFT:
			bv[ainum].Behavior = BracketLeft;   /*  Wingmen Bracket  */
			if(!(bv[ainum].behaveflag & NO_SWITCH_TARGET)) {
				bv[ainum].target = -1;
				AIRadarForwardCheck(ainum, 0, 0);
			}
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].behaveflag &= ~ENEMYNOFIRE;
			if(bv[ainum].target == -1) {
				bv[ainum].finhead = OurHead;
			}

			TXA(gstr441);
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			AIWingMenu = 0;
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
		break;

		case AIBRACRIGHT:
			bv[ainum].Behavior = BracketRight;   /*  Wingmen Bracket  */
			if(!(bv[ainum].behaveflag & NO_SWITCH_TARGET)) {
				bv[ainum].target = -1;
				AIRadarForwardCheck(ainum, 0, 0);
			}
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].behaveflag &= ~ENEMYNOFIRE;
			if(bv[ainum].target == -1) {
				bv[ainum].finhead = OurHead;
			}

			TXA(gstr442);
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			AIWingMenu = 0;
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
		break;

		case AIFLYSTRAIGHT:
			bv[ainum].Behavior = FlyStraight;   /*  Wingmen Bracket  */
			if(!(bv[ainum].behaveflag & NO_SWITCH_TARGET)) {
				bv[ainum].target = -1;
				AIRadarForwardCheck(ainum, 0, 0);
			}
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].behaveflag &= ~ENEMYNOFIRE;
			if(bv[ainum].target == -1) {
				bv[ainum].finhead = OurHead;
			}

			TXA(gstr443);
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			AIWingMenu = 0;
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
		break;

		case AIFLYLOW:
			AIWingMenu = GETNEGVSEPERATION;
			AIWingMenuTime = Gtime + 5;
			if(OtherAIDo)
				OtherAITime = AIWingMenuTime;
			ShowMenuStuff(AIWingMenu);
		break;

		case AIFLYHIGH:
			AIWingMenu = GETPOSVSEPERATION;
			AIWingMenuTime = Gtime + 5;
			if(OtherAIDo)
				OtherAITime = AIWingMenuTime;
			ShowMenuStuff(AIWingMenu);
		break;

		case AIPARADE:
			bv[ainum].Behavior = Formation;
			bv[ainum].behaveflag &= ~RADARON;
			bv[ainum].behaveflag |= ENEMYNOFIRE;
			bv[ainum].behaveflag &= ~COMBAT_SPREAD;
			bv[ainum].horizontalsep = PARADEDIST;
			TXA(gstr444);
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			AIWingMenu = 0;
			bv[ainum].verticlesep = 0;
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			return(1);
		break;

		case AICOMBATSPREAD:
			bv[ainum].Behavior = Formation;
			bv[ainum].behaveflag |= COMBAT_SPREAD;
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].behaveflag &= ~ENEMYNOFIRE;
			bv[ainum].horizontalsep = COMBATSPREADDIST;
			TXA(gstr445);
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			AIWingMenu = 0;
			ShowAIWeaponStatus(ainum);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			return(1);
		break;

		case AICRUISE:
			bv[ainum].Behavior = Formation;
			bv[ainum].behaveflag &= ~RADARON;
			bv[ainum].behaveflag |= ENEMYNOFIRE;
			bv[ainum].behaveflag &= ~COMBAT_SPREAD;
			bv[ainum].horizontalsep = CRUISEDIST;
			TXA(gstr446);
			AIWingMenu = 0;
			bv[ainum].verticlesep = 0;
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			return(1);
		break;

		case AISANITIZELEFT:
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].finhead = OurHead - 0x4000;
			bv[ainum].Behavior = FlySanitize;
			TXA(gstr447);    /*  SANITIZE DIRECTION  */
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			AIWingMenu = 0;
		break;

		case AISANITIZERIGHT:
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].finhead = OurHead + 0x4000;
			bv[ainum].Behavior = FlySanitize;
			TXA(gstr448);    /*  SANITIZE DIRECTION  */
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			AIWingMenu = 0;
		break;

		case AISANITIZEFRONT:
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].finhead = OurHead;
			bv[ainum].Behavior = FlySanitize;
			TXA(gstr449);    /*  SANITIZE DIRECTION  */
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			AIWingMenu = 0;
		break;

		case AISANITIZEBACK:
			if(!(bv[ainum].behaveflag & DAMAGERADAR))
				bv[ainum].behaveflag |= RADARON;
			bv[ainum].finhead = OurHead + 0x8000;
			bv[ainum].Behavior = FlySanitize;
			TXA(gstr450);    /*  SANITIZE DIRECTION  */
			Message(Txt,WINGMAN_MSG);
			Speech(ROGER);
			if(MyMSGTYP == WING_MENU_MSG) {
				if(msgtime)
					msgtime=GETTIME() - 60;   // End Menu Msg if still there
			}
			AIWingMenu = 0;
		break;
	}
	if(behavecommands >= AINEGVSEP1000) {
		DoVSeperation(ainum);
		if(MyMSGTYP == WING_MENU_MSG) {
			if(msgtime)
				msgtime=GETTIME() - 60;   // End Menu Msg if still there
		}
		AIWingMenu = 0;
	}
	return(0);
}

void DoVSeperation(int ainum)
{
	int htemp;
	extern char gstr451[];
	extern char gstr452[];
	extern char gstr453[];

	if(behavecommands < 61) {
		bv[ainum].verticlesep = -250 * (behavecommands - 50);
		htemp = abs(bv[i].verticlesep)* 4;
		TXA(gstr451);
	}
	else {
		bv[ainum].verticlesep = 250 * (behavecommands - 60);
		htemp = abs(bv[i].verticlesep)* 4;
		TXA(gstr452);
	}
	TXN(htemp);
	TXA(gstr453);
	Message(Txt,WINGMAN_MSG);
	Speech(ROGER);
}

void LowBombTarget()
{

	AIBombTarget();

	if(ps[i].alt < 95) {
		pdest = 0x2000 - psi->pitch;
		pdest = 0x2000;
	}
	else if(ps[i].alt > 125) {
//		pdest = 0xD000 - psi->pitch;
		pdest = 0xD000;
	}
	else
		pdest = 0;

}

void AIGetBombDist()
{
	int dx, dy, dz;
	int relangle, rrelangle, enemyhead;
	int tangle, tpitch, troll, enemyalt;
	extern char gstr454[];
	extern char gstr455[];
	extern char gstr389[];
	extern char gstr456[];
	COORD tdist, distnm, distkm;
	int wingman;
	long xval, yval;
	UWORD zval;

	bv[i].basespeed = ptype->speed;

	if(bv[i].groundt == -1) {
		ps[i].status |= BACKTOBASE;
		bv[i].Behavior = AIGoHome;
#ifdef SHOWHOME
		TXT(gstr454);
		TXN(i);
		Message(Txt,DEBUG_MSG);
#endif
		GetAIBase(i);
		return;
	}
	else {
		GetWayXYZ(bv[i].groundt, &xval, &yval, &zval);
		dx = (xval + 16) >> HTOV;
		dy = (yval + 16) >> HTOV;
		dx = dx - psi->x;
		dy = dy - psi->y;
	}

	GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &distnm, &enemyalt, 0);

	tangle += 0x8000;

	relangle = tangle - psi->head;

	if(distnm > 5) {
		bv[i].Behavior = LowBombTarget;
	}

	TurnAIPlaneBy(i, relangle);

	if(((int)LDist2D(xval, yval) >> 2) + 0x23a > ps[i].alt) {
		pdest = 0x2000;
	}
	else if(((int)LDist2D(xval, yval) >> 2) + 0x2bc < ps[i].alt) {
		pdest = 0xD000;
	}
	else
		pdest = 0;

#if ELFBMSG
	if(tempvar) {
		TXT(gstr455);
		TXN(i);
		TXA(gstr389);
		TXN(tdist);
		TXA(gstr456);
		TXN(relangle);
//		TXA(", x ");
//		TXN(aiwaypoints[bv[i].groundt].x);
//		TXA(", y ");
//		TXN(aiwaypoints[bv[i].groundt].y);
		Message(Txt,DEBUG_MSG);
	}
#endif
}

void AIBombTarget()
{
	int dx, dy, dz;
	int relangle, rrelangle, enemyhead;
	int tangle, tpitch, troll, enemyalt;
	extern char gstr457[];
	extern char gstr458[];
	extern char gstr459[];
	extern char gstr389[];
	extern char gstr456[];
	COORD tdist, distnm, distkm;
	int wingman;
	int bandit;
	UWORD distval;
	int bombalt;
	int secplace;
	long xval, yval;
	UWORD zval;
	int coverplane;
	int makecap = 0;

	secplace = (bv[i].behaveflag & 15);

	bandit = CheckDetection(i);
	if(bandit != -999) {
		if(bandit == -1) {
    		dx = Px- ps[i].x;
    		dy = Py- ps[i].y;
    		tdist = xydist(dx,dy);
		}
		else {
			tdist = xydist(ps[bandit].x- ps[i].x, ps[bandit].y- ps[i].y);
		}
		if(tdist < 450) { /*  10 nm  */
			ShowJettison(i);
			if(CheckAIMissiles(i) != -1) {
				bv[i].Behavior = StraightAttack;
			}
			else {
				ps[i].status |= BACKTOBASE;
#if 0
				TXT(gstr457);
				TXN(i);
				Message(Txt,DEBUG_MSG);
#endif
				bv[i].Behavior = AIGoHome;
				GetAIBase(i);
			}
		}
	}

	bv[i].basespeed = ptype->speed / 2;

	if(bv[i].groundt == -1) {
		if(GetSquadLeader(i) != -1) {
			ps[i].status |= BACKTOBASE;
			bv[i].Behavior = AIGoHome;
#ifdef SHOWHOME
			TXT(gstr458);
			TXN(i);
			Message(Txt,DEBUG_MSG);
#endif
			GetAIBase(i);
		}
		return;
	}
	else {
		GetWayXYZ(bv[i].groundt, &xval, &yval, &zval);
		dx = ((long)xval + 16) >> HTOV;
		dy = ((long)yval + 16) >> HTOV;
		dx = dx - psi->x;
		dy = dy - psi->y;
	}

	GetTargADP(i, dx, dy, &tdist, &tpitch, &tangle, &distnm, &enemyalt, 0);

	relangle = tangle - psi->head;

	if((distnm > 10) && (distnm < 16)) {
		if(secplace < 3)
			relangle += 0x2000;
		else if(secplace < 5)
			relangle -= 0x2000;
	}

	if((distnm < 2) && (abs(relangle) > 0x1000)) {
		bv[i].Behavior = AIGetBombDist;
	}

	TurnAIPlaneBy(i, relangle);

	if(distnm < 20) {
		bv[i].basespeed = ptype->speed;
		wingman = bv[i].wingman;
		if(wingman >= 0) {
//			if(((bv[wingman].Behavior == Formation) || (bv[wingman].Behavior == GorillaBomb)) && (bv[wingman].xloc != bv[i].xloc)) {
			if(((CheckAIBombs(wingman) != -1)) && (!IsBombing(wingman)) && (bv[i].groundt == bv[wingman].groundt)) {
				bv[wingman].Behavior = AIBombTarget;
				bv[wingman].groundt = bv[i].groundt;
				bv[wingman].xloc = bv[i].xloc;
				bv[wingman].yloc = bv[i].yloc;
				makecap = 1;
			}
		}
		wingman = bv[i].nextsec;
		if(wingman >= 0) {
//			if(((bv[wingman].Behavior == Formation) || (bv[wingman].Behavior == GorillaBomb)) && (bv[wingman].xloc != bv[i].xloc)) {
//			if(((bv[wingman].Behavior == Formation)) && (!IsBombing(wingman)) && (bv[i].groundt == bv[wingman].groundt)) {
			if(((CheckAIBombs(wingman) != -1)) && (!IsBombing(wingman)) && (bv[i].groundt == bv[wingman].groundt)) {
				bv[wingman].Behavior = AIBombTarget;
				bv[wingman].groundt = bv[i].groundt;
				bv[wingman].xloc = bv[i].xloc;
				bv[wingman].yloc = bv[i].yloc;
				makecap = 1;
			}
		}
		coverplane = bv[i].target;
		if((coverplane >= 0) && (makecap)) {
			if(bv[coverplane].Behavior == GorillaCover) {
				bv[coverplane].Behavior = SetUpCAP1;
				bv[coverplane].xloc = ps[coverplane].xL;
				bv[coverplane].yloc = ps[coverplane].yL;
				bv[coverplane].groundt = -1;
				UpdateInterceptGroup(coverplane, 1);  /*  Might Need to do base here  */
			}
		}

		BOMB_TrackingAI();   /*  Moved Bombing stuff here because if before waypoint check above will be goofed  */
		DESIGNATED_TrackingAI(relangle);
	}
	bombalt = zval;
	distval = (int)LDist2D(xval, yval) >> 2;
	if(ps[i].alt < distval + 0x2bc) {
		if(ps[i].alt < distval + 0x23a)
			pdest = 0x2000;
		else
			pdest = 0;
	}
	else if(ps[i].alt < bombalt) {
		pdest = 0x2000;
	}
	else if(ps[i].alt > bombalt + 10) {
		pdest = 0xD000;
	}
	else
		pdest = 0;

#if ELFBMSG
	if(tempvar) {
		TXT(gstr459);
		TXN(i);
		TXA(gstr389);
		TXN(tdist);
		TXA(gstr456);
		TXN(relangle);
//		TXA(", x ");
//		TXN(aiwaypoints[bv[i].groundt].x);
//		TXA(", y ");
//		TXN(aiwaypoints[bv[i].groundt].y);
		Message(Txt,DEBUG_MSG);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
void BOMB_TrackingAI ()
{
	extern UWORD Alt;
	int  f;
	long  dist;
	int  dpitch;
	int  BombAngle;

/*	if(tempvar) {
		ps[i].x = Px;
		ps[i].y = Py;
	    ps[i].xL = (long) ps[i].x<<HTOV;
    	ps[i].yL = (long) ps[i].y<<HTOV;
		ps[i].pitch = OurPitch;
		ps[i].alt = Alt;
		ps[i].head = OurHead;
		ps[i].speed = AirSpeed / FPStoKNTS;
	}  */


	dpitch = abs(ps[i].pitch);

	f = 32767-Isin(dpitch);
	BombAngle = dpitch + ( (1500L + (ps[i].alt/8)) *TrgMul(f,f) >>15);

	dist = (long)(((long) ps[i].alt * Icos(BombAngle)) /Isin(BombAngle));

	AIBOMBX = ps[i].xL + sinXL (ps[i].head, dist);
	AIBOMBY = ps[i].yL - cosXL (ps[i].head, dist);
}


//////////////////////////////////////////////////////////////////////////
extern int _pd24;
extern int _pd25;
extern int _pd26;
extern int _pd27;
extern int _pd28;
extern int _pd29;
extern int _pd30;
extern int _pd31;
extern int _pd32;
void DESIGNATED_TrackingAI (int relangle)
{
	long dx, dy;
	long  BOMBdist, DROPdist;
	int  TGTdist;
	int  angl;
	int  WentBy;
	static WasInFront=0;
	int tempspeed;
	int showpa;
	int behavenum, formnum, speedval, altval, headval, attack, launch, wpflag;
	extern char gstr460[];

	tempspeed = ps[i].speed * FPStoKNTS;
//	dx = aiwaypoints[bv[i].groundt].x-AIBOMBX;
//	dy = aiwaypoints[bv[i].groundt].y-AIBOMBY;
	if((bv[i].yloc == bv[i].xloc) && (bv[i].xloc == 0)) {
		bv[i].xloc = AIBOMBX;
		bv[i].yloc = AIBOMBY;
	}
	dx = bv[i].xloc-AIBOMBX;
	dy = bv[i].yloc-AIBOMBY;

	BOMBdist = LDist2D (dx, dy);

	angl = ps[i].head - Langle (dx, -dy);
	DROPdist = cosXL (angl, BOMBdist);
	WentBy = DROPdist<0L;
	if (WentBy) DROPdist = -DROPdist;

	AITREL = (long) DROPdist*2*4*16/(tempspeed+1); // 2x time to target intercept line in seconds
	showpa = 2;

	if(AITREL < 6) {
		if(abs(relangle) > 0x800) {
			if(bv[i].Behavior != LowBombTarget) {
				bv[i].Behavior = LowBombTarget;
				return;
			}
		}
		if(abs(relangle) > 0x2000) {
			bv[i].Behavior = LowBombTarget;
			return;
		}
		if(DropAIBomb()) {
			if(GetSquadLeader(i) == -1) {
				GetWingName(i, Txt);
				TXA(gstr460);
				Message(Txt,WINGMAN_MSG);
			}
			GetWayPointChanges(bv[i].groundt, &behavenum, &formnum, &speedval, &altval, &headval, &attack, &launch, &wpflag, -1);
			if(wpflag & DOWPGOAL) {
				LogIt (SLG_WAY_PT_GOAL, i, ps[i].xL, ps[i].yL, ps[i].alt, -(int)bv[i].path, bv[i].groundt, 0L, LOG_AIR);
				UpdateMissionData(0, ptype->PlaneEnumId);
			}
			bv[i].Behavior = FlyWayPoint;
			if(aiwaypoints[bv[i].groundt].x == -2L)
				bv[i].groundt += 2;
			else
				bv[i].groundt ++;
			{
				long gwx, gwy;
				UWORD gwz;
				GetWayXYZ(bv[i].groundt, &gwx, &gwy, &gwz);
				bv[i].xloc = gwx;
				bv[i].yloc = gwy;
			}
			DoNextWayPt(i, bv[i].groundt);
			RestoreCover(i);
#if 0
			if(aiwaypoints[bv[i].groundt].actionIndex != -1) {
				TranslateBehavior(i, aiwaypoints[bv[i].groundt].actionIndex);
//				bv[i].groundt = -1;
//				ps[i].status |= BACKTOBASE;
//				if(bv[i].leader == -999)
//					bv[i].Behavior = StraightAttack;
//				else
//					bv[i].Behavior = Formation;
			}
			else {
				bv[i].Behavior = FlyWayPoint;
			}
#endif
#if 0
			else {
				bv[i].Behavior = AIBombTarget;
				bv[i].xloc = aiwaypoints[bv[i].groundt].x;
				bv[i].yloc = aiwaypoints[bv[i].groundt].y;
			}
#endif
		}
	}
}

int DropAIBomb()
{
	int type;
	int weaponnum;
	int aistation;
    struct Sams *ssk;
    struct sam  *S;
	int missilenum;
	long xval, yval;
	extern char gstr461[];
	UWORD zval;

	aistation = CheckAIBombs(i);

	if(aistation == -1)
	{
		if(CheckAIMissiles(i) != -1) {
			bv[i].Behavior = StraightAttack;
			return(0);
		}
		if(GetSquadLeader(i) != -1) {
			ps[i].status |= BACKTOBASE;
			bv[i].Behavior = AIGoHome;
#ifdef SHOWHOME
			TXT(gstr461);
			TXN(i);
			Message(Txt,DEBUG_MSG);
#endif
			GetAIBase(i);
		}
		bv[i].Behavior = Formation;
		return(0);
	}

	weaponnum = 1;
	if(ptype->misl[aistation] == ML_FFBOMB)
		weaponnum = 1;

    type = ORDNANCE[weaponnum].samtype;

	missilenum = FindEmptyMissileSlot2 (0, NPSAM, i);
    ssk=ss + missilenum;
	if(missilenum == -1) {
	    ssk=ss;
	}
	else {
	    ssk=ss + missilenum;
	}
    if ((ssk->dist==0) && (missilenum != -1)) {         // is missile slot available?
//		GetWayXYZ(bv[i].groundt, &xval, &yval, &zval);

		if(warstatus == 0) {
			if((ps[whichourcarr].status & ALIVE) && (ps[whichourcarr].status & ACTIVE)) {
				if((aiwaypoints[bv[i].groundt].x <= (long)-1) && (aiwaypoints[bv[i].groundt].y == (long)bv[whichourcarr].orgnum)) {
					warstatus = 1;
					LastCall = -4*60;
				}
			}
		}

		ps[i].mcnt[aistation] --;

		LogIt (SLG_AIR_BOMB_DROP, i, ps[i].xL, ps[i].yL, ps[i].alt, -999, ML_FFBOMB, 0L, LOG_AIR);

        firemissile2 (missilenum,
				0,     /* Offsets for drops from Modem guy?????? */
    			ps[i].xL,
       			ps[i].yL,
                ps[i].alt,
                ps[i].head,
                ps[i].pitch,
                ps[i].roll,
                (((ps[i].speed * FPStoKNTS)>>4)>>4) * 11,
                -1,    // used for air-air only !
                i,
                aiwaypoints[bv[i].groundt].x,
                aiwaypoints[bv[i].groundt].y,
				type,
				0);

#if 0
        ssk->x       = psi->xL;
        ssk->y       = psi->yL;
        ssk->alt     = psi->alt;
        ssk->head    = psi->head;
        ssk->pitch   = psi->pitch;
        ssk->roll    = psi->roll;
//        ssk->speed   = S->speed;           /* ACCEL THESE GUYS ?? */
        ssk->speed   = (((psi->speed * FPStoKNTS)>>4)>>4) * 11;           /* ACCEL THESE GUYS ?? */
        ssk->dist    = s[type].range * (825/4);
		ssk->weapon	 = weaponnum;
        ssk->type    = type;
        ssk->source  = i;
        ssk->srctype = 1;
        ssk->target  = 0;
        ssk->tgttype = 0;
        ssk->losing  = 0;
		ssk->tgtx	 = xval;
		ssk->tgty	 = yval;
#endif

		return(1);
	}
	return(0);
}

void TurnAIPlaneBy(int ainum, int relangle)
{
	extern char gstr462[];

#if 0
	if(abs(relangle) > 0x2000) {
		if(relangle < 0)
			droll = 0xB000 - ps[ainum].roll;
		else
			droll = 0x5000 - ps[ainum].roll;
	}
	else {
		droll = relangle - ps[ainum].roll;
		droll = droll << 2;
	}
#endif

	droll = relangle - ps[ainum].roll;
	if(abs(droll) < 0x1000)
		droll = droll << 2;


#if 0
	if((tempvar) && (GetSquadLeader(ainum) == -1)) {
		sprintf(Txt, gstr462, ainum, relangle, droll);
		Message(Txt,DEBUG_MSG);
	}
#endif

}

void InitAddedWingmen (int planenum) {
	int cnt;
	int numwingmen;
	int wingspace;
	int lastplane;
	int lastsec;
	int otherplane = -999;
	int otherplane2 = -999;
	int wingnum;

	MyWingy = -999;

	if(planenum == -999)
		cnt = GetPlayerHelp();
	else
		cnt = planenum;

	if(cnt == -999)
		return;

	bv[cnt].prevsec = -1;
	WingID[1] = cnt;
	bv[cnt].behaveflag += 2;
	wingnum = bv[cnt].wingman;
	if(wingnum >= 0) {
		WingID[2] = wingnum;
		bv[wingnum].behaveflag += 2;
		otherplane2 = bv[wingnum].wingman;
		if(otherplane2 >= 0) {
			bv[wingnum].wingman = -999;
			bv[otherplane2].leader = -999;
			bv[otherplane2].Behavior = bv[cnt].Behavior;
		}
	}

	if(planenum == -999) {
		otherplane = bv[cnt].nextsec;
		if(otherplane >= 0) {
			bv[cnt].nextsec = -999;
			bv[otherplane].prevsec = -999;
			bv[otherplane].Behavior = bv[cnt].Behavior;
		}
	}
}

int CheckAIBombs(int ainum)
{
	int aistation;
//    struct sam  *S;
	struct MissileData far *S;
	struct PlaneData far *aiptype;

	aistation = -1;

	aiptype = GetPlanePtr(ps[ainum].type);
	S = GetMissilePtr(aiptype->misl[0]);
//    S=s+aiptype->misl[0];
	if((S->homing != IR_1) && (S->homing != IR_2)
			&& (S->homing != RDR_1) && (S->homing != RDR_2)
			&& (ps[ainum].mcnt[0]) > 0) {
		aistation = 0;
	}
	else {
		S = GetMissilePtr(aiptype->misl[1]);
		if((S->homing != IR_1) && (S->homing != IR_2)
				&& (S->homing != RDR_1) && (S->homing != RDR_2)
				&& (ps[ainum].mcnt[1]) > 0) {
			aistation = 1;
		}
		else {
			S = GetMissilePtr(aiptype->misl[2]);
			if((S->homing != IR_1) && (S->homing != IR_2)
					&& (S->homing != RDR_1) && (S->homing != RDR_2)
					&& (ps[ainum].mcnt[2]) > 0) {
				aistation = 2;
			}
		}
	}
	return(aistation);
}

int CheckAIMissiles(ainum)
{
	int aistation;
	struct PlaneData far *aiptype;
	struct MissileData far *S;
//  struct sam  *S;

	aistation = -1;

	aiptype = GetPlanePtr(ps[ainum].type);

	S = GetMissilePtr(aiptype->misl[0]);
	if(((S->homing == IR_1) || (S->homing == IR_2)
			|| (S->homing == RDR_1) || (S->homing == RDR_2))
			&& (ps[ainum].mcnt[0]) > 0) {
		aistation = 0;
	}
	else {
		S = GetMissilePtr(aiptype->misl[1]);
		if(((S->homing == IR_1) || (S->homing == IR_2)
				|| (S->homing == RDR_1) || (S->homing == RDR_2))
				&& (ps[ainum].mcnt[1]) > 0) {
			aistation = 1;
		}
		else {
			S = GetMissilePtr(aiptype->misl[2]);
			if(((S->homing == IR_1) || (S->homing == IR_2)
					|| (S->homing == RDR_1) || (S->homing == RDR_2))
					&& (ps[ainum].mcnt[2]) > 0) {
				aistation = 2;
			}
		}
	}
	return(aistation);
}

int NearWingTarget(int wingnum)
{
	int cnt;
	int returnval, x, y, dx, dy, founddist, targetnum;
	int tpitch, tangle, enemyalt, rangle, rrangle, winghead;
	int rangle1, rrangle1;
	COORD tdist, tdist1, tdistnm, tdistnm1;

	if(wingnum == -1) {
		x = Px;
		y = Py;
		winghead = OurHead;
	}
	else {
		x = ps[wingnum].x;
		y = ps[wingnum].y;
		winghead = ps[wingnum].head;
	}

	founddist = 0;
	targetnum = -999;
	tdist = 0;

    for (cnt=0; cnt<NPLANES; cnt++) {
        if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) &&
				(!(ps[cnt].status & (CRASH|CIVILIAN)))) { /* Planes in the air */
			if(!FriendMatch(i, cnt)) {
		    	dx = x- ps[cnt].x;
    			dy = y- ps[cnt].y;
				GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
				rangle = tangle - ps[cnt].head;
       			if ((tdist1<tdist) || ((tdistnm < 11) && (!founddist))) {
					founddist = 1;
           			tdist=tdist1;
					targetnum = cnt;
					rangle1 = rangle;
					rrangle1 = rrangle;
					tdistnm1 = tdistnm;
       			}
			}
		}
	}
	if(founddist) {
		bv[i].target = targetnum;
		bv[i].Behavior = FlyStraight;
		bv[i].behaveflag |= NO_SWITCH_TARGET;
		return(founddist);
	}

	bv[i].behaveflag &= ~NO_SWITCH_TARGET;
	return(0);
}

int AIRadarForwardCheck(int wingnum, int showinfo, int CAPrange)
{
	int otherrange;

	if(CAPrange)
		otherrange = 150;  /*  This is in KM 150 is about 85 nm...   was 40KM  */
	else
		otherrange = 0;

	return(AICoreRadarCheck(wingnum, showinfo, otherrange, 0x2000, 0));
}

int AIRadarRearCheck(int wingnum, int showinfo, unsigned Bearrange)
{
	int otherrange;

	if(Bearrange)
		otherrange = 40;
	else
		otherrange = 0;

	return(AICoreRadarCheck(wingnum, showinfo, otherrange, 0, 0x5000));
}

int AICoreRadarCheck(int wingnum, int showinfo, int otherrange, int lessangle, int greaterangle)
{
	int returnval, x, y, dx, dy, founddist, targetnum;
	int tpitch, tangle, enemyalt, rangle, rrangle, winghead;
	int cnt, rangle1, rrangle1, tangle1;
	extern char gstr463[];
	extern char gstr402[];
	extern char gstr464[];
	COORD tdist, tdist1, tdistnm, tdistnm1, distkm;
	int degreetmp;
	UWORD dangle;
	struct PlaneData far *wingptype;
	struct RadarData far *tempradar;

	int radarrangekm;

	if(wingnum == -1) {
		x = Px;
		y = Py;
		winghead = OurHead;
		radarrangekm = 150;
	} else {
		x = ps[wingnum].x;
		y = ps[wingnum].y;
		winghead = ps[wingnum].head;

		wingptype = GetPlanePtr(ps[wingnum].type);
		tempradar = GetRadarPtr(wingptype->rclass);
		radarrangekm = tempradar->range;
		/*  Could Put Diff Code here so CAP less effective  */
	}
	if((otherrange) && (radarrangekm > otherrange))
		radarrangekm = otherrange;  /*  intercept when enemy's in 60 nm  */

	if(!(bv[i].behaveflag & RADARON)) {
		radarrangekm = 10;
	}

	founddist = 0;
	targetnum = -999;
	tdist = 0;

	if(!(ps[wingnum].status & FRIENDLY)) {
    	dx = Px-x;
    	dy = Py-y;
		GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
		distkm = ((long)tdist1*32)/825;
		rangle = tangle - winghead;
		if((distkm < radarrangekm) && (((abs(rangle) < lessangle) || (!lessangle)) && ((abs(rangle) > greaterangle) || (!greaterangle)))){
			founddist = 1;
           	tdist=tdist1;
			targetnum = -1;
			rangle1 = rangle;
			rrangle1 = rrangle;
			tdistnm1 = tdistnm;
			tangle1 = tangle;
		}
	}


    for (cnt=0; cnt<NPLANES; cnt++) {
        if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) &&
				(ps[cnt].speed)) { /* Planes in the air */
			if(!FriendMatch(i, cnt)) {
		    	dx = ps[cnt].x - x;
    			dy = ps[cnt].y - y;
				GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
				distkm = (COORD)(((long)tdist1*32)/825);
				rangle = tangle - winghead;
       			if ((((tdist1<tdist) || ((distkm < radarrangekm) && (!founddist))) && (((abs(rangle) < lessangle) || (!lessangle)) && ((abs(rangle) > greaterangle) || (!greaterangle))))) {
					founddist = 1;
           			tdist=tdist1;
					targetnum = cnt;
					rangle1 = rangle;
					rrangle1 = rrangle;
					tdistnm1 = tdistnm;
					tangle1 = tangle;
       			}
			}
		}
	}
	if(founddist) {
		dangle = (UWORD)tangle1;
    	degreetmp = TrgMul (dangle>>1, 360);
		if(!(bv[wingnum].behaveflag & NO_SWITCH_TARGET)) {  /*  Check If we want to check for No switch  */
			bv[i].target = targetnum;
		}

		if(showinfo) {
			TXA(gstr463);
			TXN(degreetmp);
			TXA(gstr402);
			TXN(tdistnm1);
			Message(Txt,GAME_MSG);
		}
		return(founddist);
	}
	else {
		if(showinfo) {
			Speech(A_CLEAN);
			TXA(gstr464);
			Message(Txt,GAME_MSG);
		}
	}

	return(0);
}


void GiveSanitizeMessage(int wingnum, int groupcheck[])
{
	int returnval, x, y, dx, dy, founddist, targetnum;
	int tpitch, tangle, enemyalt, rangle, rrangle, winghead;
	extern int NPLANESx;
	int cnt, rangle1, rrangle1, tangle1;
	extern char gstr465[];
	extern char gstr466[];
	extern char gstr467[];
	extern char gstr468[];
	extern char gstr469[];
	extern char gstr470[];
	extern char gstr402[];
	extern char gstr471[];
	extern char gstr472[];
	COORD tdist, tdist1, tdistnm, tdistnm1;
	int degreetmp, groupcnt, groupalt;
	UWORD dangle;

	if(wingnum == -1) {
		x = Px;
		y = Py;
		winghead = OurHead;
	}
	else {
		x = ps[wingnum].x;
		y = ps[wingnum].y;
		winghead = ps[wingnum].head;
	}

	founddist = 0;
	targetnum = -999;
	tdist = 0;
	groupcnt = 0;

    for (cnt=0; NPLANESx>cnt; cnt++) {
		if(groupcheck[cnt]) {
			groupcnt ++;
		    dx = ps[cnt].x - x;
    		dy = ps[cnt].y - y;
			GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
			rangle = tangle - winghead;
       		if ((((tdist1<tdist) || ((tdistnm < 80) && (!founddist))) && (abs(rangle) < 0x2000))) {
				founddist = 1;
           		tdist=tdist1;
				targetnum = cnt;
				rangle1 = rangle;
				rrangle1 = rrangle;
				tdistnm1 = tdistnm;
				tangle1 = tangle;
				groupalt = (ps[cnt].alt * 4) / 1000;
				if(groupalt < 1)
					groupalt = 1;
       		}
		}
	}
	if(founddist) {
		dangle = (UWORD)tangle1;
    	degreetmp = TrgMul (dangle>>1, 360);
		if(!(bv[wingnum].behaveflag & NO_SWITCH_TARGET)) {  /*  Check If we want to check for No switch  */
			bv[wingnum].target = targetnum;
		}

		Speech(CONTACT);
		NSpeech(groupcnt, 0);
//        BearingToText (dangle,0);
		Speech(TARGET_RANGE);
        if (tdistnm1>=160) {
            TXA (gstr465);
            Speech (C_160);
        } else if (tdistnm1>=120) {
            TXA (gstr466);
            Speech (C_120);
        } else if (tdistnm1>=80) {
            TXA (gstr467);
            Speech (C_80);
        } else if (tdistnm1>=40) {
            TXA (gstr468);
            Speech (C_40);
        } else {
            NSpeech (tdistnm1, 0);
        }
		Speech(ALTITUDE);
		NSpeech(groupalt,0);

		GetWingName(wingnum, Txt);
		if(groupcnt > 1)
			TXA(gstr469);
		else
			TXA(gstr470);
		TXN(groupcnt);
		TXA(gstr402);
//		TXN(degreetmp);
//		TXA(", ");
		TXN(tdistnm1);
		TXA(gstr402);
		if(groupalt) {
			TXN(groupalt);
		}
		else {
			TXA(gstr471);
		}
		Message(Txt,WINGMAN_MSG);
	}
	else {
		Speech(A_CLEAN);
		GetWingName(wingnum, Txt);
		TXA(gstr472);
		Message(Txt,WINGMAN_MSG);
	}
}

void AISanitizeGroupCheck(int wingnum, int enemynum, int planecheck[])
{
	int returnval, x, y, dx, dy, founddist, targetnum;
	int tpitch, tangle, enemyalt, rangle, rrangle;
	int cnt, rangle1, rrangle1, tangle1;
	COORD tdist, tdist1, tdistnm, tdistnm1;
	int degreetmp, cnt2;
	UWORD dangle;
	int groupcheck[MAXPLANE];

	for(cnt = 0; cnt < MAXPLANE; cnt++)
		groupcheck[cnt] = 0;

	groupcheck[enemynum] = 1;

	founddist = 1;
	targetnum = -999;

	while(founddist != 0) {
		founddist = 0;
		for(cnt2=0; cnt2<NPLANES; cnt2++) {
			if(groupcheck[cnt2]) {
				if(cnt2 == -1) {  /*  IF WE WANT ENEMY TO SANITIZE WILL HAVE TO ADD PLAYER INTO CHECK  */
					x = Px;			  /*  WE DON'T YET, PROBABLY WILL  */
					y = Py;
				}
				else {
					x = ps[cnt2].x;
					y = ps[cnt2].y;
				}

    			for (cnt=0; cnt<NPLANES; cnt++) {
        			if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) &&
							(ps[cnt].speed) && (groupcheck[cnt] == 0)) { /* Planes in the air */
		    			dx = ps[cnt].x - x;
    					dy = ps[cnt].y - y;
						GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
       					if (tdistnm < 10) {
							groupcheck[cnt] = 1;
							planecheck[cnt] = 1;
							founddist = 1;
       					}
					}
				}
			}
		}
	}
	GiveSanitizeMessage(wingnum, groupcheck);
}

extern int _pd33;
extern int _pd34;
extern int _pd35;
extern int _pd36;
extern int _pd37;
extern int _pd38;
extern int _pd39;
extern int _pd40;
extern int _pd41;
extern int _pd42;
extern int _pd43;
extern int _pd44;
void AISanitizeCheck(int wingnum)
{
	int returnval, x, y, dx, dy, founddist, targetnum;
	int tpitch, tangle, enemyalt, rangle, rrangle, winghead;
	int cnt, rangle1, rrangle1, tangle1;
	extern char gstr464[];
	COORD tdist, tdist1, tdistnm, tdistnm1;
	int degreetmp;
	UWORD dangle;
	int planecheck[MAXPLANE];

	for(cnt = 0; cnt < MAXPLANE; cnt++)
		planecheck[cnt] = 0;

	if(wingnum == -1) {  /*  IF WE WANT ENEMY TO SANITIZE WILL HAVE TO ADD PLAYER INTO CHECK  */
		x = Px;			 /*  WE DON'T YET, PROBABLY WILL  */
		y = Py;
		winghead = OurHead;
	}
	else {
		x = ps[wingnum].x;
		y = ps[wingnum].y;
		winghead = ps[wingnum].head;
	}

	founddist = 0;
	targetnum = -999;

    for (cnt=0; cnt<NPLANES; cnt++) {
        if ((ps[cnt].status & ALIVE) && (ps[cnt].status&ACTIVE) &&
				(ps[cnt].speed) && (planecheck[cnt] == 0)) { /* Planes in the air */
			if(!FriendMatch(wingnum, cnt)) {
		    	dx = ps[cnt].x - x;
    			dy = ps[cnt].y - y;
				GetTargADP(i, dx, dy, &tdist1, &tpitch, &tangle, &tdistnm, &enemyalt, 0);
				rangle = tangle - winghead;
       			if ((tdistnm < 80) && (abs(rangle) < 0x2000)) {
					planecheck[cnt] = 1;
					AISanitizeGroupCheck(wingnum, cnt, planecheck);
					founddist = 1;
       			}
			}
		}
	}
	if(founddist == 0) {
		Speech(A_CLEAN);
		GetWingName(i, Txt);
		TXA(gstr464);
		Message(Txt,WINGMAN_MSG);
	}
}

void FlySanitize()
{
	int relangle;
    char Str[80];

	if((AltSpeedConv(ps[i].alt, Knots)) > 300)
		bv[i].basespeed = (AltSpeedConv(ps[i].alt, Knots)) - 100;
	else
		bv[i].basespeed = 200;

	pdest = 0;
	relangle = bv[i].finhead - ps[i].head;
	TurnAIPlaneBy(i, relangle);

	if((abs(relangle) < 0x500) && (abs(droll < 0x500))) {
		if(bv[i].behavecnt == 0) {
			bv[i].behavecnt = Gtime + ((MAXWINGLEVEL - bv[i].winglevel) * 5);
		}
		else if(bv[i].behavecnt < Gtime) {
			AISanitizeCheck(i);
			bv[i].Behavior = Formation;
			if(bv[i].behaveflag & ENEMYNOFIRE)
				bv[i].behaveflag &= ~RADARON;
		}
	}
}

AIFoxCall (weapon)
{
	extern char gstr473[];
	extern char gstr474[];
	extern char gstr475[];
    switch (weapon) {
        case ML_AIM7M:
			TXA (gstr473);  Speech (FOX1);         break;
        case ML_AIM9M:
			TXA (gstr474);  Speech (FOX2);         break;
        case ML_AIM120:
			TXA (gstr475);  Speech (FOX3);         break;
        case ML_AIM54C:
			TXA (gstr475);  Speech (FOX3);         break;
    }
    Message(Txt,WINGMAN_MSG);
}

void GetWingName(int ainum, char *Str)
{
	extern char gstr476[];
	extern char gstr477[];
	strcpy (Str, OurWingName);

	if(WingID[0] == ainum) {
		return;
	}
	if(WingID[1] == ainum) {
		strcpy(Str,gstr476);
	}
	else if(WingID[2] == ainum) {
		strcpy(Str,gstr477);
	}
}

void ChangeGroupBehavior(int ainum, int matchenemyfire)
{
	int prewing;
	int combatspread = 0;
	int ishelo = 0;
	int radaron = 0;
	int enemynofire = 0;
	int pitchup = 0;
	int wingcnt;

	prewing = GetSquadLeader(ainum);
	if(bv[ainum].behaveflag & RADARON)
		radaron = 1;
	if(bv[ainum].behaveflag & ENEMYNOFIRE)
		enemynofire = 1;
	if(bv[ainum].behaveflag & COMBAT_SPREAD)
		combatspread = 1;
	if(bv[ainum].behaveflag & ISHELICOPTER)
		ishelo = 1;
	if(ps[ainum].status & PITCHUP)
		pitchup = 1;

	wingcnt = prewing;
	while (prewing >= 0) {
		while (wingcnt >= 0) {
			if(!(ps[wingcnt].status & BACKTOBASE)) {
				if((!matchenemyfire) || ((bv[wingcnt].behaveflag & ENEMYNOFIRE) == (bv[ainum].behaveflag & ENEMYNOFIRE))) {
					bv[wingcnt].Behavior = bv[ainum].Behavior;
					if(!(ps[wingcnt].status & TAKEOFF))
						bv[wingcnt].varcnt = 0;

					if(radaron)
						bv[wingcnt].behaveflag |= RADARON;
					else
						bv[wingcnt].behaveflag &= ~RADARON;
					if(enemynofire)
						bv[wingcnt].behaveflag |= ENEMYNOFIRE;
					else
						bv[wingcnt].behaveflag &= ~ENEMYNOFIRE;
					if(combatspread)
						bv[wingcnt].behaveflag |= COMBAT_SPREAD;
					else
						bv[wingcnt].behaveflag &= ~COMBAT_SPREAD;
					if(ishelo)
						bv[wingcnt].behaveflag |= ISHELICOPTER;
					else
						bv[wingcnt].behaveflag &= ~ISHELICOPTER;

					if(pitchup)
						ps[wingcnt].status |= PITCHUP;

					bv[wingcnt].horizontalsep = bv[ainum].horizontalsep;
					if(!(ps[wingcnt].status & TAKEOFF))
						bv[wingcnt].groundt = bv[ainum].groundt;
				}
			}

			wingcnt = bv[wingcnt].wingman;
		}
		prewing = bv[prewing].nextsec;
		wingcnt = prewing;
	}
}

int AddSomePlanes2 (int planenum, int planeid, int AIBehavior, int formnum, int numadd, int numwingadd, int isfriend, int whattype, long xval, long yval, int altval, int headval, int path, int speedval, int howgood)
{
	int cnt;
	int numwingmen;
	int wingspace;
	int lastplane;
	int lastsec;
	int returnval = -999;
	int startval, endval;
	int imaboat;
	int incval;
	int notdone = 1;
	struct PlaneData far *tempplane;

	numwingmen = 0;
    wingspace = (Direction==1 ? 5:-5);

	tempplane = GetPlanePtr(planeid);

	if(IsBoat(planeid)) {
		startval = MAXPLANE - 1;
		endval = NPLANES;
		imaboat = 1;
		incval = -1;
	}
	else {
		startval = 0;
		endval = LASTPLANE;
		imaboat = 0;
		incval = 1;
	}

	for(cnt = startval; notdone != 0; cnt += incval) {  /*  Had been cnt = 1 for a while  */
		if(imaboat) {
			if(cnt <= endval) {
				notdone = 0;
			}
		}
		else {
			if(cnt >= endval) {
				notdone = 0;
			}
		}

    	if ((notdone) && (whichourcarr != cnt) && !((ps[cnt].status&ACTIVE) && (ps[cnt].status & ALIVE))) { /* Planes in the air */
    		ps[cnt].status = ALIVE+ACTIVE;
			if(tempplane->bclass == CIVILIAN_OBJCLASS)
    			ps[cnt].status |= CIVILIAN;
			if((NPLANES <= cnt) && (!imaboat))
				NPLANES = cnt + 1;
			if((LASTPLANE >= cnt) && (imaboat))
				LASTPLANE = cnt;

			if(isfriend == SS_FRIENDLY) {
				ps[cnt].status |= FRIENDLY;
				ps[cnt].status &= ~NEUTRAL;
			}
			else if(isfriend == SS_NEUTRAL) {
				ps[cnt].status |= NEUTRAL;
				ps[cnt].status &= ~FRIENDLY;
			}

    		ActivatePlane (cnt, ms[0].base, planeid);
			if(whattype == ST_DAMAGED) {
				ps[cnt].mcnt[0] = 0;
				ps[cnt].mcnt[1] = 0;
			}

			if(altval > 6 || imaboat)
        		ps[cnt].alt = altval;
			else
				ps[cnt].alt = 6;
			ps[cnt].head = headval;
			bv[cnt].threathead = headval;
			if(speedval > 0) {
	    		ps[cnt].speed = AltSpeedConv(ps[cnt].alt, speedval);
				bv[cnt].basespeed = AltSpeedConv(ps[cnt].alt, speedval);
			}
			else {
	    		ps[cnt].speed = 0;
				bv[cnt].basespeed = 0;
			}

			if(whattype == ST_WINGMAN) {
				if(numwingmen < 3)
					WingID[numwingmen] = cnt;
				AIWingMan = cnt;
				bv[cnt].leader = -1;
				bv[cnt].behaveflag += 2 + numwingmen;
				if(ps[cnt].alt < 25) {
					ps[cnt].alt = 25;
				}
				if(bv[cnt].basespeed < 200) {
					bv[cnt].basespeed = 200;
					ps[cnt].speed = 200;
				}
			}
			else
				bv[cnt].behaveflag += 1 + numwingmen;

			if(altval != 0 || imaboat)
				ps[cnt].status &= ~TAKEOFF;

			TranslateBehavior(cnt, AIBehavior, formnum);
			if(isfriend == SS_NEUTRAL)
				bv[cnt].behaveflag |= ENEMYNOFIRE;
			bv[cnt].path = path;
			if(path != -1) {
				bv[cnt].groundt = aipaths[path].wp0;
			}
			else {
				bv[cnt].groundt = -1;
			}

//			DoNextWayPt(cnt, bv[cnt].groundt);

			bv[cnt].xloc = Rdrs[ms[0].base].x;
    		bv[cnt].yloc = Rdrs[ms[0].base].y;
			ps[cnt].xL = xval;
			ps[cnt].x = xval>>HTOV;
			ps[cnt].yL = yval;
			ps[cnt].y = yval>>HTOV;
			bv[cnt].orgnum = (char)planenum;
			GetAIBase(cnt);
			bv[cnt].winglevel = howgood;

			if(whattype == ST_READY_5 && !imaboat)
				bv[cnt].Behavior = Ready5;

			if(AIBehavior != SA_CRUISE_MISS) {
				if(numwingadd > 0)
					bv[cnt].wingman = 0;
				DoNextWayPt(cnt, bv[cnt].groundt);
				bv[cnt].wingman = -999;
			}

#ifdef SEEIFWENEEDTHIS
			if(tempplane->PlaneEnumId == SO_KHAWKA) {
				whichourcarr = cnt;     // holds value for which boat our carrier
				wps[0].xL = xval;
				wps[0].yL = yval;
				wps[0].x = xval>>HTOV;
				wps[0].y = yval>>HTOV;
				UpdateOurCarrier(cnt);
			}
#endif

			if(returnval == -999)
				returnval = cnt;

			if(whattype == ST_READY_5) {
				bv[cnt].Behavior = Ready5;
				SetUpLaunch(cnt, bv[cnt].groundt);
//				ps[cnt].status &= ~TAKEOFF;
			}

			if(whattype == ST_NOFIRE)
				bv[cnt].behaveflag |= ISHELICOPTER;

			if(numwingadd > 0)
				SetUpWingman(cnt, ps[cnt].base, numwingadd, planeid);
		/*			if(AIBehavior < B_CAP) {
				bv[cnt].Behavior = FlyWayPoint;
			}  */
			numwingmen ++;
			if(numwingmen > (numadd - 1)) {
				break;
			}
		}
	}

	return(returnval);
}

#define MAXHELOPITCH 0x1200
void UpdateHelicopterHPR_SandP () {
    int a;
    long spd, Gspd;
	long dx, dy;
	long spd2;
	long speed2;
	long tspd;
	long precval;
	long precval2;
    int n;
	int fdroll;
	int temppitch;
	int temppitch2;
	int dpitchrange;
	long temppitchl;
	long temppitchl2;


/* Update ROLL and HEADING */
	fdroll = droll*4/FrameRate;
   	psi->roll += fdroll;
    psi->head += (psi->roll>>3)/FrameRate;

    dpitch = pdest;  /*  In this case pitch will show speed so */
					 /*  pdest will be used instead of pitch to change alt  */

	if((abs(dpitch) > 0x200) || (abs(dpitch) > 0x200) || (!IsCAPFlier(i) && (!(bv[i].behaveflag & ENEMYNOFIRE))))
		ps[i].status &= ~LOWDROLL;
	else
		ps[i].status |= LOWDROLL;

	/* Update Speed and Position */
	if(((ailoc[i].xloc / AILOCDIV) != ps[i].xL) || ((ailoc[i].xloc / AILOCDIV) != ps[i].xL)) {
		ailoc[i].xloc = ps[i].xL * AILOCDIV;
		ailoc[i].yloc = ps[i].yL * AILOCDIV;
	}

	if(bv[i].behaveflag & DAMAGEENGINE) {
		if(bv[i].basespeed > (ptype->speed / 2))
			bv[i].basespeed = ptype->speed / 2;
	}

    if (!(psi->status&(TAKEOFF|LANDING|LANDED))) {
//        spd = ((long)ptype->speed*(0x8000-(psi->pitch/2))) >>15;
//        spd = ((long)bv[i].basespeed*(0x8000-(psi->pitch/2))) >>15;

		temppitchl2 = -((long)ps[i].pitch * 1000);
		temppitchl = temppitchl2 / MAXHELOPITCH;
//		sprintf(Txt, "tpl %ld, tpl2 %ld, p %d", temppitchl, temppitchl2, ps[i].pitch);
        spd = ((long)ptype->speed*temppitchl)/1000;

		if(spd < 0L)
			spd = 0L;

		if(abs(psi->roll) > 0xA00)
	        spd -= labs( sinXL(psi->roll,spd)) >>1;

        if (REALFLIGHT) {
            a = rng(psi->alt, 5000/4, 30000/4) - 5000/4;
            spd -= ((long)a * spd) / (20*2500L);
        }

		if(spd < 0L)
			spd = 0L;

        psi->speed = (int)spd;
		if(psi->speed == 0)
			psi->speed = 1;
    }

//    spd = (psi->speed*27 >>6) /FrameRate;

    if (psi->alt>2500) {     /* Too high, level off */
		if(psi->pitch > 0)
	        dpitch = 0;
    }

/* Trail smoke if CRASHing */
    if (psi->status & CRASH) {
//        dpitch=-2<<8;
		dpitch = 0;
		psi->head += 0x1000;
		psi->alt -= 40;
    }

#if 0
    Gspd = cosX (dpitch,spd);

    psi->xL += sinX(psi->head,Gspd);
    psi->yL -= cosX(psi->head,Gspd);
    psi->alt += sinX(dpitch,spd);

    psi->x = (psi->xL+16) >> HTOV;
    psi->y = (psi->yL+16) >> HTOV;
#endif
	if(Speedy == 1) {
	    tspd = (((long)psi->speed*27 >>6) * Fticks * AILOCDIV) /60;
		if(tspd >= 0)
			spd = tspd;
		else
			spd = 0L;
    	speed2 = (((long)psi->speed*27 >>6) * Fticks) /60;
	    Gspd = cosXL (dpitch, spd);
		if(Gspd < 0L)
			Gspd = 0L;
    	ailoc[i].xloc += sinXL(psi->head,Gspd);
    	ailoc[i].yloc -= cosXL(psi->head,Gspd);
		ps[i].xL = ailoc[i].xloc / AILOCDIV;
		ps[i].yL = ailoc[i].yloc / AILOCDIV;
		if(bv[i].basespeed > 3)
		    psi->alt += sinXL(dpitch,(spd/AILOCDIV));
		else {
	    	psi->alt += sinXL(dpitch,(spd/AILOCDIV)) + sinX(dpitch, 7);
		}
	}
	else {
    	speed2 = (((long)psi->speed*27 >>6) * Fticks) /60;
		if(speed2 < 0L)
			speed2 = 0L;
	    Gspd = cosXL (dpitch, speed2);
		if(Gspd < 0L)
			Gspd = 0L;
	    psi->xL += sinXL(psi->head,Gspd);
    	psi->yL -= cosXL(psi->head,Gspd);
		if(bv[i].basespeed > 3)
	    	psi->alt += sinXL(dpitch, speed2);
		else {
	    	psi->alt += sinXL(dpitch, speed2) + sinX(dpitch, 7);
		}
	}

	if(GetSquadLeader(i) != -1) {
		ailoc[i].xloc = ((long)ps[i].xL * AILOCDIV) + (AILOCDIV>>1);
		ailoc[i].yloc = ((long)ps[i].yL * AILOCDIV) + (AILOCDIV>>1);
	}
	else {
		precval = FPARAMS.X / AILOCDIV;
		precval = FPARAMS.X - (precval * AILOCDIV);
		ailoc[i].xloc = ((long)ps[i].xL * AILOCDIV) + precval;
		precval2 = FPARAMS.Z / AILOCDIV;
		precval2 = FPARAMS.Z - (precval2 * AILOCDIV);
		ailoc[i].yloc = ((long)ps[i].yL * AILOCDIV) + precval2;
		lfpx = FPARAMS.X;
		lfpy = FPARAMS.Z;
		lox = ailoc[i].xloc;
		loy = ailoc[i].yloc;
	}

    psi->x = (psi->xL+16) >> HTOV;
    psi->y = (psi->yL+16) >> HTOV;


/* Update PITCH */

	temppitchl = -(((long)psi->speed * 1000) / ptype->speed);
	temppitchl = (temppitchl * MAXHELOPITCH) / 1000;
	temppitchl2 = -(((long)bv[i].basespeed * 1000) / ptype->speed);
	temppitchl2 = (temppitchl2 * MAXHELOPITCH) / 1000;
	temppitch = (temppitchl2 - temppitchl);
	dpitchrange = 0x1000/FrameRate;
    temppitch = rng2 (temppitch, -dpitchrange, dpitchrange);
    psi->pitch += temppitch;
    psi->pitch = rng2 (psi->pitch, -MAXHELOPITCH, 0x800);

    if (abs(psi->pitch)>0x4000) {
        psi->head += 0x8000;
        psi->roll += 0x8000;
        psi->pitch = 0x8000 - psi->pitch;
    }

    if (psi->status & CRASH) {
		StartASmoke( psi->xL, psi->alt, psi->yL, SMKTRAIL, 5 );
    }

	if (psi->alt<0) {  /* Too low, hit the ground */
        psi->status &= PERMANENTBITS;

		if(CloseEnoughFor(SNDSTUFF, psi->x, psi->y))
        	Sound (N_FarExplo, 2, 0);
		StartASmoke( psi->xL, psi->alt,psi->yL, GRND2XPLO, 17 );

        if (atarg==i) atarg=-1;
    }
}


void UpdateI(int newi)
{
	i = newi;
	psi = ps + i;
}

int ImNotaFighter(int planenum, int checkguns)
{
	struct PlaneData far *tempplane;

	tempplane = GetPlanePtr(ps[planenum].type);

	if((tempplane->bclass == FIGHTER_OBJCLASS) || (tempplane->bclass == HELICOPTER_OBJCLASS))
		return(0);

	if(tempplane->bclass == BOMBER_OBJCLASS) {
		if((CheckAIMissiles(planenum) != -1) && (CheckAIBombs(planenum)== -1))
			return(0);
	}

	if(!checkguns)
		return(1);

	if(tempplane->gun != NO_GUN)
		return(1);

	return(0);
}

