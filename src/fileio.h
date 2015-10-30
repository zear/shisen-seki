#ifndef _FILEIO_H_
#define _FILEIO_H_

#define SAVE_FORMAT_VERSION	1

void getConfigDir();
void getConfig();
void storeConfig();
int getBoard(int probe);
void storeBoard();

extern char *configDir;

#endif /* _FILEIO_H_ */
