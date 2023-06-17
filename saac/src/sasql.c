#define _SASQL_C_

#include "version.h"

#ifdef _SASQL//锟斤拷锟斤拷锟�

#include "main.h"
#include "sasql.h"

#include <mysql/mysql.h>
#include <util.h>

MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;

typedef struct tagConfig
{
    char    sql_IP[16]; 

		int     sql_Port; 
		
		char    sql_Port1[16];
		
    char    sql_ID[16];
    
    char    sql_PS[16];

    char    sql_DataBase[16];  

    char    sql_Table[16];    
    
    char    sql_LOCK[16];   
    
    char    sql_NAME[16];  
    
    char    sql_PASS[16];  
}Config;

Config config;

int AutoReg;

static int readConfig( char *path )
{
    char buf[255];
    FILE *fp;

    fp = fopen( path , "r" );
    if( fp == NULL ){ return -2; }

    while( fgets( buf , sizeof( buf ) , fp )){
        char command[255];
        char param[255];
        chop(buf);
        
        easyGetTokenFromString( buf , 1 , command , sizeof( command ));
        easyGetTokenFromString( buf , 2 , param , sizeof( param ));

        if( strcmp( command , "sql_IP" ) == 0 ){
            strcmp( config.sql_IP , param ) ;
            snprintf( config.sql_IP , sizeof( config.sql_IP) , param );
            printf("\n锟斤拷锟捷匡拷锟街凤拷锟  %s",config.sql_IP);
        } else if( strcmp( command , "sql_Port" ) == 0 ){
        		config.sql_Port = atoi( param );
            snprintf( config.sql_Port1 , sizeof( config.sql_Port1) , param );
				  	printf("\n锟斤拷锟捷匡拷丝冢锟  %d",config.sql_Port);
        } else if( strcmp( command , "sql_ID" ) == 0 ){
        		strcmp( config.sql_ID , param ) ;
            snprintf( config.sql_ID , sizeof( config.sql_ID) , param );
						printf("\n锟斤拷锟捷匡拷锟矫伙拷锟斤拷  %s",config.sql_ID);
        } else if( strcmp( command , "sql_PS" ) == 0 ){
        		strcmp( config.sql_PS , param ) ;
            snprintf( config.sql_PS , sizeof( config.sql_PS) , param );
						printf("\n锟斤拷锟捷匡拷锟斤拷锟诫：  %s",config.sql_PS);
        } else if( strcmp( command , "sql_DataBase" ) == 0 ){
        		strcmp( config.sql_DataBase , param ) ;
            snprintf( config.sql_DataBase , sizeof( config.sql_DataBase) , param );
						printf("\n锟斤拷陆锟斤拷锟捷匡拷锟斤拷锟斤拷%s",config.sql_DataBase);
        } else if( strcmp( command , "sql_Table" ) == 0 ){
        		strcmp( config.sql_Table , param ) ;
            snprintf( config.sql_Table , sizeof( config.sql_Table) , param );
				  	printf("\n锟矫伙拷锟斤拷息锟斤拷锟斤拷锟斤拷  %s",config.sql_Table);
				} else if( strcmp( command , "sql_LOCK" ) == 0 ){
        		strcmp( config.sql_LOCK , param ) ;
            snprintf( config.sql_LOCK , sizeof( config.sql_LOCK) , param );
				  	printf("\n锟矫伙拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷  %s",config.sql_LOCK);
				} else if( strcmp( command , "sql_NAME" ) == 0 ){
        		strcmp( config.sql_NAME , param ) ;
            snprintf( config.sql_NAME , sizeof( config.sql_NAME) , param );
				  	printf("\n锟剿猴拷锟街讹拷锟斤拷锟狡ｏ拷  %s",config.sql_NAME);
				} else if( strcmp( command , "sql_PASS" ) == 0 ){
        		strcmp( config.sql_PASS , param ) ;
            snprintf( config.sql_PASS , sizeof( config.sql_PASS) , param );
				  	printf("\n锟斤拷锟斤拷锟街讹拷锟斤拷锟狡ｏ拷  %s",config.sql_PASS);
				} else if( strcmp( command , "AutoReg" ) == 0 ){
        		AutoReg = atoi( param );
        		if(AutoReg){
        			printf("\n锟斤拷锟斤拷锟皆讹拷注锟结：YES");
        		}else{
        			printf("\n锟斤拷锟斤拷锟皆讹拷注锟结：NO");
        		}
    fclose(fp);
    return 0;
    		}
    }
}

