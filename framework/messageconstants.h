#ifdef __WAND__
target[name[messageconstants.h] type[include]]
#endif

#ifndef MESSAGECONSTANTS_H
#define MESSAGECONSTANTS_H

#include <cstdint>

static constexpr uint32_t MESSAGE_TYPE_NORMAL=0x40;
static constexpr uint32_t MESSAGE_TYPE_WARNING=0x30;
static constexpr uint32_t MESSAGE_TYPE_ERROR=0x10;

static constexpr uint32_t MESSAGE_ANSWERS_OK=0x0;
static constexpr uint32_t MESSAGE_ANSWERS_RETRYCANCEL=0x5;
static constexpr uint32_t MESSAGE_ANSWERS_YESNO=0x4;
static constexpr uint32_t MESSAGE_ANSWERS_YESNOCANCEL=0x3;

static constexpr uint32_t MESSAGE_ANSWER_CANCEL=2;
static constexpr uint32_t MESSAGE_ANSWER_OK=1;
static constexpr uint32_t MESSAGE_ANSWER_NO=7;
static constexpr uint32_t MESSAGE_ANSWER_RETRY=4;
static constexpr uint32_t MESSAGE_ANSWER_YES=6;

#endif
