#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdlib.h>
#include "setup.h"

struct _logObject_ {                   


        int             objectNum ;                                      
                                                                            
                                                                    

        int             objectType ;                              

        int             side ;                                
        long            x ;
        long            y ;
        long            z ;
        int             speed ;                    

        long            dramaticEvent ;                        
                                                                      

        int             missileOwner ;                                   
                                                                               
                                                      

		int             missileType;                                   

        int             time ;                                   

		int 			damageLevel;	                                    


        };
extern struct _logObject_ _logObject;


extern FILE * tfile;

extern FILE * outfile;

extern char buf[15];


extern int recsize;

extern int flstat;

extern int planecnt;

extern int i;

extern long recseek;


main()
{
	char s[80];
	int cnt = 0;


	outfile = fopen("log.f14","rb");

	if (outfile == NULL) {
		printf("\nError:  Cannot find file log.f14\n");
		exit(-1);
	}

	printf("LOG Start\n");

	while ((flstat = fread (&_logObject, recsize, 1, outfile)) != 0) {

		printf(" \n", flstat);
		switch(_logObject.dramaticEvent) {
			case SLG_AIR_MISSILE_LAUNCH:
				strcpy(s,"AIR MISSILE LAUNCH");
				break;

			case SLG_BOAT_MISSILE_LAUNCH:
				strcpy(s,"BOAT MISSILE LAUNCH");
				break;

			case SLG_GROUND_MISSILE_LAUNCH:
				strcpy(s,"GROUND MISSILE LAUNCH");
				break;

			case SLG_AIR_CRUISE_LAUNCH:
				strcpy(s,"AIR CRUISE LAUNCH");
				break;

			case SLG_BOAT_CRUISE_LAUNCH:
				strcpy(s,"BOAT CRUISE LAUNCH");
				break;

			case SLG_GROUND_CRUISE_LAUNCH:
				strcpy(s,"GROUND CRUISE LAUNCH");
				break;

			case SLG_AIR_MISSILE_HIT:
				strcpy(s,"AIR MISSILE HIT");
				break;

			case SLG_BOAT_MISSILE_HIT:
				strcpy(s,"BOAT MISSILE HIT");
				break;

			case SLG_GROUND_MISSILE_HIT:
				strcpy(s,"GROUND MISSILE HIT");
				break;

			case SLG_AIR_BULLET_HIT:
				strcpy(s,"AIR BULLET HIT");
				break;

			case SLG_BOAT_BULLET_HIT:
				strcpy(s,"BOAT BULLET HIT");
				break;

			case SLG_GROUND_BULLET_HIT:
				strcpy(s,"GROUND BULLET HIT");
				break;

			case SLG_AIR_CRUISE_HIT:
				strcpy(s,"AIR CRUISE HIT");
				break;

			case SLG_BOAT_CRUISE_HIT:
				strcpy(s,"BOAT CRUISE HIT");
				break;

			case SLG_GROUND_CRUISE_HIT:
				strcpy(s,"GROUND CRUISE HIT");
				break;

			case SLG_AIR_DESTROYED_MISSILE:
				strcpy(s,"AIR DESTROYED MISSILE");
				break;

			case SLG_BOAT_DESTROYED_MISSILE:
				strcpy(s,"BOAT DESTROYED MISSILE");
				break;

			case SLG_GROUND_DESTROYED_MISSILE:
				strcpy(s,"GROUND DESTROYED MISSILE");
				break;

			case SLG_AIR_DESTROYED_BULLET:
				strcpy(s,"AIR DESTROYED BULLET");
				break;

			case SLG_BOAT_DESTROYED_BULLET:
				strcpy(s,"BOAT DESTROYED BULLET");
				break;

			case SLG_GROUND_DESTROYED_BULLET:
				strcpy(s,"GROUND DESTROYED BULLET");
				break;

			case SLG_AIR_DESTROYED_CRUISE:
				strcpy(s,"AIR DESTROYED CRUISE");
				break;

			case SLG_BOAT_DESTROYED_CRUISE:
				strcpy(s,"BOAT DESTROYED CRUISE");
				break;

			case SLG_GROUND_DESTROYED_CRUISE:
				strcpy(s,"GROUND DESTROYED CRUISE");
				break;

			case SLG_AIR_BOMB_DROP:
				strcpy(s,"AIR BOMB DROP");
				break;

			case SLG_AIR_BOMB_HIT:
				strcpy(s,"AIR BOMB HIT");
				break;

			case SLG_BOAT_BOMB_HIT:
				strcpy(s,"BOAT BOMB HIT");
				break;

			case SLG_GROUND_BOMB_HIT:
				strcpy(s,"GROUND BOMB HIT");
				break;

			case SLG_AIR_DESTROYED_BOMB:
				strcpy(s,"AIR DESTORYED BOMB");
				break;

			case SLG_BOAT_DESTROYED_BOMB:
				strcpy(s,"BOAT DESTORYED BOMB");
				break;

			case SLG_GROUND_DESTROYED_BOMB:
				strcpy(s,"GROUND DESTORYED BOMB");
				break;

			case SLG_AIR_TROOP_DROP:
				strcpy(s,"AIR TROOP DROP");
				break;

			case SLG_AIR_TAKEOFF:
				strcpy(s,"AIR TAKEOFF");
				break;

			case SLG_AIR_LAND:
				strcpy(s,"AIR LAND");
				break;

			case SLG_AIR_CRASH:
				strcpy(s,"AIR CRASH");
				break;

			case SLG_AIR_REFUEL:
				strcpy(s,"AIR REFUEL");
				break;

			case SLG_CONFIG_CHANGE:
				strcpy(s,"CONFIG CHANGE");
				break;

			case SLG_AIR_BAD_EJECT:
				strcpy(s,"BAD EJECTION");
				break;

			case SLG_AIR_GOOD_EJECT:
				strcpy(s,"GOOD EJECTION");
				break;

			case SLG_TRAINING_TURNED_ON:
				strcpy(s,"TRAINING NOW ON");
				break;

			case SLG_TRAINING_TURNED_OFF:
				strcpy(s,"TRAINING NOW OFF");
				break;

			case SLG_AAA_HIT:
				strcpy(s,"AAA HIT");
				break;

			case SLG_AAA_DESTROY:
				strcpy(s,"AAA DESTROY");
				break;

			case SLG_WAY_PT_GOAL:
				strcpy(s,"WAY POINT GOAL");
				break;

			case SLG_MIN_TIME_REACHED:
				strcpy(s,"REACHED MIN TIME ALOFT");
				break;

			default:
				strcpy(s,"UNKNOWN");
				break;
		}

		printf("%d: %d %d %s %d %ld %ld %ld %d Owner %d missile %d, time %d %d\n",
			cnt, _logObject.objectNum, _logObject.objectType, s, _logObject.side,
			_logObject.x, _logObject.y, _logObject.z,
			_logObject.speed,_logObject.missileOwner, _logObject.missileType, _logObject.time, _logObject.damageLevel);
		cnt += 1;
	}

	fclose(outfile);
	printf("LOG END\n");
}
