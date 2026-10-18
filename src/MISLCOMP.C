#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdlib.h>

extern FILE * tfile;

extern FILE * outfile;

extern char buf[15];


extern int MissileEnumId;

extern char name[12];

extern int range;

extern int minrange;

extern int maxalt;

extern int speed;

extern int homing;

extern int maneuver;

extern int cmeffect;

extern int damage;

extern char filename[12];

extern int type;


struct cl {
	int MissileEnumId;
	char name[12];
	int range;
	int minrange;
	int maxalt;
	int speed;
	int homing;
	int maneuver;
	int cmeffect;
	int damage;
	char filename[12];
	int type;
};
extern struct cl missilerec;


extern int recsize;

extern int flstat;

extern int planecnt;

extern int i;

extern long recseek;


main()
{
	tfile = fopen("F14MD01.TXT","r");

	printf("\nFleet Defender - Missile Data File Compiler\n");
	printf("\nCompiling F14MD01.TXT...");
	if (tfile == NULL) {
		printf("\nError:  Cannot find file F14MD01.TXT\n");
		exit(-1);
	}

	outfile = fopen("F14MD01.DAT","wb");


	while ((flstat = fscanf(tfile,"%d %s %d %d %d %d %d %d %d %d %s %d\n",&MissileEnumId,name,&range,
		&minrange,&maxalt,&speed,&homing,&maneuver,&cmeffect,&damage,filename,&type)) != EOF) {
				missilerec.MissileEnumId = MissileEnumId;
				strcpy(missilerec.name,name);
				missilerec.range = range;
				missilerec.minrange = minrange;
				missilerec.maxalt = maxalt;
				missilerec.speed = speed;
				missilerec.homing = homing;
				missilerec.maneuver = maneuver;
				missilerec.cmeffect = cmeffect;
				missilerec.damage = damage;
				strcpy(missilerec.filename,filename);
				missilerec.type = type;
				fwrite(&missilerec,recsize,1,outfile);
				planecnt++;
		}

	fclose(tfile);
	fclose(outfile);

	printf("Compile Complete.");

	outfile = fopen("F14MD01.DAT","rb");

	for (i=0; i<planecnt; i++) {
		recseek = (long)(i * recsize);
		fseek(outfile,recseek,SEEK_SET);
		fread(&missilerec,recsize,1,outfile);

		printf("%-3d %-12s %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-12s %-3d\n",
			missilerec.MissileEnumId, missilerec.name,missilerec.range, missilerec.minrange,
			missilerec.maxalt,missilerec.speed,missilerec.homing,missilerec.maneuver,
			missilerec.cmeffect,missilerec.damage,missilerec.filename,missilerec.type);
	}
	fclose(outfile);
}
