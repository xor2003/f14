#include "stdio.h"
#include <dos.h>
#include "library.h"
#include "world.h"
#include "f15defs.h"

#include	<dos.h>
#include <fcntl.h>

extern	char	far MAPBUF  [256] [256]     ;
extern	char	far STAMPBUF[96]  [16]  [16];
extern	char	far GROUPBUF[96]  [16]  [16];
extern	char	far GREYBUF [32]  [256]     ;
extern	char	far TILEBUF [128] [16]  [16];

extern	char	far	*FillTransTbl;


extern UWORD   *PalPtr3D;
extern	long	ViewX,ViewY;

extern char STAMPLIGHTS[96];

extern char OBJSCALES[MAXTARGETS];

extern int OBJDISTS[MAXTARGETS];


extern char far MAPCOLORS[256];


extern char far NULLCOLORS[256];


extern int XYDSP[14*2];
    // Map offsets to make roads appear

extern UBYTE far AAFIRE[];
extern int FrameRate;
extern int InFriendly;

#define MaxAAA 40

extern int AAACnt;


extern UBYTE AAAi[MaxAAA];

extern int AAAX[MaxAAA];

extern int AAAZ[MaxAAA];

extern int AAAY;


extern UWORD REJECTDIST[];


extern UWORD GREYSEG;

extern UWORD CLOUDSEG;


extern	long	XSLine( long A, int E, int LN, int HT, int M );
extern	long	XSADDLine( int C, int HT, int M );



extern   int   NGXLO;
extern   int   NGYLO;
extern   int   NGXHI;
extern   int   NGYHI;
extern   int   NGXMID;
extern   int   NGYMID;

extern	long	NXSLine( long A, int E, int LN, int HT, int M );
extern	long	NXSADDLine( int C, int HT, int M );
extern int cldhndl;





extern  char		far TILECOLORS[256];
extern  char		far GROUPCOLORS[96];
extern  char		far STAMPCOLORS[96];

extern  char    *TRANSGREYPTRS[];

extern	struct RastPort    *Rp;
extern	int	GMODE;
extern	int	GRPAGE;
extern	int	GRPAGE0[];
extern	int	GXLO;
extern	int	GYLO;
extern	int	GXHI;
extern	int	GYHI;
extern	int	GXMID;
extern	int	GYMID;

extern  int     detected;
extern  UWORD   Seg3D;

extern  UWORD   identitymat[];
extern	int	viewmat[];

extern  UWORD   ObjSegs [];
extern  UWORD   ObjHands[];

extern  int     CLOUDAMNT;
extern  int     LightsOn;

extern int GrndAnimVals[32];


extern int BelowDeck;

// UWORD	CLOUDHEIGHT=2200;
extern UWORD CLOUDHEIGHT;


extern char SHADOWCOLORS[256];

extern char	far GROUPMAPCOLORS[96];


struct OBJ3D far OBJECTS[96][16];



struct STK3D{
	int OBJ;
	int X;
	int Y;
	int Z;
    int ID;
	int NXT;
	UWORD DST;
	int NEW3D;
};
extern struct STK3D STACK3D[52];


extern  LASTSTACK;

extern  FARSTACK;



extern UWORD OBJSEG;

extern UWORD PLANESEG;

extern UWORD LASTOBJ;



extern int ABOVECLOUDS;




extern int MTNDETAIL;

extern int GRNDDETAIL;

extern int OBJECTDETAIL;

extern int OVERCAST;

extern int SKYDETAIL;


#define Width2 160

#define MTNTILE 95

extern int MTNRAD;

extern int STKWID;


extern  char    *WorldName();


LoadHorz()
{
 	unsigned char	far	*D;
	FILE	*Handle;
	int	DHNDL;

	unsigned 	len;
	int	i,j;

  // ** LOAD THE WORLD DATA **

	if( _dos_open(WorldName("xx.WLD"),O_RDONLY,&DHNDL)==0 )
	{
	_dos_read(DHNDL,MAPBUF[0],128*256,&len);
	_dos_read(DHNDL,MAPBUF[128],128*256,&len);
	_dos_read(DHNDL,GROUPBUF[0],96*256,&len);
	_dos_read(DHNDL,STAMPBUF[0],96*256,&len);
//	_dos_read(DHNDL,TILEBUF[0],128*256,&len);
	for (i=0; i<128; i++) {
		_dos_read(DHNDL,TILEBUF[0],1*256,&len);
	}
	_dos_read(DHNDL,OBJECTS[0],96*16*sizeof(struct OBJ3D),&len);
	_dos_close(DHNDL);
	}


  // ** LOAD THE GREY-OUT TABLE **

	if( _dos_open(WorldName("xx.GRY"),O_RDONLY,&DHNDL)==0 )
	{

	_dos_read(DHNDL,GREYBUF[0],256,&len);

	for (i=1;i<64;i++)
		_dos_read(DHNDL,GREYBUF[1],256,&len);

	_dos_read(DHNDL,TILECOLORS,256,&len);
	_dos_read(DHNDL,GROUPCOLORS,96,&len);
	_dos_read(DHNDL,STAMPCOLORS,96,&len);
	_dos_read(DHNDL,SHADOWCOLORS,256,&len);
	TRANSGREYPTRS[1]= SHADOWCOLORS;
	_dos_close(DHNDL);
	}


  // ** LOAD THE CLOUD PICTURE **

	LoadPicNoPal ( Rp->PageNr, "Clouds.PIC" );
    CLOUDSEG = MapInEMSSprite (SPRTXPLSEG, 5);
	FP_SEG(D)=CLOUDSEG;
	FP_OFF(D)=0;
	for (i=0;i<128;i++)
		{
		for (j=0;j<128;j++)
			{
			*(D++)= GetRpPixel(Rp, j&0x7f,i&0x7f );
			}
		}


  // ** LOAD THE MAP COLOR TABLE **

	LoadPicNoPal ( Rp->PageNr, WorldName("xxMPCLR.PIC"));
	for (i=0;i<96;i++)
		{
		GROUPMAPCOLORS[i]= GetRpPixel(Rp, i&7,i/8 );
		}

//for (i=32; i<=96; i++) {
//    MAPBUF[128][i] = i;
//}

}



