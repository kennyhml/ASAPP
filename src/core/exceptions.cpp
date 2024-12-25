#include "asa/core/exceptions.h"

namespace asa
{
    bool crash_aware = false;

    void set_crash_aware(const bool aware) { crash_aware = aware; }
    bool get_crash_aware() { return crash_aware; }
}
