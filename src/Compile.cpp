#include <cassert>
#include <cstdlib>
#include "CodeGen.h"
#include "CodeGen_Bash.h"
#include "Compile.h"
#include "Config.h"
#include "Parser.h"
#include "TypeChecker.h"
#include "Util.h"

using namespace Bish;

namespace {

// Add necessary stdlib functions to the given module.
void link_stdlib(Bish::Module *m) {
    Parser p;
    Module *stdlib = p.parse(get_stdlib_path());
    // TODO: this seems clunky. Trying to avoid importing stdlib if
    // the user is compiling stdlib itself.
    if (m->path.compare(stdlib->path) != 0) {
        m->import(stdlib);
    }
}

}

void Bish::compile_to(Module *m, CodeGenerator *cg) {
    link_stdlib(m);

    // Type checking
    TypeChecker types;
    m->accept(&types);
    
    cg->ostream() << "#!/usr/bin/env bash\n"
    << "# Autogenerated script, compiled from the Bish language.\n"
    << "# Bish version " << BISH_VERSION << "\n"
    << "# Please see " << BISH_URL << " for more information about Bish.\n\n";
    m->accept(cg);
}
