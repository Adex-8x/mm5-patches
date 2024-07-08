#include "pmdsky.h"

/*typedef unsigned char   undefined;

typedef unsigned char    bool;*/
typedef unsigned char    byte;
/*typedef long long    longlong;
typedef unsigned char    uchar;*/
typedef unsigned int    uint;
/*typedef unsigned int    uint3;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined6;
typedef unsigned long long    undefined8;*/
typedef unsigned short    ushort;
/*typedef int    wchar_t;*/
/*typedef struct Performer Performer, *PPerformer;

typedef struct CommandData CommandData, *PCommandData;

typedef struct animation animation, *Panimation;

typedef struct ActorMovementRelatedFunctionList ActorMovementRelatedFunctionList, *PActorMovementRelatedFunctionList;

typedef struct LiveActor LiveActor, *PLiveActor;*/

/*typedef enum direction_id {
    DIR_NONE=-1,
    DIR_DOWN=0,
    DIR_DOWN_RIGHT=1,
    DIR_RIGHT=2,
    DIR_UP_RIGHT=3,
    DIR_UP=4,
    DIR_UP_LEFT=5,
    DIR_LEFT=6,
    DIR_DOWN_LEFT=7,
    DIR_CURRENT=8
} direction_id;*/

/*typedef struct vec2_uint vec2_uint, *Pvec2_uint;

typedef struct SubCommandData SubCommandData, *PSubCommandData;

typedef struct unknown_related_ unknown_related_, *Punknown_related_;

typedef struct animation_control animation_control, *Panimation_control;

typedef struct vec2_int vec2_int, *Pvec2_int;*/

typedef enum RunStatus {
    ENDED=0,
    HOLD=1,
    CONTINUE=2,
    WAIT=3,
    WAIT_SECOND=4,
    WAIT_TRIS_PROCESS_SPECIAL=5,
    DESTROY=6
} RunStatus;

#pragma pack(push, 2)
ENUM_16_BIT(RunStatus);
#pragma pack(pop)

ASSERT_SIZE(struct RunStatus_16, 0x2);

/*typedef struct animation_control_bitflag animation_control_bitflag, *Panimation_control_bitflag;

typedef struct vec2_16 vec2_16, *Pvec2_16;

typedef ushort uint16_t;

typedef struct WanAnimationFrame WanAnimationFrame, *PWanAnimationFrame;

typedef struct WanOffset WanOffset, *PWanOffset;

typedef struct WanPalettes WanPalettes, *PWanPalettes;*/


/* WARNING! conflicting data type names: /pmdsky.h/bool - /bool */

/*typedef short int16_t;

typedef uchar uint8_t;

typedef struct animation_frame_flag animation_frame_flag, *Panimation_frame_flag;

typedef struct vec2_u16 vec2_u16, *Pvec2_u16;

typedef struct RGBA8 RGBA8, *PRGBA8;

typedef uint uint32_t;*/

struct RGBA8 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

struct vec2_int {
    int x;
    int y;
};

struct SubCommandData {
    undefined2 field0_0x0;
    struct RunStatus_16 run_status;
    ushort currently_playing_animation;
    char field3_0x6;
    undefined field4_0x7;
    undefined4 field5_0x8;
    undefined4 field6_0xc;
    char *command_pointer;
    ushort *field8_0x14;
    undefined4 field9_0x18;
    undefined4 field10_0x1c;
    undefined4 field11_0x20;
    undefined field12_0x24;
    undefined field13_0x25;
    undefined field14_0x26;
    undefined field15_0x27;
    char *current_opcode_pointer;
    char *current_argument_pointer;
    short field18_0x30;
    undefined2 field19_0x32;
    undefined2 current_animation_something;
    char new_direction;
    undefined field22_0x37;
    undefined2 field23_0x38;
    undefined2 special_process_related;
    undefined2 field25_0x3c;
    undefined field26_0x3e;
    undefined field27_0x3f;
    undefined4 field28_0x40;
    struct vec2_int some_coord; // destination?
    struct vec2_int some_coord_2; //destination?
    undefined field31_0x54;
    undefined field32_0x55;
    undefined field33_0x56;
    undefined field34_0x57;
    undefined field35_0x58;
    undefined field36_0x59;
    undefined field37_0x5a;
    undefined field38_0x5b;
    undefined field39_0x5c;
    undefined field40_0x5d;
    undefined field41_0x5e;
    undefined field42_0x5f;
    undefined4 field43_0x60[4];
};

