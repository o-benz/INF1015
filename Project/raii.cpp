#include "RAII.h"
#include "model.h"
#include <memory>

RAII::RAII(std::shared_ptr<model::Board> grid): grid_(grid) {}

RAII::~RAII() {}
