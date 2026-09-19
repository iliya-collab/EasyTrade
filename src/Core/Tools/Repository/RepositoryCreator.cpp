#include "RepositoryCreator.hpp"

namespace Core::Tools {

    RepositoryCreator& RepositoryCreator::instance() {
        static RepositoryCreator creater;
        return creater;
    }

}
