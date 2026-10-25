#pragma once


#include <byondapi.h>
#include <string>

/* #define NUMBER 0x2A
#define DATUM 0x21 */

enum ByondDataType : unsigned char
{
	NULL_D = 0x00,
	TURF = 0x01,
	OBJ = 0x02,
	MOB = 0x03,
	AREA = 0x04,
	CLIENT = 0x05,
	STRING = 0x06,
	MOB_TYPEPATH = 0x08,
	OBJ_TYPEPATH = 0x09,
	TURF_TYPEPATH = 0x0A,
	AREA_TYPEPATH = 0x0B,
	RESOURCE = 0x0C,
	IMAGE = 0x0D,
	WORLD_D = 0x0E,
	LIST = 0x0F,
	LIST_ARGS = 0x10,
	LIST_MOB_VERBS = 0x11,
	LIST_VERBS = 0x12,
	LIST_TURF_VERBS = 0x13,
	LIST_AREA_VERBS = 0x14,
	LIST_CLIENT_VERBS = 0x15,
	LIST_SAVEFILE_DIR = 0x16,
	LIST_MOB_CONTENTS = 0x17,
	LIST_TURF_CONTENTS = 0x18,
	LIST_AREA_CONTENTS = 0x19,
	LIST_WORLD_CONTENTS = 0x1A,
	LIST_GROUP = 0x1B,
	LIST_CONTENTS = 0x1C,
	DATUM_TYPEPATH = 0x20,
	DATUM = 0x21,
	SAVEFILE = 0x23,
	SAVEFILE_TYPEPATH = 0x24,
	PROCPATH = 0x26,
	FILE_ = 0x27,
	LIST_TYPEPATH = 0x28,
	PREFAB = 0x29,
	NUMBER = 0x2A,
	LIST_MOB_VARS = 0x2C,
	LIST_OBJ_VARS = 0x2D,
	LIST_TURF_VARS = 0x2E,
	LIST_AREA_VARS = 0x2F,
	LIST_CLIENT_VARS = 0x30,
	LIST_VARS = 0x31, //maybe?
	LIST_MOB_OVERLAYS = 0x32,
	LIST_MOB_UNDERLAYS = 0x33,
	LIST_OVERLAYS = 0x34,
	LIST_UNDERLAYS = 0x35,
	LIST_TURF_OVERLAYS = 0x36,
	LIST_TURF_UNDERLAYS = 0x37,
	LIST_AREA_OVERLAYS = 0x38,
	LIST_AREA_UNDERLAYS = 0x39,
	APPEARANCE = 0x3A,
	CLIENT_TYPEPATH = 0x3B,
	IMAGE_TYPEPATH = 0x3F,
	LIST_IMAGE_OVERLAYS = 0x40,
	LIST_IMAGE_UNDERLAYS = 0x41,
	LIST_IMAGE_VARS = 0x42,
	LIST_IMAGE_VERBS = 0x43,
	LIST_IMAGE_CONTENTS = 0x44, // wait wtf
	LIST_CLIENT_IMAGES = 0x46,
	LIST_CLIENT_SCREEN = 0x47,
	LIST_TURF_VIS_CONTENTS = 0x4B,
	LIST_VIS_CONTENTS = 0x4C,
	LIST_MOB_VIS_CONTENTS = 0x4D,
	LIST_TURF_VIS_LOCS = 0x4E,
	LIST_VIS_LOCS = 0x4F,
	LIST_MOB_VIS_LOCS = 0x50,
	LIST_WORLD_VARS = 0x51,
	LIST_GLOBAL_VARS = 0x52,
	FILTERS = 0x53,
	LIST_IMAGE_VIS_CONTENTS = 0x54,
};

enum WrappedType {
    W_NULL,
    W_TURF,
    W_OBJ,
    W_MOB,
    W_AREA,
    W_CLIENT,
    W_STRING,
    W_DATUM,
    W_SAVEFILE,
    W_PROCPATH,
    W_FILE,
    W_PREFAB,
    W_APPEARANCE,
    W_FILTERS,
    W_LIST,
    W_TYPEPATH,
	W_UNKNOWN
};

std::string Byond_TypeName(CByondValue &val);
WrappedType Byond_WrappedType(ByondDataType dt);

// THIS IS ONLY FOR USE INSIDE FUNCTIONS
class LByondValue {
    private:
        CByondValue val;
        WrappedType type;
		LByondValue() {
			val.type = NULL_D;
			val.data.ref = 0;
		}
		void update_type() {
			type = Byond_WrappedType((ByondDataType) val.type);
		}
    public:
        LByondValue(float val);
        LByondValue(ByondDataType type, u4c refid);
        LByondValue(CByondValue val);

        operator std::string();
        operator float();

        LByondValue const operator[](std::string var);

        LByondValue& operator=(float val);
		LByondValue& operator=(std::string val);

		inline unsigned int ref() {
			return val.data.ref;
		}

		inline unsigned int set_ref(unsigned int r) {
			return val.data.ref = r;
		}

		inline WrappedType get_type() {
			return this->type;
		}

		inline WrappedType set_type(ByondDataType t) {
			val.type = t;
			update_type();
			return this->type;
		}

		void set_var(std::string var, LByondValue &val);
};

static CByondValue ByondTrue = {
    .type = NUMBER,
    .data = {.num = 1}
};

static CByondValue ByondFalse = {
    .type = NUMBER,
    .data = {.num = 0}
};

static CByondValue ByondNull = {
    .type = NULL_D,
    .data = {.num = 0}
};

static CByondValue ByondGlobal = {
    .type = WORLD_D,
    .data = {.num = 1}
};

static CByondValue ByondWorld = {
    .type = WORLD_D,
    .data = {.num = 0}
};