
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
#include "common.h"
#include "f15defs.h"
#include "cockpit.h"
#include "awg9.h"
#include "armt.h"
#include "planeai.h"
#include "setup.h"

extern  long	 PxLng,PyLng;
extern  UWORD	 MouseSprite;
extern	int      OurHead;
extern	int      sx,sy,sz;
extern	UWORD    Rtime;
extern  int	whichourcarr;
extern	UWORD	OurAlt;
extern  UWORD   Alt;
extern int 	lastmsgview;

extern confData ConfigData;

extern struct PlaneData far *GetPlanePtr(int PlaneNo);
extern struct MissileData far *GetMissilePtr(int MissileNo);
extern struct RadarData far *GetRadarPtr(int RadarNo);

extern   int AIWingMan;
extern  int     whichourcarr;     // holds value for which boat our carrier

extern	UWORD   GRPAGE0[];
extern   RPS   *Rp1;
extern   RPS   *Rp2;
extern   RPS   *Rp3;

//***************************************************************************

#define  RPLNCOLOR 1
#define  WPLNCOLOR 2
#define  WPTCOLOR 3
#define  CARCOLOR 4
#define  MSGCOLOR 3
#define  TXTCOLOR 4


#define  RESUMEXL 259
#define  RESUMEYL 172
#define  RESUMEXH 316
#define  RESUMEYH 188

#define  OBJXL 265
#define  OBJYL 136
#define  OBJXH 316
#define  OBJYH 148

#define  TELEXL 265
#define  TELEYL 152
#define  TELEXH 316
#define  TELEYH 164

#define  TXTXL 40
#define  TXTYL 64
#define  TXTXH 220
#define  TXTYH 164
#define  TXTWID TXTXH-TXTXL+1
#define  TXTHT TXTYH-TXTYL+1

#define  RPLNXL 267
#define  RPLNYL 28
#define  RPLNXH 277
#define  RPLNYH 38
#define  RPLNWID RPLNXH-RPLNXL+1
#define  RPLNHT RPLNYH-RPLNYL+1
#define  RPLNXMID ((RPLNXH+RPLNXL)/2)
#define  RPLNYMID ((RPLNYH+RPLNYL)/2)


#define  WPLNXL 267
#define  WPLNYL 42
#define  WPLNXH 277
#define  WPLNYH 52
#define  WPLNWID WPLNXH-WPLNXL+1
#define  WPLNHT WPLNYH-WPLNYL+1
#define  WPLNXMID ((WPLNXH+WPLNXL)/2)
#define  WPLNYMID ((WPLNYH+WPLNYL)/2)

#define  CARXL 268
#define  CARYL 56
#define  CARXH 275
#define  CARYH 67
#define  CARWID CARXH-CARXL+1
#define  CARHT CARYH-CARYL+1
#define  CARXMID ((CARXH+CARXL)/2)
#define  CARYMID ((CARYH+CARYL)/2)


//**********************************************************************
extern int NextRioTime;

extern int iconmode;

BigRioDot(RPS *Rp,int x, int y, int color)
{
   DrawLine(Rp,x,y-2,x+3,y+2,color);
   DrawLine(Rp,x+3,y+2,x-3,y+2,color);
   DrawLine(Rp,x-3,y+2,x,y-2,color);
}

extern struct _wayPoint_ aiwaypoints[];
extern struct _path_ aipaths[];
extern int DoMustLive,DoMustDie,DoMustEsc;
extern int MinTimeSet;
extern int CheckOurAlt();
extern int RioWingOK;


