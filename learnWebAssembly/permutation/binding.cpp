#include <emscripten/bind.h>
#include "permutation.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(permutation) {
   class_<Permutation>("Permutation")
      .constructor()
      .function("InputArray", &Permutation::InputArray)
      .function("GetPermutations", &Permutation::GetPermutations)
      ;
}