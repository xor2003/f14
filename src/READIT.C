#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <dos.h>


extern FILE * tfile;

extern FILE * outfile;

extern char buf[15];


extern char name[7];

extern int speed;

extern int maneuver;

extern int rclass;

extern char filename[13];

extern int miss1;

extern int miss2;

extern int mcount1;

extern int mcount2;


struct cl {
	int plane_no;
	char name[7];
	int speed;
	int maneuver;
	int rclass;
	char filename[13];
	int misl[2];
	int mcnt[2];
	int behav;
	int ecm;
	int status;
};

extern struct cl planerec;


struct PlaneData {
	int plane_no;
	char name[7];
	int speed;
	int maneuver;
	int rclass;
	int misl[2];
	int mcnt[2];
	int behav;
	int ecm;
	int status;
};


extern int recsize;

extern int flstat;

extern int planecnt;

extern int i;

extern long recseek;

extern int planeno;

extern int behav;


extern struct PlaneData far * MasterList;

extern struct PlaneData far * templist;


extern unsigned newseg;

extern int PlaneCnt;


char far *fstrcpy( char far *D, char far *S);
struct PlaneData far *GetPtr(int num);

extern unsigned memsize;

extern int planecount;

extern unsigned int maxsize;

extern unsigned maxseg;

extern unsigned newmem;


main()
{
	outfile = fopen("F14PD01.DAT","rb");

	_dos_allocmem(memsize,&newseg);
	FP_SEG(MasterList) = newseg;
	FP_OFF(MasterList) = 0;

	while (!feof(outfile)) {
		recseek = (long)(PlaneCnt * recsize);
		fseek(outfile,recseek,SEEK_SET);
		fread(&planerec,recsize,1,outfile);
		MasterList[PlaneCnt].speed = planerec.speed;
		MasterList[PlaneCnt].rclass = planerec.rclass;
		fstrcpy(MasterList[PlaneCnt].name,planerec.name);
		PlaneCnt++;
	}

	fclose(outfile);

	PrintList();
	templist = GetPtr(20);
	printf("\n\nName: %Fs\n",templist->name);
	_dos_freemem(newseg);
}

struct PlaneData far *GetPtr(int num)
{
	return &(MasterList[num]);
}

PrintList()
{
	int i;

	for (i=0; i<(PlaneCnt-1); i++)
		printf("%d %d %Fs\n",MasterList[i].speed,MasterList[i].rclass,
				MasterList[i].name);
}

char far *fstrcpy( char far *D, char far *S)
{
   while ( (*(D++)=*(S++))!=0 );
   return(D);
}