extern int _pq0;
extern int _pq1;
extern int _pq2;
extern int _pq3;
extern int _pq4;
extern int _pq5;
extern int _pq6;
extern int _pq7;
extern int _pq8;
extern int _pq9;
extern int _pq10;
extern int _pq11;
extern int _pq12;
extern int _pq13;
extern int _pq14;
extern int _pq15;
extern int _pq16;
extern int _pq17;
extern int _pq18;
extern int _pq19;
extern int _pq20;
extern int _pq21;
RioWpMark(RPS *Rp,int num,int type)
{
   long	xL,yL;
   UWORD	z;
   int	x,y,px,py;
   int	ox,oy;
   int	i,end;
   int	start;
   register struct PlaneData far *tp;
   volatile int	pcid;

   if (num>=LASTPLANE || num<0) return;
   if (ps[num].status&(LANDED|LANDING|BACKTOBASE)) return;
   if (!(ps[num].status&ALIVE)) return;
   if (!(ps[num].status&ACTIVE)) return;
   if (!(ps[num].status&FRIENDLY)) return;
   tp=GetPlanePtr(ps[num].type);
   pcid=tp->PlaneEnumId;
   if (tp->PlaneEnumId>=SO_F_14FDA && tp->PlaneEnumId<=SO_AS_4) {
      if (type==0) return;
   } else {
      if (type!=0) return;
   }
   ox=ps[num].xL>>12;
   oy=MulDiv((int)(ps[num].yL>>12),192,256);
   BigRioDot(Rp,ox,oy,7);
   start=bv[num].groundt;
   end=aipaths[bv[num].path].wp0+aipaths[bv[num].path].numWPs;
   for (i=start;i<end;i++) {
      GetWayXYZ(i,&xL,&yL,&z);
      x=(int)(xL>>12);
      y=MulDiv((int)(yL>>12),192,256);
      if (i==start)
         DrawLine(Rp,ox,oy,x,y,7);
      else
         DrawLine(Rp,px,py,x,y,7);
      BigRioDot(Rp,x,y,7);
      px=x;
      py=y;
      if (aiwaypoints[i].x==-2L) i++;
   }
}

DrawRioWayPoints(RPS *Rp,int type)
{
   int   i,wp;

   for (i=0;i<aipaths[OurPath].numWPs;i++)
      {
      wp=aipaths[OurPath].wp0+i;
      if (aiwaypoints[wp].x<0)
         RioWpMark(Rp,GetPlaneOrgNum((int)aiwaypoints[wp].y),type);
      if (aiwaypoints[wp].x==-2L)
         i++;
      }
}

RioCircle(RPS *Rp,int x,int y,int color,int radius,char *str)
{
   int	r34;
   int	i,ang;
   int	nx,ny;
   int	px,py;

   r34=radius*3/4;
   i=0;
   ang=0;
   for (i=0;i<0x80;i++,ang+=0x200)
      {
      nx=ASinB(radius,ang)+x;
      ny=y-ACosB(r34,ang);
      if (i==0)
         {
         px=nx;
         py=ny;
         }
      DrawLine(Rp,px,py,nx,ny,color);
      px=nx;
      py=ny;
      }
   RpPrint(Rp,x-pstrlen(Rp,str)/2,y-r34,str);
}

DrawRioTrack(RPS *Rp)
{
   int	cnt,i;
   int	x,y;
   int	px,py;
   int	ox,oy;
	extern char gstr220[];
	extern char gstr221[];

   cnt=UpdateOurWPS(-999,1);
   ox=(int)(PxLng>>12);
   oy=MulDiv((int)((0x100000L-PyLng)>>12),192,256);
   RioCircle(Rp,ox,oy,7,29,gstr220);
   RioCircle(Rp,ox,oy,7,58,gstr221);
   for (i=0;i<=cnt;i++)
      {
      x=(int)(wps[i].xL>>12);
      y=MulDiv((int)(wps[i].yL>>12),192,256);
      if (i!=0)
         DrawLine(Rp,px,py,x,y,7);
      if (i==NextWayPoint)
         DrawLine(Rp,ox,oy,x,y,7);
      BigRioDot(Rp,x,y,7);
      px=x;
      py=y;
      }
}

GetToken(char *S, char *D)
{
   int l;
   l=0;


   while (isspace(*(S)))
      {
      *(D++)=*(S++);
      l++;
      }
   while (  isprint(*(S))&&(isspace(*(S))==0) )
      {
      *(D++)=*(S++);
      l++;
      }

   *(D)=0;
   return(l);
}

TextInTheBox(RPS *Rp,char *S, int xl, int yl, int xh, int yh, int color)
{
   int   i,j,l,tl;
   char T[120];
   int   Xp,Yp;
   int   oldcolor;
   int   fontht;

   oldcolor=Rp->APen;
   Rp->APen=color;

   xl+=6;
   yl+=6;
   xh-=6;
   yh-=6;

   fontht=FontHeight(Rp->FontNr);

   Yp= yl;
   i=0;
   while (1)
      {
      if ((Yp+fontht)>yh) break;
      Xp=xl;
      j=i;
      l=0;
      while (isspace(*(S+j)))
         j++;
      do
         {
         tl=GetToken(S+j,&T[l]);
         if (tl==0) break;
         if ( (Xp+pstrlen(Rp,T))>xh  ) break;
         j+=tl;
         l+=tl;
         } while (T[l]!='\n');
      T[l]=0;
      RpPrint(Rp,Xp,Yp,T);
      Yp+=fontht;
      i=j;
      if (tl==0) break;
      }
   Rp->APen=oldcolor;
}

