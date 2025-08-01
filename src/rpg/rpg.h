
#ifndef rpg_h
#define rpg_h

typedef struct RPGMap RPGMap;

#define RPG_DIM 32

/* you must call this before calling any rpg methods */
void rpg_init();
void rpg_quit();

/* this should never be freed by the user */
RPGMap* rpg_newmap(const char* tmj_path);
void rpg_rendermap(RPGMap* map);

#endif