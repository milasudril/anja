#ifdef __WAND__
target[name[mainloop.h] type[include]]
dependency[mainloop.o]
#endif

#ifndef MAINLOOP_H
#define MAINLOOP_H

void init(int& argc,char**& argv);
void run();

#endif