//int   RioFont=0;

DrawRioMapInfo(RPS *Rp,int force)
{
   RPS	sav;
   int	x,y,i;
   struct PlaneData far *tp;

   RectCopy (Rp3,0,0,256,200,Rp2,0,0);
   sav=*Rp2;
   Rp2->XBgn=0;
   Rp2->YBgn=0;
   Rp2->Width1=0xff;
   Rp2->Length1=0xbb;
   switch (force)
      {
      case 1:
         x=(int)(ps[whichourcarr].xL>>12);
         y=MulDiv((int)(ps[whichourcarr].yL>>12),192,256);
         ScaleRotate(GRPAGE0[2],0x10f,0xe,6,9,Rp2,x,y,0x100,0,0,0);
         break;
      case 2:
         x=(int)(PxLng>>12);
         y=MulDiv((int)((0x100000L-PyLng)>>12),192,256);
         ScaleRotate(GRPAGE0[2],0x110,4,9,9,Rp2,x,y,0x100,0,0,0);
         DrawRioTrack(Rp2);
         break;
      case 3:
         if (RioWingOK)
            {
            x=(int)(ps[AIWingMan].xL>>12);
            y=MulDiv((int)(ps[AIWingMan].yL>>12),192,256);
            ScaleRotate(GRPAGE0[2],0x11a,4,9,9,Rp2,x,y,0x100,0,0,0);
            }
         break;
      case 4:
         for (i=0;i<LASTPLANE;i++)
            {
            if ((ps[i].status&ALIVE)&&(ps[i].status&FRIENDLY))
               {
               tp=GetPlanePtr(ps[i].type);
               if (tp->PlaneEnumId>=SO_F_14FDA && tp->PlaneEnumId<=SO_AS_4)
                  {
                  x=(int)(ps[i].xL>>12);
                  y=MulDiv((int)(ps[i].yL>>12),192,256);
                  ScaleRotate(GRPAGE0[2],0x110,4,9,9,Rp2,x,y,0x100,0,0,0);
                  }
               }
            }
         DrawRioWayPoints(Rp,1);
         break;
      case 5:
         for (i=0;i<LASTPLANE;i++)
            {
            if (!((ps[i].status&ALIVE)&&(ps[i].status&FRIENDLY)))
               continue;
            tp=GetPlanePtr(ps[i].type);
            if (tp->PlaneEnumId<SO_F_14FDA || tp->PlaneEnumId>SO_AS_4)
               {
               x=(int)(ps[i].xL>>12);
               y=MulDiv((int)(ps[i].yL>>12),192,256);
               switch (tp->bclass)
                  {
               case FIGHTER_OBJCLASS:
                  ScaleRotate(GRPAGE0[2],0x124,4,9,9,Rp2,x,y,0x100,0,0,0);
                  break;
               case BOMBER_OBJCLASS:
               case HELICOPTER_OBJCLASS:
                  ScaleRotate(GRPAGE0[2],0x138,4,9,9,Rp2,x,y,0x100,0,0,0);
                  break;
               case CARRIER_OBJCLASS:
                  ScaleRotate(GRPAGE0[2],0x11a,0xe,3,9,Rp2,x,y,0x100,0,0,0);
                  break;
               case CMISSILE_OBJCLASS:
                  ScaleRotate(GRPAGE0[2],0x12e,4,9,9,Rp2,x,y,0x100,0,0,0);
                  break;
                  }
               }
            }
         DrawRioWayPoints(Rp,0);
         break;
      }
   *Rp2=sav;
   MouseHIDE();
   RectCopy (Rp2,0,0,320,200,Rp1,0,0);
   MouseSHOW();
}
//**********************************************************************

RioLegend()
{
   if (!(Status & TRAINING))
      RectCopy (Rp3,268,72,51,13,Rp2,268,72);
   if (!RioWingOK)
      RectCopy (Rp3,268,117,51,13,Rp2,268,117);
   RectCopy (Rp2,256,50,63,149,Rp3,256,50);
}
//**********************************************************************

