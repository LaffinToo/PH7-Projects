#include <sqlite3.h>

#include "ph7.h"

typedef struct _ph7_sqlite3_db_object  {
	int initialised;
	sqlite3 *db;
} ph7_sqlite3_db_object;


int ph7_sqlite3_libversion(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	ph7_value *pArray;    /* Our Array */
	ph7_value *pValue;    /* Array entries value */
	char *pString;
	int pLen;
	
	
	/* Create a new array */
	pArray = ph7_context_new_array(pCtx);
	/* Create a worker scalar value */
	pValue = ph7_context_new_scalar(pCtx);
	if( pArray == 0 || pValue == 0 ){
		/*
		 * If the supplied memory subsystem is so sick that we are unable
		 * to allocate a tiny chunk of memory, there is no much we can do here.
		 * Abort immediately.
		 */
		ph7_context_throw_error(pCtx, PH7_CTX_ERR, "Fatal, PH7 is running out of memory");
		/* emulate the die() construct */
		return PH7_ABORT; /* die('Fatal, PH7 is running out of memory'); */
	}

	/* Prepare the character for insertion */
	ph7_value_string(pValue,sqlite3_libversion(),-1 /* Compute length automatically */);
	/* Insert the string */
	ph7_array_add_strkey_elem(pArray, "versionString", pValue /* Will make it's own copy*/);
	/* Overwrite the previous value */
	ph7_value_int(pValue, sqlite3_libversion_number()); /* Version numbver */
	/* Insert iny */
	ph7_array_add_strkey_elem(pArray, "versionNumber", pValue /* Will make it's own copy */);
	
	/* Return the array as the function return value */
	ph7_result_value(pCtx, pArray);
	
	return PH7_OK;
}

int ph7_sqlite3_open(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	char *fn,*key;
	int rc=0,kLen;
	ph7_int64 flags;
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if( argc < 1 || !ph7_value_is_string(argv[0]) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Missing sqlite3 filename");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	if(argc > 1 && !ph7_value_is_numeric(argv[1])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Flags is not numeric");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	if(argc==3 && !ph7_value_is_string([argv[2]])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Encryption key must be a string");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	fn=ph7_value_to_string(argv[0]);
	flags=ph7_value_to_int64(argv[1]);
	
	rc=sqlite3_open_v2(fn,&ph7_sqlite3->db,flags,NULL);
	
	if(rc!=SQLITE_OK) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Unable to open database");
		ph7_result_bool(pCtx, 0);
	}
	if(argc==3)
		key=ph7_value_to_string(argv[2],&kLen);
		
#if SQLITE_HAS_CODEC
	
	if (kLen > 0) {
		if (sqlite3_key(ph7_sqlite3->db, key, kLen) != SQLITE_OK) {
			sqlite3_close(ph7_sqlite3->db);
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Unable to open database");
			ph7_result_bool(pCtx, 0);
		}
	}
#endif
	ph7_sqlite3->initialized=1;

	ph7_result_resource(*pCtx,(void *)ph7_sqlite3);
	return PH7_OK;
}

 
int ph7_sqlite3_close(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	int rc;
	sqlite3 *db;
	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	ph7_sqlite3=(ph7_sqlite3_db_object *)ph7_value_to_resource(argv[0]);
	rc=SQLITE_ERROR;
	if(ph7_sqlite3->initialized) {
		if(ph7_sqlite3->db) {
			rc=sqlite3_close(ph7_sqlite3->db);
		}
	}
	ph7_sqlite3->initialized=0;
	ph7_result_int(*pCtx,rc);
	return PH7_OK;
}

