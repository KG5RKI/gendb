#include "common.h"
#include "userdb.h"


#include <fstream>
#include <string.h>
#include <string>

#include <vector>

using namespace std;

void printUser(user_t* user){
    printf("\r\nDMRID: %s", user->id);
    printf("\r\nCallsign: %s", user->callsign);
    printf("\r\nFirstName: %s", user->firstname);
    printf("\r\nName: %s", user->name);
    printf("\r\nCountry: %s", user->country);
    printf("\r\nPlace: %s", user->place);
    printf("\r\nState: %s", user->state);
    printf("\r\n----------------");
}



typedef struct _dictentry {
    string entry;
    int count;
    _dictentry(){
        entry="";
        count=1;
    }
    bool operator== (const _dictentry & rhs) const {
        return (entry == rhs.entry);
    }
    
} dictentry;

int find (vector<_dictentry> *ent, _dictentry* nent)
{
    for(int i=0; i<ent->size(); i++)
    {
        if (ent->at(i).entry == nent->entry) 
            return i;
    }
    return -1;
}

int processDict (vector<_dictentry> *main, vector<_dictentry> *toadd)
{
    for(int i=0; i<toadd->size(); i++)
    {
        if (toadd->at(i).count >= 5){
            main->push_back(toadd->at(i));
        }
    }
    return -1;
}