RioMap()
{
   int	i,j;
   int	key;
   int	done;
   int	MyMouseHits;
   int	val,ofont;
	extern char gstr702[];
	extern char gstr302[];
	extern char gstr2[];
	extern char gstr703[];
	extern char gstr704[];
	extern char gstr705[];
	extern char gstr706[];
	extern char gstr707[];
	extern char gstr708[];
	extern char gstr709[];
	extern char gstr710[];
	extern char gstr711[];
	extern char gstr712[];
	extern char gstr713[];
	extern char gstr714[];
	extern char gstr715[];
	extern char gstr716[];
	extern char gstr717[];
   RPS Rpdef  =  {1, 0,0, 319,199, JAM3, GREEN, BLACK, 2};
   RPS *Rtemp  = &Rpdef;
   FILE    *File;
   struct _missionHeader_ start_data;
   char S[255];

   RioWingOK=0;
   if (AIWingMan != -999)
      if (ps[AIWingMan].status & ALIVE)
         RioWingOK=1;

   S[0]=0;
   strcpy (S,gstr702);
   if ((File = fopen (gstr302, gstr2)) != NULL)
      {
      fread (&start_data, sizeof (struct _missionHeader_), 1, File);
      if (start_data.goalLoc && start_data.goalLen)
         {
         fseek  (File, start_data.goalLoc, SEEK_SET) ;
         j=start_data.goalLen;
         if (j>0xfe) j=0xfe;
         fread (S, sizeof(char), j, File);
         fclose(File);
         for (i=0;i<j;i++)
            S[i]=toupper(S[i]);
         S[j]=0;
         S[254]=0;
         }
      else
         fclose(File);
      }
   if (DoMustLive==1)
      strcat (S,gstr703);
   else if (DoMustLive==2)
      strcat (S,gstr704);
   if (DoMustDie==1)
      strcat (S,gstr705);
   else if (DoMustDie==2)
      strcat (S,gstr706);
   if (DoMustEsc==2)
      strcat (S,gstr707);
   if (MinTimeSet==1)
      strcat (S,gstr708);
   else if (MinTimeSet==2)
      strcat (S,gstr709);

   MouseSETUP(MouseSprite,0,0);
   ScreenOff();
   SndSounds(N_Pause);

   LoadPicture(2,gstr710);

   switch(ConfigData.World) {
      case 0:
         LoadPicture(1,gstr711);
      break;
      case 1:
         LoadPicture(1,gstr712);
      break;
      case 2:
         LoadPicture(1,gstr713);
      break;
      case 3:
         LoadPicture(1,gstr714);
      break;
      case 4:
         LoadPicture(1,gstr715);
      break;
      default:
         LoadPicture(1,gstr713);
      break;
   }
   RioLegend();
   RectCopy (Rp2, 0,0,256,200,Rp3,0,0);

   val=1;
   DrawRioMapInfo(Rtemp,1);

   ScreenOn();
   MouseSHOW();

   while (kbhit()) _bios_keybrd (_KEYBRD_READ); /* Flush kbd buffer */

   done=0;
   while (!done)
      {
      key=0;
      MyMouseHits = MouseHITS();

      if (kbhit())
         key = _bios_keybrd (_KEYBRD_READ);

      if (key==0)
         {
         if (MyMouseHits & 1)
            {
            if (InBox(MouseX,MouseY,268,177,318,194))
               key=ESCKEY;
            if (InBox(MouseX,MouseY,268,57,318,69))
               key=BKEY;
            if (InBox(MouseX,MouseY,268,72,318,84))
               key=MKEY;
            if (InBox(MouseX,MouseY,268,87,318,99))
               key=CKEY;
            if (InBox(MouseX,MouseY,268,102,318,114))
               key=PKEY;
            if (InBox(MouseX,MouseY,268,117,318,129))
               key=WKEY;
            if (InBox(MouseX,MouseY,268,132,318,144))
               key=FKEY;
            if (InBox(MouseX,MouseY,268,147,318,159))
               key=OKEY;
            while (Button & 1);
            MouseHITS();
            }
         }

      if (key)
         {
         switch (key)
            {
            case ESCKEY:
            case RKEY:
            case SLASH:
               done++;
               break;
            case WKEY:
               if (RioWingOK)
                  {
                  val=3;
                  DrawRioMapInfo(Rtemp,3);
                  }
               break;
            case OKEY:
               val=5;
               DrawRioMapInfo(Rtemp,5);
               break;
            case PKEY:
               val=2;
               DrawRioMapInfo(Rtemp,2);
               break;
            case FKEY:
               val=4;
               DrawRioMapInfo(Rtemp,4);
               break;
            case CKEY:
               val=1;
               DrawRioMapInfo(Rtemp,1);
               break;
            case BKEY:
               DrawRioMapInfo(Rtemp,-1);
               RectFill(Rp2, 0x28,0x40,0xb5,0x65,0);
               RectFill(Rp2, 0x29,0x41,0xb3,0x63,4);
               RectFill(Rp2, 0x2a,0x42,0xb1,0x61,0);
               TextInTheBox(Rtemp,S,0x28,0x40,0xdc,0xa4,0xff);
               Rtemp->APen=MSGCOLOR;
               CenterPrint (Rtemp, 0x82,0x9a,gstr716);
               MouseHIDE();
               RectCopy (Rp2, 0x28,0x40,0xb5,0x65, Rp1,0x28,0x40);
               MouseHITS();
               while (1)
                  {
                  MyMouseHits = MouseHITS();
                  if (kbhit()) break;
                  if (MyMouseHits&3) break;
                  }
               while (kbhit()) _bios_keybrd (_KEYBRD_READ); /* Flush kbd buffer */
               MouseHITS();
               DrawRioMapInfo(Rtemp,val);
               MouseSHOW();
               break;
            case MKEY:
               if (Status & TRAINING)
                  {
                  MouseHIDE();
                  val=2;
                  DrawRioMapInfo(Rtemp,2);
                  ofont=Rp1->FontNr;
                  Rp1->FontNr=Rtemp->FontNr;
                  TextInTheBox(Rp1,gstr717,0x15,2,0xff,0x1e,0);
                  TextInTheBox(Rp1,gstr717,0x14,1,0xff,0x1e,4);
                  Rp1->FontNr=ofont;
                  MouseSHOW();
                  while (1)
                     {
                     key=0;
                     MyMouseHits = MouseHITS();
                     if (kbhit())
                        key = _bios_keybrd (_KEYBRD_READ);
                     if (key==ESCKEY) break;
                     if (MyMouseHits&1)
                        {
                        if ((MouseX<0x100)&&(MouseY<0xc0))
                           {
                           PxLng = MouseX;
                           PxLng = (PxLng << 12);
                           PyLng = MulDiv(256,MouseY,192);
                           PyLng = (PyLng << 12);
                           PyLng = (0x100000 - (PyLng));
                           CheckOurAlt();
                           }
                        break;
                        }
                     }
                  DrawRioMapInfo(Rtemp,2);
                  }
               break;
            }
         }
      }

   MouseHIDE();
   ScreenOff();
   lastmsgview=-1;

   if (View<FRONTAL) {
      LoadNewCockpit();
   } else {
      ClearPage(0, BLACK);
      InitSpecialPal(-1);
   }
   UpdatePalette();
   ScreenOn();
   SndSounds(N_UnPause);
   return 1;
}

