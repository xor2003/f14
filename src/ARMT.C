//***************************************************************************
//
//  ARMT.C
//
//  ARMT screen & onboard weapons logic
//
//  Microprose Software, Inc.
//  180 Lakefront Drive
//  Hunt Valley, Maryland  21030
//
//***************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include "types.h"
#include "library.h"
#include "keycodes.h"
#include "world.h"
#include "sdata.h"
#include "sounds.h"
#include "f15defs.h"
#include "math.h"
#include "armt.h"
#include "awg9.h"

extern   RPS         *RpCRT;
extern   int         PLYR_M[];
extern   long        PxLng;
extern   long        PyLng;
extern   int         Alt;
extern   int         OurHead;
extern   int         OurPitch;
extern   UWORD       AirSpeed;
extern   long        HarmX;
extern   int         OurRoll;
extern   UWORD	LastSlotNMount;
extern   int		 MasterArmSW;

UWORD   LastSlotNMount;

int      MODE_MASTER    = MASTER_NAV;
int      MODE_NAV       = NAV_NAV;
extern int MODE_AA;


//***************************************************************************
extern int F14_STATION[F14_STATION_NUM];


int OurLoadOut=0;

extern WEAPON ORDNANCE[W_TYPES];


LAUNCHPOS   LP;

//***************************************************************************
ARMT_MODE(int MODE)
{
	MODE_MASTER = MODE;

	if (MODE_MASTER >= MASTER_N)
		MODE_MASTER = 0;
#ifdef YEP
	switch (MODE_MASTER) {
		case MASTER_NAV:
			AWG9_CHANGE_MODE(AWG9_SRCH_WIDE);
		break;

		case MASTER_AA:
			AWG9_CHANGE_MODE(AWG9_SRCH_WIDE);
		break;
	}
#endif
}

//***************************************************************************
AA_MODE(int MODE)
{
	if (MODE_MASTER!=MASTER_AA)
		ARMT_MODE(MASTER_AA);
	MODE_AA = MODE;
}

//***************************************************************************
ARMT_INIT()
{
	// If easy mode, then turn master arm switch on
	if (AWG9_REALITY_LEVEL == AWG9_EASY) {
		MasterArmSW=1;
	}
	load_aa_stations();
}

//***************************************************************************
int load_aa_stations()
{
	switch(OurLoadOut) {
		case 0:  // Fleet Defense Bravo
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_54;
			F14_STATION[3] = W_AIM_54;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_54;
	   		F14_STATION[7] = W_AIM_54;
			F14_STATION[8] = W_AIM_54;
			F14_STATION[9] = W_AIM_54;
			F14_STATION[10] = -1;
			F14_STATION[11] = -1;
		break;

		case 1:	// MIG CAP Bravo
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_7;
			F14_STATION[3] = W_AIM_7;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_7;
	   		F14_STATION[7] = W_AIM_7;
			F14_STATION[8] = -1;
			F14_STATION[9] = -1;
			F14_STATION[10] = W_AIM_7;
			F14_STATION[11] = W_AIM_7;
		break;
		case 2: // Fleet Defense Alpha
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_7;
			F14_STATION[3] = W_AIM_7;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_54;
	   		F14_STATION[7] = W_AIM_54;
			F14_STATION[8] = W_AIM_54;
			F14_STATION[9] = W_AIM_54;
			F14_STATION[10] = -1;
			F14_STATION[11] = -1;
		break;
		case 3: // Fleet Defense Charlie
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_9;
			F14_STATION[3] = W_AIM_9;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_54;
	   		F14_STATION[7] = W_AIM_54;
			F14_STATION[8] = -1;
			F14_STATION[9] = -1;
			F14_STATION[10] = -1;
			F14_STATION[11] = W_AIM_7;
		break;
		case 4:  // MIG CAP Charlie
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_9;
			F14_STATION[3] = W_AIM_9;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_54;
	   		F14_STATION[7] = W_AIM_54;
			F14_STATION[8] = W_AIM_54;
			F14_STATION[9] = W_AIM_54;
			F14_STATION[10] = -1;
			F14_STATION[11] = -1;
		break;
		case 5:  // MIG CAP Alpha/TARPS Support
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = W_AIM_9;
			F14_STATION[3] = W_AIM_9;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = W_AIM_7;
	   		F14_STATION[7] = W_AIM_7;
			F14_STATION[8] = -1;
			F14_STATION[9] = -1;
			F14_STATION[10] = W_AIM_7;
			F14_STATION[11] = W_AIM_7;
		break;
		default:
			F14_STATION[0] = W_AIM_9;
			F14_STATION[1] = W_AIM_9;
			F14_STATION[2] = -1;
			F14_STATION[3] = -1;
			F14_STATION[4] = W_FUEL_TANK;
			F14_STATION[5] = W_FUEL_TANK;
			F14_STATION[6] = -1;
	   		F14_STATION[7] = -1;
			F14_STATION[8] = -1;
			F14_STATION[9] = -1;
			F14_STATION[10] = -1;
			F14_STATION[11] = -1;
		break;
	}
	Init3DMissiles();
}

//***************************************************************************
int ARMT_get_priority()
{
	switch (MODE_MASTER) {
		case MASTER_AA:
			return(AAGetPriority());
		case MASTER_NAV:
			return W_NONE;
	}
}

//***************************************************************************
int AAGetPriority()
{
	int p;

	if ((p = AAGetStation()) == -1) return W_NONE;
	return F14_STATION[p];
}


//***************************************************************************
int AAGetStation()
{
	int i;

	for(i=0; i<F14_STATION_NUM; i++)
		switch (F14_STATION[i]) {
			case W_AIM_9:
				if (MODE_AA == AA_SRM) return i;
            break;
			case W_AIM_54:
				if (MODE_AA == AA_LRM) return i;
			break;
			case W_AIM_120:
			case W_AIM_7:
				if (MODE_AA == AA_MRM) return i;
			break;
		}
	return -1;
}


//***************************************************************************
int aa_cnt_weapons()
{
	int i, cnt;

	cnt = 0;

	for(i=0;i<F14_STATION_NUM;i++)
		switch (F14_STATION[i]) {
			case W_AIM_9:
			case W_AIM_120:
			case W_AIM_54:
			case W_AIM_7:
				cnt++;
			break;
		}
	return cnt;
}

//***************************************************************************
static LAUNCHPOS *aa_release()
{
	static int n=0;
	int p;
	long VL[3];

   	if (n > 0) {
		n = 0;
		return (LAUNCHPOS *) 0;
	} else {
		if ((p = AAGetStation()) == -1)
			return(LAUNCHPOS *)0;


 	LastSlotNMount = p;

	n = 1;

	LP.pos[0] = 0;
	LP.pos[1] = 0;
	LP.pos[2] = 0;

	Rotate(LP.pos,PLYR_M);

	LP.weapon = F14_STATION[p];
	F14_STATION[p] = W_NONE;

	return &LP;
	}
}

//***************************************************************************
LAUNCHPOS *ARMT_release()
{
	switch (MODE_MASTER) {
		case MASTER_AA:
			return(aa_release());
		case MASTER_NAV:
			return(LAUNCHPOS *)0;
	}
}

//***************************************************************************
ARMT_jettison_all()
{
    int i;

    int DroppedSomething;

    DroppedSomething = 0;

    if (DroppedSomething) {
        Sound(N_DropBomb,2,0);
    }

    JettisonFuel();
	for (i=0; i<12; i++) {
		if (F14_STATION[i] == W_FUEL_TANK)
			F14_STATION[i] = W_NONE;
	}
}


