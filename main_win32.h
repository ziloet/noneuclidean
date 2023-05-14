#define Assert(Condition) if(Condition); else __debugbreak()
#define return_if(Condition) if(!(Condition)); else return