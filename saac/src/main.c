#define _MAIN_C_

#include "main.h"
#include "util.h"
#include "mail.h"
#include "db.h"
#include "saacproto_util.h"
#include "saacproto_serv.h"
// CoolFish: Family 2001/5/9
#include "acfamily.h"
#include "version.h"
#ifdef _SEND_EFFECT				  // WON ADD 锟斤拷锟斤拷雪锟斤拷锟斤拷锟斤拷锟斤拷锟叫� 
#include "recv.h"
#endif

#include "char.h"

#ifdef _SASQL
#include "sasql.h"
#endif

#include <stdio.h> 
#include <time.h> 

#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/wait.h>
#include <getopt.h>
#include <stdio.h>
#ifndef __APPLE__
#include <malloc.h>
#endif
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include "lock.h"
#define BACKLOGNUM 5

int worksockfd;

struct membuf
{
    int use;
    char buf[512];
//    char buf[1024*128];
    int len;
    int next;
};

struct connection
{
    int use;    
    int fd;       
    int mbtop_ri;
    int mbtop_wi;
    struct sockaddr_in remoteaddr;
    int closed_by_remote;
};

struct membuf *mb;
int mbsize;
int mbuse ;

int cpuuse;


int mainsockfd;             /* accept 锟斤拷  锟斤拷娄锟窖憋拷锟斤拷木锟斤拷 */
struct sockaddr_in localaddr;       /* bind 锟斤拷锟斤拷失锟斤拷锟斤拷锟斤拷 */
    

struct connection *con;        /* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�     */

static int mb_finder=0;              /* mb锟斤拷锟斤拷锟斤拷毛锟斤拷锟斤拷锟斤拷锟斤拷锟阶伙拷锟斤拷
                               锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷  锟斤拷盛锟斤拷 */
// WON FIX
//char tmpbuf[65536];
char tmpbuf[1024*32];
//char tmpbuf[65536*3];         /* read锟斤拷 */

struct timeval select_timeout;

time_t sys_time =0; // Robin add

extern gmsv gs[MAXCONNECTION];

int tcpstruct_init( char *addr, int port, int timeout_ms, int mem_use, int deb);
int tcpstruct_accept1( void );
int tcpstruct_accept( int *tis , int ticount );
int tcpstruct_close( int ti );
int tcpstruct_read( int ti , char *buf , int len );
int tcpstruct_readline( int ti , char *buf , int len ,int k ,int r);
int tcpstruct_readline_chop( int ti , char *buf, int len );
int tcpstruct_write( int ti , char *buf , int len );
int tcpstruct_countmbuse( void );
int tcpstruct_connect( char *addr , int port );
void set_nodelay( int sock );


#define OK     0        /* 锟斤拷   */
#define TCPSTRUCT_ENOMEM -1       /* malloc 锟斤拷   */
#define TCPSTRUCT_ESOCK -2        /* socket 锟斤拷   */
#define TCPSTRUCT_EBIND -3        /* bind 锟斤拷   */
#define TCPSTRUCT_ELISTEN -4      /* listen 锟斤拷   */
#define TCPSTRUCT_EBUG -6         /* 锟斤拷锟狡ヘわ拷锟� */
#define TCPSTRUCT_EINVCIND -7     /* con锟竭硷拷index锟斤拷锟狡撅拷锟斤拷锟叫凤拷 */
#define TCPSTRUCT_EREADFIN -8     /* read 锟斤拷锟铰凤拷锟斤拷锟斤拷锟斤拷卅锟绞伙拷 closed by remote */
#define TCPSTRUCT_EHOST -9        /* gethostbyname 锟斤拷   */
#define TCPSTRUCT_ECONNECT -10    /* connect 锟斤拷   */
#define TCPSTRUCT_ECFULL -11      /* con 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 */
#define TCPSTRUCT_ETOOLONG -12    /* 锟芥互卅锟斤拷锟绞匡拷 */
#define TCPSTRUCT_EMBFULL -13     /* mb 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷  */
#define TCPSTRUCT_ECLOSEAGAIN -14 /* close 锟斤拷2锟皆斤拷木锟斤拷 */


int port;               /* 锟截★拷丞锟接★拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷苹锟斤拷锟斤拷潞獭锟斤拷锟� */
int Total_Charlist;
int Expired_mail;
int Del_Family_or_Member;
int Write_Family;

// Nuke start
char *chartime()
{
    static char buf[80];
    time_t t;
    t=time(0);
    strcpy(buf,ctime(&t));
    buf[strlen(buf)-1]=0;
    return(buf);
}
#ifdef _ANGEL_SUMMON
extern struct MissionTable missiontable[MAXMISSIONTABLE];
static int initMissionTable( void );
int saveMissionTable( void );
void checkMissionTimelimit( void);
#endif

static int initRankTable( void );

/*
  sigaction锟斤拷锟斤拷
 */
void sighandle( int a )
{
  if (a==SIGUSR1) log("sigusr1锟脚猴拷!\n");
  log("锟矫碉拷一锟斤拷锟脚猴拷! 锟届常锟叫讹拷......\n" );
  writeFamily(familydir);
  writeFMPoint(fmpointdir);
  writeFMSMemo(fmsmemodir);
#ifdef _ANGEL_SUMMON
	saveMissionTable();
#endif
  exit(1);
}

// Arminius 7.20 memory unlock
void sigusr1(int a)
{
  int i;
  FILE *f;
  char key[4096],buf[4096];

  signal(SIGUSR1, sigusr1);
  
  f = fopen("./unlock.arg", "r");

  if (f) {
    memset(key, 0, 4096);
    fread(key, 4096, 1, f);
    for (i=0; i<strlen(key); i++) if (key[i]=='\n') key[i]='\0';

    switch (key[0]) {
    case 'P':	// unlock player
      if (DeleteMemLock(getHash(&key[1]) & 0xff,&key[1],&i)) {
        log("ADM: memunlock: %s success.\n", key);
      } else {
        log("ADM: memunlock: %s failed.\n", key);
      }
    break;
    case 'S':	// unlock server
      DeleteMemLockServer(&key[1]);
      log("ADM: memunlock: %s\n", key);
    break;
    case 'C':	// check player lock
      GetMemLockState(getHash(&key[1]) & 0xff, &key[1], buf);
      sprintf(key, "echo \"%s\" > ./sigusr1.result", buf);
      system(key);
    break;
#ifdef _SEND_EFFECT		   // WON ADD 锟斤拷锟斤拷雪锟斤拷锟斤拷锟斤拷锟斤拷锟叫�
	case 'E':	
		log("\nAC 锟斤拷 GS 锟斤拷锟斤拷锟斤拷雪锟斤拷效!!\n");
	    SendEffect(&key[1]);
	break;
#endif

	case 'L':  // Robin 锟叫筹拷锟斤拷锟斤拷Server锟斤拷锟斤拷
		log("\nList All Server Conncet!!!!!\n");
		for( i =0; i <MAXCONNECTION; i++)
			if( gs[i].use)
				log("\n gs[%d] fd:%d name:%s ", i, gs[i].fd, gs[i].name );
	break;

  }
	log(" sigusr1_over_1 ");
	fclose(f);
	log(" sigusr1_over_2 ");

  }
}

