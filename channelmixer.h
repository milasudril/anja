#ifdef __WAND__
target[name[channelmixer.h] type[include]]
#endif

#ifndef CHANNELMIXER_H
#define CHANNELMIXER_H

#include "channel.h"
#include "framework/array_fixed.h"

typedef ArrayFixed<Channel,16> ChannelMixer;

#endif