int ph7_sqlite3_exec(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	const char *errs[]={"resource handle","sql statement","callback function","callback args","error message"};
	char pString[160],*pSql,*pErr;
	int rc=0;
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if(argc<1 || !ph7_value_is_resource(argv[0]))
		rc=1;
	else if(argc<2 || !ph7_value_is_string(argv[1]))
		rc|=2;
	if(rc) {
		while(rc) {
			if(rc&1) {
				sprintf(pString,"Missing sqlite3 %s",errs[i]);
				ph7_context_throw_error(pCtx, PH7_CTX_WARNING, pString);
			}
			i++;
			rc=(rc>>1)&31;
		}
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(!ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	pSql=value_to_string(argv[1]);
	rc=sqlite3_exec(ph7_sqlite3->db,pSql,NULL,NULL,&pErr);
	if(rc!=SQLITE_OK) {
		if(!pErr) {
			sprintf(pStr,"SQLite err: %s",pErr);
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, pStr);
			sqlit3_free(pErr);
		}
	}
			
	ph7_result_int(*pCtx,rc);
	return PH7_OK;
}

int ph7_sqlite3_lastInsertRowID(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	int rc;
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	rc=sqlite3_last_insert_rowid(ph7_sqlite3->db)
	ph7_result_int(*pCtx,rc);
	
	return PH7_OK;
}

int ph7_sqlite3_sqlite3_errcode(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	int rc;
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	rc=sqlite3_sqlite3_errcode(ph7_sqlite3->db)
	ph7_result_int(*pCtx,rc);
	
	return PH7_OK;
}

int ph7_sqlite3_lastErrorMsg(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	int rc;
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_result_string(*pCtx,sqlite3_errmsg(ph7_sqlite3->db));
	
	return PH7_OK;
}

int ph7_sqlite3_changes(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	ph7_sqlite3_db_object *ph7_sqlite3;
	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	ph7_result_int(*pCtx,sqlite3_changes(ph7_sqlite3->db));
	
	return PH7_OK;
}

int ph7_sqlite3_escapeString(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	char *pStr=NULL,*pSql;
	
	if( argc < 1 || !ph7_value_is_string(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid string");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	pSql=value_to_string(argv[0]);
	if(strlen(pSql)) {
		pStr = sqlite3_mprintf("%q", pSql);
		if (pStr) {
			ph7_result_string(*pCtx,pStr);
			sqlite3_free(pStr);
		}
	} 
	if(!pStr) {
		ph7_result_string(*pCtx,"");
	}
	
	return PH7_OK;
}

int ph7_sqlite3_querySingle(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	ph7_sqlite3_db_object *ph7_sqlite3;
	char *pSql,*pResult[], *pString;;
	char *pErr = NULL;
	int i,rc,pRows,pCols,pLen,pEntireRow=FALSE;
	ph7_value *pArray;    /* Our Array */
	ph7_value *pValue;    /* Array entries value */

	
	if( argc < 1 || !ph7_value_is_resource(argv[0])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 resouce handle");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	if( argc < 2 || !ph7_value_is_string(argv[1])) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 query");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	if( argc==3 ) {
		if(!(ph7_value_is_bool(argv[2]) || ph7_value_is_int(argv[2]))
		{
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Invalid sqlite3 entire row");
			ph7_result_bool(pCtx, 0);
			return PH7_OK;
		}
		pEntireRow=ph7_value_to_bool(argv[2]);
	}
	
	ph7_sqlite3->db=(ph7_sqlite3_db_object *)value_to_resource(argv[0]);
	if(!ph7_sqlite3->initialized) {
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "db already closed");
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	
	pSql=value_to_string(argv[1]);
	if(strlen(pSql)) {
		if ((rc=sqlite3_get_table(ph7_sqlite3->db, pSql,&pResult,&pRows,&pCols,&pErr) != SQLITE_OK) {
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, pErr);
			sqlite3_free(pErr);
			return PH7_OK;
		}
	} else rc=SQLITE_OK;
	if(pEntireRow) {
		/* Create a new array */
		pArray = ph7_context_new_array(pCtx);
		/* Create a worker scalar value */
		pValue = ph7_context_new_scalar(pCtx);
		if( pArray == 0 || pValue == 0 ){
			ph7_context_throw_error(pCtx, PH7_CTX_ERR, "Fatal, PH7 is running out of memory");
			/* emulate the die() construct */
			return PH7_ABORT; /* die('Fatal, PH7 is running out of memory'); */
		}
	}
	
	if(pRows) {
		if(pEntireRow) {
			for(i=0;i<pCols,i++) {
				/* Prepare the character for insertion */
				ph7_value_string(pValue,pResult[pCols+i],-1 /* Compute length automatically */);
				/* Insert the string */
				ph7_array_add_elem(pArray, i, pValue /* Will make it's own copy*/);
			}
			ph7_result_value(pCtx, pArray);
		} else {
			ph7_value_string(pValue,pResult[pCols],-1 /* Compute length automatically */);
			ph7_result_value(pCtx, pValue);
		}
		sqlite3_free(pResult);
	} else {
		ph7_result_value(pCtx, 0);
	}
	
	return PH7_OK;
}


