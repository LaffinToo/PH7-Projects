#include <stdlib.h>

#include "ph7.h"

#define BITMAN_AND 1
#define BITMAN_OR 2
#define BITMAN_XOR 3
#define BITMAN_CLEAR 4
#define BITMAN_GET 5
#define BITMAN_SET 6
#define BITMAN_FLIP  7
#define BITMAN_SHL 8
#define BITMAN_SHR 9
#define BITMAN_ROL 10
#define BITMAN_ROR 11
#define BITMAN_STRING 12

#define FALSE (0)
#define TRUE (1)

#define SX_ARRAYSIZE(X) (sizeof(X)/sizeof(X[0]))
#define SXUNUSED(P)	(P = 0)

int bitman_func(
	ph7_context *pCtx, /* Call Context */
	int argc,          /* Total number of arguments passed to the function */
	ph7_value **argv   /* Array of function arguments */
	)
{
	int func,val,bit,max;
	char *pBuf,*p;
	unsigned int mask;
	/* Make sure there is at least one argument and is of the
	 * expected type [i.e: numeric].
	 */
	if( argc < 3 || !ph7_value_is_numeric(argv[0]) ||
			!ph7_value_is_numeric(argv[1]) ||
			!ph7_value_is_numeric(argv[2])){
		/*
		 * Missing/Invalid argument, throw a warning and return FALSE.
		 * Note that you do not need to log the function name, PH7 will
		 * automatically append the function name for you.
		 */
		ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Missing numeric argument");
		/* Return false */
		ph7_result_bool(pCtx, 0);
		return PH7_OK;
	}
	/* Extract the number */
	func = ph7_value_to_int(argv[0]);
	val = ph7_value_to_int(argv[1]);
	bit = ph7_value_to_int(argv[2]);
	max=0;
	if(func==BITMAN_STRING) max=bit;
	else
	if(func==BITMAN_ROL || func==BITMAN_ROR)
	{
		if(argc<4 || !ph7_value_is_numeric(argv[3])) {
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Missing numeric argument (max)");
			/* Return false */
			ph7_result_bool(pCtx, 0);
			return PH7_OK;
		}
		max = ph7_value_to_int(argv[3]);
	}
	if(max) {
		if(max<((func==BITMAN_STRING)?1:0) &&
				max>(int)(sizeof(unsigned int)*8)
				-((func==BITMAN_STRING)?0:1) ) {
			ph7_context_throw_error(pCtx, PH7_CTX_WARNING, "Max out of range");
			/* Return false */
			ph7_result_bool(pCtx, 0);
			return PH7_OK;
		}
		mask=(max==sizeof(unsigned int)*8)?
				-1 : ((1<<max)-1);
	}
				
	switch (func) {
		case BITMAN_AND:
			val &= bit;
			break;
		case BITMAN_OR:
			val |= bit;
			break;
		case BITMAN_XOR:
			val ^= bit;
			break;
		case BITMAN_CLEAR:
			val&=(~(1<<bit));
			break;
		case BITMAN_GET:
			val=(val&(1<<bit))?1:0;
			break;
		case BITMAN_SET:
			val|=(1<<bit);
			break;
		case BITMAN_FLIP:
			val^=(1<<bit);
			break;
		case BITMAN_SHL:
			val<<=bit;
			break;
		case BITMAN_SHR:
			val>>=bit;
			break;
		case BITMAN_ROL:
			val=((val>>(max-bit))|(val<<bit))&mask;
			break;
		case BITMAN_ROR:
			val=((val>>bit)|(val<<(max-bit)))&mask;
			break;
		case BITMAN_STRING:
			max+=(max%8);
			pBuf = ph7_context_alloc_chunk(pCtx,(unsigned int)max,FALSE,FALSE);
			
			p=pBuf;
			*(p+max)=0;
			for(bit=0;bit<max;bit++)
			{
				*p++=(val&1?'1':'0');
				val=(val>>1)&mask;
				if(bit%8==7)  *p++=' ';
			}
			ph7_result_string(pCtx,(const char *)pBuf,(int)max);
			ph7_context_free_chunk(pCtx,pBuf);
	}
	/* Return the new value */
	if(func!=BITMAN_STRING)
		ph7_result_int(pCtx, val);
	/* All done */
	return PH7_OK;
}

static void PH7_BIT_AND_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_AND);
}

static void PH7_BIT_OR_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_OR);
}

static void PH7_BIT_XOR_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_XOR);
}

static void PH7_BIT_CLEAR_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_CLEAR);
}

static void PH7_BIT_GET_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_GET);
}

static void PH7_BIT_SET_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_SET);
}

static void PH7_BIT_FLIP_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_FLIP);
}

static void PH7_BIT_SHL_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_SHL);
}

static void PH7_BIT_SHR_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_SHR);
}

static void PH7_BIT_ROL_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_ROL);
}

static void PH7_BIT_ROR_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_ROR);
}


static void PH7_BIT_STRING_Const(ph7_value *pVal,void *pUserData)
{
	SXUNUSED(pUserData); /* cc warning */
	ph7_value_int(pVal,BITMAN_STRING);
}

typedef void (*ProcConstant)(ph7_value *,void *);

struct ph7_bitman_constant
{
	const char *zName;     /* Constant name */
	ProcConstant xExpand;  /* C routine responsible of expanding constant value*/
} BitmanConst[] = {
	{"BIT_AND",          PH7_BIT_AND_Const     },
	{"BIT_OR", 			PH7_BIT_OR_Const		},
	{"BIT_XOR",          PH7_BIT_XOR_Const     },
	{"BIT_CLR", 			PH7_BIT_CLEAR_Const		},
	{"BIT_GET",          PH7_BIT_GET_Const     },
	{"BIT_SET",          PH7_BIT_SET_Const     },
	{"BIT_FLP", 			PH7_BIT_FLIP_Const		},
	{"BIT_SHL",          PH7_BIT_SHL_Const     },
	{"BIT_SHR", 			PH7_BIT_SHR_Const		},
	{"BIT_ROL",          PH7_BIT_ROL_Const     },
	{"BIT_ROR", 			PH7_BIT_ROR_Const		},
	{"BIT_STRING",          PH7_BIT_STRING_Const     }
}; 


void PH7_Register_Bitman(ph7_vm *pVm)
{
	int rc,n;
	/* 
	 * Note that all built-in constants have access to the ph7 virtual machine
	 * that trigger the constant invocation as their private data.
	 */
	for( n = 0 ; n < (int)SX_ARRAYSIZE(BitmanConst) ; ++n ){
		ph7_create_constant(&(*pVm),BitmanConst[n].zName,BitmanConst[n].xExpand,&(*pVm));
	}
	rc = ph7_create_function(pVm, "bitman", bitman_func, 0 /* NULL: No private data */);
	if( rc != PH7_OK ){
		puts("Error while registering bitman");
		/* Shutdown the library */
		ph7_lib_shutdown();
		/* Exit immediately */
		exit(0);
	}
}
