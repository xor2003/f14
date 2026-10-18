extern char gstr746[];

#include <stdlib.h>
#include "library.h"
#include "f15defs.h"
#include "world.h"
#include <dos.h>

extern UWORD HudSprite1;

extern UWORD RadarCursorSprite[3];

extern UWORD HudSprite3;

extern UWORD HudSprite4;

extern UWORD HudSprite5;

extern UWORD MouseSprite;

extern UWORD TEWSprites[6];


extern RPS *Rp3D, *RpCRT;
extern	UWORD   GRPAGE0[];
extern	int	    GXMID;
extern	int	    GYMID;
extern	int	    Nviewmat[];
extern	int	    vRoll;
extern	int		ABOVECLOUDS;
extern   int MISSILETRAIL;


extern int InXPLO;

extern int XPLODETAIL;



//***************************************************************************
LoadSprites()
{
    int i,j;

    OpenSprite();
    LoadPicGmem(gstr746,1);

	TEWSprites[0] = BuildSprite(1,34,1,11,11);
	TEWSprites[1] = BuildSprite(1,45,1,11,11);
	TEWSprites[2] = BuildSprite(1,58,2,6,8);
	TEWSprites[3] = BuildSprite(1,69,2,6,8);
	TEWSprites[4] = BuildSprite(1,58,13,3,3);
	TEWSprites[5] = BuildSprite(1,61,13,3,3);

    RadarCursorSprite[0] = BuildSprite(1, 14,1,6,6);
    RadarCursorSprite[2] = BuildSprite(1, 23,2,7,5);

    HudSprite1 = BuildSprite(1,1,0,11,7);
    HudSprite3 = BuildSprite(1,2,13,9,9);
    HudSprite4 = BuildSprite(1,11,13,9,9);

    MouseSprite = BuildSprite (1,27,14,9,11);
    MouseSETUP(MouseSprite,0,0);
	CloseSprite();
}

/****************************************************************************/
ScaleRotate(UWORD SOURCE, int XSC, int YSC, int WID, int HT, RPS *Rp, int XDC, int YDC, int SCALE, int ROLL, int TRNSCLR, int MIRROR)
{
	UWORD	ADDRESS[321],ADDBASE;
	int		i;
	int 	XS,YS,XADD,YADD,TX,TY,SSX,SSY;
	int 	X1,Y1,X2,Y2;
	long	T1,T2;

	if (SCALE<1) SCALE=1;
	T1= ((long)WID*(unsigned long)SCALE)/256L;
	T2= ((long)HT*(unsigned long)SCALE)/256L;

	X1= XDC-T1/2;
	Y1= YDC-T2/2;
//	X2= X1+T1-1;
//	Y2= Y1+T2-1;
	X2= X1+T1;
	Y2= Y1+T2;

	if (X1>(Rp->XBgn+Rp->Width1)) return(1);
	if (Y1>(Rp->YBgn+Rp->Length1)) return(1);
	if (X2<Rp->XBgn) return(1);
	if (Y2<Rp->YBgn) return(1);

	if (X1<Rp->XBgn) X1=Rp->XBgn;
	if (X2>(Rp->XBgn+Rp->Width1)) X2=(Rp->XBgn+Rp->Width1);
	if (Y1<Rp->YBgn) Y1=Rp->YBgn;
	if (Y2>(Rp->YBgn+Rp->Length1)) Y2=(Rp->YBgn+Rp->Length1);

#ifdef YEP
   if (ROLL==0)
      {
      SSX=TX=((256*32)/SCALE);
      SSY=TY=0;
      }
   else
      {
#endif
	   SSX = TX = ACosB(256*32,ROLL)/SCALE;
	   SSY = TY = ASinB(256*32,ROLL)/SCALE;
#ifdef YEP
      }
#endif

	if (MIRROR) TX=-TX;

	for (i=0;i<=(X2-X1);i++) {
		ADDRESS[i]= 320*((i*TY)/32)+ (i*TX)/32;
	}

	for (i=Y1;i<=Y2;i++) {
		ADDBASE= 320*(((X1-XDC)*TY+(i-YDC)*SSX)/32 +YSC) +
					((X1-XDC)*TX)/32+ -((i-YDC)*TY)/32+XSC;
		TransScaleLine(SOURCE, ADDBASE,
			ADDRESS, X1, i, (X2-X1)+1, GRPAGE0[Rp->PageNr],TRNSCLR);
	}
}

