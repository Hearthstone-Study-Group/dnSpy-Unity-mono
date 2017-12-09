/*
 * status.h:  Status return codes
 *
 * Author:
 *	Dick Porter (dick@ximian.com)
 *
 * (C) 2002 Ximian, Inc.
 */

#ifndef _WAPI_STATUS_H_
#define _WAPI_STATUS_H_

typedef enum {
	STATUS_WAIT_0 			= (int) 0x00000000,
	STATUS_ABANDONED_WAIT_0 	= (int) 0x00000080,
	STATUS_USER_APC			= (int) 0x000000C0,
	STATUS_TIMEOUT			= (int) 0x00000102,
	STATUS_PENDING			= (int) 0x00000103,
	STATUS_SEGMENT_NOTIFICATION	= (int) 0x40000005,
	STATUS_GUARD_PAGE_VIOLATION	= (int) 0x80000001,
	STATUS_DATATYPE_MISALIGNMENT	= (int) 0x80000002,
	STATUS_BREAKPOINT		= (int) 0x80000003,
	STATUS_SINGLE_STEP		= (int) 0x80000004,
	STATUS_ACCESS_VIOLATION		= (int) 0xC0000005,
	STATUS_IN_PAGE_ERROR		= (int) 0xC0000006,
	STATUS_NO_MEMORY		= (int) 0xC0000017,
	STATUS_ILLEGAL_INSTRUCTION	= (int) 0xC000001D,
	STATUS_NONCONTINUABLE_EXCEPTION	= (int) 0xC0000025,
	STATUS_INVALID_DISPOSITION	= (int) 0xC0000026,
	STATUS_ARRAY_BOUNDS_EXCEEDED	= (int) 0xC000008C,
	STATUS_FLOAT_DENORMAL_OPERAND	= (int) 0xC000008D,
	STATUS_FLOAT_DIVIDE_BY_ZERO	= (int) 0xC000008E,
	STATUS_FLOAT_INEXACT_RESULT	= (int) 0xC000008F,
	STATUS_FLOAT_INVALID_OPERATION	= (int) 0xC0000090,
	STATUS_FLOAT_OVERFLOW		= (int) 0xC0000091,
	STATUS_FLOAT_STACK_CHECK	= (int) 0xC0000092,
	STATUS_FLOAT_UNDERFLOW		= (int) 0xC0000093,
	STATUS_INTEGER_DIVIDE_BY_ZERO	= (int) 0xC0000094,
	STATUS_INTEGER_UNDERFLOW	= (int) 0xC0000095,
	STATUS_PRIVILEGED_INSTRUCTION	= (int) 0xC0000096,
	STATUS_STACK_OVERFLOW		= (int) 0xC00000FD,
	STATUS_CONTROL_C_EXIT		= (int) 0xC000013A
} WapiStatus;

#endif /* _WAPI_STATUS_H_ */