extern UWORD ADDRESS[320];


//#define MYWID 208

extern int MYWID;

#define MYHT	304

#ifdef YEP
Horz(long XC,long ZC,int YC,int HEAD,int PITCH,int ROLL,int ARIEL,UWORD BUFFER, int HRES)
{
    static int LASTHEIGHT=-1;

	int	i;

	void	far *P;

	long	XS,ZS,XADD,ZADD;

	long 		FA,FB;
	int		HT,CHT,FC,FD,FE,FF;

	int	ARADD,M;

    int     LinesToDo;

	long	T1DEL,T2DEL;
	int	DELSWITCH;

	int	W, CXS,CYS,CXADD,CYADD,ZD,XZD;


	int	YM,XM;
	char	N[13],S[40];


    int ADDBASE;

	int	NWID;

    MapInEMSSprite (SPRTXPLSEG, 5);

	if (GRNDDETAIL)
		{
		HRES=0;
		STKWID=2;
		}
	else
		{
		HRES=1;
		STKWID=1;
		}



	XZD=1;

	ZD= 64;

	P= (void far *)GREYBUF;

	GREYSEG= FP_SEG(P);
	GREYSEG+= ((FP_OFF(P)>>4)&0xfff);


	HT= 2*YC;  /* Why ?? */

	ABOVECLOUDS=0;
	CHT = CLOUDHEIGHT - HT;
	if (CHT<0) {
		CHT= -(CHT>>1);
		ABOVECLOUDS=1;
	}

    CLOUDAMNT = (CHT<300)? 100-CHT/3 :0;


	if (SKYDETAIL==0) (CLOUDAMNT>>=2);


    ARADD=  YC/8;

	if (ARADD>28) ARADD=28;


    LinesToDo= GYHI-GYMID+2;
    if (GYMID-GYLO >LinesToDo) LinesToDo=GYMID-GYLO;


	NWID= (((GXHI-GXLO)+7)&0xfff8);


	YM= abs(ASinB((128*(NWID/2))/160,ROLL))+abs(ACosB(LinesToDo,ROLL));
	if (YM>152) YM=152;
	if (YM<LinesToDo) YM=LinesToDo;

	XM= abs(ACosB((NWID/2)+8,ROLL))+abs(ASinB((148*LinesToDo)/100,ROLL));
	if (XM>208) XM=208;
	if (XM<160) XM=160;



	MYWID=XM;



	i= ACosB(MYWID,HEAD)+ ASinB(ACosB(256/XZD,PITCH),HEAD);
	FA= (long)(i)<<10;
	i= ASinB(-MYWID,HEAD)+ ACosB(ACosB(256/XZD,PITCH),HEAD);
	FB= (long)(i)<<10;

	FC= ACosB(-2048,HEAD);
	FD= ASinB(2048,HEAD);


	FE= -ASinB(ASinB(1024,PITCH),HEAD);
	FF= -ACosB(ASinB(1024,PITCH),HEAD);


	for (i=152-YM;i<=(152+YM);i++)
		{
		M= ACosB( 4*(MYHT/2-i)/3, PITCH)+ASinB(256/XZD,PITCH);
        M--;
		if (abs(M)<2) M= (M<0)? -2:2;




		if (M>0)
			{
			if ( ABOVECLOUDS || (SKYDETAIL==0) )
				{
				CloudFillLine2( (int)((XS-(XC>>1))>>13),(int)((ZS-(ZC>>1))>>13), (int)(XADD>>13),
					(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
					// , ((M)>64) ? GREYSEG: GREYSEG+16*(32-(M/2)), MYWID );
				}
			else
				{
				XS= XSLine( FA, FE, (4*(MYHT/2-i)/3), CHT, M );
				ZS= XSLine( FB, FF, (4*(MYHT/2-i)/3), CHT, M );
				XADD= XSADDLine( FC, CHT, M );
				ZADD= XSADDLine( FD, CHT, M );
				XS= XS<<1;
				ZS= ZS<<1;
				if (HRES)
					{
					CloudLine2( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
					//	, ((M)>64) ? GREYSEG: GREYSEG+16*(32-(M/2)), MYWID );
					}
				else
					{
					CloudLine( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
					 //	, ((M)>64) ? GREYSEG: GREYSEG+16*(32-(M/2)), MYWID );
					}
				}


			}
		else
			{

//
			if ( (!ABOVECLOUDS) || (OVERCAST==0) || (SKYDETAIL==0))
				{
				XS= XSLine( FA, FE, (4*(MYHT/2-i)/3), HT, M );
				ZS= XSLine( FB, FF, (4*(MYHT/2-i)/3), HT, M );
				XADD= XSADDLine( FC, HT, M );
				ZADD= XSADDLine( FD, HT, M );
				XS= XS<<1;
				ZS= ZS<<1;

				if (ARIEL)
					{
					T1DEL= (XADD>0) ? XADD:-XADD;
					T2DEL= (ZADD>0) ? ZADD:-ZADD;
					T1DEL= (T1DEL>T2DEL) ? T1DEL:T2DEL;

					DELSWITCH=0;
					if (T1DEL>=0x4000L) DELSWITCH++;
					if (T1DEL>=0x40000L) DELSWITCH++;
					if (T1DEL>=0x400000L) DELSWITCH++;

					if (DELSWITCH == 1) DELSWITCH=2;
					if (DELSWITCH==0) DELSWITCH=2;

					if (HRES)
						{
					switch (DELSWITCH)
						{
						case 1:
							FlyTileLine2( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,TILECOLORS,
								MYWID*(i), BUFFER	,
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
							break;
						case 2:
							FlyGroupLine2( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,GROUPCOLORS,
								MYWID*(i), BUFFER	,
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
							break;
						case 3:
							FlyStampLine2( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,STAMPCOLORS,
								MYWID*(i), BUFFER	,
								//((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
							break;
						default:
							FlyLine2( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,TILEBUF,
								MYWID*(i), BUFFER	,
								//((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
							break;
						}

						}
					else
						{
					switch (DELSWITCH)
						{
						case 1:
							FlyTileLine( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,TILECOLORS,
								MYWID*(i), BUFFER	,
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
							break;
						case 2:
							FlyGroupLine( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,GROUPCOLORS,
								MYWID*(i), BUFFER	,
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
							break;
						case 3:
							FlyStampLine( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,STAMPCOLORS,
								MYWID*(i), BUFFER	,
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
							break;
						default:
							FlyLine( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,TILEBUF,
								MYWID*(i), BUFFER	,
								((-M)>ARADD) ? GREYBUF[0]: GREYBUF[0], MYWID );
								// ((-M)>ARADD) ? GREYBUF[0]: GREYBUF[(ARADD-((-M)&0xff))], MYWID );
							break;
						}

						}


					}
				else
					{
					FlyLine( XS+XC,ZS+ZC,XADD,ZADD, MAPBUF, STAMPBUF, GROUPBUF,TILEBUF,
							MYWID*(i), BUFFER	,
							GREYBUF, MYWID );



					}

				}
//



			if ( (ABOVECLOUDS) && (SKYDETAIL!=0) )
				{
				XS= XSLine( FA, FE, (4*(MYHT/2-i)/3), CHT, -M );
				ZS= XSLine( FB, FF, (4*(MYHT/2-i)/3), CHT, -M );
				XADD= XSADDLine( FC, CHT, -M );
				ZADD= XSADDLine( FD, CHT, -M );
				XS= XS<<1;
				ZS= ZS<<1;


				if (OVERCAST==1)
					{
					SolidCloudLine( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
						, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
						// , ((-M)>64) ? GREYSEG: GREYSEG+16*(32-((-M)/2)), MYWID );
					}
				else
					{
					if (HRES)
						{
						TransCloudLine2( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
							(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
						, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
						//	, ((-M)>64) ? GREYSEG: GREYSEG+16*(32-((-M)/2)), MYWID );
						}
					else
						{
						TransCloudLine( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
							(int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
							, ((M)>64) ? GREYSEG:GREYSEG, MYWID );
							// , ((-M)>64) ? GREYSEG: GREYSEG+16*(32-((-M)/2)), MYWID );

						}

					}

				}





			}

		}


		if ( 1 )
			{
			W=NWID /*MYWID*/;
// make the address table

			CXADD= ACosB(32*2,ROLL);
			CYADD= ASinB(24*2,ROLL);
			CXS= (-W/2)*CXADD;
			CYS= (-W/2)*CYADD;

			for (i=0;i<W;i++)
				{
				ADDRESS[i]= MYWID*(CYS/ZD)+ (CXS/(2*ZD));
				CXS+= CXADD;
				CYS+= CYADD;
				}
// draw all the lines
			for (i=GYLO;i<=GYHI;i++)
				{
	ADDBASE= MYWID*(ACosB(i-GYMID,ROLL)+MYHT/2)+ ASinB((-2*(i-GYMID))/3,ROLL)+ MYWID/2;

				ScaleLine( BUFFER, ADDBASE,
								ADDRESS, GXLO/*160-W/2*/, i, NWID/*W*/, GRPAGE0[1] );

				}
			}
}
#endif


//***************************************************************************
LoadClouds()
{
   cldhndl=ReadEMSPCXSize( "cldwtr3.pcx", 1, 256, 256);
}


//***************************************************************************
NHorz(long XC,long ZC,int YC,int HEAD,int PITCH,int ROLL,int ARIEL,UWORD BUFFER, int HRES)
{
    static int LASTHEIGHT=-1;
	UWORD ADDRESS[320];
	int	i;

	void	far *P;

	long	XS,ZS,XADD,ZADD;

	long 		FA,FB;
	int		HT,CHT,FC,FD,FE,FF;

	int	ARADD,M;

    int     LinesToDo;

	long	T1DEL,T2DEL;
	int	DELSWITCH;

	int	W, CXS,CYS,CXADD,CYADD,ZD,XZD;


	int	YM,XM;
	char	N[13],S[40];


    int ADDBASE;

	int	NWID;


	int GSEG;


	XZD=1;
	ZD= 64;


   CLOUDSEG=MapEmsFile( cldhndl, 0);


	HT= 2*YC;  /* Why ?? */

	ABOVECLOUDS=0;
	CHT = CLOUDHEIGHT - HT;
	if (CHT<0) {
		CHT= -(CHT>>1);
		ABOVECLOUDS=1;
	}

    CLOUDAMNT = (CHT<300)? 100-CHT/3 :0;

	if (SKYDETAIL==0) (CLOUDAMNT>>=2);

    ARADD=  YC/8;

	if (ARADD>28) ARADD=28;

    LinesToDo= NGYHI-NGYMID+2;
    if (NGYMID-NGYLO >LinesToDo) LinesToDo=NGYMID-NGYLO;


	NWID= (((NGXHI-NGXLO)+7)&0xfff8);


	YM= abs(ASinB((128*(NWID/2))/160,ROLL))+abs(ACosB(LinesToDo,ROLL));
	if (YM>152) YM=152;
	if (YM<LinesToDo) YM=LinesToDo;

	XM= abs(ACosB((NWID/2)+8,ROLL))+abs(ASinB((148*LinesToDo)/100,ROLL));
	if (XM>208) XM=208;
	if (XM<160) XM=160;


	MYWID=XM;



	i= ACosB(MYWID,HEAD)+ ASinB(ACosB(256/XZD,PITCH),HEAD);
	FA= (long)(i)<<10;
	i= ASinB(-MYWID,HEAD)+ ACosB(ACosB(256/XZD,PITCH),HEAD);
	FB= (long)(i)<<10;

	FC= ACosB(-2048,HEAD);
	FD= ASinB(2048,HEAD);


	FE= -ASinB(ASinB(1024,PITCH),HEAD);
	FF= -ACosB(ASinB(1024,PITCH),HEAD);


	for (i=152-YM;i<=(152+YM);i++)
		{
		M= ACosB( 4*(MYHT/2-i)/3, PITCH)+ASinB(256/XZD,PITCH);
        M--;
		if (abs(M)<2) M= (M<0)? -2:2;

		if (M>0)
			{
			if ( ABOVECLOUDS || (SKYDETAIL==0) )
				{
            FP_SEG(P)=BUFFER;
            FP_OFF(P)=MYWID*(i);
            fmemfill(P,0x8484,MYWID); // remember to grey out the 0x84
				}
			else
				{
				XS= NXSLine( FA, FE, (4*(MYHT/2-i)/3), CHT, M );
				ZS= NXSLine( FB, FF, (4*(MYHT/2-i)/3), CHT, M );
				XADD= NXSADDLine( FC, CHT, M );
				ZADD= NXSADDLine( FD, CHT, M );
				XS= XS<<1;
				ZS= ZS<<1;
            switch (HRES)
               {
               case 0:
					   NCloudLine( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						   (int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					   , ((M)>47) ? GREYSEG: GREYSEG+16*(12-(M/4)), MYWID );
                  break;
               case 1:
					   NCloudLine2( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						   (int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					   , ((M)>47) ? GREYSEG: GREYSEG+16*(12-(M/4)), MYWID );
                  break;
               case 2:
					   NCloudLine3( (int)((XS-(XC<<1))>>13),(int)((ZS-(ZC<<1))>>13), (int)(XADD>>13),
						   (int)(ZADD>>13), CLOUDSEG, MYWID*(i), BUFFER
					   , ((M)>47) ? GREYSEG: GREYSEG+16*(12-(M/4)), MYWID );
                  break;
               }
				}
			}
		else
			{
         GSEG= (ABOVECLOUDS)? CLOUDSEG+2048:CLOUDSEG+8;
         if (ABOVECLOUDS)
            {
			   XS= NXSLine( FA, FE, (4*(MYHT/2-i)/3), CHT, -M );
			   ZS= NXSLine( FB, FF, (4*(MYHT/2-i)/3), CHT, -M );
			   XADD= NXSADDLine( FC, CHT, -M );
			   ZADD= NXSADDLine( FD, CHT, -M );
			   XS= XS<<1;
			   ZS= ZS<<1;
            switch (HRES)
               {
               case 0:
				      NCloudLine( (int)((XS-(XC<<1))>>11),(int)((ZS-(ZC<<1))>>11), (int)(XADD>>11),
					      (int)(ZADD>>11), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               case 1:
				      NCloudLine2( (int)((XS-(XC<<1))>>11),(int)((ZS-(ZC<<1))>>11), (int)(XADD>>11),
					      (int)(ZADD>>11), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               case 2:
				      NCloudLine3( (int)((XS-(XC<<1))>>11),(int)((ZS-(ZC<<1))>>11), (int)(XADD>>11),
					      (int)(ZADD>>11), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               }
            }
         else
            {
			   XS= NXSLine( FA, FE, (4*(MYHT/2-i)/3), HT, -M );
			   ZS= NXSLine( FB, FF, (4*(MYHT/2-i)/3), HT, -M );
			   XADD= NXSADDLine( FC, HT, -M );
			   ZADD= NXSADDLine( FD, HT, -M );
			   XS= XS<<1;
			   ZS= ZS<<1;
            switch (HRES)
               {
               case 0:
				      NCloudLine( (int)((XS-(XC<<1))>>6),(int)((ZS-(ZC<<1))>>6), (int)(XADD>>6),
					      (int)(ZADD>>6), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               case 1:
				      NCloudLine2( (int)((XS-(XC<<1))>>6),(int)((ZS-(ZC<<1))>>6), (int)(XADD>>6),
					      (int)(ZADD>>6), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               case 2:
				      NCloudLine3( (int)((XS-(XC<<1))>>6),(int)((ZS-(ZC<<1))>>6), (int)(XADD>>6),
					      (int)(ZADD>>6), GSEG, MYWID*(i), BUFFER
				      , ((-M)>8) ? GREYSEG: GREYSEG+16*(8-(-M)), MYWID );
                  break;
               }
            }
			}
		}


		if ( 1 )
			{
			W=NWID /*MYWID*/;
// make the address table

			CXADD= ACosB(32*2,ROLL);
			CYADD= ASinB(24*2,ROLL);
			CXS= (-W/2)*CXADD;
			CYS= (-W/2)*CYADD;

			for (i=0;i<W;i++)
				{
				ADDRESS[i]= MYWID*(CYS/ZD)+ (CXS/(2*ZD));
				CXS+= CXADD;
				CYS+= CYADD;
				}
// draw all the lines
			for (i=NGYLO;i<=NGYHI;i++)
				{
	ADDBASE= MYWID*(ACosB(i-NGYMID,ROLL)+MYHT/2)+ ASinB((-2*(i-NGYMID))/3,ROLL)+ MYWID/2;

            if (HRES)
               {
				   NScaleLine2( BUFFER, ADDBASE,
								   ADDRESS, NGXLO/*160-W/2*/, i, NWID/*W*/, GRPAGE0[1] );
               }
            else
               {
   				NScaleLine( BUFFER, ADDBASE,
								ADDRESS, NGXLO/*160-W/2*/, i, NWID/*W*/, GRPAGE0[1] );
               }
				}
			}
}



#ifdef YEP
/****************************************************************************/
Draw3dStamp(int STAMP, int STAMPLOC, int X, int Y, int Z , int *VM )
{
	int	i, NUM, ID;

	for (i=0;i<16;i++) {

        NUM = OBJECTS[STAMP][i].NUM;

        if (NUM!=-1) {
            ID = 0;
            if (NUM&0x80) {
                NUM &= 0x7F;
                ID = CheckException (STAMPLOC, i);
            }

			if (Stack3DObject((GRNDSEG <<8) | NUM, X+OBJECTS[STAMP][i].X,
           				       Y, Z+OBJECTS[STAMP][i].Y,
                               ID, OBJSCALES[NUM],0)) {
				CheckAAA(NUM, X+OBJECTS[STAMP][i].X, Y, Z+OBJECTS[STAMP][i].Y, i, ID);
            }
		}
	}
}
#endif


extern int FLSHXOFF;

extern int FLSHZOFF;


#ifdef YEP
Draw3dGroundObjects(long X, int Y, long Z, int ForceIt)
{
	int	MapX,MapZ;
	int	XOFF,ZOFF,i,j;
	int	TM[9];

    AAACnt=0;


	if (ABOVECLOUDS==0 || ForceIt) {
		MakeMatHPR(0,0,0,TM);

		MapX= (X>>12)&0xff;
		MapZ= (Z>>12)&0xff;

		FLSHXOFF=XOFF= (X&0xfff);
		FLSHZOFF=ZOFF= (Z&0xfff);

		DrawHills(X, Y, Z, TM );

		for (i=-STKWID;i<(STKWID+1);i++) {
			for (j=-STKWID;j<(STKWID+1);j++) {
				Draw3dStamp(MAPBUF[MapZ+i][MapX+j], ((MapZ+i) <<8) + MapX+j,
					(4096*j)-XOFF,-Y, (4096*i)-ZOFF, TM );
				}
		}
	}

	EndStack3D();
}


DrawHills(long X, int Y, long Z, int *VM )
{
	int	MapX,MapZ;
	int	XOFF,ZOFF,i,j;
	int	TM[9];

	int	N;


	char	far	*P;

    static int HVM[9];


	if (MTNDETAIL==2)
		MTNRAD=6;
	else
		MTNRAD=2;



    if (MapIn3dObjects (MTNSEG))
		{
		MakeMatHPR(0,0,0,HVM);

		MapX= (X>>12)&0xff;
		MapZ= (Z>>12)&0xff;

		XOFF= (X&0xfff);
		ZOFF= (Z&0xfff);

		P=FillTransTbl;


		for (i=-MTNRAD;i<0;i++)
			{
			for (j=-MTNRAD;j<0;j++)
				{
				if (MAPBUF[MapZ+i][MapX+j]!=MTNTILE) continue;

				N=0;
				if (MAPBUF[MapZ+i-1][MapX+j]==MTNTILE) N|=0x8;
				if (MAPBUF[MapZ+i][MapX+j+1]==MTNTILE) N|=0x4;
				if (MAPBUF[MapZ+i+1][MapX+j]==MTNTILE) N|=0x2;
				if (MAPBUF[MapZ+i][MapX+j-1]==MTNTILE) N|=0x1;

				if ( (abs(i)>STKWID) || (abs(j)>STKWID) || (abs(i)+abs(j)==(2*STKWID)) )
					{
					FillTransTbl= GREYBUF[2* ( abs(i)>abs(j) )? abs(i):abs(j) ];

					Draw3DObject( N+16,
						(1024*j)-XOFF/4,-Y/4, (1024*i)-ZOFF/4, HVM, GrndAnimVals );
					}
				else
					{
					if (MTNDETAIL==0)	N+=16;
					Stack3DObject( (MTNSEG<<8) | N,
						(4096*j)-XOFF,-Y, (4096*i)-ZOFF,
						0, 1,0);
					}

				}
			for (j=MTNRAD;j>=0;j--)
				{
				if (MAPBUF[MapZ+i][MapX+j]!=MTNTILE) continue;

				N=0;
				if (MAPBUF[MapZ+i-1][MapX+j]==MTNTILE) N|=0x8;
				if (MAPBUF[MapZ+i][MapX+j+1]==MTNTILE) N|=0x4;
				if (MAPBUF[MapZ+i+1][MapX+j]==MTNTILE) N|=0x2;
				if (MAPBUF[MapZ+i][MapX+j-1]==MTNTILE) N|=0x1;

				if ( (abs(i)>STKWID) || (abs(j)>STKWID) || (abs(i)+abs(j)==(2*STKWID)) )
					{
					FillTransTbl= GREYBUF[2* ( abs(i)>abs(j) )? abs(i):abs(j) ];
					Draw3DObject( N+16,
						(1024*j)-XOFF/4,-Y/4, (1024*i)-ZOFF/4, HVM, GrndAnimVals );
					}
				else
					{
					if (MTNDETAIL==0)	N+=16;
					Stack3DObject( (MTNSEG<<8) | N,
						(4096*j)-XOFF,-Y, (4096*i)-ZOFF,
						0, 1,0 );
					}

				}
			}
		for (i=MTNRAD;i>=0;i--)
			{
			for (j=-MTNRAD;j<0;j++)
				{
				if (MAPBUF[MapZ+i][MapX+j]!=MTNTILE) continue;

				N=0;
				if (MAPBUF[MapZ+i-1][MapX+j]==MTNTILE) N|=0x8;
				if (MAPBUF[MapZ+i][MapX+j+1]==MTNTILE) N|=0x4;
				if (MAPBUF[MapZ+i+1][MapX+j]==MTNTILE) N|=0x2;
				if (MAPBUF[MapZ+i][MapX+j-1]==MTNTILE) N|=0x1;

				if ( (abs(i)>STKWID) || (abs(j)>STKWID) || ((abs(i)+abs(j))==(2*STKWID)) )
					{
					FillTransTbl= GREYBUF[2* ( abs(i)>abs(j) )? abs(i):abs(j) ];
					Draw3DObject( N+16,
						(1024*j)-XOFF/4,-Y/4, (1024*i)-ZOFF/4, HVM, GrndAnimVals );
					}
				else
					{
					if (MTNDETAIL==0)	N+=16;
					Stack3DObject( (MTNSEG<<8) | N,
						(4096*j)-XOFF,-Y, (4096*i)-ZOFF,
						0, 1,0 );
					}

				}
			for (j=MTNRAD;j>=0;j--)
				{
				if (MAPBUF[MapZ+i][MapX+j]!=MTNTILE) continue;

				N=0;
				if (MAPBUF[MapZ+i-1][MapX+j]==MTNTILE) N|=0x8;
				if (MAPBUF[MapZ+i][MapX+j+1]==MTNTILE) N|=0x4;
				if (MAPBUF[MapZ+i+1][MapX+j]==MTNTILE) N|=0x2;
				if (MAPBUF[MapZ+i][MapX+j-1]==MTNTILE) N|=0x1;

				if ( (abs(i)>STKWID) || (abs(j)>STKWID) || ((abs(i)+abs(j))==(2*STKWID)) )
					{
					FillTransTbl= GREYBUF[2* ( abs(i)>abs(j) )? abs(i):abs(j) ];
					Draw3DObject( N+16,
						(1024*j)-XOFF/4,-Y/4, (1024*i)-ZOFF/4, HVM, GrndAnimVals );
					}
				else
					{
					if (MTNDETAIL==0)	N+=16;
					Stack3DObject( (MTNSEG<<8) | N,
						(4096*j)-XOFF,-Y, (4096*i)-ZOFF,
						0, 1,0 );
					}

				}

			}

		FillTransTbl=P;
		}
}
#endif


//***************************************************************************
EndStack3D()
{
	int	    i;
    int     SegIX;
    UWORD   SEGOBJ;
    UWORD   NUM;

	for (i=FARSTACK;i!=-1;) {

	 if (!(STACK3D[i].NEW3D)) {
        SEGOBJ  = STACK3D [i].OBJ;
        SegIX   = SEGOBJ >>8;
        NUM     = SEGOBJ & 255;

        if (SegIX==GRNDSEG) {
            if ((STACK3D [i].DST>OBJDISTS[NUM]) || (OBJECTDETAIL==0)) SegIX=MGRNDSEG;
            if (NUM>=LASTOBJ) SegIX++;
        }

        if (MapIn3dObjects(SegIX)) {
            if (SegIX<=LASTGRNDSEG) {
//                Draw3dGroundObject (NUM, STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID);

            } else if (SegIX==SPRTXPLSEG) {
				DrawASmoke(STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID, 1 );

            } else if (SegIX==MTNSEG) {
            	Draw3DObject(NUM, STACK3D[i].X>>2, STACK3D[i].Y>>2, STACK3D[i].Z>>2, identitymat, GrndAnimVals);

			} else if (SegIX==AMERSEG) {
                DrawCarrier(STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z,STACK3D[i].ID);

			} else if (SEGOBJ==SHADOWPX) {
				DrawShadow(NUM, STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID);

			} else if (SEGOBJ==F14L02) {
				DrawCoolF14(STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z);

			} else if (SEGOBJ==BOMB500PX || SEGOBJ==SIDEWPX) {
                DrawMissile (NUM, STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID);

			} else {
                DrawTexPlane(NUM, STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID);
            }
        }
	 } else {
        DrawTexPlane(STACK3D[i].OBJ, STACK3D[i].X, STACK3D[i].Y, STACK3D[i].Z, STACK3D[i].ID);
	 }

		i=STACK3D[i].NXT;
	}

	LASTSTACK=0;
	FARSTACK=-1;

}

//***************************************************************************
Stack3Dobject(int OBJ, int X, int Y, int Z, int ID, UWORD SCL, int new3d)
{
	int	i,prev;
	UWORD DST;
	int	V[3];

	if (LASTSTACK>49) return 0;

	DST=Dist3D(X,Y,Z);

	/* Mike KLUDGE */
	if (((OBJ>>8) == F14COOLSEG) && !(BelowDeck)) DST=4;

    if (DST>(REJECTDIST[SCL])) return 0;

	if ((OBJ>>8) != MTNSEG)	{
		V[0]=X;
		V[1]=Y;
		V[2]=Z;
		Rotate(V,viewmat);
		V[2]+= 500;

		if (abs(V[0])>(2*V[2])) return 0;
		if (abs(V[1])>(2*V[2])) return 0;
	}

	STACK3D[LASTSTACK].OBJ=OBJ;
	STACK3D[LASTSTACK].X=X;
	STACK3D[LASTSTACK].Y=Y;
	STACK3D[LASTSTACK].Z=Z;
	STACK3D[LASTSTACK].ID=ID;
	STACK3D[LASTSTACK].NXT=-1;
	STACK3D[LASTSTACK].DST=DST;
	STACK3D[LASTSTACK].NEW3D=new3d;

	if (LASTSTACK==0) {
		FARSTACK=0;
		LASTSTACK++;
		return 1;
	}

	for (i=FARSTACK,prev=-1;i!=-1;) {
		if (DST>STACK3D[i].DST) break;
		prev=i;
		i=STACK3D[i].NXT;
	}
	STACK3D[LASTSTACK].NXT=i;
	if (prev==-1)
		FARSTACK=LASTSTACK;
	else
		STACK3D[prev].NXT=LASTSTACK;
	LASTSTACK++;

    return 1;
}
extern int GFLSH;


#ifdef YEP
Draw3dGroundObject (int NUM, int X, int Y, int Z, int BlowUpFlags) {

    UWORD   SCL;
    int     i;

	int	N;

    GrndAnimVals[10] =
    GrndAnimVals[11] = LightsOn & !BlowUpFlags;

    GrndAnimVals[1] =
    GrndAnimVals[18]=
    GrndAnimVals[19]=
    GrndAnimVals[20]=
    GrndAnimVals[21]=
    GrndAnimVals[22]=
    GrndAnimVals[23]=
    GrndAnimVals[24]=
    GrndAnimVals[25]=0;

	N= (X+FLSHXOFF+Z+FLSHZOFF)&7;
	N= (GFLSH+N)&7;

	*(PalPtr3D+146)= ( N>3 )?
						147*256+147:251*256+251;

	*(PalPtr3D+149)= 250*256+149;


    if (BlowUpFlags==-1) {
        GrndAnimVals[1]=1;                      /* Single Blown-up objects */

    } else if (BlowUpFlags!=0) {            /* Group Blown-up objects */
        for (i=0; i<8; i++) {
            GrndAnimVals[i+18]=BlowUpFlags&1;
            BlowUpFlags >>=1;
        }
    }


    /* SAM firing: var3==1 launch fire/smoke, var2==1 no missile */

    SCL = OBJSCALES[NUM];

    if (NUM>=LASTOBJ) NUM -= LASTOBJ;

	Draw3DObject( NUM, X*SCL, Y*SCL, Z*SCL, identitymat, GrndAnimVals );

	*(PalPtr3D+146)= 146*256+146;
}

DrawLights(long X, int Y, long Z )
{
	int	MapX,MapZ;
	int	XOFF,ZOFF,i,j;
	int	TM[9];


	GFLSH++;

    if (MapIn3dObjects (LIGHTSEG) && LightsOn && (ABOVECLOUDS==0) )
		{
		MakeMatHPR(0,0,0,TM);

		MapX= (X>>12)&0xff;
		MapZ= (Z>>12)&0xff;

		XOFF= (X&0xfff);
		ZOFF= (Z&0xfff);

		for (i=-4;i<5;i++)
			{
			for (j=-4;j<5;j++)
				{
				if (STAMPLIGHTS[MAPBUF[MapZ+i][MapX+j]])
					{
					Draw3DObject( MAPBUF[MapZ+i][MapX+j],
						(2048*j)-XOFF/2,-Y/2, (2048*i)-ZOFF/2, TM, GrndAnimVals );

					}
				}
			}

		}
}
#endif

Map3d( RPS *Rp, long XP, long YP, int SCALE )
{

	int	i,j;

	long	X,Y;
	long	STEP;

	long	TMP;

    char    far *TransTbl;

    TransTbl = MAPCOLORS;

	if (SCALE<0)  SCALE=0;
	if (SCALE>13) SCALE=13;


	XP-= (long) ((long)((Rp->Width1 +1)/2)<<SCALE);
	YP-= (long) ((long)((Rp->Length1+1)/2)<<SCALE);


	STEP= 1<< SCALE;

	TMP= (long)(XP&(0xffffffff<<SCALE));

	TMP+= (XYDSP[SCALE*2]<<SCALE)/8;

	for (i=0,Y= (YP&(0xffffffff<<SCALE))+(XYDSP[SCALE*2+1]<<SCALE)/8;i<=Rp->Length1;i++,Y+=STEP) {
		if ( (SCALE>7) ) {
		    S2MapLine( TMP,Y,STEP,Rp->Width1+1,
			    TILEBUF,GROUPMAPCOLORS,STAMPBUF,MAPBUF,Rp->XBgn,Rp->YBgn+i,GRPAGE0[Rp->PageNr], TransTbl );
        } else {
		    SMapLine( TMP,Y,STEP,Rp->Width1+1,
			    TILEBUF,GROUPBUF,STAMPBUF,MAPBUF,Rp->XBgn,Rp->YBgn+i,GRPAGE0[Rp->PageNr], TransTbl );
		}
    }

}

/***************************************************************************/

struct	SNRM{
	int	NI;
	int	NJ;
	int	NK;
	long	D;
	};

InPoly( int far *P1, int far *P2, int X, int Z )
{
	int	Z1,Z2,T;

	Z1=*(P1+2);
	Z2=*(P2+2);
	if (Z1>Z2)
		{
		T=Z1;
		Z1=Z2;
		Z2=T;
		}
	if ( (Z<Z1) || (Z>Z2) ) return(0);

	T= MulDiv( Z-*(P1+2),*(P2)-*(P1),*(P2+2)-*(P1+2) )+*(P1);

	return(T);

}


SurfHt(struct SNRM far *NRM, int X, int Z)
{
	long	t,t2,t3;


	t= -NRM->D-(long)NRM->NI*(long)X -(long)NRM->NK*(long)Z;
	if ( (t2=t)<0 ) t2=-t;
	t3= ((long)NRM->NJ*1000L);
	if (t3<0) t3=-t3;

	if (t3<t2) return(0);

	t2= t/(long)NRM->NJ;

	if (t2<0) return(0);
	return((int)t2);

}

GroundHeight(long X, long Z )
{
	int	MapX,MapZ;
	int	XOFF,ZOFF;
	int	HEIGHT,i,j;

	struct	SNRM	far *NRM;
	UWORD	far	*GOPBASE;
	UWORD	far	*PTSBASE;
	char	far	*GOP;
	char	far	*C;
	char	far	*L;
	UWORD	far	*P;

	int	NUMSRFS;
	int	NUMPTS;
	int	N;
	char	CNT;

	int	MINVL,MAXVL,VAL;


	HEIGHT=0;

	MapX= (X>>12)&0xff;
	MapZ= (Z>>12)&0xff;

	if (MAPBUF[MapZ][MapX]==MTNTILE)
		{
        if (MapIn3dObjects (MTNSEG))
			{
            FP_SEG(P)=Seg3D;
			FP_OFF(P)=0;

			XOFF= (X&0xfff)/4;
			ZOFF= (Z&0xfff)/4;
			N=0;
			if (MAPBUF[MapZ-1][MapX]==MTNTILE) N|=0x8;
			if (MAPBUF[MapZ][MapX+1]==MTNTILE) N|=0x4;
			if (MAPBUF[MapZ+1][MapX]==MTNTILE) N|=0x2;
			if (MAPBUF[MapZ][MapX-1]==MTNTILE) N|=0x1;

// for low detail level
			if (MTNDETAIL==0)	N+=16;

// N= pic num of hill
			FP_OFF(P)= *(P+N+1);
// P points to object
			FP_OFF(P)= *(P+1);
// P Points to radius
			N= *(P+1);
			if (N>1)
				P+= 4*(N-1);
			P+=2;
// P point to num of pts
			NUMPTS=*(P++);
// P Point to num surfs
			if ( (NUMSRFS=*(P++))==0) return(0);
			NRM=(struct SNRM far *)P;

			C= (UBYTE far *) (P+(5*abs(NUMSRFS)));

			for (i=0;i<abs(NUMSRFS);i++)
				C+= 1+*(C);
			if (NUMSRFS<0)
				{
				NUMSRFS=-NUMSRFS;
				C+= (2*NUMSRFS)+1;
				}
			PTSBASE= (int far *)C;
			C+= 6*NUMPTS;

			L= C+2;
			P=(UWORD far *)C;
			C+= (*(P)*2)+2;
			GOPBASE=(UWORD far *) C;

// now look thru all surfs for inside of polys

			for (i=0;i<NUMSRFS;i++)
				{
				FP_SEG(GOP)=FP_SEG(GOPBASE);
				FP_OFF(GOP)=*(GOPBASE+i);
				while ( (CNT=*(GOP++))!=-1)
					{
					if ( ((CNT&0xc0)==0) &&(*(GOP+(CNT&0x1f))!=-1) )
						{
						MINVL=0x7fff;
						MAXVL=0x8001;
						for (j=0;j<(CNT&0x1f);j++)
							{
							VAL= InPoly(
										(int far *) (PTSBASE+3*( *(L+(*(GOP+j)*2)))),
										(int far *) (PTSBASE+3*( *(L+(*(GOP+j)*2)+1))),
										XOFF,ZOFF	);
							if (VAL)
								{
								if (VAL<MINVL) MINVL=VAL;
								if (VAL>MAXVL) MAXVL=VAL;
								}
							}
						if ( (XOFF<=MAXVL)&&(XOFF>=MINVL) )
							{
							HEIGHT= SurfHt( (struct SNRM far *)(NRM+i),XOFF,ZOFF);
							i=NUMSRFS;
							}
						}
					GOP+=(CNT&0x1f)+1;
					}
				}
			}
		}
	return(HEIGHT*4);
}


RotatingMap( RPS *Rp, long XP, long YP, int SCALE , int HEAD, char far *P,
								int	XOFF,int YOFF)
{
	int	i;

	long	XS,ZS,XADD,ZADD;
	long	DXADD,DZADD;
	int	    SCZ;
    int     WID, HT;

    WID = Rp->Width1+1;
    HT  = Rp->Length1+1;

	if (P==(char far *) 0) P= (char far *) GREYBUF;

	if (SCALE<0) SCALE=0;
	if (SCALE>13) SCALE=13;
	SCZ= (1<<4);

	XS=  ACosB(   SCZ*(-(WID/2+XOFF))   ,HEAD);
	ZS= -ASinB(   SCZ*(-(WID/2+XOFF))   ,HEAD);
	XS+= ASinB( 4*SCZ*(-( HT/2+YOFF))/3 ,HEAD);
	ZS+= ACosB( 4*SCZ*(-( HT/2+YOFF))/3 ,HEAD);

	XADD=    ACosB((SCZ<<7),HEAD);
	ZADD=   -ASinB((SCZ<<7),HEAD);
	DXADD= 4*ASinB((SCZ<<8),HEAD)/3;
	DZADD= 4*ACosB((SCZ<<8),HEAD)/3;


	XS= (XS<<(8+SCALE))+(XP<<12);
	ZS= (ZS<<(8+SCALE))+(YP<<12);

	XADD= (XADD<<(SCALE));
	ZADD= (ZADD<<(SCALE));

	DXADD= (DXADD<<(SCALE));
	DZADD= (DZADD<<(SCALE));

	XS&= (0xfffff000<<(SCALE));
	ZS&= (0xfffff000<<(SCALE));

	for (i=0;i<HT;i++)
		{
		switch (SCALE)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			FlyLine( XS,ZS,XADD,ZADD, MAPBUF,STAMPBUF,GROUPBUF,TILEBUF,
							320*(Rp->YBgn+i)+Rp->XBgn,GRPAGE0[Rp->PageNr],P, WID);
				break;
			FlyTileLine( XS,ZS,XADD,ZADD, MAPBUF,STAMPBUF,GROUPBUF,TILECOLORS,
							320*(Rp->YBgn+i)+Rp->XBgn,GRPAGE0[Rp->PageNr],P, WID);
				break;
			case 8:
			case 9:
			case 10:
			case 11:
			FlyGroupLine( XS,ZS,XADD,ZADD, MAPBUF,STAMPBUF,GROUPBUF,GROUPCOLORS,
							320*(Rp->YBgn+i)+Rp->XBgn,GRPAGE0[Rp->PageNr],P, WID);
				break;
			case 12:
			FlyStampLine( XS,ZS,XADD,ZADD, MAPBUF,STAMPBUF,GROUPBUF,STAMPCOLORS,
							320*(Rp->YBgn+i)+Rp->XBgn,GRPAGE0[Rp->PageNr],P, WID);
			}

		XS+= DXADD;
		ZS+= DZADD;
		}
}

#ifdef YEP
DrawTgtFlirGroundObjects(long X, int Y, long Z, long TgtX, long TgtZ)
{
	int	MapX,MapZ;
	int	XOFF,ZOFF,i,j;
	int	TM[9];

	MakeMatHPR(0,0,0,TM);

	MapX= (X>>12)&0xff;
	MapZ= (Z>>12)&0xff;

	XOFF= (X&0xfff);
	ZOFF= (Z&0xfff);

//	DrawHills(X, Y, Z, TM );

	j = ((TgtX>>12)&0xff)-MapX;
	i = ((TgtZ>>12)&0xff)-MapZ;

	Draw3dStamp(MAPBUF[MapZ+i][MapX+j], ((MapZ+i) <<8) + MapX+j,
		(4096*j)-XOFF,-Y, (4096*i)-ZOFF, TM );

	EndStack3D();

}
#endif

//***************************************************************************
CheckAAA (int NUM, int X, int Y, int Z, int i, UWORD ID) {
    int              j;
    int              HitFlags;
    struct Target   *T;
    static char       mix[] = {0, 14, 5, 9, 2, 13, 8, 15, 7, 4, 10, 1, 3, 12, 6, 11};

	if (ABOVECLOUDS || !detected || InFriendly) return;

    T = Targets+NUM;

    if (T->Type&AAATGT) {           // AAA

        if (T->Type&GROUPTGT) {
            T = GTargets + T->Hardness; /* Hardness contains index */

            j=0;
            HitFlags=1;
            do {
                if ((T->Type&AAATGT) && !(HitFlags&ID)) {
                    AAAi[AAACnt] = i+j;
                    AAAX[AAACnt] = X+T->Xoff;
                    AAAZ[AAACnt] = Z-T->Yoff;

                    if (AAACnt<MaxAAA) ++AAACnt;

                    j+=5;
                }

                HitFlags <<=1;
            } while (!(T++->Type&LASTTGT));

        } else if (!ID) {

            AAAi[AAACnt] = mix[i&15];
            AAAX[AAACnt] = X;
            AAAZ[AAACnt] = Z;

            if (AAACnt+1<MaxAAA) ++AAACnt;
        }

        AAAY         = Y;
    }

}

//***************************************************************************
DrawAAA ()
{
    int         i;
    UWORD       Save;
    int        VM[9];
    int        Angle2;

    UBYTE far  *Fred=AAFIRE;
    static UWORD Angle1=0;


    Save = Seg3D;
    Seg3D = FP_SEG(Fred);

    for (i=0; i<AAACnt; i++) {
        Angle2 = (Angle1+(AAAi[i]&15)*512)&8191;
        MakeMatRPH (0, 5471, (AAAi[i]&15)*0x1000, VM);
        Draw3DObject( 0, AAAX[i], AAAY, AAAZ[i], VM, &Angle2 );
    }

    Angle1 += 2048/FrameRate;
    Seg3D = Save;
}