BOOL sasql_init( void )
{
		if( mysql_init(&mysql) == NULL & readConfig("acserv.cf"))
		{
			  printf("\n锟斤拷锟捷匡拷锟绞硷拷锟绞э拷埽锟�");
			  exit(1);
		    return FALSE;
		}
	
	  if( !mysql_real_connect( &mysql,
	          config.sql_IP,
	          config.sql_ID,//锟绞猴拷
	          config.sql_PS,//锟斤拷锟斤拷
	          config.sql_DataBase,//选锟斤拷锟斤拷锟斤拷峡锟�
	          config.sql_Port,
	          NULL,
	          0 ) )
		{
			printf("\n锟斤拷锟捷匡拷锟斤拷锟斤拷失锟杰ｏ拷\n");
			return FALSE;
		}

	mysql_query(&mysql, ""
			"create table if not exists USERS ("
			"  USERNAME varchar(16) character set utf8 collate utf8_bin NOT NULL,"
			"  PASSWORD varchar(16) character set utf8 collate utf8_bin NOT NULL,"
			"  REGISTER datetime NOT NULL default '0000-00-00 00:00:00',"
			"  PATH varchar(10) default '',"
			"  PRIMARY KEY (USERNAME)"
			")");
	printf("\n鏁版嵁搴撹繛鎺ユ垚鍔燂紒\n");

	return TRUE;
}

void sasql_close( void )
{
  mysql_close( &mysql );
}


int sasql_query(char *nm, char *pas){
	char sqlstr[256];
	
	sprintf(sqlstr, "select * from USERS where USERNAME='%s'", nm);
	printf("sqlstr=%s\n",sqlstr);
	if(!mysql_query(&mysql,sqlstr)){
		printf("여기까지옴1111\n");
	    int num_row=0;
	    mysql_result=mysql_store_result(&mysql);
	   num_row=mysql_num_rows(mysql_result);
	    printf("여기까지옴222\n");
	  if(num_row>0){
	  	mysql_row=mysql_fetch_row(mysql_result);
		printf("여기까지옴333,%s\n",mysql_row[1]);
	  	if(strcmp(pas,mysql_row[1]) == 0){
	  		return 1;
		  }else{
		  	printf("锟矫伙拷%s锟斤拷锟斤拷锟斤拷锟絓n",nm);
		  	return 2;
		  }
		
	    
	  }else{
	  	printf("锟矫伙拷%s未注锟结！\n",nm);
	  	return 3;
	  }
	  mysql_free_result(mysql_result);
	}else{
		printf("\n锟斤拷锟捷匡拷锟斤拷锟绞э拷埽锟絓n");
		printf("锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷匡拷...");
		sasql_close();
		sasql_init();
		printf("锟斤拷锟絓n");
		return 0;
	}
}

#ifdef _SQL_REGISTER
BOOL sasql_register(char *id, char *ps)
{
	char sqlstr[256];
	printf("계정생성!!\n");
//	if(AutoReg!=1)return FALSE;
	sprintf(sqlstr, "insert into USERS (USERNAME,PASSWORD,REGISTER,PATH) VALUES (BINARY'%s','%s',NOW(),'char/0x%x')", id, ps, getHash(id) & 0xff);
	if(!mysql_query(&mysql,sqlstr)){
		printf("\n锟斤拷锟矫伙拷注锟斤拷晒锟斤拷锟絓n");
		return TRUE;
	}
	printf("\n锟斤拷锟矫伙拷注锟斤拷失锟杰ｏ拷\n");
	return FALSE;
}
#endif


BOOL sasql_chehk_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr, "select * from %s where USERNAME=BINARY'%s'", config.sql_LOCK, idip);
	printf("sqlstr=%s\n",sqlstr);

	if(!mysql_query(&mysql,sqlstr)){
		return TRUE;
	}
	return FALSE;
}

BOOL sasql_add_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr, "INSERT INTO %s (USERNAME) VALUES (BINARY'%s')", config.sql_LOCK, idip);
	if(!mysql_query(&mysql,sqlstr)){
			printf("\n锟斤拷锟斤拷锟斤拷锟�%s锟缴癸拷锟斤拷\n",idip);
			return TRUE;
	}
	return FALSE;
}

BOOL sasql_del_lock( char *idip )
{
	char sqlstr[256];
	sprintf(sqlstr, "delete from %s where USERNAME=BINARY'%s'", config.sql_LOCK, idip);
	if(!mysql_query(&mysql,sqlstr)){
		printf("\n锟斤拷锟斤拷锟斤拷锟�%s锟缴癸拷锟斤拷\n",idip);
		return TRUE;
	}
	return FALSE;
}

BOOL sasql_craete_lock( void )
{

}

BOOL sasql_craete_userinfo( void )
{

}



#endif
