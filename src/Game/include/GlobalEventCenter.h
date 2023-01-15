#pragma once
#include "eventpp/eventdispatcher.h"

static eventpp::EventDispatcher<std::size_t, void()> GlobalDispatcher;