//***************************************************************************
TransRectCopy(UWORD SOURCE,int XSL,int YSL,int WID,int HT,RPS *Rp,int XD,int YD,int TRNSCLR,int MIRROR)
{
	UWORD	ADDRESS[321],ADDBASE;
	int		i,j;
	int 	X1,Y1,X2,Y2;
	long	T1,T2;

	X1 = XD;
	Y1 = YD;
	X2 = X1+WID;
	Y2 = Y1+HT;

	if (X1>(Rp->XBgn+Rp->Width1)) return(1);
	if (Y1>(Rp->YBgn+Rp->Length1)) return(1);
	if (X2<Rp->XBgn) return(1);
	if (Y2<Rp->YBgn) return(1);


	if (X1<Rp->XBgn) {
		XSL+= (MIRROR)? -(Rp->XBgn-X1):Rp->XBgn-X1;
		X1=Rp->XBgn;
	}
	if (X2>(Rp->XBgn+Rp->Width1)) X2=(Rp->XBgn+Rp->Width1);
	if (Y1<Rp->YBgn) {
		YSL+= Rp->YBgn-Y1;
		Y1=Rp->YBgn;
	}
	if (Y2>(Rp->YBgn+Rp->Length1)) Y2=(Rp->YBgn+Rp->Length1);

	if (MIRROR) {
   		for (i=0,j=(X2-X1)+XSL;i<=(X2-X1);i++)
         	ADDRESS[i]=j--;
	} else {
   		for (i=0;i<=(X2-X1);i++)
	   		ADDRESS[i]= XSL++;
	}

	ADDBASE=320*YSL;

	for (i=Y1;i<=Y2;i++) {
		TransScaleLine(SOURCE,ADDBASE,ADDRESS,X1,i,(X2-X1)+1,
				GRPAGE0[Rp->PageNr],TRNSCLR);
		ADDBASE+=320;
	}
}



#define	SMKNUM	30

struct SMK{
	long	X;
	int	Y;
	long	Z;
	int	TYP;
	int	CNT;
	int	PIC;
	unsigned int	UPCNT;
};
extern struct SMK SMOKES[SMKNUM];


struct SPRTCRD{
	UWORD PAGE;
	int XC;
	int YC;
	int WID;
	int HT;
	};
extern struct SPRTCRD SPRTVLS[];


struct XPLANM{
	int	START;
	int 	LAST;
	};
extern struct XPLANM XPLOANIMS[];


//***************************************************************************
DrawXploSprite(RPS *Rp, int X, int Y, int NUM, int SCL, int ROLL)
{
	UWORD SEG;

	if (SEG=MapInEMSSprite(SPRTXPLSEG,SPRTVLS[NUM].PAGE))	{
		ScaleRotate( SEG, SPRTVLS[NUM].XC, SPRTVLS[NUM].YC, SPRTVLS[NUM].WID, SPRTVLS[NUM].HT,
			Rp, X, Y, SCL,ROLL, 0, 0 );
	}
}
//***************************************************************************
StartASmoke(long X, UWORD Y, long Z, int TYPE, int CNT)
{
	unsigned int	i,locnt,loi;
	int	R;


	for (loi=0;loi<SMKNUM;loi++) {
		if (SMOKES[loi].CNT<=0) break;
	}
	if (loi==SMKNUM) {
		for (loi=locnt=i=0;loi<SMKNUM;loi++) {
			if (SMOKES[loi].UPCNT>locnt) {
				locnt=SMOKES[loi].UPCNT;
				i=loi;
			}
		}
		loi=i;
	}
	SMOKES[loi].CNT=CNT;
	SMOKES[loi].X=X;
	SMOKES[loi].Y=Y;
	SMOKES[loi].Z=Z;
	SMOKES[loi].TYP=TYPE;
	SMOKES[loi].PIC=-1;
	SMOKES[loi].UPCNT=0;

	switch (SMOKES[loi].TYP) {
		case AIRXPLO:
			R=(Rnd2(255)&3)+3;
			SMOKES[loi].PIC= XPLOANIMS[R].START;
			SMOKES[loi].CNT= XPLOANIMS[R].LAST-XPLOANIMS[R].START;
			break;
		case GRNDXPLO:
			SMOKES[loi].PIC=0;
			SMOKES[loi].Y=50;
			SMOKES[loi].CNT=17;
			break;
		case SMKTRAIL:
		case EXHAUST:
			SMOKES[loi].PIC=XPLOANIMS[(Rnd2(256)&1)+1].START+(Rnd2(256)&3);
			break;
		case SMKPLUME:
			SMOKES[loi].PIC=XPLOANIMS[(Rnd2(256)&1)+1].START;
			break;
		case GRND2XPLO:
			SMOKES[loi].PIC=0;
			SMOKES[loi].Y=50;
			SMOKES[loi].X+=Rnd2(95);
			SMOKES[loi].Z+=Rnd2(95);
			SMOKES[loi].CNT=17;
			break;
		case AIR2XPLO:
			R=(Rnd2(255)&3)+3;
			SMOKES[loi].PIC= XPLOANIMS[R].START;
			SMOKES[loi].CNT= XPLOANIMS[R].LAST-XPLOANIMS[R].START;
			break;
		case CLUSTERXPLO:
			R=3;
			SMOKES[loi].PIC= XPLOANIMS[R].START;
			SMOKES[loi].CNT= 20;
			SMOKES[loi].Y=30;
			break;
		case CLUSTER2XPLO:
			R=3;
			SMOKES[loi].PIC= XPLOANIMS[R].START;
			SMOKES[loi].CNT= (XPLOANIMS[R].LAST-XPLOANIMS[R].START)/4;
			SMOKES[loi].Y=30;
			break;
		case CARRIERXPLO:
			SMOKES[loi].PIC=0;
			SMOKES[loi].Y=50;
			SMOKES[loi].CNT=2;
			break;
		}



}

