/* $Id: ihex.h,v 1.3 2016/05/22 04:04:55 clare Exp $ */
void ihex_set_fname(char const *);
int ihex_open();
int ihex_close();
int ihex_parse();
int ihex_dump();