int getDictIndex (vector<_dictentry> *main, string srch)
{
    for(int i=0; i<main->size(); i++)
    {
        if (main->at(i).entry == srch){
            return i;
        }
    }
    return -1;
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

    char* iter = filebuffer;
    char* end = (char*)(filebuffer + flen);

    vector<dictentry> countrydict;
    vector<dictentry> callsigndict;
    vector<dictentry> placedict;
    vector<dictentry> statedict;
    long spacesaved=0;

    printf("\r\nReading in strings, creating dictionary\r\n");

    for(int i=0; iter < end && (end-iter)>3; i++){

        if(i%5==0){
            float progress = (((float)(iter-filebuffer) / (float)(end-filebuffer))*100.0f);
            printf("\r%02.f", progress);
        }
        /*if(!find_dmr(user.buffer, 0, iter, end, 200)){
            printf("Error finding next user in db\r\n");
            fgetc(stdin);
            exit(1);
        }*/
        memset(&user, 0, sizeof(user));
        iter = (char*)next_line_ptr(iter);
        readline(user.buffer, iter, 200);
        usr_splitbuffer(&user);
        //printUser(&user);
        if(strlen(user.country)>0){
            dictentry* newent = new dictentry();
            newent->entry = (string)user.country;

            int it = find (&countrydict, newent);
            if(it>=0){
                countrydict.at(it).count++;
                //printf("\r\nFound: %s - %d", dict.at(it).entry.c_str(), dict.at(it).count);
                spacesaved+=strlen(countrydict.at(it).entry.c_str());
                delete newent;
            }else{
                countrydict.push_back(*newent);
                //printf("\r\nCountry Add: %s", newent->entry.c_str());

            }
        }
        if(strlen(user.callsign)>0){
            dictentry* newent = new dictentry();
            newent->entry = (string)user.callsign;

            int it = find (&callsigndict, newent);
            if(it>=0){
                callsigndict.at(it).count++;
                spacesaved+=strlen(callsigndict.at(it).entry.c_str());
                //printf("\r\nFound: %s - %d", dict.at(it).entry.c_str(), dict.at(it).count);
                delete newent;
            }else{
                callsigndict.push_back(*newent);
                //printf("\r\nCallsign Add: %s", newent->entry.c_str());

            }
        }         
        if(strlen(user.place)>0){
            dictentry* newent = new dictentry();
            newent->entry = (string)user.place;

            int it = find (&placedict, newent);
            if(it>=0){
                placedict.at(it).count++;
                spacesaved+=strlen(placedict.at(it).entry.c_str());
                //printf("\r\nFound: %s - %d", dict.at(it).entry.c_str(), dict.at(it).count);
                delete newent;
            }else{
                placedict.push_back(*newent);
                //printf("\r\nPlace Add: %s", newent->entry.c_str());

            }
        }       
        if(strlen(user.state)>0){
            dictentry* newent = new dictentry();
            newent->entry = (string)user.state;

            int it = find (&statedict, newent);
            if(it>=0){
                statedict.at(it).count++;
                spacesaved+=strlen(statedict.at(it).entry.c_str());
                //printf("\r\nFound: %s - %d", dict.at(it).entry.c_str(), dict.at(it).count);
                delete newent;
            }else{
                statedict.push_back(*newent);
                //printf("\r\nState Add: %s", newent->entry.c_str());

            }
        }                
    }
    printf("\r\n-------------------\r\nCountries: %d", countrydict.size());
    printf("\r\nCallsigns: %d", callsigndict.size());
    printf("\r\nPlaces: %d", placedict.size());
    printf("\r\nStates: %d", statedict.size());
    printf("\r\n-- Space Saved: %08X", spacesaved);
    //for(int i=0; i<dict.size(); i++){
        //printf("\r\n%s - %d", dict.at(i).entry.c_str(), dict.at(i).count);
    //}
    vector<dictentry> finaldict;
    printf("\r\nProcessing dicts to main dict");

    printf("\r\nCountries..");
    processDict(&finaldict, &countrydict);
    countrydict.empty();
    printf("\r\nStates..");
    processDict(&finaldict, &statedict);
    statedict.empty();
    printf("\r\nPlaces..");
    processDict(&finaldict, &placedict);
    placedict.empty();

    printf("\r\n---------");
    printf("\r\n Dict Size: %d", finaldict.size());


    printf("\r\nReprocessing and replacing strings in records..");
    iter = filebuffer;
    char buffer[200] = {0};
    
    FILE* fOut = fopen("out.bin", "wb");

    printf("\r\nCompiling lookup tables..");
    
    unsigned long tablesize = (unsigned long)finaldict.size();
    fwrite(&tablesize, 4, 1, fOut);

    unsigned long tsize = 0;
    for(int i=0; i<finaldict.size(); i++){
        tsize += strlen(finaldict.at(i).entry.c_str());
    }

    printf("\r\nString table size: %d", tsize);

    char* strtable = new char[tsize];
    char* tableit = strtable;
    for(int i=0; i<finaldict.size(); i++){
        strcpy(tableit, finaldict.at(i).entry.c_str());
        unsigned long pt = (unsigned long)(tableit-strtable);
        fwrite(&pt, 4, 1, fOut);
        tableit+=strlen(finaldict.at(i).entry.c_str());
    }
    printf("Writing first table..");
    
    fflush(fOut);
    printf("Writing second table..");
    for(int i=0; i<tsize; i+=16){
        fwrite(strtable+i, ((tsize-i)>=16?16:((tsize)-i)), 1, fOut);
    }
    fflush(fOut);

    printf("\r\nSorting and writing records..");

    for(int i=0; iter < end && (end-iter)>3; i++){

        
        /*if(!find_dmr(user.buffer, 0, iter, end, 200)){
            printf("Error finding next user in db\r\n");
            fgetc(stdin);
            exit(1);
        }*/
        memset(buffer, 0, 200);
        memset(&user, 0, sizeof(user));
        iter = (char*)next_line_ptr(iter);
        readline(user.buffer, iter, 200);
        usr_splitbuffer(&user);
        //printUser(&user);
        if(strlen(user.country)>0){
            int ret = getDictIndex(&finaldict, (string)user.country);
            if(ret>=0){
                strcpy(user.country, "\x01\x00\x00\x00");
                unsigned short ind = (unsigned short)ret;
                memcpy(&user.country[1], (void*)&ind, sizeof(short));
            }
        }
        if(strlen(user.place)>0){
            int ret = getDictIndex(&finaldict, (string)user.place);
            if(ret>=0){
                strcpy(user.place, "\x01\x00\x00\x00");
                unsigned short ind = (unsigned short)ret;
                memcpy(&user.place[1], (void*)&ind, sizeof(short));
            }
        }
        if(strlen(user.state)>0){
            int ret = getDictIndex(&finaldict, (string)user.state);
            if(ret>=0){
                strcpy(user.state, "\x01\x00\x00\x00");
                unsigned short ind = (unsigned short)ret;
                memcpy(&user.state[1], (void*)&ind, sizeof(short));
            }
        }
        usr_tobuffer(&user, buffer);
        printf("\r\n%s", buffer);
        fwrite(buffer, (int)(next_line_ptr(buffer)-buffer), 1, fOut);
        fflush(fOut);
    }
    fclose(fOut);

    
    free(filebuffer);
    printf("\r\nDone");
    fgetc(stdin);
    return 0;
}