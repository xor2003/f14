#define LB   0x01
#define PG   0x02
#define NC   0x04
#define CE   0x08

#define COLD 0x01
#define LIM  0x02
#define HOT  0x04

#define NIGHT   0x01
#define TIMED   0x02
#define LIGHTS  0x04
#define BAY4    0x08
#define BEACON  0x10
#define DISAPPEAR 0x20

#define M_AIRSTRIP    1
#define M_SAMRAD      2  
#define M_MOBSAM      3  
#define M_OTHRAD      4  
#define M_LPARRAD     5 
#define M_REFINERY    6
#define M_TANKFARM    7
#define M_HANGAR      8  
#define M_HQ          9      
#define M_CARRIER     10 
#define M_WARSHIP     11 
#define M_MBOAT       12   
#define M_SAM         13     
#define M_OILRIG      14  
#define M_NUKEP       15   
#define M_SUBPEN      16  
#define M_MSHIP       17   
#define M_SUB         18    
#define M_DEPOT       19   
#define M_BRIDGE      20  
#define M_AIRBASE     21
#define M_GPLANE      22
#define M_CAMP        23

#define O_SAMRAD    0
#define O_MOBSAM    1
#define O_OTHRAD    0
#define O_LPARRAD   0
#define O_REFINERY  9
#define O_TANKFARM  4
#define O_STEALTH   5
#define O_PLANES    6
#define O_AN72      6
#define O_TCAMP    11
#define O_KREMLIN   3
#define O_WARSHIP   7
#define O_MBOAT     7
#define O_SAM       1
#define O_OILRIG    8
#define O_NUKEP     2
#define O_MSHIP     0
#define O_DEPOT     0
#define O_HQ        0

#define PHOTO       1
#define STRIKE      2
#define AIRDROP     3
#define DELIVERY    4
#define INTERCEPT   5
#define RETURN      6
#define RAID        7
#define PENETRATE   8

struct MissionData {
        int where;
        int when;
        int type;
        int target;
        int special;
        int object;
    };

#ifdef _mission_data
extern struct MissionData md[];
 

#define NMISSIONS (sizeof(md)/sizeof(struct MissionData))

#else
extern struct MissionData md[];
#endif




typedef struct _MissionPoint {

        int     x ;
        int     y ;

        } MissionPoint ;


typedef struct _Objects {
        int     offset ;        // 1 based, 0 for end of list, 
                                //     neg for crit targs, 
                                //     pos for all inclusive (including point targs)
        int     crit_flag ;     // bits set for critical targets
        int     non_crit_flag ; // bits set for non-critical targets
        int     num_non_crit ;  // # non-crits needed
        
        } Objects ;



#ifdef _TESTING

        typedef struct Mission_ {

                UBYTE   stamp ;
                UBYTE   class ;
                int     priority ;

                Objects Objects[10] ;

                int      num_non_crit ;
                MissionPoint location ;
                unsigned StampLoc ;
                unsigned accomplished ;


                char    *BriefingText ;

                } Mission ;

#else


        typedef struct Mission_ {

                UBYTE   stamp ;
                UBYTE   class ;
                int     priority ;

                Objects Objects[10] ;

                int      num_non_crit ;
                MissionPoint location ;
                unsigned StampLoc ;
                unsigned accomplished ;


                char    BriefingText[64] ;

                } Mission ;


#endif

#define CSOFT_A  1
#define CHARD_A  2
#define CAREA_S  3
#define CAREA_H  4
#define NAVAL_S  5
#define NAVAL_H  6



extern Mission AllMissions[] ;


//#define _NDEBUG