//***************************************************************************
DrawASmoke(int X, int Y, int Z, int ID, int SCL )
{

	int	V[3];
	int	size;
	long	T;
 	int	R;
	int	XR[3];
	int	i,CNT;

	UWORD D;



	if (SMOKES[ID].TYP==SMKPLUME) {
		D= abs(X)+abs(Y/4)+abs(Z);
		if (D<1000 && Rp3D!=RpCRT) {
			R= 100-(D/10);
			if (R>InXPLO) InXPLO=R;
		}

		CNT= SMOKES[ID].CNT;
		for (i=0;i<4;i++,CNT+=2) {
			if (CNT==0)
				SMOKES[ID].PIC=XPLOANIMS[(Rnd2(256)&1)+1].START;
			XR[1]= Y+ 50*(7-(CNT&7));
			XR[0]= X+ (CNT&0x18)+10*(CNT&7)+ Rnd2(31);
			XR[2]= Z+ (CNT&0x18)+10*(CNT&7)+ Rnd2(31);
			Rotate(XR, Nviewmat);
			if (XR[2]<16) continue;
			if (XR[2]<16) XR[2]=16;
			T= XR[0];
			T= T<<8;
			T= T/(long)XR[2];
			XR[0]=T;
			T= XR[1];
			T= T<<8;
			T= T/(long)XR[2];
			XR[1]=T;
			XR[0]+= GXMID;
			XR[1]+= GYMID;
			size= (((0x7fff/XR[2])/SCL)<<2)+4;
			DrawXploSprite( Rp3D, XR[0], XR[1], SMOKES[ID].PIC+((7-CNT)&7), size,0 );
			if (XPLODETAIL==0) i=4;
		}
		return(1);
	}


	Rotate(&X, Nviewmat, &Y, &Z); /* KLUDGE!!! Really only uses first two parms */
	if (Z<16) return(1);
//	if (Z<256) Z=256;

	T= X;
	T= T<<8;
	T= T/(long)Z;
	X=T;
	T= Y;
	T= T<<8;
	T= T/(long)Z;
	Y=T;

	X+= GXMID;
	Y+= GYMID;

	size= (((0x7fff/Z)/SCL)<<2)+4;
	switch (SMOKES[ID].TYP) {
		case AIRXPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, 3*(size/2),0 );
			break;
		case GRNDXPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, size,vRoll );
			break;
		case SMKTRAIL:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, (size>>1),0 );
			break;
		case EXHAUST:
         if (MISSILETRAIL)
            DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+(SMOKES[ID].UPCNT&3), (size>>2),0);
			break;
		case GRND2XPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, 3*(size/2),vRoll );
			break;
		case AIR2XPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, 3*(size/2),0 );
			break;
		case CLUSTERXPLO:
