#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdlib.h>

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

extern int miss3;

extern int mcount1;

extern int mcount2;

extern int mcount3;

extern int radarsig;

extern int gun;

extern int alignment;


struct cl {
	int plane_no;
	char name[7];
	int speed;
	int maneuver;
	int rclass;
	char filename[13];
	int radarsig;
	int gun;
	int alignment;
	int ecm;
	int misl[3];
	int mcnt[3];
	int behav;
};
extern struct cl planerec;


extern int recsize;

extern int flstat;

extern int planecnt;

extern int i;

extern long recseek;

extern int planeno;

extern int behav;

extern int ecm;

extern int status;


main()
{
	tfile = fopen("F14PD01.TXT","r");

	printf("\nFleet Defender - Plane Data File Compiler\n");
	printf("\nCompiling F14PD01.TXT...");
	if (tfile == NULL) {
		printf("\nError:  Cannot find file F14PD01.TXT\n");
		exit(-1);
	}

	outfile = fopen("F14PD01.DAT","wb");


	while ((flstat = fscanf(tfile,"%d %s %d %d %d %s %d %d %d %d %d %d %d %d %d %d %d\n",&planeno,name,&speed,
		&maneuver,&rclass,filename,&radarsig,&gun,&alignment,&ecm,&miss1,&miss2,&miss3,&mcount1,&mcount2,&mcount3,&behav)) != EOF) {
				planerec.plane_no = planeno;
				strcpy(planerec.name,name);
				planerec.speed = speed;
				planerec.maneuver = maneuver;
				planerec.rclass = rclass;
				strcpy(planerec.filename,filename);
				planerec.radarsig = radarsig;
				planerec.gun = gun;
				planerec.alignment = alignment;
				planerec.ecm = ecm;
	   			planerec.misl[0] = miss1;
				planerec.misl[1] = miss2;
				planerec.misl[2] = miss3;
				planerec.mcnt[0] = mcount1;
				planerec.mcnt[1] = mcount2;
				planerec.mcnt[2] = mcount3;
				planerec.behav = behav;
				fwrite(&planerec,recsize,1,outfile);
				planecnt++;
		}

	fclose(tfile);
	fclose(outfile);
	printf("Compile Complete.\n");

	outfile = fopen("F14PD01.DAT","rb");

	for (i=0; i<planecnt; i++) {
		recseek = (long)(i * recsize);
		fseek(outfile,recseek,SEEK_SET);
		fread(&planerec,recsize,1,outfile);

		printf("%-3d %-8s %-4d %-3d %-3d %-12s %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
			planerec.plane_no,planerec.name,planerec.speed,
			planerec.maneuver,planerec.rclass,planerec.filename,
			planerec.radarsig,planerec.gun,planerec.alignment,planerec.ecm,
			planerec.misl[0],planerec.misl[1],planerec.misl[2],planerec.mcnt[0],
			planerec.mcnt[1],planerec.mcnt[2],planerec.behav);
	}
	fclose(outfile);
}
