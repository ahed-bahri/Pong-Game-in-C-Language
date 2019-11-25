
//Defines and Utilities

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32     b32;

typedef float   f32;
typedef double  f64;

#define internal static
#define local_persist static
#define global_variable static

#define array_count(array) (sizeof(array)/sizeof((array)[0]))

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#define min(a, b) ((a < b) ? (a) : (b))
#define max(a, b) ((a > b) ? (a) : (b))

#if ASSERTS
#define assert(expression) if (!(expression)) {*(int*)0 = 0;}
#else
#define assert(expression)
#endif

#define invalid_default_case default: assert(!"Invalid defauld case");

#define zero_struct(instance) zero_size(sizeof(instance), &(instance))
#define zero_array(pointer) zero_size(array_count(pointer)*sizeof((pointer)[0]), (pointer))

inline void
zero_size(size_t size, void *ptr) {
    
    u8 *byte = (u8*)ptr;
    while(size--) {
        
        *byte++ = 0;
    }
}


//Input

struct {
    
    int half_transition_count;
    b32 ended_down;
} typedef Game_Button_State;

inline b32
pressed(Game_Button_State state) {
    
    return state.half_transition_count > 1 ||
        (state.half_transition_count == 1 && state.ended_down);
}

inline b32
is_down(Game_Button_State state) {
    return state.ended_down;
}

enum {
    B_MOVE_UP,
    B_MOVE_DOWN,
    B_SPEED_UP,
    
    
    B_COUNT, //This should always be in the bottom
};

struct {
    
    Game_Button_State buttons[B_COUNT];
    f32 last_dt;
} typedef Game_Input;


//Render

struct {
    
    //Pixels are always 32-bit wide, memory order XX BB GG RR
    s32   width;
    s32   height;
    s32   pitch;
    void *memory;
} typedef Game_Offscreen_Buffer;
