#pragma once
#include <memory>
#include "model.h"
//#include "ChessGrid.h"

class RAII {
public:
    RAII(std::shared_ptr<model::Board> grid);
    ~RAII();
    
private:
    std::shared_ptr<model::Piece> piece_;
    std::shared_ptr<model::Board> grid_;
};
