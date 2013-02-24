#ifndef _OSX_REGISTRY_H_
#define _OSX_REGISTRY_H_

#include <string>

namespace Registry {
inline std::string ReadStrValue(std::string const& /* section */, std::string const& /* key */, std::string const&) { return ""; }
}

#endif
