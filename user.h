#ifndef USER_H
#define USER_H

#include "types.h"

typedef struct
{
	i32 id;
	char name[256];
	char email[256];
} User;

#endif