//			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, (size/3),0 );
			break;
		case CLUSTER2XPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, (size/5),0 );
			break;
		case CARRIERXPLO:
			DrawXploSprite( Rp3D, X, Y, SMOKES[ID].PIC+SMOKES[ID].UPCNT, size>>1,vRoll );
			break;
	}
}

//***************************************************************************
ShowSmokesNoChange(long ViewX, long ViewY, UWORD ViewZ)
{
	int	i;
	int	OLDUP [SMKNUM];
	int	OLDCNT[SMKNUM];

	for (i=0;i<SMKNUM;i++) {
		OLDUP [i]=SMOKES[i].UPCNT;
		OLDCNT[i]=SMOKES[i].CNT;
	}

	HandleSmoke(ViewX, ViewY, ViewZ);

	for (i=0;i<SMKNUM;i++) {
		SMOKES[i].UPCNT=OLDUP [i];
		SMOKES[i].CNT  =OLDCNT[i];
	}
}

//***************************************************************************
HandleSmoke(long ViewX, long ViewY, UWORD ViewZ)
{
	int	i,Y;
	long	X,Z;
	int	V[3];

	for (i=0;i<SMKNUM;i++) {
		if (SMOKES[i].CNT>0) {
			X= SMOKES[i].X-ViewX;
			Y= SMOKES[i].Y-ViewZ;
    		Z = ViewY - (0x100000L-SMOKES[i].Z);
			if ((labs(X)>0x2000) || (labs(Z)>0x2000)) {
				if ((labs(X)<0x3000) && (labs(Z)<0x3000)) {
					if ( !ABOVECLOUDS || (SMOKES[i].TYP==AIRXPLO)
						               || (SMOKES[i].TYP==AIR2XPLO)
						               || (SMOKES[i].TYP==SMKTRAIL)
                                 || (SMOKES[i].TYP==EXHAUST) )
						DrawASmoke((int)X,Y,(int)Z,i, 1);
				}
			} else {
				if (!ABOVECLOUDS || (SMOKES[i].TYP==AIRXPLO)
						            || (SMOKES[i].TYP==AIR2XPLO)
						            || (SMOKES[i].TYP==SMKTRAIL)
						            || (SMOKES[i].TYP==EXHAUST) )
					Stack3Dobject((SPRTXPLSEG<<8), (int) X, Y, (int) Z, i, 1,SPRTXPL_OBJ);
			}


			switch (SMOKES[i].TYP) {
				case AIRXPLO:
					if (SMOKES[i].UPCNT<3)
						StartASmoke( SMOKES[i].X+(long)(Rnd2(127)), SMOKES[i].Y+Rnd2(127), SMOKES[i].Z+(long)(Rnd2(127)),AIR2XPLO, 20 );
					break;
				case GRNDXPLO:
					if (SMOKES[i].CNT<4)
						if ((Rnd2(255)&3))
							StartASmoke( SMOKES[i].X, SMOKES[i].Y, SMOKES[i].Z, GRND2XPLO, 20 );
					if (SMOKES[i].CNT==1)
						StartASmoke( SMOKES[i].X, SMOKES[i].Y, SMOKES[i].Z, SMKPLUME, 2000 );
					break;
				case CLUSTERXPLO:
					if (SMOKES[i].UPCNT<15)
						StartASmoke( SMOKES[i].X+127L-(long)(Rnd2(255)), SMOKES[i].Y, SMOKES[i].Z+127L-(long)(Rnd2(255)),CLUSTER2XPLO, 20 );
					break;
				case SMKPLUME:
					if ( ((SMOKES[i].CNT+Rnd2(255))&0x3c)==0 )
						StartASmoke( SMOKES[i].X, SMOKES[i].Y, SMOKES[i].Z, GRND2XPLO, 20 );
					break;
				case CARRIERXPLO:
					if (SMOKES[i].CNT<4)
						if (!(Rnd2(255)& 31))
							StartASmoke( SMOKES[i].X, SMOKES[i].Y, SMOKES[i].Z, GRND2XPLO, 20 );
					break;
				}

			SMOKES[i].UPCNT++;
			SMOKES[i].CNT--;
		}
	}
}


