#include <emscripten/bind.h>
#include "resolver.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(resolver) {
   class_<Resolver>("Resolver")
      .constructor()
      .function("GetResolvedResult", &Resolver::GetResolvedResult)
      ;
}
