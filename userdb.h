#define BSIZE 100

typedef struct {
    char buffer[BSIZE];
    char *id ;
    char *callsign ;
    char *firstname ;
    char *name ;
    char *place ;
    char *state ;
    char *country ;
	char fUserType;
} user_t ;

int usr_find_by_dmrid( user_t *up, int dmrid, char* data );