static int netWrite( int ti , char *buf , int len)
{
    return tcpstruct_write( ti , buf, len );
}

gmsv gs[MAXCONNECTION];
#ifdef _VIP
int login_game_server( int ti , char *svname , char *svpas , int checkvip,
               char *result , int resultlen ,
               char *retdata , int retdatalen )
#else
int login_game_server( int ti , char *svname , char *svpas ,
               char *result , int resultlen ,
               char *retdata , int retdatalen )
#endif
{
#ifndef _VIP
		char buff[50];
		sprintf(buff,"longzoro-%s-%d",svpass,123);
    if( strcmp( svpas , buff ) == 0 ){
#else
		if( strcmp( svpas , svpass ) == 0 ){
#endif
        log( "锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷确 %s\n" , svname );
    } else {
        log( "锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟� %s\n" , svname );
        snprintf( result , resultlen , "失锟斤拷" );
        snprintf( retdata , retdatalen , "锟斤拷锟斤拷锟斤拷锟�" );
        return 0;
    }
#ifdef _VIP
    if(checkvip==0 || checkvip!=55575556 * 2)
    {
    	snprintf( result , resultlen , "failed" );
      snprintf( retdata , retdatalen , "duplicate login vip" );
      //return 0;
    }
#endif
    {
        int i;
        for(i=0;i<MAXCONNECTION;i++){
            if( gs[i].use &&
                strcmp( gs[i].name , svname ) == 0 ){
                snprintf( result, resultlen, "failed" );
                snprintf( retdata , retdatalen, "duplicate login" );
                return 0;
            } 
        }
    }
    snprintf( gs[ti].name , sizeof(gs[ti].name), "%s" , svname );
	  gs[ti].fd = ti;
    snprintf( result , resultlen ,SUCCESSFUL );
    snprintf( retdata , retdatalen , "Nothing special" );
    DeleteMemLockServer(svname);	// Arminius 7.31 unlock server
    return 0;
}

int logout_game_server( int ti )
{

    gs[ti].use = 0;
	gs[ti].fd = -1;
    gs[ti].name[0] = 0;
    tcpstruct_close( ti );
    return 0;
}
int is_game_server_login( int ti )
{
    return gs[ti].use;
}

static int readConfig( char *path )
{
    char buf[2048];
    FILE *fp;

    fp = fopen( path , "r" );
    if( fp == NULL ){ return -2; }

    while( fgets( buf , sizeof( buf ) , fp )){
        char command[128];
        char param[128];
        chop(buf);
        
        easyGetTokenFromString( buf , 1 , command , sizeof( command ));
        easyGetTokenFromString( buf , 2 , param , sizeof( param ));

        if( strcmp( command , "port" ) == 0 ){
            port = atoi( param );
        		log( "锟剿匡拷:%d\n",port );
        } else if( strcmp( command , "logdir" ) == 0 ){
            snprintf( logdir , sizeof( logdir) , param );
            log( "锟斤拷志目录:%s\n",logdir );
        } else if( strcmp( command , "chardir" ) == 0 ){
            snprintf( chardir , sizeof( chardir) , param );
            log( "锟斤拷锟斤拷目录:%s\n",chardir );
#ifdef _SLEEP_CHAR
						snprintf( sleepchardir , sizeof( sleepchardir), "%s_sleep", chardir);
						log( "睡锟斤拷目录:%s\n",sleepchardir );
#endif
        } else if( strcmp( command , "pass" ) == 0 ){
        		snprintf( svpass , sizeof( svpass ) , param);
            log( "锟斤拷锟斤拷:%s\n",param );
        } else if( strcmp( command , "dbdir" ) == 0 ){
            snprintf( dbdir , sizeof( dbdir) , param );    
            log( "锟斤拷锟斤拷目录:%s\n",dbdir );        
        } else if( strcmp( command, "rotate_interval" ) == 0 ){
            log_rotate_interval = atoi( param );
            log( "锟斤拷志循锟斤拷锟斤拷锟�:%d\n",log_rotate_interval ); 
        } else if( strcmp( command, "maildir" ) == 0 ){
            snprintf( maildir, sizeof( maildir ), param );
            log( "锟绞硷拷目录:%s\n",maildir );
#ifdef	_FAMILY
        // CoolFish: Family 2001/5/9
        } else if( strcmp( command, "familydir" ) == 0 ){
            snprintf( familydir, sizeof( familydir ), param );
            log( "锟斤拷锟斤拷目录:%s\n",familydir );
        } else if( strcmp( command, "fmpointdir" ) == 0 ){
            snprintf( fmpointdir, sizeof( fmpointdir ), param );
            log( "庄园锟斤拷锟斤拷:%s\n",fmpointdir );
        } else if( strcmp( command, "fmsmemodir" ) == 0 ){
            snprintf( fmsmemodir, sizeof( fmsmemodir ), param );
            log( "锟斤拷锟藉备锟斤拷:%s\n",fmsmemodir );
#endif
			  } else if( strcmp( command , "Total_Charlist" ) == 0 ){
        	Total_Charlist = atoi( param );
        	log( "锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷:%d锟斤拷\n",Total_Charlist );
        } else if( strcmp( command , "Expired_mail" ) == 0 ){
        	Expired_mail = atoi( param );
        	log( "锟斤拷锟铰癸拷锟斤拷锟绞硷拷锟斤拷锟�:%d锟斤拷\n",Expired_mail );
        } else if( strcmp( command , "Del_Family_or_Member" ) == 0 ){
        	Del_Family_or_Member = atoi( param );
        	log( "删锟斤拷锟斤拷锟斤拷锟皆憋拷锟斤拷:%d锟斤拷\n",Del_Family_or_Member );
        } else if( strcmp( command , "Write_Family" ) == 0 ){
        	Write_Family = atoi( param );
        	log( "锟斤拷锟铰硷拷锟斤拷锟斤拷息锟斤拷锟�:%d锟斤拷\n",Write_Family );
				} else if( strcmp( command , "SameIpMun" ) == 0 ){
        	sameipmun = atoi( param );
        	if(sameipmun>0){
        		log( "同IP锟斤拷锟斤拷同时锟斤拷陆:%d锟斤拷\n",sameipmun );
        	}else{
        		log( "同IP锟斤拷锟斤拷同时锟斤拷陆:锟斤拷锟斤拷锟斤拷\n" );
        	}
				}
    }

    fclose(fp);
    return 0;
}


static void parseOpts( int argc, char **argv )
{
    int c , option_index;

		while(1){
        static struct option long_options[] = {
            {"nice" , 1 , 0 , 'n'},
            {"help" , 0 , 0 , 'h' },
            {"userinfo",0 , 0 , 'i'},
            {"lockuser",0 , 0 , 'l'}
        };
        c = getopt_long( argc , argv , "n:hil" , long_options , &option_index );
        if( c == -1 )break;
        switch( c ){
        case 'h':
            fprintf( stderr ,
                     "使锟矫凤拷锟斤拷: saac [-h] [-w port] [-w port] ... \n"
                     "-h : 锟斤拷示saac锟侥帮拷锟斤拷\n"
                     "-w port : 锟斤拷锟揭伙拷锟斤拷锟斤拷锟秸撅拷锟斤拷潭丝锟絓n"
                     "Copyright 2006 锟斤拷zoro锟斤拷锟斤拷锟斤拷 "
                     "( Longzoro System Supply )\n");
            exit(0);
            break;
        case 'i':
#ifdef _SASQL
        		sasql_init();
        		sasql_craete_userinfo();
        		sasql_close();
#endif
						exit(0);
            break;
        case 'l':
#ifdef _SASQL
        		sasql_init();
        		sasql_craete_lock();
        		sasql_close();
#endif
        		exit(0);
            break;
        case 'n':
        		nice(atoi( optarg ));
        		break;
        default:
            log( "锟斤拷锟杰讹拷锟斤拷选锟斤拷 %c\n" , c );
            exit(0);
        }
    }
}
double time_diff(struct timeval subtrahend,  struct timeval subtractor);


int main( int argc , char **argv )
{
		parseOpts( argc, argv );
		
    // Nuke +1 1012: Loop counter
    int counter1 = 0;
    
    int counter2 = 0;
    
    int counter3 = 0;
    
    int counter4 = 0;

    signal(SIGUSR1, sigusr1);

    log_rotate_interval = 3600 * 24 * 7;

    Lock_Init();	// Arminius 7.17 memory lock

		UNlockM_Init();
    
    if(readConfig( "acserv.cf" )<0){
        log( "锟睫凤拷锟节碉拷前目录锟斤拷锟饺� acserv.cf .\n" );
        exit(1);
    }

#ifdef _SASQL
    sasql_init();
#endif	
    log( "锟斤拷取锟斤拷锟斤拷目录\n" );
    dbRead( dbdir );
#ifdef	_FAMILY
    log("锟斤拷取 锟斤拷锟斤拷庄园\n");
    readFMSMemo(fmsmemodir);
    log("锟斤拷取 锟斤拷锟斤拷锟斤拷锟斤拷\n");
    readFMPoint(fmpointdir);
    log("锟斤拷取 锟斤拷锟斤拷目录\n");
    readFamily(familydir);
#endif
    log( "准锟斤拷 锟斤拷锟斤拷目录\n" );
    prepareDirectories( chardir );
    log( "准锟斤拷 锟斤拷志目录\n" );
    prepareDirectories( logdir );
    log( "准锟斤拷 锟绞硷拷目录\n" );
    prepareDirectories( maildir );

#ifdef _SLEEP_CHAR
    prepareDirectories( sleepchardir );
    log( "准锟斤拷 睡锟竭碉拷锟斤拷目录\n" );
#endif

    /* 锟斤拷锟斤拷锟饺伙拷锟铰讹拷锟斤拷锟斤拷毛  锟侥筹拷锟� */
    if( readMail(maildir) < 0 ){
        log( "锟斤拷锟杰筹拷始锟斤拷锟绞硷拷\n" );
        exit(1);
		}

    /* TCPSTRUCT 毛锟斤拷锟斤拷锟� */
    {
        int tcpr;
        if( ( tcpr = tcpstruct_init( NULL , port , 0 ,
						CHARDATASIZE * 16 * MAXCONNECTION , 1 /* DEBUG */ ) ) < 0 ){
            log( "锟斤拷锟杰匡拷锟斤拷TCP: %d\n", tcpr );
            return 1;
        }
    }
    saacproto_InitServer( netWrite , CHARDATASIZE );

    {
        struct sigaction s,os;
        
        bzero( &s, sizeof(s));
        s.sa_handler = sighandle;
        s.sa_flags = SA_NOMASK;
        sigaction( SIGTERM,  &s, &os );

        bzero( &s, sizeof(s));
        s.sa_handler = sighandle;
        s.sa_flags = SA_NOMASK;
        sigaction( SIGINT,  &s, &os );

        bzero( &s, sizeof( s ));
        s.sa_handler = SIG_IGN;
        s.sa_flags = SA_NOMASK;
        sigaction( SIGPIPE, &s, &os );

    }

#ifdef _AC_SEND_FM_PK		 // WON ADD 庄园锟斤拷战锟叫憋拷锟斤拷锟斤拷AC
	load_fm_pk_list();
#endif

#ifdef _ACFMPK_LIST
	FMPK_LoadList();
#endif
#ifdef _ALLDOMAN
	LOAD_herolist();  // Syu ADD 锟斤拷锟叫帮拷NPC
#endif

#ifdef _VIP
		log( "\n锟斤拷锟斤拷税姹�: <%s 锟斤拷员锟斤拷>\n" , SERVER_VERSION );
#else
		log( "\n锟斤拷锟斤拷税姹�: <%s 锟斤拷通锟斤拷>\n" , SERVER_VERSION );
#endif
		log( "\n锟斤拷锟斤拷吮锟斤拷锟斤拷锟斤拷时锟斤拷:%s %s by 锟斤拷zoro锟斤拷锟斤拷锟斤拷\n" , __DATE__ , __TIME__ );
		
    log( "\n锟斤拷始锟斤拷锟斤拷...\n" );

    signal(SIGUSR1,sigusr1);	// Arminius 7.20 memory lock
	int itime=0;
	while(1){
		itime++;
		if(itime>cpuuse){
			itime=0;
			usleep(1);
		}
    int newti,i;
    static time_t main_loop_time;
		
		sys_time = time(NULL);

		if( main_loop_time != sys_time){
            main_loop_time = time(NULL);
            counter1++;
            counter2++;
            counter3++;
            counter4++;
			//andy add 2002/06/20
			UNlockM_UnlockPlayer();

#ifdef _ANGEL_SUMMON
			checkMissionTimelimit();
#endif
            // Nuke *1 1012
            if( counter1 > Total_Charlist ){
            		counter1=0;
                char *c = ctime( &main_loop_time );
                if( c ){
                    struct timeval st,et;
                    log( "\nTIME:%s\n",c );
                    gettimeofday( &st,NULL);
                    dbFlush(dbdir);
                    gettimeofday( &et,NULL);
                    log( "Flushed db(%fsec)\n", time_diff(et,st) );
                    log( "锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷:%d NG:%d\n",
                         total_ok_charlist, total_ng_charlist );
                }
            }
            // Nuke **1 1012
            //if( ( counter % 600 ) == 0 ){
            if( counter2 > Expired_mail ){
            		counter2=0;
                struct timeval st,et;
                gettimeofday( &st,NULL);
                expireMail();
                gettimeofday( &et,NULL);
                log( "锟斤拷锟斤拷锟绞硷拷(%fsec)\n", time_diff(et,st) );
            }
#ifdef	_FAMILY
            //if ((counter % 300) == 0) // 300( -> 60)
      if( counter4 > Write_Family ) // 300( -> 60)
      {
      	  counter4=0;
         	struct timeval st, et;
         	gettimeofday(&st, NULL);
         	writeFamily(familydir);
         	writeFMPoint(fmpointdir);
         	writeFMSMemo(fmsmemodir);
         	gettimeofday(&et, NULL);
         	log("锟斤拷录锟斤拷锟斤拷(%fsec)\n", time_diff(et, st));
      }
#endif
        }
    newti = tcpstruct_accept1();
    if( newti >= 0 ){
			log( "同锟斤拷: %d\n" , newti );
			gs[newti].use = 1;
    }

        for(i=0;i<MAXCONNECTION;i++){
//        	char buf[CHARDATASIZE * 16;
            char buf[CHARDATASIZE];
            int l;
            l = tcpstruct_readline_chop( i , buf , sizeof( buf )- 1);
						{
                if( !gs[i].use )continue;
                if( l > 0 ){
									char debugfun[256];
                    buf[l]=0;
                    if( saacproto_ServerDispatchMessage( i , buf, debugfun)<0){
                        // Nuke start
                        tcpstruct_close(i);// avoid the shutdown the gmsv ttom
                        log( "GMSV(%s) 锟斤拷息:%s\n", gs[i].name, debugfun);
                    }
                } else if( l == TCPSTRUCT_ETOOLONG ){
                    log( "锟杰筹拷:%d 锟斤拷锟斤拷锟斤拷锟斤拷::%s\n", i , gs[i].name );
                    logout_game_server( i );
                } else if( l < 0 ){
                    log( "锟截憋拷:%d 锟斤拷锟斤拷锟斤拷锟斤拷:%s\n", i , gs[i].name );
                    logout_game_server(i);
                } else if( l == 0 ){
                    ;
                }
            }
        }
        {
            static struct timeval tv_loop_store ;
            struct timeval now;
            double dif;
            gettimeofday( &now , NULL );
            dif = time_diff( now, tv_loop_store );
            if( dif > 0.5 ){ // CoolFish 0.1 -> 0.5 2001/5/19
                //log( "Sl:%f" , dif );
            }
            tv_loop_store = now;
        }

        /* 锟斤拷锟斤拷锟叫★拷锟斤拷锟斤拷锟斤拷丞失锟斤拷锟斤拷毛譬锟斤拷锟铰ｏ拷
           锟斤拷锟斤拷丞失锟斤拷锟斤拷锟斤拷锟斤拷锟秸凤拷锟矫�锟斤拷锟狡伙拷锟斤拷锟斤拷锟叫★拷毛诮锟狡伙拷锟斤拷锟斤拷锟斤拷锟斤拷
           匹锟截★拷丞锟接★拷锟斤拷锟斤拷锟�  锟轿讹拷锟藉反骚锟斤拷锟斤拷锟斤拷           */
    }
    return 0;       
}
double
time_diff(struct timeval subtrahend,
          struct timeval subtractor)
{
    return( (subtrahend.tv_sec - subtractor.tv_sec)
            + (subtrahend.tv_usec
            - subtractor.tv_usec  ) / (double)1E6 );
}


/*
  锟斤拷爰帮拷锟斤拷  锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷毛  锟铰ｏ拷
  锟斤拷卅元锟斤拷锟藉及锟街硷拷锟斤拷锟斤拷锟酵伙拷锟斤拷锟斤拷锟斤拷锟斤拷锟教伙互锟斤拷卅元锟斤拷卅锟铰凤拷锟斤拷锟斤拷锟斤拷锟铰ｏ拷

  锟金及匡拷  锟斤拷锟斤拷侬锟斤拷瑕伙拷锟斤拷锟斤拷锟斤拷卅锟铰伙拷锟斤拷    匹锟教硷拷锟斤拷锟斤拷锟斤拷锟矫�  锟斤拷锟狡伙拷锟斤拷
    锟斤拷屺分ｏ拷
  
 */
int get_rotate_count(void )
{
    int a;
    unsigned int t = (unsigned int ) time(NULL);
        
    a = ( t / log_rotate_interval ) * log_rotate_interval;
    
    return a;
}

static int findregBlankMemBuf( void );
static int unregMemBuf( int index );
static int findregBlankCon( void );
static int getFreeMem( void );

static int appendReadBuffer( int index, char *data, int len );
static int appendWriteBuffer( int index , char *data, int len );
static int appendMemBufList( int top, char *data, int len );
static int consumeMemBufList( int top, char *out, int len, int flag, int copyflag );
static int getLineReadBuffer( int index, char *buf, int len );

int tcpstruct_init( char *addr , int p , int timeout_ms , int mem_use , int db )
{
    mbsize = mem_use / sizeof( struct membuf );
    mbuse =0;
    mb_finder = 0;
    mb = ( struct membuf * ) calloc( 1, mbsize * sizeof(struct membuf ));

    if( mb == NULL ) return TCPSTRUCT_ENOMEM;
    bzero( mb , mbsize * sizeof( struct membuf ));

    con = ( struct connection *) calloc( 1, MAXCONNECTION * sizeof( struct connection ));
    if( con == NULL ){
        free( mb );
        return TCPSTRUCT_ENOMEM;
    } else {
        int i;
        for(i=0;i<MAXCONNECTION;i++){
            con[i].use = 0;
            con[i].fd = -1;
        }
    }
    select_timeout.tv_sec = timeout_ms / 1000;
    select_timeout.tv_usec = (timeout_ms - ( timeout_ms/1000)*1000)*1000;

    /* socket */
    mainsockfd = socket( AF_INET , SOCK_STREAM ,  0 );
    if( mainsockfd < 0 ) return TCPSTRUCT_ESOCK;
    
    /* bind */
    bzero( &localaddr ,sizeof( localaddr ));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons( p );
    
    if( addr ){
        localaddr.sin_addr.s_addr = inet_addr( addr );
    } else {
        localaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    }
    if( bind( mainsockfd , (struct sockaddr*) &localaddr ,
              sizeof( localaddr )) < 0 ) return TCPSTRUCT_EBIND;

    /* listen */
    if( listen( mainsockfd , BACKLOGNUM )<0) return TCPSTRUCT_ELISTEN;

    return OK;
}

int tcpstruct_accept1( void )
{
    int tis[BACKLOGNUM];
    int ret;

    ret = tcpstruct_accept( tis , 1 );
    if( ret < 0 ){
        return ret;
    } else if( ret == 1 ){
        return tis[0];
    } else {
        return TCPSTRUCT_EBUG ;
    }
}

int tcpstruct_accept( int *tis , int ticount )
{
    int i;
    int sret;
    int accepted = 0;
    struct timeval t;
    fd_set rfds, wfds , efds;  
    FD_ZERO( & rfds );
    FD_ZERO( & wfds );
    FD_ZERO( & efds );    

    for(i=0;i<MAXCONNECTION;i++){
        if( con[i].use &&
            con[i].fd >= 0 && con[i].closed_by_remote ==0 ){
            FD_SET( con[i].fd , & rfds );
            FD_SET( con[i].fd , & wfds );
            FD_SET( con[i].fd , & efds );
        }
    }
    
    t = select_timeout;
    sret = select( 1024, & rfds , (fd_set*)NULL, & efds , &t);
	if( sret > 0 ) {
		for(i=0;i< MAXCONNECTION;i++){
			if( ( con[i].fd >= 0 ) && FD_ISSET( con[i].fd , &rfds ) ){
				int fr = getFreeMem( );
				int rr , readsize ;
				if( fr <= 0 ) continue;
				if( fr > sizeof(tmpbuf ) ){
					readsize = sizeof( tmpbuf);
				} else {
					readsize = fr;
				}
				rr = read( con[i].fd , tmpbuf , readsize );
				if( rr <= 0 ){
					con[i].closed_by_remote = 1;
				} else {
					appendReadBuffer( i , tmpbuf , rr );
				}
			}
		}
    }    
    /* write */
    t = select_timeout;    
    sret = select( 1024, (fd_set*)NULL, &wfds, & efds , &t);
	if( sret > 0 ) {
		for(i=0;i<MAXCONNECTION;i++){
			if( ( con[i].fd >= 0 ) && FD_ISSET( con[i].fd , &wfds )){
				char send_buf[4096];
				int l , rr;
				memset( send_buf, 0, sizeof( send_buf));
				l = consumeMemBufList( con[i].mbtop_wi ,send_buf, sizeof(send_buf),0 , 1 );
				rr = write( con[i].fd , send_buf , l );
				if( rr < 0 ){
					con[i].closed_by_remote = 1;
				} else {
					consumeMemBufList( con[i].mbtop_wi , send_buf, l, 1 , 0 );
				}
			}
		}
	}

    for( i=0; i<ticount; i++){
        int asret;
        struct timeval t;
        t.tv_sec =0;
        t.tv_usec =0;
        FD_ZERO( & rfds );
        FD_ZERO( & wfds );
        FD_ZERO( & efds );

        FD_SET( mainsockfd , & rfds );
        FD_SET( mainsockfd , & wfds );
        FD_SET( mainsockfd , & efds );
        asret = select( 1024, &rfds , &wfds , &efds, &t );
		// Nuke 20040610: add asret>0 to avoid signal interrupt in select
        if( (asret>0) && FD_ISSET( mainsockfd , & rfds )){
            struct sockaddr_in c;
            int len , newsockfd;
            int newcon;
            bzero( &c , sizeof( c ));
            len = sizeof( c );
            fprintf( stderr, "i can accept " );
            newcon = findregBlankCon( );
            if( newcon < 0 ) continue;
            newsockfd = accept( mainsockfd, (struct sockaddr*)&c , &len );
        log( "同锟斤拷: %d\n" , newsockfd );
            if( newsockfd < 0 ){
                unregMemBuf( newcon );
                continue;
            }
            set_nodelay( newsockfd );
            con[newcon].fd = newsockfd;
            memcpy( &con[newcon].remoteaddr , &c ,sizeof(c));
            tis[accepted] = newcon;
            accepted ++;
        }
    }

    return accepted;
}

int tcpstruct_close( int ti )
{

    if( ti < 0 || ti >= MAXCONNECTION )return TCPSTRUCT_EINVCIND;
    if( con[ti].use == 0 ){
        return TCPSTRUCT_ECLOSEAGAIN;
    }
    close( con[ti].fd );
    con[ti].use = 0;
    con[ti].fd = -1;

    /* 锟斤拷锟斤拷锟斤拷毛锟斤拷锟斤拷锟饺伙拷锟斤拷  毛锟斤拷锟斤拷失锟斤拷锟斤拷 */
    consumeMemBufList( con[ti].mbtop_ri , NULL,
                   mbsize * sizeof( mb[0].buf ), 1, 0 );
    consumeMemBufList( con[ti].mbtop_wi , NULL,
                   mbsize * sizeof( mb[0].buf ), 1, 0 );
    unregMemBuf( con[ti].mbtop_ri );
    unregMemBuf( con[ti].mbtop_wi );
    con[ti].mbtop_ri = -1;
    con[ti].mbtop_wi = -1;    
    return OK;
}

/*
    锟侥筹拷锟斤拷旨锟斤拷锟斤拷值锟截︼拷驶锟斤拷锟斤拷凭锟斤拷锟� remoteclose 锟斤拷锟斤拷锟斤拷锟斤拷 -1 毛锟斤拷锟斤拷锟斤拷
  
 */
int tcpstruct_read( int ti , char *buf , int len )
{
    int l;

    if( ti < 0 || ti >= MAXCONNECTION || con[ti].use == 0 )
        return TCPSTRUCT_EINVCIND;
    l = consumeMemBufList( con[ti].mbtop_ri , buf , len , 1 , 1);
    if( l == 0  && con[ti].closed_by_remote ) return TCPSTRUCT_EREADFIN;

    return l;
}

/*
  1锟斤拷毛锟斤拷锟侥筹拷锟斤拷
  int kend : 1卅锟秸碉拷  锟斤拷 \n 毛诮锟斤拷
  int kend_r : 1卅锟秸碉拷  锟斤拷 \r 锟斤拷诮锟斤拷(丐木锟斤拷)

    锟侥筹拷锟斤拷旨锟斤拷锟斤拷值锟截︼拷驶锟斤拷锟斤拷锟斤拷锟� remote closed 锟斤拷锟斤拷锟斤拷锟斤拷-1毛锟斤拷锟斤拷锟斤拷
  // Nuke
	Read 1 line
	if kend==1 then delete \n at the tail
	if kend_r== 1 then delete \r at the tail (if any)
	if no data read, it means 'remote closed' then return -1
 */
int tcpstruct_readline( int ti , char *buf , int len , int kend , int kend_r )
{
    int l;
    int minus = 0;

    if( ti < 0 || ti >= MAXCONNECTION || con[ti].use == 0 )
        return TCPSTRUCT_EINVCIND;

    l = getLineReadBuffer( ti , buf , len );
    if( l == 0 ){
        if( con[ti].closed_by_remote ){
            return TCPSTRUCT_EREADFIN;
        } else {
            return 0;
        }
    }
    
    if( kend ){
        if( buf[l-1]=='\n' ){
            buf[l-1] = 0; minus =-1;
        }
    }
    if( kend_r ){
        if( buf[l-1]=='\r' ){
            buf[l-1] = 0; minus = -1;
        }
        if( buf[l-2]=='\r' ){
            buf[l-2] = 0; minus = -2;
        }
    }
    return l + minus;
}
int tcpstruct_readline_chop( int ti , char *buf, int len )
{
    return tcpstruct_readline( ti , buf , len , 1,1);
}

int tcpstruct_write( int ti , char *buf , int len )
{
    if( ti < 0 || ti >= MAXCONNECTION || con[ti].use == 0 )
        return TCPSTRUCT_EINVCIND;    
    return appendWriteBuffer( ti , buf , len );
}

int tcpstruct_connect( char *addr , int port )
{
    int newti ;
    int s, r;
    struct sockaddr_in svaddr;
    struct hostent *he;
    
    s = socket( AF_INET, SOCK_STREAM , 0 );
    if(s<0)return -2;

    bzero( &svaddr , sizeof( svaddr ));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons( port );

    if( inet_aton( addr, &svaddr.sin_addr ) == 0 ){
        he = gethostbyname( addr );
        if( he == NULL ){
            return TCPSTRUCT_EHOST;
        }
        memcpy( & svaddr.sin_addr.s_addr , he->h_addr ,
                sizeof( struct in_addr));
    }
    r = connect( s , ( struct sockaddr*)&svaddr,sizeof(svaddr));
    if( r < 0 ){
        return TCPSTRUCT_ECONNECT;
    }
    set_nodelay( s );
    newti = findregBlankCon( );
    if( newti < 0 ){
        fprintf( stderr , "锟斤拷锟斤拷失锟斤拷: newti:%d\n", newti );
        return TCPSTRUCT_ECFULL;
    }
    con[newti].fd = s;
    memcpy( & con[newti].remoteaddr , &svaddr ,
            sizeof( struct sockaddr_in));
    return newti;
}

static int appendReadBuffer(  int index , char *data , int len )
{
    int top;

    top = con[index].mbtop_ri;
    for(;;){
        int nextind = mb[top].next;

        if( nextind == -1 ) break;
        top = nextind;
    }
    return appendMemBufList( top , data , len );
}

static int appendWriteBuffer( int index , char *data , int len )
{
    int top;
    top = con[index].mbtop_wi;
    for(;;){
        int nextind = mb[top].next;
        if( nextind == -1 ) break;
        top = nextind;
    }
    return appendMemBufList( top , data , len );
}
static int appendMemBufList( int top , char *data , int len )
{
    int fr = getFreeMem( );
    int rest = len;
    int data_topaddr = 0;
    
    if( len >= fr ){
		FILE *fp;
		if( (fp=fopen( "badsysinfo.txt", "a+")) != NULL ){
			fprintf( fp, "appendMemBufList() len:%d / fr:%d err !! \n", len, fr);
			fclose( fp);
		}
		//andy_log
		log( "appendMemBufList() len:%d / fr:%d err !! \n", len, fr);
        return -1;
    }
	data[len] = 0;
    for(;;){
        int blanksize = sizeof( mb[0].buf ) - mb[top].len;
        int cpsize = ( rest <= blanksize ) ? rest : blanksize;
        memcpy( mb[top].buf + mb[top].len ,
                data + data_topaddr , cpsize );
        mb[top].len += cpsize;
        if( rest <= blanksize ){
            return len;
        } else {
            int newmb;
            rest -= cpsize;
            data_topaddr += cpsize;
            if( (newmb = findregBlankMemBuf( ) ) == TCPSTRUCT_EMBFULL ){
				FILE *fp;
				if( (fp=fopen( "badsysinfo.txt", "a+")) != NULL ){
					fprintf( fp, "find newmb == TCPSTRUCT_EMBFULL err data:%s !!\n", data);
					fclose( fp);
				}
				log( "find newmb == TCPSTRUCT_EMBFULL err data:%s !!\n", data);
			}
            mb[top].next = newmb;
            top = mb[top].next;
        }
    }
    return TCPSTRUCT_EBUG;
}

static int consumeMemBufList( int top , char *out , int len ,
                          int consumeflag , int copyflag )
{
    int total = 0;
    int top_store = top;
    for(;;){
        int cpsize;
        if( top == -1 ) break;
        cpsize = ( mb[top].len <= ( len - total) ) ?
            mb[top].len : ( len - total );

        if( copyflag ) memcpy( out + total , mb[top].buf , cpsize );
        total += cpsize;

        if( consumeflag ){
            mb[top].len -= cpsize;
            if( mb[top].len > 0 ){
                /* 锟斤拷锟斤拷锟叫凤拷锟斤拷锟狡伙拷卅锟叫硷拷匹memmove */
                memmove( mb[top].buf , mb[top].buf + cpsize ,
                         sizeof( mb[top].buf ) - cpsize );
            }
        }
        top = mb[top].next;
        if( total == len ){
            break;
        }
    }

    if( consumeflag ){
        /* 卅锟斤拷锟斤拷0锟斤拷卅锟饺伙拷锟斤拷卅锟斤拷荸  锟斤拷匹锟斤拷  锟劫硷拷支锟斤拷锟斤拷荸  锟斤拷卅锟斤拷锟斤拷 */
        top = mb[top_store].next;
        for(;;){
            if( top == -1 )break;
            if( mb[top].len == 0 ){
                int prev;
                mb[top_store].next = mb[top].next;
                prev = top;
                top = mb[top].next;
                unregMemBuf( prev );
            } else {
                top = mb[top].next;
            }
        }
    }
    
    return total;
}

static int getLineReadBuffer( int index , char *buf, int len )
{

    int top = con[index].mbtop_ri;
    int ti = 0 , breakflag = 0;

    for(;;){
        int i;        
        int l = mb[top].len;
        if( top == -1 )break;
        for( i=0 ; i < l ; i++){
            if( mb[top].buf[i] == '\n' ){
                breakflag = 1;
                break;
            }
            ti ++;
        }
        if( breakflag )break;
        top = mb[top].next;
    }
    if( ti > len ){
        /* 1锟芥互卅锟斤拷锟绞匡拷锟铰ｏ拷    卅锟睫仿★拷毛锟斤拷锟斤拷锟斤拷 */
        return TCPSTRUCT_ETOOLONG;
    }
    /* 锟芥互锟斤拷锟斤拷锟狡伙拷卅锟斤拷 */
    if( breakflag == 0 ){
        return 0;
    }

    return consumeMemBufList( con[index].mbtop_ri , buf , ti+1 , 1 , 1 );
}



/*
    锟侥筹拷锟斤拷    赢锟斤拷毛忒锟斤拷
  int index : con index

  return:
    锟斤拷锟睫仿★拷
  0锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 read 锟狡伙拷锟街凤拷锟斤拷赢锟斤拷锟�


  mbsize 锟斤拷锟斤拷mbuse 毛娄锟叫伙拷锟斤拷锟斤拷锟斤拷毛锟斤拷锟斤拷锟铰凤拷锟斤拷锟�
  锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷曰锟斤拷锟叫凤拷  锟斤拷锟斤拷锟铰［癸拷锟斤拷卅锟狡撅拷锟叫帮拷锟斤拷锟街达拷锟斤拷卅锟绞伙拷锟街第ｏ拷
  
 */   
static int getFreeMem( void )
{
    return ( mbsize - mbuse ) * sizeof( mb[0].buf );
}

/*
  
  membuf 锟斤拷锟斤拷锟斤拷毛锟斤拷锟狡凤拷锟绞ｏ拷

  return : 锟侥癸拷锟斤拷锟斤拷锟斤拷锟斤拷 >=0 匹 index.
  锟侥癸拷锟斤拷锟斤拷卅锟斤拷锟斤拷锟斤拷锟斤拷  

  锟斤拷锟斤拷锟斤拷锟斤拷宸� mb_finder 毛锟斤拷锟斤拷锟斤拷锟斤拷
  锟斤拷木匹锟斤拷锟斤拷锟狡伙拷锟斤拷    锟斤拷reg锟斤拷锟铰ｏ拷
 */

static int findregBlankMemBuf( void  )
{
    int i;
    for(i=0;i<mbsize;i++){
        mb_finder ++;
        if( mb_finder >= mbsize || mb_finder < 0 ) mb_finder = 0;

        if( mb[mb_finder].use == 0 ){
            mb[mb_finder].use = 1;
            mb[mb_finder].len = 0;
            mb[mb_finder].next = -1;
            mbuse ++;
            return mb_finder;
        }
    }
    return TCPSTRUCT_EMBFULL;
}

/*
  mb 毛荸  锟斤拷锟斤拷
  
 */
static int unregMemBuf(  int index )
{
    mb[index].use=0;
    mb[index].next = -1;
    mb[index].len = 0;
    mbuse --;
    return OK;
}

static int findregBlankCon( void )
{
    int i;
		// Nuke changed 0->1
		//for(i=0;i<MAXCONNECTION;i++){
    for(i=1;i<MAXCONNECTION;i++){
        if( con[i].use == 0 ){
            con[i].use = 1;
            con[i].fd = -1;

            con[i].mbtop_ri = findregBlankMemBuf();
            if( con[i].mbtop_ri < 0 ){
                fprintf( stderr , "EMBFULL\n" );
                return TCPSTRUCT_EMBFULL;
            }
            
            con[i].mbtop_wi = findregBlankMemBuf();
            if( con[i].mbtop_wi < 0 ){
                unregMemBuf( con[i].mbtop_ri );
                fprintf( stderr , "EMBFULL\n" );
                return TCPSTRUCT_EMBFULL;
            }
            bzero( & con[i].remoteaddr , sizeof( struct sockaddr_in));
            con[i].closed_by_remote = 0;
            return i;
        }
    }
    return TCPSTRUCT_ECFULL;
}

int tcpstruct_countmbuse( void )
{
    int i,c=0;
    for(i=0;i<mbsize;i++){
        if( mb[i].use )c++;
    }
    return c;
}

char * getGSName( int i )
{
    return gs[i].name;
}

void checkGSUCheck( char *id )
{
    int i;
    char gname[256];
    if(!id[0])	return;
	memset( gname, 0,  sizeof( gname) );
	if( LockNode_getGname( (getHash(id) & 0xff), id, gname) <= 0 ){
		log("锟睫凤拷锟斤拷锟斤拷戏锟斤拷锟揭碉拷锟剿猴拷:%x/%s !!\n", getHash( id), id);
		return;
	}
	log("\n");
    for(i=0; i < MAXCONNECTION; i++ ){
        if( gs[i].name[0] && strcmp( gs[i].name , gname )==0){
          log("锟斤拷锟酵斤拷锟斤拷锟斤拷锟絒%s] 锟斤拷 %d.%x/%s 锟斤拷锟斤拷锟斤拷:%d !!\n", id, i, getHash( id), gname, gs[i].fd);
					saacproto_ACUCheck_send( gs[i].fd , id );
					return;
        }
    }
//	log("Can't find gname:%s sending err !!\n", gname);

	int ret = -1;
		if( !isLocked( id) ) {
			log( "删锟斤拷锟节达拷锟斤拷息: 锟矫伙拷:%x/%s 没锟斤拷锟斤拷锟斤拷!!\n", getHash(id), id);
		}
		if( DeleteMemLock( getHash(id) & 0xff, id, &ret) ) {

		} else {
			log( "锟斤拷锟杰斤拷锟斤拷 %x:%s !\n", getHash(id), id);
		}
}

void set_nodelay( int sock )
{
    int flag = 1;
    int result = setsockopt( sock, IPPROTO_TCP, TCP_NODELAY,
                             (char*)&flag, sizeof(int));
    if( result < 0 ){
        log( "锟斤拷锟斤拷锟斤拷锟斤拷锟接筹拷.\n" );
    } else {
        log( "锟斤拷锟斤拷锟接筹拷: fd:%d\n", sock );
    }
}


/*
  锟斤拷锟酵伙拷锟斤拷锟截★拷丞锟接★拷锟斤拷锟斤拷锟�  霜锟斤拷

  int flag : 1锟斤拷锟斤拷锟斤拷锟斤拷霜锟斤拷锟斤拷宸达拷锟斤拷锟斤拷锟截︼拷锟�
  
 */
void gmsvBroadcast( int fd, char *p1, char *p2, char *p3 , int flag )
{
    int i,c=0;
    
    for(i=0;i<MAXCONNECTION;i++){
        if( ( flag == 1 ) && ( i == fd ) )continue;
        if( gs[i].use && gs[i].name[0] ){
            saacproto_Broadcast_send( i, p1,p2,p3);
            c++;
        }
    }
/*
#if 1
    {
        char fn[1000];
        FILE *fp;
        snprintf( fn, sizeof(fn), "%s/broadcast.log" , logdir );
        fp = fopen( fn,"a" );
        if(fp){
            fprintf( fp, "From %s(%d) Id:[%s] Char:[%s] Mesg:[%s]\n",
                     gs[fd].name, fd, p1,p2,p3 );
            fclose(fp);
        }
    }
#endif
*/
}

#ifdef _ANGEL_SUMMON

#define MISSIONFILE "db/missiontable.txt"

static int initMissionTable( void )
{
	FILE *fp;
	char onedata[1024];
	char buf[1024];
	int index =0;

	memset( missiontable, 0, sizeof(missiontable));
	fp = fopen( MISSIONFILE, "r");
	if( !fp ) {
		log("\n锟斤拷锟截撅拷锟斤拷锟劫伙拷锟斤拷锟斤拷!!!! \n");
		//return false;
	}
	log("\n锟斤拷锟截撅拷锟斤拷锟劫伙拷...");
	while(1) {
		//
		if( fgets( onedata, sizeof(onedata), fp) == NULL)
			break;
		if( onedata[0] == '\0' || onedata[0] == '#' )
			continue;
		//easyGetTokenFromBuf( onedata, ",", 1, buf, sizeof( buf));
		//index = atoi( buf);
		easyGetTokenFromBuf( onedata, ",", 1, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		strcpy( missiontable[index].angelinfo, buf);
		easyGetTokenFromBuf( onedata, ",", 2, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		strcpy( missiontable[index].heroinfo, buf);
		easyGetTokenFromBuf( onedata, ",", 3, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		missiontable[index].mission = atoi( buf);
		easyGetTokenFromBuf( onedata, ",", 4, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		missiontable[index].flag = atoi( buf);
		easyGetTokenFromBuf( onedata, ",", 5, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		missiontable[index].time = atoi( buf);
		easyGetTokenFromBuf( onedata, ",", 6, buf, sizeof( buf));
		if( buf[0] == '\0' ) continue;
		missiontable[index].limittime = atoi( buf);

		log("%d=%s,%s,%d,%d,%d,%d \n", index,
			missiontable[index].angelinfo,
			missiontable[index].heroinfo,
			missiontable[index].mission,
			missiontable[index].flag,
			missiontable[index].time,
			missiontable[index].limittime );

		index++;
		if( index >= MAXMISSIONTABLE) break;
	}
	fclose( fp);
	log("..锟缴癸拷! \n");
	//return true;
}


int saveMissionTable( void )
{
	FILE *fp;
	char onedata[1024];
	char buf[1024];
	int index =0;

	fp = fopen( MISSIONFILE, "w");
	if( !fp ) {
		log("\n锟津开撅拷锟斤拷锟劫伙拷锟斤拷锟斤拷!!!! \n");
		//return false;
	}
	log("\n锟斤拷锟芥精锟斤拷锟劫伙拷...");
	for( index =0; index < MAXMISSIONTABLE; index++) {

		if( missiontable[index].angelinfo[0] == '\0' )
			continue;
		sprintf( onedata, "%s,%s,%d,%d,%d,%d\n",
			missiontable[index].angelinfo,
			missiontable[index].heroinfo,
			missiontable[index].mission,
			missiontable[index].flag,
			missiontable[index].time,
			missiontable[index].limittime );
		fputs( onedata, fp);
	}
	fclose( fp);
	log("..锟缴癸拷! \n");
	//return true;
}

void delMissionTableOnedata( int index)
{
	int gi;

	log("\n删锟斤拷锟斤拷锟斤拷锟劫伙拷:%d:%s:%s \n", index, missiontable[index].angelinfo, missiontable[index].heroinfo);

	if( index <0 || index >=MAXMISSIONTABLE) return;
	strcpy( missiontable[index].angelinfo, "");
	strcpy( missiontable[index].heroinfo, "");
	missiontable[index].mission = 0;
	missiontable[index].flag = MISSION_NONE;
	missiontable[index].time = 0;
	missiontable[index].limittime = 0;
	
	for( gi=0; gi<MAXCONNECTION; gi++) {
		if (gs[gi].use && gs[gi].name[0]) {
			saacproto_ACMissionTable_send( gi, index, 3, "", "");
		}
	}
}

#define ANSWERTIME 1 // 锟饺达拷锟截达拷时锟斤拷(小时)
//#define DOINGTIME 3*24 // 锟斤拷锟斤拷时锟斤拷(小时)
#define BOUNDSTIME 1*24 // 锟斤拷锟斤拷时锟斤拷(小时)

void checkMissionTimelimit( void)
{
	int index;
	static time_t lastcheck =0;

	if( sys_time < lastcheck + 5*60 )
		return;

	log("\n锟斤拷榫�锟斤拷锟劫伙拷时锟斤拷锟斤拷锟斤拷:%d \n", sys_time);
	for( index =0; index < MAXMISSIONTABLE; index++) {
		if( missiontable[index].flag == MISSION_NONE) {
			continue;
		}
		// 锟饺达拷使锟竭伙拷应1小时
		else if( missiontable[index].flag == MISSION_WAIT_ANSWER
				&& sys_time > missiontable[index].time + ANSWERTIME*60*60 ) {
			
			delMissionTableOnedata( index);// 删
		}
		// 锟饺达拷锟届奖锟斤拷锟� limittime小时
		else if( ( missiontable[index].flag == MISSION_DOING || missiontable[index].flag == MISSION_HERO_COMPLETE )
				&& ( sys_time > (missiontable[index].time + missiontable[index].limittime*60*60))		 ) {

			char buf[1024];
			int gi;
			// 锟斤拷TIMEOVER
			log("锟斤拷锟斤拷锟劫伙拷锟斤拷锟届奖时锟斤拷锟�:%d ", index);
			missiontable[index].flag = MISSION_TIMEOVER;
			missiontable[index].time = time(NULL);
			missiontable[index].limittime = BOUNDSTIME;
			
			sprintf( buf, "%d|%s|%s|%d|%d|%d|%d ", index,
				missiontable[index].angelinfo,
				missiontable[index].heroinfo,
				missiontable[index].mission,
				missiontable[index].flag,
				missiontable[index].time,
				missiontable[index].limittime );
			for( gi=0; gi<MAXCONNECTION; gi++) {
				if (gs[gi].use && gs[gi].name[0]) {
					saacproto_ACMissionTable_send( gi, 1, 1, buf, "");
				}
			}
			
			continue;
		}
		//else if( missiontable[index].flag == MISSION_HERO_COMPLETE
		//		&& sys_time > missiontable[index].time + BOUNDSTIME*60*60 ) {
		//	log(" 锟届奖时锟斤拷锟�:%d ", index);
		//	delMissionTableOnedata( index);// 删
		//}

		// 锟斤拷锟较憋拷锟斤拷时锟斤拷(BOUNDSTIME小时)
		else if( missiontable[index].flag == MISSION_TIMEOVER
				&& sys_time > missiontable[index].time + BOUNDSTIME*60*60 ) {
			log(" 锟斤拷锟斤拷时锟斤拷锟�:%d ", index);
			delMissionTableOnedata( index);// 删
		}

	}
	saveMissionTable();

	lastcheck = sys_time;
}

#endif