#if 0
		// Re-position our plane

		if (MyMouseHits & 2) {
			PxLng = MouseX;
			PyLng = MouseY;
			PyLng += PyLng/3;
			PxLng = (PxLng << 12);
			PyLng = (0x100000 - (PyLng << 12));

			GroundH = GetQHeight(PxLng,(0x100000-PyLng));

			if ((GroundH+500) > OurAlt) {
				OurAlt = GroundH + 500;
				Alt = OurAlt >> 2;
				InitScottFlight();
			}

			MapX = PxLng >> 12;
   			MapY = (0x100000 - PyLng) >> 12;
			MapY -= MapY/4;

			if ((mseg=MapInEMSSprite(MISCSPRTSEG,1))) {
				ScaleRotate(mseg,(214+5),(62+5),10,10,Rtemp,(int)MapX,
							(int)MapY,0x0100,0,0,0);
			}
		}
	}























	extern  MapX;

	extern  MapY;


	extern  MapX;

	extern  MapY;

	MapY -= (MapY/4);

	if ((mseg=MapInEMSSprite(MISCSPRTSEG,1))) {
		ScaleRotate(mseg,(214+5),(62+5),10,10,Rtemp,(int)MapX,
			(int)MapY,0x0100,0,0,0);
	}

    for (i=0, psi=ps; i<MAXPLANE; i++, psi++) {
        if ((psi->status&ACTIVE) && (psi->status&ALIVE)) {
			MapX = psi->xL >> 12;
			MapY = (0x100000 - psi->yL) >> 12;
			MapY -= MapY/4;
            a = (psi->head-OurHead) + 0x0800;
			tempplane = GetPlanePtr(psi->type);

			if (psi->status & FRIENDLY)
				b=2;
			else
				b=0;

			xpos = 154 + (((a>>12)&15)*10);

			switch(tempplane->bclass) {
				case FIGHTER_OBJCLASS:
					if (b==2)
						ypos=22;
					else
						ypos=82;
				break;
				case BOMBER_OBJCLASS:
					if (b==2)
						ypos=42;
					else
						ypos=102;
				break;
				case CARRIER_OBJCLASS:
					if (b==2) {
						ypos=52;
					} else {
						xpos=194;
						ypos=62;
					}
				break;
				case SHIP_OBJCLASS:
					if (b==2) {
						xpos=154;
						ypos=22;
					} else {
						xpos=194;
						ypos=62;
					}
				break;
				case CMISSILE_OBJCLASS:
					if (b==2)
						ypos=22;
					else
						ypos=72;
				break;
				default:
					if (b==2)
						ypos=22;
					else
						ypos=82;
				break;

			}
			if ((mseg=MapInEMSSprite(MISCSPRTSEG,1))) {
				ScaleRotate(mseg,(xpos+5),(ypos+5),10,10,Rtemp,(Rtemp->XBgn+(int)MapX),
							(Rtemp->YBgn+(int)MapY),0x0100,0,0,0);
			}
		}
    }

	for (i=0; i<4; i++) {
		MapX = wps[i].xL >> 12;
		MapY = wps[i].yL >> 12;
		MapY -= MapY/4;
		RectFill(Rtemp,(int)MapX-3,(int)MapY-3,2,2,6);
	}


	while (kbhit()) _bios_keybrd (_KEYBRD_READ); /* Flush kbd buffer */

	while (!done) {
		key=0;
		MyMouseHits=0;
		MyMouseHits = MouseHITS();

		if (kbhit())
			key = _bios_keybrd (_KEYBRD_READ);

		if (key) {
			switch (key) {
         		case ESCKEY:
				case SLASH:
					done++;
				break;
         		default:
            	break;
			}
		}

		// Re-position our plane

		if (MyMouseHits & 2) {
			PxLng = MouseX;
			PyLng = MouseY;
			PyLng += PyLng/3;
			PxLng = (PxLng << 12);
			PyLng = (0x100000 - (PyLng << 12));

			GroundH = GetQHeight(PxLng,(0x100000-PyLng));

			if ((GroundH+500) > OurAlt) {
				OurAlt = GroundH + 500;
				Alt = OurAlt >> 2;
				InitScottFlight();
			}

			MapX = PxLng >> 12;
   			MapY = (0x100000 - PyLng) >> 12;
			MapY -= MapY/4;

			if ((mseg=MapInEMSSprite(MISCSPRTSEG,1))) {
				ScaleRotate(mseg,(214+5),(62+5),10,10,Rtemp,(int)MapX,
							(int)MapY,0x0100,0,0,0);
			}
		}
	}


	MouseHIDE();
	ScreenOff();
	lastmsgview=-1;

	if (View<FRONTAL) {
    	LoadNewCockpit();
	} else {
       	ClearPage(0, BLACK);
		LoadPalNoPic(gstr357);
	}
	SndSounds(N_UnPause);
    return 1;
#endif


#define  HMAPBANK 0

RioTop(RPS *Rp, long X, long Z, int zoom )
{
   int   XP,ZP;
   int   WID,HT,XDC,YDC;
   char far *PTR;
   int   SEG;

   XP= (X>>12);
   ZP= (Z>>12);
   WID = Rp->Width1+1;
   HT  = Rp->Length1+1;
   XDC= Rp->XBgn+WID/2;
   YDC= Rp->YBgn+HT/2;

   FP_OFF(PTR)=0;
   FP_SEG(PTR)=SEG=MapInBank(HMAPBANK);

   TacScaleRotate256(SEG, (255/2),(191/2),255,191,Rp,(320/2),(200/2), 0x0100, 0,0x52,0);

}