ASSERT_SIZE(struct SubCommandData, 0x70);

struct animation_control_bitflag {
    undefined field0_0x0;
    bool loop:1;
    bool finished:1;
};

/*struct vec2_16 {
    int16_t x;
    int16_t y;
};*/

struct WanPalettes {
    struct RGBA8 *palette_bytes_start;
    uint16_t unk1;
    uint16_t nb_color;
    uint16_t unk2;
    uint8_t unk3;
    uint8_t unk5;
    uint32_t unk4; /* Normally always 0 */
};

struct vec2_u16 {
    uint16_t x;
    uint16_t y;
};

struct unknown_related_ {
    undefined2 field0_0x0;
    undefined2 field1_0x2;
    undefined2 field2_0x4;
    undefined2 field3_0x6;
    undefined2 field4_0x8;
    undefined2 field5_0xa;
};

struct animation_frame_flag {
    bool ret:1;
    bool ret2:1;
};

struct WanAnimationFrame {
    uint8_t duration;
    struct animation_frame_flag flag;
    uint16_t frame_id;
    struct vec2_16 offset;
    struct vec2_16 shadow_offset;
};

struct WanOffset {
    struct vec2_u16 head;
    struct vec2_u16 hand_left;
    struct vec2_u16 hand_right;
    struct vec2_u16 center;
};

struct CommandData { /* Maybe just part of ScriptStruct */
    struct ActorMovementRelatedFunctionList *functions;
    struct LiveActor *actor_ref;
    short other_movement_function;
    short field3_0xa;
    struct SubCommandData subcommand_data_first;
    struct SubCommandData subcommand_data_second;
};

/*struct animation_control {
    struct animation_control_bitflag some_bitfield;
    ushort field1_0x2;
    short field2_0x4;
    ushort anim_frame_duration;
    ushort field4_0x8;
    ushort field5_0xa;
    short unk;
    undefined field7_0xe;
    undefined field8_0xf;
    struct unknown_related_ unk_bis;
    short field10_0x1c;
    short field11_0x1e;
    struct vec2_16 anim_frame_offset;
    struct vec2_16 anim_frame_shadow_offset;
    uint anim_frame_flag;
    uint anim_frame_flag_sum;
    uint16_t animations_or_group_len;
    ushort field17_0x32;
    undefined field18_0x34;
    undefined field19_0x35;
    undefined2 field20_0x36;
    short field21_0x38;
    ushort anim_frame_id;
    undefined2 field23_0x3c;
    undefined field24_0x3e;
    undefined field25_0x3f;*/
    //undefined1 palette_pos_high; /* Created by retype action */
    //undefined1 palette_pos_low; /* Created by retype action */
    /*undefined field28_0x42;
    undefined field29_0x43;
    undefined field30_0x44;
    undefined field31_0x45;
    undefined field32_0x46;
    undefined field33_0x47;
    struct WanAnimationFrame *first_animation_frame;
    struct WanAnimationFrame *next_animation_frame;
    struct WanOffset *wan_offsets;
    undefined *wan_frames;
    void **wan_fragments_byte_store;
    struct WanPalettes *wan_palettes;
    uint16_t wan_is_256_color;
    uint16_t loop_start;
    undefined field42_0x64;
    undefined field43_0x65;
    undefined field44_0x66;
    undefined field45_0x67;
    undefined *sprite_override; *//* seems to point to a SIRO file containing another sprite, that is sometimes instead of the one specified here */
    /*undefined4 field47_0x6c;
    uint16_t loaded_sprite_id; *//* Id in the WAN_TABLE static list */
    /*undefined2 field49_0x72;
    undefined field50_0x74;
    undefined field51_0x75;
    ushort animation_group_id;
    ushort animation_id;
    undefined1 palette_bank;
    undefined field55_0x7b;
};*/

