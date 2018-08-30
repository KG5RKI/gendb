#include "common.h"
#include "userdb.h"


#include <fstream>

void printUser(user_t* user){
    printf("\r\nCallsign: %s", user->callsign);
    printf("\r\nName: %s", user->firstname);
    printf("\r\nCountry: %s", user->country);
}

int main(int argc, char** argv){
    
    FILE* fuserdb = fopen("user.bin", "rb");
    long flen = 0;

    if(!fuserdb){
        printf("Error opening user.bin\r\n");
        fgetc(stdin);
        exit(1);
    }


    fseek (fuserdb, 0, SEEK_END);
    flen=ftell (fuserdb);
    fseek (fuserdb, 0, SEEK_SET);

    char* filebuffer = (char*)malloc(flen);
    fread(filebuffer, flen, 1, fuserdb);

    fclose (fuserdb);

    int id = 1148110;

    user_t user;
    if(usr_find_by_dmrid(&user, id, filebuffer) != 1){
        printf("Error finding dmr id %d\r\n", id);
        fgetc(stdin);
        exit(1);
    }
    printUser(&user);

    printf("\r\nDone");
    fgetc(stdin);
    return 0;
}