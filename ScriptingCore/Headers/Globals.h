#pragma once

#define _FILE_ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if defined (DEBUG) | defined(_DEBUG)
#define NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define NEW new
#endif