/*struct animation {
    struct unknown_related_ unk;
    struct animation_control sub_content;
    short sprite_id;
    byte field3_0x8a;
    undefined field4_0x8b;
    short alloc_for_max_frame;
    undefined2 field6_0x8e;
    ushort field7_0x90;
    ushort field8_0x92;
    short field9_0x94;
    short field10_0x96;
    char field11_0x98;
    undefined field12_0x99;
    short field13_0x9a;
    undefined2 field14_0x9c;
    short field15_0x9e;
    undefined field16_0xa0;
    undefined field17_0xa1;
    short field18_0xa2;
    undefined field19_0xa4;
    undefined field20_0xa5;
    undefined field21_0xa6;
    undefined field22_0xa7;
    undefined4 field23_0xa8;
    undefined2 field24_0xac;
    short field25_0xae;
    short field26_0xb0;
    undefined field27_0xb2;
    undefined field28_0xb3;
    undefined field29_0xb4;
    undefined field30_0xb5;
    undefined field31_0xb6;
    undefined field32_0xb7;
    int field33_0xb8;
    int field34_0xbc;
    undefined4 field35_0xc0;
};*/

struct LiveActor { /* from skytemple */
    short id; /* The id of the Actor in the actor list */
    short static_npc_id; /* The specie of the Actor */
    bool is_enabled; /* true when the actor is loaded, false otherwise (should be checked if unsure) */
    undefined field3_0x5;
    short hanger;
    char sector;
    char field6_0x9;
    short field7_0xa;
    struct vec2_int collision_box; /* The size of the collision box of the the pokemon */
    undefined field9_0x14;
    undefined field10_0x15;
    undefined field11_0x16;
    undefined field12_0x17;
    struct vec2_int size_div2; /* The size of the collision box divided by two */
    undefined field14_0x20;
    undefined field15_0x21;
    undefined field16_0x22;
    undefined field17_0x23;
    undefined field18_0x24;
    undefined field19_0x25;
    undefined field20_0x26;
    undefined field21_0x27;
    struct vec2_int limit_min_pos; /* minimum possible coordinates, for random move in free roam */
    struct vec2_int limit_max_pos; /* maximum possible coordinates, for random move in free roam */
    struct CommandData maybe_command_data; /* Seems to be a script-related struct */
    short field25_0x124;
    undefined field26_0x126;
    undefined field27_0x127;
    uint bitfied_collision_layer; /* not sure */
    int field29_0x12c;
    char current_direction; /* not sure */
    undefined field31_0x131;
    undefined field32_0x132;
    undefined field33_0x133;
    undefined field34_0x134;
    undefined field35_0x135;
    undefined field36_0x136;
    undefined field37_0x137;
    struct vec2_int field38_0x138;
    undefined field39_0x140;
    undefined field40_0x141;
    undefined field41_0x142;
    undefined field42_0x143;
    undefined field43_0x144;
    undefined field44_0x145;
    undefined field45_0x146;
    undefined field46_0x147;
    undefined field47_0x148;
    undefined field48_0x149;
    undefined field49_0x14a;
    undefined field50_0x14b;
    undefined field51_0x14c;
    undefined field52_0x14d;
    ushort field53_0x14e;
    short field54_0x150;
    bool field55_0x152;
    undefined field56_0x153;
    short field57_0x154;
    bool field58_0x156;
    undefined field59_0x157;
    short field60_0x158;
    char direction;
    undefined field62_0x15b;
    struct vec2_int coord_min; /* the top-left coordinate of the collision box of the actor */
    struct vec2_int coord_max; /* the bottom-right coordinate of the collision box of the actor */
    undefined4 field65_0x16c;
    undefined4 field66_0x170;
    byte field67_0x174;
    char field68_0x175;
    short field69_0x176;
    short field70_0x178;
    undefined field71_0x17a;
    undefined field72_0x17b;
    int movement_related; /* not sure. seem to be 0 when not moving and 3 when moving most of time. */
    short second_bitfield; /* not sure */
    short field75_0x182;
    short field76_0x184;
    undefined field77_0x186;
    undefined field78_0x187;
    undefined field79_0x188;
    undefined field80_0x189;
    undefined field81_0x18a;
    undefined field82_0x18b;
    struct animation animation; /* data about animation */
};

