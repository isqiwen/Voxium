#include <assert.h>

#include "Core/Meta/Json.h"
#include "Core/Meta/Reflection/Reflection.h"
#include "Core/Meta/Reflection/ReflectionRegister.h"
#include "Core/Meta/Serializer/Serializer.h"

#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace Voxium::Core::Reflection
{
    void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
} // namespace Voxium::Core::Reflection
