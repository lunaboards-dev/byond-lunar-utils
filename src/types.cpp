#include <lunarutils/types.hpp>
#include <lunarutils/helpers.hpp>
#include <unordered_map>

std::string Byond_TypeName(CByondValue &val) {
    ByondDataType dt = (ByondDataType) val.type;
    static const std::unordered_map<ByondDataType, std::string> type_names = {
        {NULL_D, "NULL"},
        {TURF, "TURF"},
        {OBJ, "OBJ"},
        {MOB, "MOB"},
        {AREA, "AREA"},
        {CLIENT, "CLIENT"},
        {STRING, "STRING"},
        {DATUM, "DATUM"},
        {SAVEFILE, "SAVEFILE"},
        {PROCPATH, "PROC"},
        {FILE_, "FILE"},
        {PREFAB, "PREFAB"},
        {APPEARANCE, "APPEARANCE"},
        {FILTERS, "FILTERS"}
    };   
}

WrappedType Byond_WrappedType(ByondDataType dt) {
    static const std::unordered_map<ByondDataType, WrappedType> type_mapping = {
        {NULL_D, W_NULL},
        {TURF, W_TURF},
        {OBJ, W_OBJ},
        {MOB, W_MOB},
        {AREA, W_AREA},
        {CLIENT, W_CLIENT},
        {STRING, W_STRING},
        {DATUM, W_DATUM},
        {SAVEFILE, W_SAVEFILE},
        {PROCPATH, W_PROCPATH},
        {FILE_, W_FILE},
        {PREFAB, W_PREFAB},
        {APPEARANCE, W_APPEARANCE},
        {FILTERS, W_FILTERS}
    };
    if ((dt >= 0xF && dt <= 0x1c) || (dt >= 0x2c && dt <= 0x39) || (dt >= 0x40 && dt <= 0x52) || dt == 0x54)
        return W_LIST;
    if ((dt >= 0x8 && dt <= 0xb) || dt == 0x20 || dt == 0x24 || dt == 0x3b || dt == 0x3f)
        return W_TYPEPATH;
    if (type_mapping.contains(dt))
        return type_mapping.at(dt);
    return W_UNKNOWN;
}

LByondValue::LByondValue(float v) {
    val.data.num = v;
    val.type = NUMBER;
    update_type();
}

LByondValue::LByondValue(ByondDataType type, u4c refid) {
    val.data.ref = refid;
    val.type = type;
    update_type();
}

LByondValue::LByondValue(CByondValue v) {
    val = v;
    update_type();
}

LByondValue::operator std::string() {
    return ByondValue_ToString(&val);
}

LByondValue::operator float() {
    return val.data.num;
}

LByondValue const LByondValue::operator[](std::string var) {
    LByondValue rv;
    Byond_ReadVar(&val, var.c_str(), &rv.val);
    rv.update_type();
    return rv;
}

LByondValue& LByondValue::operator=(float v) {
    val.data.num = v;
    return *this;
}

LByondValue& LByondValue::operator=(std::string v) {
    ByondValue_SetStr(&val, v.c_str());
    return *this;
}

void LByondValue::set_var(std::string var, LByondValue &v) {
    Byond_WriteVar(&val, var.c_str(), &v.val);
}