struct Performer {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined2 some_id;
    undefined field7_0x8;
    undefined field8_0x9;
    undefined field9_0xa;
    undefined field10_0xb;
    undefined field11_0xc;
    undefined field12_0xd;
    undefined field13_0xe;
    undefined field14_0xf;
    undefined field15_0x10;
    undefined field16_0x11;
    undefined field17_0x12;
    undefined field18_0x13;
    undefined field19_0x14;
    undefined field20_0x15;
    undefined field21_0x16;
    undefined field22_0x17;
    undefined field23_0x18;
    undefined field24_0x19;
    undefined field25_0x1a;
    undefined field26_0x1b;
    undefined field27_0x1c;
    undefined field28_0x1d;
    undefined field29_0x1e;
    undefined field30_0x1f;
    undefined field31_0x20;
    undefined field32_0x21;
    undefined field33_0x22;
    undefined field34_0x23;
    undefined field35_0x24;
    undefined field36_0x25;
    undefined field37_0x26;
    undefined field38_0x27;
    undefined field39_0x28;
    undefined field40_0x29;
    undefined field41_0x2a;
    undefined field42_0x2b;
    undefined field43_0x2c;
    undefined field44_0x2d;
    undefined field45_0x2e;
    undefined field46_0x2f;
    undefined field47_0x30;
    undefined field48_0x31;
    undefined field49_0x32;
    undefined field50_0x33;
    undefined field51_0x34;
    undefined field52_0x35;
    undefined field53_0x36;
    undefined field54_0x37;
    undefined field55_0x38;
    undefined field56_0x39;
    undefined field57_0x3a;
    undefined field58_0x3b;
    struct CommandData command_data;
    undefined field60_0x128;
    undefined field61_0x129;
    undefined field62_0x12a;
    undefined field63_0x12b;
    undefined field64_0x12c;
    undefined field65_0x12d;
    undefined field66_0x12e;
    undefined field67_0x12f;
    struct vec2_int hitbox_1;
    struct vec2_int hitbox_2;
    undefined field84_0x140;
    undefined field85_0x141;
    undefined field86_0x142;
    undefined field87_0x143;
    undefined field88_0x144;
    undefined field89_0x145;
    undefined field90_0x146;
    undefined field91_0x147;
    undefined field92_0x148;
    undefined field93_0x149;
    undefined field94_0x14a;
    undefined field95_0x14b;
    undefined field96_0x14c;
    undefined field97_0x14d;
    undefined field98_0x14e;
    undefined field99_0x14f;
    struct animation animation; /* Created by retype action */
};

ASSERT_SIZE(struct Performer, 0x214);

struct vec2_uint {
    uint x;
    uint y;
};

struct ActorMovementRelatedFunctionList {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    void* field4_0x4;
    void* field5_0x8;
    void* field6_0xc;
    void* field7_0x10;
    void (*get_direction)(struct LiveActor *, enum direction_id *); /* Created by retype action */
    void* field9_0x18;
    void* field10_0x1c;
    void* field11_0x20;
    void* field12_0x24;
    void* field13_0x28;
    void (*set_direction)(struct LiveActor *, undefined4);
    void* set_animation; /* Created by retype action */
    void* field16_0x34;
    void* field17_0x38;
    void* field18_0x3c;
    void* field19_0x40;
    int (*move)(struct LiveActor *, struct vec2_uint *);
    void* field21_0x48;
    void* field22_0x4c;
};

